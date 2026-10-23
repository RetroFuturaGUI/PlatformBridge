#include "FileSystem.hpp"
#include <Windows.h>
#include <ShlObj.h>
#include <ShObjIdl.h>
#include <sherrors.h>
#include <wrl/client.h>
#include <KnownFolders.h>
#include <cwchar>
#include <string_view>

void PlatformBridge::FileSystem::Refresh()
{
    refreshRoots();
    refreshKnownFolders();
}

const std::vector<PlatformBridge::FileSystem::Root>& PlatformBridge::FileSystem::GetRoots()
{
    return _roots;
}

const std::filesystem::path& PlatformBridge::FileSystem::GetKnownFolder(const KnownFolder folder)
{
    static const std::filesystem::path none {};
    const size_t index { static_cast<size_t>(folder) };

    if(_knownFolderCount <= index)
        return none;

    return _knownFolders[index];
}

bool PlatformBridge::FileSystem::ListDirectory(const std::filesystem::path& directory, std::vector<Entry>& entriesOut, std::error_code& errorOut)
{
    entriesOut.clear();
    errorOut.clear();

    if(directory.empty())
    {
        errorOut = std::make_error_code(std::errc::no_such_file_or_directory);
        return false;
    }

    //An empty card reader or disc drive fails here instead of showing a "no disk in drive" box
    DWORD previousErrorMode { 0 };
    SetThreadErrorMode(SEM_FAILCRITICALERRORS, &previousErrorMode);

    //The attributes come with each name, so the hidden flag costs no extra call per file
    const std::filesystem::path pattern { directory / L"*" };
    WIN32_FIND_DATAW data {};
    const HANDLE find { FindFirstFileExW(pattern.c_str(), FindExInfoBasic, &data, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH) };

    if(INVALID_HANDLE_VALUE == find)
    {
        const DWORD error { GetLastError() };
        SetThreadErrorMode(previousErrorMode, nullptr);

        //A drive's root has no "." or "..", so a drive without files finds nothing at all
        if(ERROR_FILE_NOT_FOUND == error)
            return true;

        errorOut = std::error_code(static_cast<int>(error), std::system_category());
        return false;
    }

    do
    {
        const std::wstring_view name { data.cFileName };

        if(name == L"." || name == L"..")
            continue;

        const bool isDirectory { 0 != (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) };
        const uint64_t
            size { (static_cast<uint64_t>(data.nFileSizeHigh) << 32) | data.nFileSizeLow },
            ticks { (static_cast<uint64_t>(data.ftLastWriteTime.dwHighDateTime) << 32) | data.ftLastWriteTime.dwLowDateTime };
        EntryKind kind { EntryKind::File };

        if(isDirectory)
            kind = EntryKind::Directory;
        else if(0 != (data.dwFileAttributes & FILE_ATTRIBUTE_DEVICE))
            kind = EntryKind::Other;

        entriesOut.push_back(Entry
            {
                ._Path = directory / name,
                ._Kind = kind,
                ._Size = isDirectory ? 0 : size,
                //MSVC's file clock counts 100 ns ticks from 1601, as FILETIME does
                ._LastWriteTime = std::filesystem::file_time_type(std::filesystem::file_time_type::duration(static_cast<int64_t>(ticks))),
                //Protected system files are hidden too, so the system flag alone doesn't hide a file
                ._IsHidden = 0 != (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN),
                //Cloud Files placeholders (OneDrive, kDrive) and files moved to slow storage
                ._IsOnlineOnly = 0 != (data.dwFileAttributes & (FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS | FILE_ATTRIBUTE_RECALL_ON_OPEN | FILE_ATTRIBUTE_OFFLINE))
            });
    }
    while(FindNextFileW(find, &data));

    const DWORD error { GetLastError() };
    FindClose(find);
    SetThreadErrorMode(previousErrorMode, nullptr);

    if(ERROR_NO_MORE_FILES == error)
        return true;

    errorOut = std::error_code(static_cast<int>(error), std::system_category());
    return false;
}

void PlatformBridge::FileSystem::refreshRoots()
{
    _roots.clear();
    const DWORD length { GetLogicalDriveStringsW(0, nullptr) };

    if(0 == length)
        return;

    //"C:\", "D:\" and so on, each ended by a null, the whole list by a second one
    std::wstring drives(length + 1, L'\0');

    if(0 == GetLogicalDriveStringsW(length, drives.data()))
        return;

    DWORD previousErrorMode { 0 };
    SetThreadErrorMode(SEM_FAILCRITICALERRORS, &previousErrorMode);

    for(const wchar_t* drive { drives.c_str() }; L'\0' != *drive; drive += std::wcslen(drive) + 1)
    {
        RootKind kind { RootKind::Other };

        switch(GetDriveTypeW(drive))
        {
            case DRIVE_FIXED:
                kind = RootKind::Fixed;
                break;
            case DRIVE_REMOVABLE:
                kind = RootKind::Removable;
                break;
            case DRIVE_CDROM:
                kind = RootKind::Optical;
                break;
            case DRIVE_REMOTE:
                kind = RootKind::Network;
                break;
            default:
                break;
        }

        std::string label {};

        //Fixed drives only: an empty card reader or disc drive errors, and a network drive can stall
        if(RootKind::Fixed == kind)
        {
            wchar_t volumeName[MAX_PATH + 1] {};

            if(GetVolumeInformationW(drive, volumeName, MAX_PATH + 1, nullptr, nullptr, nullptr, nullptr, 0))
            {
                const std::u8string utf8 { std::filesystem::path(volumeName).u8string() };
                label = std::string(utf8.begin(), utf8.end());
            }
        }

        _roots.push_back(Root
            {
                ._Path = std::filesystem::path(drive),
                ._Label = std::move(label),
                ._Kind = kind
            });
    }

    SetThreadErrorMode(previousErrorMode, nullptr);
}

void PlatformBridge::FileSystem::refreshKnownFolders()
{
    //In the order of KnownFolder
    const std::array<KNOWNFOLDERID, _knownFolderCount> folderIDs
    {
        FOLDERID_Profile,
        FOLDERID_Desktop,
        FOLDERID_Documents,
        FOLDERID_Downloads,
        FOLDERID_Music,
        FOLDERID_Pictures,
        FOLDERID_Videos
    };

    for(size_t index = 0; index < _knownFolderCount; ++index)
    {
        _knownFolders[index].clear();
        PWSTR folder { nullptr };

        if(SUCCEEDED(SHGetKnownFolderPath(folderIDs[index], KF_FLAG_DEFAULT, nullptr, &folder)))
            _knownFolders[index] = folder;

        CoTaskMemFree(folder); //owed even when the call fails
    }
}

PlatformBridge::FileSystem::DirectoryWatcher::~DirectoryWatcher()
{
    Stop();
}

bool PlatformBridge::FileSystem::DirectoryWatcher::Watch(const std::filesystem::path& directory, std::error_code& errorOut)
{
    Stop();
    errorOut.clear();

    const HANDLE handle { FindFirstChangeNotificationW(directory.c_str(), FALSE, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME
        | FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE) };

    if(INVALID_HANDLE_VALUE == handle)
    {
        errorOut = std::error_code(static_cast<int>(GetLastError()), std::system_category());
        return false;
    }

    _handle = handle;
    return true;
}

void PlatformBridge::FileSystem::DirectoryWatcher::Stop()
{
    if(!_handle)
        return;

    FindCloseChangeNotification(static_cast<HANDLE>(_handle));
    _handle = nullptr;
}

bool PlatformBridge::FileSystem::DirectoryWatcher::HasChanged()
{
    if(!_handle)
        return false;

    if(WAIT_OBJECT_0 != WaitForSingleObject(static_cast<HANDLE>(_handle), 0))
        return false;

    //Armed again for the changes after the ones the caller lists now. It can't be once the folder is gone.
    if(!FindNextChangeNotification(static_cast<HANDLE>(_handle)))
        Stop();

    return true;
}

bool PlatformBridge::FileSystem::DirectoryWatcher::IsWatching() const
{
    return _handle != nullptr;
}

/// @brief Hears from the Shell before an item is deleted whether it goes to the recycle bin, and stops one that wouldn't. It lives on
///        MoveToTrash's stack, so it only counts its references.
class PlatformBridge::FileSystem::TrashSink final : public IFileOperationProgressSink
{
public:
    bool _IsRefused { false }; //the Shell would have deleted the item for good, so it was left alone
    HRESULT _DeleteResult { S_OK };

    IFACEMETHODIMP QueryInterface(REFIID interfaceId, void** objectOut) override
    {
        if(!objectOut)
            return E_POINTER;

        if(IID_IUnknown != interfaceId && __uuidof(IFileOperationProgressSink) != interfaceId)
        {
            *objectOut = nullptr;
            return E_NOINTERFACE;
        }

        *objectOut = static_cast<IFileOperationProgressSink*>(this);
        AddRef();
        return S_OK;
    }

    IFACEMETHODIMP_(ULONG) AddRef() override { return ++_referenceCount; }
    IFACEMETHODIMP_(ULONG) Release() override { return --_referenceCount; }

    //Without the flag there is no bin for the item: its drive has none, or the item is too big for it
    IFACEMETHODIMP PreDeleteItem(DWORD flags, IShellItem*) override
    {
        if(0 != (flags & TSF_DELETE_RECYCLE_IF_POSSIBLE))
            return S_OK;

        _IsRefused = true;
        return HRESULT_FROM_WIN32(ERROR_CANCELLED);
    }

    IFACEMETHODIMP PostDeleteItem(DWORD, IShellItem*, HRESULT result, IShellItem*) override
    {
        if(FAILED(result))
            _DeleteResult = result;

        return S_OK;
    }

    IFACEMETHODIMP StartOperations() override { return S_OK; }
    IFACEMETHODIMP FinishOperations(HRESULT) override { return S_OK; }
    IFACEMETHODIMP PreRenameItem(DWORD, IShellItem*, LPCWSTR) override { return S_OK; }
    IFACEMETHODIMP PostRenameItem(DWORD, IShellItem*, LPCWSTR, HRESULT, IShellItem*) override { return S_OK; }
    IFACEMETHODIMP PreMoveItem(DWORD, IShellItem*, IShellItem*, LPCWSTR) override { return S_OK; }
    IFACEMETHODIMP PostMoveItem(DWORD, IShellItem*, IShellItem*, LPCWSTR, HRESULT, IShellItem*) override { return S_OK; }
    IFACEMETHODIMP PreCopyItem(DWORD, IShellItem*, IShellItem*, LPCWSTR) override { return S_OK; }
    IFACEMETHODIMP PostCopyItem(DWORD, IShellItem*, IShellItem*, LPCWSTR, HRESULT, IShellItem*) override { return S_OK; }
    IFACEMETHODIMP PreNewItem(DWORD, IShellItem*, LPCWSTR) override { return S_OK; }
    IFACEMETHODIMP PostNewItem(DWORD, IShellItem*, LPCWSTR, LPCWSTR, DWORD, HRESULT, IShellItem*) override { return S_OK; }
    IFACEMETHODIMP UpdateProgress(UINT, UINT) override { return S_OK; }
    IFACEMETHODIMP ResetTimer() override { return S_OK; }
    IFACEMETHODIMP PauseTimer() override { return S_OK; }
    IFACEMETHODIMP ResumeTimer() override { return S_OK; }

private:
    ULONG _referenceCount { 1 };
};

PlatformBridge::FileSystem::TrashResult PlatformBridge::FileSystem::MoveToTrash(const std::filesystem::path& path, std::error_code& errorOut)
{
    errorOut.clear();

    //The Shell parses whole paths with backslashes only
    const std::filesystem::path fullPath { std::filesystem::absolute(path, errorOut).make_preferred() };

    if(errorOut)
        return TrashResult::Failed;

    if(!fitsRecycleBin(fullPath))
        return TrashResult::NotTrashable;

    //A thread already in the multithreaded apartment keeps it; the Shell's file operations work there too
    const HRESULT initialized { CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE) };
    TrashResult result { TrashResult::Failed };

    {
        TrashSink sink {}; //before the operation, so it outlives the operation's reference to it
        Microsoft::WRL::ComPtr<IFileOperation> operation {};
        Microsoft::WRL::ComPtr<IShellItem> item {};
        DWORD cookie { 0 };
        HRESULT step { CoCreateInstance(CLSID_FileOperation, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&operation)) };

        if(SUCCEEDED(step))
            step = operation->SetOperationFlags(FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI | FOFX_RECYCLEONDELETE);

        if(SUCCEEDED(step))
            step = SHCreateItemFromParsingName(fullPath.c_str(), nullptr, IID_PPV_ARGS(&item));

        if(SUCCEEDED(step))
            step = operation->Advise(&sink, &cookie);

        if(SUCCEEDED(step))
        {
            step = operation->DeleteItem(item.Get(), nullptr);

            if(SUCCEEDED(step))
                step = operation->PerformOperations();

            operation->Unadvise(cookie);
        }

        if(sink._IsRefused)
            result = TrashResult::NotTrashable;
        else if(FAILED(step))
            errorOut = shellError(step);
        else if(FAILED(sink._DeleteResult))
            errorOut = shellError(sink._DeleteResult);
        else
            result = TrashResult::Moved;
    }

    if(SUCCEEDED(initialized))
        CoUninitialize();

    return result;
}

std::error_code PlatformBridge::FileSystem::shellError(const long result)
{
    DWORD error { 0 };

    switch(result)
    {
        case COPYENGINE_E_SHARING_VIOLATION_SRC:
        case COPYENGINE_E_SHARING_VIOLATION_DEST:
            error = ERROR_SHARING_VIOLATION;
        break;
        case COPYENGINE_E_ACCESS_DENIED_SRC:
        case COPYENGINE_E_ACCESS_DENIED_DEST:
        case COPYENGINE_E_REQUIRES_ELEVATION:
            error = ERROR_ACCESS_DENIED;
        break;
        case COPYENGINE_E_PATH_NOT_FOUND_SRC:
        case COPYENGINE_E_PATH_NOT_FOUND_DEST:
        case COPYENGINE_E_CANT_REACH_SOURCE:
            error = ERROR_PATH_NOT_FOUND;
        break;
        case COPYENGINE_E_NET_DISCONNECT_SRC:
        case COPYENGINE_E_NET_DISCONNECT_DEST:
            error = ERROR_BAD_NETPATH;
        break;
        case COPYENGINE_E_PATH_TOO_DEEP_SRC:
        case COPYENGINE_E_PATH_TOO_DEEP_DEST:
            error = ERROR_FILENAME_EXCED_RANGE;
        break;
        case COPYENGINE_E_DISK_FULL:
        case COPYENGINE_E_DISK_FULL_CLEAN:
            error = ERROR_DISK_FULL;
        break;
        default:
            error = FACILITY_WIN32 == HRESULT_FACILITY(result) ? HRESULT_CODE(result) : static_cast<DWORD>(result);
        break;
    }

    return std::error_code(static_cast<int>(error), std::system_category());
}

bool PlatformBridge::FileSystem::fitsRecycleBin(const std::filesystem::path& path)
{
    std::array<wchar_t, MAX_PATH + 1> volumePath {};

    if(!GetVolumePathNameW(path.c_str(), volumePath.data(), static_cast<DWORD>(volumePath.size())))
        return true;

    //The bin's size and whether it is off are set per volume, under the volume's GUID
    uint64_t capacity { 0 };
    bool hasCapacity { false };
    std::array<wchar_t, 64> volumeName {}; //"\\?\Volume{GUID}\"

    if(GetVolumeNameForVolumeMountPointW(volumePath.data(), volumeName.data(), static_cast<DWORD>(volumeName.size())))
    {
        const std::wstring_view name { volumeName.data() };
        const size_t
            guidStart { name.find(L'{') },
            guidEnd { name.find(L'}') };

        if(std::wstring_view::npos != guidStart && std::wstring_view::npos != guidEnd)
        {
            const std::wstring key { std::wstring(L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\BitBucket\\Volume\\") + std::wstring(name.substr(guidStart, guidEnd - guidStart + 1)) };
            DWORD
                value { 0 },
                valueSize { sizeof(value) };

            if(ERROR_SUCCESS == RegGetValueW(HKEY_CURRENT_USER, key.c_str(), L"NukeOnDelete", RRF_RT_REG_DWORD, nullptr, &value, &valueSize) && 0 != value)
                return false; //"Don't move files to the Recycle Bin"

            valueSize = sizeof(value);

            if(ERROR_SUCCESS == RegGetValueW(HKEY_CURRENT_USER, key.c_str(), L"MaxCapacity", RRF_RT_REG_DWORD, nullptr, &value, &valueSize))
            {
                capacity = static_cast<uint64_t>(value) * 1024 * 1024; //in MB
                hasCapacity = true;
            }
        }
    }

    //A volume without settings has Windows' default bin: a tenth of its first 40 GB and a twentieth of the rest
    if(!hasCapacity)
    {
        ULARGE_INTEGER volumeSize {};

        if(!GetDiskFreeSpaceExW(volumePath.data(), nullptr, &volumeSize, nullptr))
            return true;

        constexpr uint64_t firstPart { 40ull * 1024 * 1024 * 1024 };
        capacity = firstPart >= volumeSize.QuadPart ? volumeSize.QuadPart / 10 : firstPart / 10 + (volumeSize.QuadPart - firstPart) / 20;
    }

    std::error_code error {};

    if(!std::filesystem::is_directory(path, error))
    {
        const uint64_t size { std::filesystem::file_size(path, error) };
        return error || capacity >= size; //a file that can't be read is the Shell's to report
    }

    //A folder's files are added up only until they are too many for the bin
    uint64_t total { 0 };

    for(std::filesystem::recursive_directory_iterator iterator { path, std::filesystem::directory_options::skip_permission_denied, error }, end {}; !error && iterator != end; iterator.increment(error))
    {
        std::error_code entryError {};

        if(!iterator->is_regular_file(entryError))
            continue;

        total += iterator->file_size(entryError);

        if(capacity < total)
            return false;
    }

    return true;
}

bool PlatformBridge::FileSystem::IsSameVolume(const std::filesystem::path& first, const std::filesystem::path& second)
{
    //By the volume's serial number, which every path onto the volume shares however it is written
    const auto readSerial { [](const std::filesystem::path& path, DWORD& serialOut)
        {
            const HANDLE handle { CreateFileW(path.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr) }; //folders open only with the backup flag

            if(INVALID_HANDLE_VALUE == handle)
                return false;

            BY_HANDLE_FILE_INFORMATION information {};
            const BOOL isRead { GetFileInformationByHandle(handle, &information) };
            CloseHandle(handle);
            serialOut = information.dwVolumeSerialNumber;
            return FALSE != isRead;
        } };

    DWORD
        firstSerial { 0 },
        secondSerial { 0 };

    if(!readSerial(first, firstSerial) || !readSerial(second, secondSerial))
        return true;

    return firstSerial == secondSerial;
}

bool PlatformBridge::FileSystem::RestoreFromTrash(const std::filesystem::path& originalPath, std::filesystem::path& restoredPathOut, std::error_code& errorOut)
{
    errorOut.clear();
    restoredPathOut.clear();
    const std::filesystem::path fullPath { std::filesystem::absolute(originalPath, errorOut).make_preferred() };

    if(errorOut)
        return false;

    const std::filesystem::path
        folder { fullPath.parent_path() },
        name { fullPath.filename() };

    std::filesystem::create_directories(folder, errorOut); //a folder it was in that went too

    if(errorOut)
        return false;

    //The bin's "Original Location" and "Date Deleted" columns, which the SDK names only by their property set, and the file's name
    constexpr PROPERTYKEY
        deletedFromKey { .fmtid = { 0x9b174b33, 0x40ff, 0x11d2, { 0xa2, 0x7e, 0x00, 0xc0, 0x4f, 0xc3, 0x08, 0x71 } }, .pid = 2 },
        dateDeletedKey { .fmtid = { 0x9b174b33, 0x40ff, 0x11d2, { 0xa2, 0x7e, 0x00, 0xc0, 0x4f, 0xc3, 0x08, 0x71 } }, .pid = 3 },
        fileNameKey { .fmtid = { 0x41cf5ae0, 0xf75a, 0x4806, { 0xbd, 0x87, 0x59, 0xc7, 0xd9, 0x24, 0x8e, 0xb9 } }, .pid = 100 };
    const auto isSameName { [](const wchar_t* text, const std::filesystem::path& path) { return CSTR_EQUAL == CompareStringOrdinal(text, -1, path.c_str(), -1, TRUE); } };

    const HRESULT initialized { CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE) };
    bool isRestored { false };

    {
        Microsoft::WRL::ComPtr<IShellItem>
            bin {},
            newest {};
        Microsoft::WRL::ComPtr<IEnumShellItems> items {};
        HRESULT step { SHGetKnownFolderItem(FOLDERID_RecycleBinFolder, KF_FLAG_DEFAULT, nullptr, IID_PPV_ARGS(&bin)) };

        if(SUCCEEDED(step))
            step = bin->BindToHandler(nullptr, BHID_EnumItems, IID_PPV_ARGS(&items));

        //The same path may be in the bin more than once; the one deleted last goes back
        ULONGLONG newestTime { 0 };

        for(Microsoft::WRL::ComPtr<IShellItem> item {}; SUCCEEDED(step) && S_OK == items->Next(1, &item, nullptr); item.Reset())
        {
            Microsoft::WRL::ComPtr<IShellItem2> details {};

            if(FAILED(item.As(&details)))
                continue;

            PWSTR
                deletedFrom { nullptr },
                itemName { nullptr };
            const bool isMatch { SUCCEEDED(details->GetString(deletedFromKey, &deletedFrom)) && SUCCEEDED(details->GetString(fileNameKey, &itemName))
                && isSameName(deletedFrom, folder) && isSameName(itemName, name) };
            CoTaskMemFree(deletedFrom);
            CoTaskMemFree(itemName);

            if(!isMatch)
                continue;

            FILETIME deleted {};
            details->GetFileTime(dateDeletedKey, &deleted);
            const ULONGLONG time { (static_cast<ULONGLONG>(deleted.dwHighDateTime) << 32) | deleted.dwLowDateTime };

            if(newest && newestTime >= time)
                continue;

            newest = item;
            newestTime = time;
        }

        if(FAILED(step))
        {
            errorOut = shellError(step);
        }
        else if(!newest)
        {
            errorOut = std::make_error_code(std::errc::no_such_file_or_directory);
        }
        else
        {
            //A name taken meanwhile gets a free one the way the rest of the library names them; the Shell's would be "Name - Copy"
            SFGAOF attributes { 0 };
            newest->GetAttributes(SFGAO_FOLDER, &attributes);
            const std::filesystem::path target { FreeName(fullPath, 0 != (attributes & SFGAO_FOLDER)) };

            //Where the bin keeps it, "$R" and an id; its record beside it is "$I" and the same id
            PWSTR binPath { nullptr };
            std::filesystem::path record {};

            if(SUCCEEDED(newest->GetDisplayName(SIGDN_FILESYSPATH, &binPath)))
            {
                const std::filesystem::path stored { binPath };
                const std::wstring storedName { stored.filename().native() };

                if(storedName.starts_with(L"$R"))
                    record = stored.parent_path() / (L"$I" + storedName.substr(2));
            }

            CoTaskMemFree(binPath);

            //Moved out of the bin, the Shell drops its record of the item
            Microsoft::WRL::ComPtr<IFileOperation> operation {};
            Microsoft::WRL::ComPtr<IShellItem> destination {};
            BOOL isAborted { FALSE };
            step = CoCreateInstance(CLSID_FileOperation, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&operation));

            if(SUCCEEDED(step))
                step = operation->SetOperationFlags(FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR);

            if(SUCCEEDED(step))
                step = SHCreateItemFromParsingName(folder.c_str(), nullptr, IID_PPV_ARGS(&destination));

            if(SUCCEEDED(step))
                step = operation->MoveItem(newest.Get(), destination.Get(), target.filename().c_str(), nullptr);

            if(SUCCEEDED(step))
                step = operation->PerformOperations();

            if(SUCCEEDED(step))
                step = operation->GetAnyOperationsAborted(&isAborted);

            if(FAILED(step))
                errorOut = shellError(step);
            else if(isAborted)
                errorOut = std::make_error_code(std::errc::operation_canceled);
            else
            {
                isRestored = true;
                restoredPathOut = target;

                //The move takes the item but leaves its record, which Windows Explorer's Restore removes too
                std::error_code recordError {};

                if(!record.empty())
                    std::filesystem::remove(record, recordError);
            }
        }
    }

    if(SUCCEEDED(initialized))
        CoUninitialize();

    return isRestored;
}