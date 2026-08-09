#pragma once
#include <cstdint>
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
        
    private:
        Clipboard(const Clipboard&) = delete;
        Clipboard(Clipboard&&) = delete;
        auto operator =(const Clipboard&) = delete;
        auto operator =(Clipboard&&) = delete;
        
        static inline std::vector<uint8_t> _clipboardDataBuffer {};
    };
}