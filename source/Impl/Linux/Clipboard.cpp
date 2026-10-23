#include "Clipboard.hpp"

//Until there is an X11 clipboard, text and files are copied and pasted only inside the app

PlatformBridge::Clipboard::ClipboardErrorCode PlatformBridge::Clipboard::CopyToClipboard(const ClipboardDatatype type, const void* dataIn, const size_t size)
{
    if(ClipboardDatatype::Text != type)
        return ClipboardErrorCode::UnsupportedDatatype;

    if(!dataIn)
        return ClipboardErrorCode::ClipboardDataEmpty;

    const uint8_t* bytes { static_cast<const uint8_t*>(dataIn) };
    _appText.assign(bytes, bytes + size);
    _appFiles.clear(); //the clipboard holds one thing at a time
    return ClipboardErrorCode::Success;
}

PlatformBridge::Clipboard::ClipboardErrorCode PlatformBridge::Clipboard::PasteFromClipboard(const ClipboardDatatype type, void*& dataOut, size_t* sizeOut)
{
    if(!sizeOut)
        return ClipboardErrorCode::GetClipboardDataFailed;

    if(ClipboardDatatype::Text != type)
        return ClipboardErrorCode::UnsupportedDatatype;

    if(_appText.empty())
        return ClipboardErrorCode::NoMatchingRequestedDatatype;

    _clipboardDataBuffer = _appText;
    dataOut = _clipboardDataBuffer.data();
    *sizeOut = _clipboardDataBuffer.size();
    return ClipboardErrorCode::Success;
}

void PlatformBridge::Clipboard::ClearClipboardDataBuffer()
{
    _clipboardDataBuffer.clear();
}

PlatformBridge::Clipboard::ClipboardErrorCode PlatformBridge::Clipboard::CopyFilesToClipboard(const std::vector<std::filesystem::path>& paths, const FileOperation operation)
{
    if(paths.empty())
        return ClipboardErrorCode::ClipboardDataEmpty;

    _appFiles.clear();

    for(const std::filesystem::path& path : paths)
    {
        std::error_code error {};
        _appFiles.push_back(std::filesystem::absolute(path, error));
    }

    _appFileOperation = operation;
    _appText.clear();
    return ClipboardErrorCode::Success;
}

PlatformBridge::Clipboard::ClipboardErrorCode PlatformBridge::Clipboard::PasteFilesFromClipboard(std::vector<std::filesystem::path>& pathsOut, FileOperation& operationOut)
{
    pathsOut = _appFiles;
    operationOut = _appFileOperation;
    return pathsOut.empty() ? ClipboardErrorCode::NoMatchingRequestedDatatype : ClipboardErrorCode::Success;
}

bool PlatformBridge::Clipboard::HasFilesOnClipboard()
{
    return !_appFiles.empty();
}

void PlatformBridge::Clipboard::ClearCutFiles(const std::vector<std::filesystem::path>& paths)
{
    if(FileOperation::Cut != _appFileOperation || _appFiles != paths)
        return;

    _appFiles.clear();
}
