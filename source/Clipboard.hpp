#pragma once
#include <cstdint>

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

        Clipboard() = default;
        ~Clipboard() = default;
        
        static void CopyToClipboard(const ClipboardDatatype type, const void* data, const size_t size);
        
    private:
        Clipboard(const Clipboard&) = delete;
        Clipboard(Clipboard&&) = delete;
        auto operator =(const Clipboard&) = delete;
        auto operator =(Clipboard&&) = delete;
        
    };
}