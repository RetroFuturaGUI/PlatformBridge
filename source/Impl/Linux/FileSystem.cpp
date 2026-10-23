#include "FileSystem.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string_view>
#include <utility>
#include <cerrno>
#include <charconv>
#include <ctime>
#include <format>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <unistd.h>

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

    std::filesystem::directory_iterator iterator { directory, std::filesystem::directory_options::skip_permission_denied, errorOut };

    if(errorOut)
        return false;

    //A failed step ends the iterator and sets errorOut, which quits the loop
    for(const std::filesystem::directory_iterator end {}; iterator != end; iterator.increment(errorOut))
    {
        const std::filesystem::directory_entry& found { *iterator };
        std::error_code entryError {};
        EntryKind kind { EntryKind::Other };
        uint64_t size { 0 };

        if(found.is_directory(entryError))
            kind = EntryKind::Directory;
        else if(found.is_regular_file(entryError))
        {
            kind = EntryKind::File;
            const uintmax_t fileSize { found.file_size(entryError) };
            size = entryError ? 0 : fileSize;
        }

        const std::filesystem::file_time_type lastWriteTime { found.last_write_time(entryError) };

        entriesOut.push_back(Entry
            {
                ._Path = found.path(),
                ._Kind = kind,
                ._Size = size,
                ._LastWriteTime = entryError ? std::filesystem::file_time_type {} : lastWriteTime,
                ._IsHidden = found.path().filename().native().starts_with('.'),
                ._IsOnlineOnly = false
            });
    }

    return !errorOut;
}

void PlatformBridge::FileSystem::refreshRoots()
{
    _roots.clear();
    _roots.push_back(Root
        {
            ._Path = std::filesystem::path("/"),
            ._Label = {},
            ._Kind = RootKind::Fixed
        });

    //Each line: device, mount point, file system type, options. Spaces in the mount point are written as \040 (32 in base 8)
    std::ifstream mounts { "/proc/self/mounts" };
    std::string line {};

    while(std::getline(mounts, line))
    {
        std::istringstream fields { line };
        std::string
            device {},
            escapedMountPoint {},
            type {};

        if(!(fields >> device >> escapedMountPoint >> type))
            continue;

        std::string mountPoint {};

        for(size_t index = 0; index < escapedMountPoint.size(); ++index)
        {
            const bool isEscape { '\\' == escapedMountPoint[index] && index + 3 < escapedMountPoint.size() };

            if(!isEscape)
            {
                mountPoint.push_back(escapedMountPoint[index]);
                continue;
            }

            mountPoint.push_back(static_cast<char>(std::stoi(escapedMountPoint.substr(index + 1, 3), nullptr, 8)));
            index += 3;
        }

        //Mounted volumes are what a user browses. the system's own mounts are left out
        const bool
            isMedia { mountPoint.starts_with("/media/") || mountPoint.starts_with("/run/media/") },
            isMnt { mountPoint.starts_with("/mnt/") },
            isNetwork { type == "nfs" || type == "nfs4" || type == "cifs" || type == "smb3" || type == "fuse.sshfs" };

        if(!isMedia && !isMnt && !isNetwork)
            continue;

        RootKind kind { RootKind::Other };

        if(isNetwork)
            kind = RootKind::Network;
        else if(isMedia)
            kind = RootKind::Removable;

        const std::filesystem::path path { mountPoint };
        const std::u8string label { path.filename().u8string() };

        _roots.push_back(Root
            {
                ._Path = path,
                ._Label = std::string(label.begin(), label.end()),
                ._Kind = kind
            });
    }
}

void PlatformBridge::FileSystem::refreshKnownFolders()
{
    _knownFolders.fill({});
    const char* home { std::getenv("HOME") };

    if(!home)
        return;

    const std::filesystem::path homePath { home };
    _knownFolders[static_cast<size_t>(KnownFolder::Home)] = homePath;

    //In the order of KnownFolder: folder's key and its name
    const std::array<std::pair<std::string_view, std::string_view>, _knownFolderCount> folderKeys
    {{
        { "", "" },
        { "XDG_DESKTOP_DIR", "Desktop" },
        { "XDG_DOCUMENTS_DIR", "Documents" },
        { "XDG_DOWNLOAD_DIR", "Downloads" },
        { "XDG_MUSIC_DIR", "Music" },
        { "XDG_PICTURES_DIR", "Pictures" },
        { "XDG_VIDEOS_DIR", "Videos" }
    }};

    const char* configHome { std::getenv("XDG_CONFIG_HOME") };
    const std::filesystem::path configPath { configHome && '\0' != *configHome ? std::filesystem::path(configHome) : homePath / ".config" };

    //Lines like XDG_MUSIC_DIR="$HOME/Music"
    std::ifstream userDirs { configPath / "user-dirs.dirs" };
    std::string line {};

    while(std::getline(userDirs, line))
    {
        const size_t equals { line.find('=') };

        if(line.starts_with('#') || std::string::npos == equals)
            continue;

        const std::string_view key { std::string_view(line).substr(0, equals) };
        std::string_view value { std::string_view(line).substr(equals + 1) };

        if(2 <= value.size() && '"' == value.front() && '"' == value.back())
            value = value.substr(1, value.size() - 2);

        //Set to home itself, a folder is turned off
        if(value == "$HOME" || value == "$HOME/")
            continue;

        const std::filesystem::path folder { value.starts_with("$HOME/") ? homePath / value.substr(6) : std::filesystem::path(value) };

        for(size_t index = 1; index < _knownFolderCount; ++index)
        {
            if(folderKeys[index].first == key)
                _knownFolders[index] = folder;
        }
    }

    for(size_t index = 1; index < _knownFolderCount; ++index)
    {
        if(!_knownFolders[index].empty())
            continue;

        const std::filesystem::path fallback { homePath / folderKeys[index].second };
        std::error_code error {};

        if(std::filesystem::is_directory(fallback, error))
            _knownFolders[index] = fallback;
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

    const int inotify { inotify_init1(IN_NONBLOCK | IN_CLOEXEC) };

    if(0 > inotify)
    {
        errorOut = std::error_code(errno, std::system_category());
        return false;
    }

    //IN_DELETE_SELF and IN_MOVE_SELF tell of the folder itself going away
    const int watch { inotify_add_watch(inotify, directory.c_str(), IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_MODIFY
        | IN_ATTRIB | IN_CLOSE_WRITE | IN_DELETE_SELF | IN_MOVE_SELF | IN_ONLYDIR) };

    if(0 > watch)
    {
        errorOut = std::error_code(errno, std::system_category());
        close(inotify);
        return false;
    }

    _inotify = inotify;
    _watch = watch;
    return true;
}

void PlatformBridge::FileSystem::DirectoryWatcher::Stop()
{
    if(0 > _inotify)
        return;

    close(_inotify); //closing the instance removes its watch
    _inotify = -1;
    _watch = -1;
}

bool PlatformBridge::FileSystem::DirectoryWatcher::HasChanged()
{
    if(0 > _inotify)
        return false;

    bool hasChanged { false };
    bool isFolderGone { false };
    alignas(inotify_event) char buffer[4096];

    //Every previous event is read, so the next call reports only later ones. Non-blocking: read fails with EAGAIN once empty.
    for(ssize_t length { read(_inotify, buffer, sizeof(buffer)) }; 0 < length; length = read(_inotify, buffer, sizeof(buffer)))
    {
        hasChanged = true;

        for(ssize_t offset { 0 }; offset < length;)
        {
            const inotify_event* event { reinterpret_cast<const inotify_event*>(buffer + offset) };

            if(0 != (event->mask & (IN_DELETE_SELF | IN_MOVE_SELF | IN_IGNORED)))
                isFolderGone = true;

            offset += static_cast<ssize_t>(sizeof(inotify_event) + event->len);
        }
    }

    if(isFolderGone)
        Stop();

    return hasChanged;
}

bool PlatformBridge::FileSystem::DirectoryWatcher::IsWatching() const
{
    return 0 <= _inotify;
}

std::filesystem::path PlatformBridge::FileSystem::trashDirectory()
{
    const char* dataHome { std::getenv("XDG_DATA_HOME") };

    if(dataHome && '/' == dataHome[0]) //ignore relative paths
        return std::filesystem::path(dataHome) / "Trash";

    const char* home { std::getenv("HOME") };

    if(!home)
        return {};

    return std::filesystem::path(home) / ".local" / "share" / "Trash";
}

std::string PlatformBridge::FileSystem::encodeTrashPath(const std::string& path)
{
    std::string encoded {};
    encoded.reserve(path.size());

    for(const char character : path)
    {
        const bool isKept { ('a' <= character && 'z' >= character) || ('A' <= character && 'Z' >= character) || ('0' <= character && '9' >= character)
            || std::string_view::npos != std::string_view("-._~/").find(character) };

        if(isKept)
            encoded += character;
        else
            encoded += std::format("%{:02X}", static_cast<unsigned char>(character));
    }

    return encoded;
}

PlatformBridge::FileSystem::TrashResult PlatformBridge::FileSystem::MoveToTrash(const std::filesystem::path& path, std::error_code& errorOut)
{
    errorOut.clear();
    const std::filesystem::path trash { trashDirectory() };

    if(trash.empty())
        return TrashResult::NotTrashable;

    std::filesystem::path fullPath { std::filesystem::absolute(path, errorOut).lexically_normal() };

    if(errorOut)
        return TrashResult::Failed;

    if(!fullPath.has_filename())
        fullPath = fullPath.parent_path();

    const std::filesystem::path
        filesDirectory { trash / "files" },
        infoDirectory { trash / "info" };
    std::filesystem::create_directories(filesDirectory, errorOut);

    if(!errorOut)
        std::filesystem::create_directories(infoDirectory, errorOut);

    if(errorOut)
        return TrashResult::Failed;

    //Only an item on the trash's own file system goes: anywhere else it would have to be copied
    struct stat
        itemStatus {},
        trashStatus {};

    if(0 != lstat(fullPath.c_str(), &itemStatus) || 0 != stat(filesDirectory.c_str(), &trashStatus))
    {
        errorOut = std::error_code(errno, std::generic_category());
        return TrashResult::Failed;
    }

    if(itemStatus.st_dev != trashStatus.st_dev)
        return TrashResult::NotTrashable;

    //The info file is made first and exclusively, so two trashings never take the same name
    constexpr uint32_t freeNameLimit { 10000 };
    const std::string name { fullPath.filename().string() };
    std::string trashName {};
    int infoFile { -1 };

    for(uint32_t number = 1; freeNameLimit > number && 0 > infoFile; ++number)
    {
        trashName = 1 == number ? name : std::format("{}.{}", name, number);
        infoFile = open((infoDirectory / (trashName + ".trashinfo")).c_str(), O_CREAT | O_EXCL | O_WRONLY, 0600);

        if(0 > infoFile && EEXIST != errno)
        {
            errorOut = std::error_code(errno, std::generic_category());
            return TrashResult::Failed;
        }
    }

    if(0 > infoFile)
    {
        errorOut = std::make_error_code(std::errc::file_exists);
        return TrashResult::Failed;
    }

    const std::time_t now { std::time(nullptr) };
    std::tm localNow {};
    localtime_r(&now, &localNow);
    std::array<char, 32> date {};
    std::strftime(date.data(), date.size(), "%Y-%m-%dT%H:%M:%S", &localNow);

    const std::string info { std::format("[Trash Info]\nPath={}\nDeletionDate={}\n", encodeTrashPath(fullPath.string()), date.data()) };
    const bool isWritten { static_cast<ssize_t>(info.size()) == write(infoFile, info.data(), info.size()) };
    close(infoFile);

    const std::filesystem::path infoPath { infoDirectory / (trashName + ".trashinfo") };

    if(!isWritten || 0 != rename(fullPath.c_str(), (filesDirectory / trashName).c_str()))
    {
        const int error { isWritten ? errno : EIO };
        unlink(infoPath.c_str()); //the item stays, so its info goes

        if(EXDEV == error)
            return TrashResult::NotTrashable;

        errorOut = std::error_code(error, std::generic_category());
        return TrashResult::Failed;
    }

    return TrashResult::Moved;
}

bool PlatformBridge::FileSystem::IsSameVolume(const std::filesystem::path& first, const std::filesystem::path& second)
{
    struct stat
        firstStatus {},
        secondStatus {};

    if(0 != stat(first.c_str(), &firstStatus) || 0 != stat(second.c_str(), &secondStatus))
        return true;

    return firstStatus.st_dev == secondStatus.st_dev;
}

std::string PlatformBridge::FileSystem::decodeTrashPath(std::string_view text)
{
    std::string decoded {};
    decoded.reserve(text.size());

    for(size_t index = 0; index < text.size(); ++index)
    {
        unsigned int byte { 0 };
        const bool isEscape { '%' == text[index] && index + 2 < text.size()
            && std::errc {} == std::from_chars(text.data() + index + 1, text.data() + index + 3, byte, 16).ec };

        if(!isEscape)
        {
            decoded += text[index];
            continue;
        }

        decoded += static_cast<char>(byte);
        index += 2;
    }

    return decoded;
}

bool PlatformBridge::FileSystem::RestoreFromTrash(const std::filesystem::path& originalPath, std::filesystem::path& restoredPathOut, std::error_code& errorOut)
{
    errorOut.clear();
    restoredPathOut.clear();
    const std::filesystem::path trash { trashDirectory() };
    std::filesystem::path fullPath { std::filesystem::absolute(originalPath, errorOut).lexically_normal() };

    if(errorOut)
        return false;

    if(!fullPath.has_filename())
        fullPath = fullPath.parent_path();

    //The info file naming this path that was written last; the dates sort as text
    std::filesystem::path newestInfo {};
    std::string newestDate {};
    std::error_code listError {};

    for(std::filesystem::directory_iterator iterator { trash / "info", listError }, end {}; !listError && iterator != end; iterator.increment(listError))
    {
        if(".trashinfo" != iterator->path().extension())
            continue;

        std::ifstream info { iterator->path() };
        std::string
            line {},
            path {},
            date {};

        while(std::getline(info, line))
        {
            if(line.starts_with("Path="))
                path = decodeTrashPath(std::string_view(line).substr(5));
            else if(line.starts_with("DeletionDate="))
                date = line.substr(13);
        }

        if(path != fullPath.string() || (!newestInfo.empty() && newestDate >= date))
            continue;

        newestInfo = iterator->path();
        newestDate = date;
    }

    if(newestInfo.empty())
    {
        errorOut = std::make_error_code(std::errc::no_such_file_or_directory);
        return false;
    }

    std::filesystem::create_directories(fullPath.parent_path(), errorOut); //a folder it was in that went too

    if(errorOut)
        return false;

    //A name taken meanwhile gets a free one
    const std::filesystem::path stored { trash / "files" / newestInfo.stem() };
    std::error_code statusError {};
    const std::filesystem::path target { FreeName(fullPath, std::filesystem::is_directory(std::filesystem::symlink_status(stored, statusError))) };
    std::filesystem::rename(stored, target, errorOut);

    if(errorOut)
        return false;

    std::error_code removeError {};
    std::filesystem::remove(newestInfo, removeError);
    restoredPathOut = target;
    return true;
}