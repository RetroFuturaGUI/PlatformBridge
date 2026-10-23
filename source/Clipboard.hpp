#pragma once
#include <cstdint>
#include <filesystem>
#include <vector>

namespace PlatformBridge
{
    class Clipboard
    {
    public:
        enum ClipboardDatatype : int32_t
        {
            Unknown = -1,
            Text = 0,
            Bitmap,
        };

        enum ClipboardErrorCode : uint32_t
        {
            Success = 0,
            OpenClipboardFailed,
            EmptyClipboardFailed,
            SetClipboardDataFailed,
            GetClipboardDataFailed,
            CloseClipboardFailed,
            NoMatchingRequestedDatatype,
            NoDataAvailable,
            UnsupportedDatatype,
            ClipboardDataEmpty
        };

        /// @brief What pasting files on the clipboard does with them.
        enum class FileOperation : uint32_t
        {
            Copy,
            Cut //moved where they are pasted
        };

        Clipboard() = default;
        ~Clipboard() = default;

        /// @brief Writes data of the given type to the system clipboard. Text data is expected to be UTF-32 encoded (char32_t).
        /// @param type The ClipboardDatatype describing how dataIn should be interpreted.
        /// @param dataIn Pointer to the raw data to copy to the clipboard.
        /// @param size Size of dataIn in bytes.
        /// @return ClipboardErrorCode::Success on success, or an error code describing what failed.
        static ClipboardErrorCode CopyToClipboard(const ClipboardDatatype type, const void* dataIn, const size_t size);

        /// @brief Reads data of the given type from the system clipboard into the internal data buffer. Text data is returned UTF-32 encoded (char32_t).
        /// @param type The ClipboardDatatype to request from the clipboard.
        /// @param dataOut Set to point at the internal data buffer. Valid until the next Clipboard call or ClearClipboardDataBuffer().
        /// @param sizeOut Set to the size of the returned data in bytes.
        /// @return ClipboardErrorCode::Success on success, or an error code describing what failed.
        static ClipboardErrorCode PasteFromClipboard(const ClipboardDatatype type, void*& dataOut, size_t* sizeOut);

        /// @brief Clears the internal buffer used to hold data retrieved via PasteFromClipboard().
        static void ClearClipboardDataBuffer();

        /// @brief Puts files and folders on the clipboard to be pasted, copied or moved. On Windows they go on the system
        ///        clipboard the way Windows Explorer puts them, so either side pastes what the other copied. Linux keeps them
        ///        inside the app for now.
        static ClipboardErrorCode CopyFilesToClipboard(const std::vector<std::filesystem::path>& paths, const FileOperation operation);

        /// @brief Reads the files and folders on the clipboard, and whether they were copied or cut.
        /// @return NoMatchingRequestedDatatype when the clipboard holds no files
        static ClipboardErrorCode PasteFilesFromClipboard(std::vector<std::filesystem::path>& pathsOut, FileOperation& operationOut);

        /// @brief Returns whether the clipboard holds files or folders.
        static bool HasFilesOnClipboard();

        /// @brief Empties the clipboard if it still holds exactly these files, cut: once they are moved, pasting them again
        ///        would find them gone.
        static void ClearCutFiles(const std::vector<std::filesystem::path>& paths);

    private:
        Clipboard(const Clipboard&) = delete;
        Clipboard(Clipboard&&) = delete;
        auto operator =(const Clipboard&) = delete;
        auto operator =(Clipboard&&) = delete;

        static inline std::vector<uint8_t> _clipboardDataBuffer {};
#ifdef __linux__
        //The app's own clipboard until there is an X11 one
        static inline std::vector<uint8_t> _appText {};
        static inline std::vector<std::filesystem::path> _appFiles {};
        static inline FileOperation _appFileOperation { FileOperation::Copy };
#endif
    };
}