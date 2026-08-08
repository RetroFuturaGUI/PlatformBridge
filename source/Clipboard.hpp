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
        
        static ClipboardErrorCode CopyToClipboard(const ClipboardDatatype type, const void* dataIn, const size_t size);
        static ClipboardErrorCode PasteFromClipboard(const ClipboardDatatype type, void*& dataOut, size_t* sizeOut);
        static void ClearClipboardDataBuffer();
        
    private:
        Clipboard(const Clipboard&) = delete;
        Clipboard(Clipboard&&) = delete;
        auto operator =(const Clipboard&) = delete;
        auto operator =(Clipboard&&) = delete;
        
        static inline std::vector<uint8_t> _clipboardDataBuffer {};
    };
}