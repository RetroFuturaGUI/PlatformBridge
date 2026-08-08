#include "Clipboard.hpp"
#include <Windows.h>
#include <algorithm>
#include <string>
#include <string_view>

namespace
{
    wchar_t
        leftMask { 0b1101100000000000 },
        rightMask { 0b1101110000000000 };

    [[maybe_unused]] std::u32string utf16ToUtf32(std::wstring_view input)
    {
        std::u32string utf32(input.size(), '\0');
        size_t count { 0 };

        for(size_t i = 0; i < input.size(); ++i)
        {
            if(input[i] < 0xD800)
            {
                utf32[i] = static_cast<uint32_t>(input[i]);
                ++count;
            }
            else if((input[i] & leftMask) == leftMask)
            {
                utf32[count] = (0x10000 | static_cast<uint32_t>(input[i] & ~leftMask)) << 10;
                utf32[count + 1] = (static_cast<uint32_t>(input[i + 1] & ~rightMask));
                count += 2;
            }
            else
                break;
        }

        utf32.resize(count);
        return utf32;
    }

   std::wstring utf32ToUtf16(std::u32string_view input)
   {
        std::wstring utf16(input.size() * 2, '\0');
        size_t count { 0 };

        for(size_t i = 0; i < input.size(); ++i)
        {
            if((input[i] >= 0x10000) && (input[i] < 0x110000))
            {
                utf16[count] = leftMask | static_cast<wchar_t>((input[i] - 0x10000) >> 10);
                utf16[count +1] = rightMask | static_cast<wchar_t>(((input[i] - 0x10000) & 0b1111111111));
                count += 2;
            }
            else if(input[i] < 0xD800)
            {
                utf16[count] = static_cast<wchar_t>(input[i]);
                ++count;
            }
            else
                break;
        }

        utf16.resize(count);
        return utf16;
   }
}

PlatformBridge::Clipboard::ClipboardErrorCode PlatformBridge::Clipboard::CopyToClipboard(const ClipboardDatatype type, const void* dataIn, const size_t size)
{
    if (!OpenClipboard(nullptr))
        return ClipboardErrorCode::OpenClipboardFailed;

    if(EmptyClipboard() == NULL)
        return ClipboardErrorCode::EmptyClipboardFailed;

    switch(type)
    {
        case ClipboardDatatype::Text:
        {
            std::u32string utf32(static_cast<const char32_t*>(dataIn), size / sizeof(char32_t));
            std::wstring utf16 = utf32ToUtf16(utf32);
            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (utf16.size() + 1) * sizeof(wchar_t));

            if (!hMem)
            {
                CloseClipboard();
                return ClipboardErrorCode::SetClipboardDataFailed;
            }

            wchar_t* pMem = static_cast<wchar_t*>(GlobalLock(hMem));

            if (pMem)
            {
                std::copy(utf16.begin(), utf16.end(), pMem);
                pMem[utf16.size()] = L'\0';
                GlobalUnlock(hMem);
                
                if(SetClipboardData(CF_UNICODETEXT, hMem) == NULL)
                {
                    GlobalFree(hMem);
                    CloseClipboard();
                    return ClipboardErrorCode::SetClipboardDataFailed;
                }
            }
        } break;
        default:
        [[unlikely]] return ClipboardErrorCode::UnsupportedDatatype;
    }

    if(CloseClipboard() == NULL)
        return ClipboardErrorCode::CloseClipboardFailed;

    return ClipboardErrorCode::Success;
}

PlatformBridge::Clipboard::ClipboardErrorCode PlatformBridge::Clipboard::PasteFromClipboard(const ClipboardDatatype type, void*& dataOut, size_t* sizeOut)
{
    if (!OpenClipboard(nullptr))
        return ClipboardErrorCode::OpenClipboardFailed;

    switch(type)
    {
        case ClipboardDatatype::Text:
        {
            if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) 
            {
                CloseClipboard();
                return ClipboardErrorCode::NoMatchingRequestedDatatype;
            }

            HANDLE hData = GetClipboardData(CF_UNICODETEXT);
            
            if (!hData) 
            {
                CloseClipboard();
                return ClipboardErrorCode::GetClipboardDataFailed;
            }

            wchar_t* text = static_cast<wchar_t*>(GlobalLock(hData));

            if (!text) 
            {
                CloseClipboard();
                return ClipboardErrorCode::ClipboardDataEmpty;
            }

            std::wstring_view textView(text);
            std::u32string utf32 = utf16ToUtf32(textView);
            _clipboardDataBuffer.assign(reinterpret_cast<const uint8_t*>(utf32.data()), reinterpret_cast<const uint8_t*>(utf32.data()) + utf32.size() * sizeof(char32_t));
            GlobalUnlock(hData);
            dataOut = _clipboardDataBuffer.data();
            *sizeOut = _clipboardDataBuffer.size();
        } break;
        default:
        [[unlikely]] return ClipboardErrorCode::UnsupportedDatatype;
    }

    if(CloseClipboard() == NULL)
        return ClipboardErrorCode::CloseClipboardFailed;

    return ClipboardErrorCode::Success;;
}

void PlatformBridge::Clipboard::ClearClipboardDataBuffer()
{
    _clipboardDataBuffer.clear();
}