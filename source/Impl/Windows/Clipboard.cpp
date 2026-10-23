#include "Clipboard.hpp"
#include <Windows.h>
#include <ShlObj.h>
#include <oleidl.h>
#include <algorithm>
#include <cstring>
#include <string>
#include <string_view>

namespace
{
    //A pair of surrogates becomes one code point; a surrogate without its partner becomes U+FFFD
    std::u32string utf16ToUtf32(std::wstring_view input)
    {
        std::u32string utf32 {};
        utf32.reserve(input.size());

        for(size_t index = 0; index < input.size(); ++index)
        {
            const char32_t unit { input[index] };
            const bool
                isHigh { 0xD800 <= unit && 0xDBFF >= unit },
                isLowNext { index + 1 < input.size() && 0xDC00 <= input[index + 1] && 0xDFFF >= input[index + 1] };

            if(isHigh && isLowNext)
            {
                utf32.push_back(0x10000 + ((unit - 0xD800) << 10) + (static_cast<char32_t>(input[index + 1]) - 0xDC00));
                ++index;
            }
            else if(0xD800 <= unit && 0xDFFF >= unit)
            {
                utf32.push_back(0xFFFD);
            }
            else
            {
                utf32.push_back(unit);
            }
        }

        return utf32;
    }

    //A code point past the first 65536 becomes a pair of surrogates; one that can't be written becomes U+FFFD
    std::wstring utf32ToUtf16(std::u32string_view input)
    {
        std::wstring utf16 {};
        utf16.reserve(input.size());

        for(const char32_t codePoint : input)
        {
            if(0x10000 <= codePoint && 0x110000 > codePoint)
            {
                utf16.push_back(static_cast<wchar_t>(0xD800 + ((codePoint - 0x10000) >> 10)));
                utf16.push_back(static_cast<wchar_t>(0xDC00 + ((codePoint - 0x10000) & 0x3FF)));
            }
            else if((0xD800 <= codePoint && 0xDFFF >= codePoint) || 0x110000 <= codePoint)
            {
                utf16.push_back(static_cast<wchar_t>(0xFFFD));
            }
            else
            {
                utf16.push_back(static_cast<wchar_t>(codePoint));
            }
        }

        return utf16;
    }

    //Windows Explorer's format telling a paste whether the files were copied or cut
    UINT preferredDropEffectFormat()
    {
        static const UINT format { RegisterClipboardFormatW(L"Preferred DropEffect") }; //CFSTR_PREFERREDDROPEFFECT, which is narrow without UNICODE
        return format;
    }
}

PlatformBridge::Clipboard::ClipboardErrorCode PlatformBridge::Clipboard::CopyToClipboard(const ClipboardDatatype type, const void* dataIn, const size_t size)
{
    if(ClipboardDatatype::Text != type)
        return ClipboardErrorCode::UnsupportedDatatype;

    if (!OpenClipboard(nullptr))
        return ClipboardErrorCode::OpenClipboardFailed;

    if(EmptyClipboard() == NULL)
    {
        CloseClipboard();
        return ClipboardErrorCode::EmptyClipboardFailed;
    }

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

    if(CloseClipboard() == NULL)
        return ClipboardErrorCode::CloseClipboardFailed;

    return ClipboardErrorCode::Success;
}

PlatformBridge::Clipboard::ClipboardErrorCode PlatformBridge::Clipboard::PasteFromClipboard(const ClipboardDatatype type, void*& dataOut, size_t* sizeOut)
{
    if(ClipboardDatatype::Text != type)
        return ClipboardErrorCode::UnsupportedDatatype;

    if (!OpenClipboard(nullptr))
        return ClipboardErrorCode::OpenClipboardFailed;

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

    if(CloseClipboard() == NULL)
        return ClipboardErrorCode::CloseClipboardFailed;

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

    //Whole paths, each ended by a null, the list by a second one
    std::wstring list {};

    for(const std::filesystem::path& path : paths)
    {
        std::error_code error {};
        list += std::filesystem::absolute(path, error).make_preferred().native();
        list += L'\0';
    }

    list += L'\0';

    //CF_HDROP is a DROPFILES header with the list after it; the system owns both blocks once they are set
    HGLOBAL files { GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(DROPFILES) + list.size() * sizeof(wchar_t)) };

    if(!files)
        return ClipboardErrorCode::SetClipboardDataFailed;

    DROPFILES* drop { static_cast<DROPFILES*>(GlobalLock(files)) };

    if(!drop)
    {
        GlobalFree(files);
        return ClipboardErrorCode::SetClipboardDataFailed;
    }

    drop->pFiles = sizeof(DROPFILES);
    drop->fWide = TRUE;
    std::memcpy(reinterpret_cast<std::byte*>(drop) + sizeof(DROPFILES), list.data(), list.size() * sizeof(wchar_t));
    GlobalUnlock(files);

    HGLOBAL effect { GlobalAlloc(GMEM_MOVEABLE, sizeof(DWORD)) };

    if(!effect)
    {
        GlobalFree(files);
        return ClipboardErrorCode::SetClipboardDataFailed;
    }

    DWORD* effectValue { static_cast<DWORD*>(GlobalLock(effect)) };

    if(!effectValue)
    {
        GlobalFree(files);
        GlobalFree(effect);
        return ClipboardErrorCode::SetClipboardDataFailed;
    }

    *effectValue = FileOperation::Cut == operation ? DROPEFFECT_MOVE : DROPEFFECT_COPY | DROPEFFECT_LINK; //as Windows Explorer writes them
    GlobalUnlock(effect);

    if(!OpenClipboard(nullptr))
    {
        GlobalFree(files);
        GlobalFree(effect);
        return ClipboardErrorCode::OpenClipboardFailed;
    }

    if(!EmptyClipboard())
    {
        CloseClipboard();
        GlobalFree(files);
        GlobalFree(effect);
        return ClipboardErrorCode::EmptyClipboardFailed;
    }

    if(!SetClipboardData(CF_HDROP, files))
    {
        CloseClipboard();
        GlobalFree(files);
        GlobalFree(effect);
        return ClipboardErrorCode::SetClipboardDataFailed;
    }

    //Without the effect a paste copies, so a cut that lost it is still pasted safely
    if(!SetClipboardData(preferredDropEffectFormat(), effect))
        GlobalFree(effect);

    if(!CloseClipboard())
        return ClipboardErrorCode::CloseClipboardFailed;

    return ClipboardErrorCode::Success;
}

PlatformBridge::Clipboard::ClipboardErrorCode PlatformBridge::Clipboard::PasteFilesFromClipboard(std::vector<std::filesystem::path>& pathsOut, FileOperation& operationOut)
{
    pathsOut.clear();
    operationOut = FileOperation::Copy;

    if(!OpenClipboard(nullptr))
        return ClipboardErrorCode::OpenClipboardFailed;

    HDROP drop { static_cast<HDROP>(GetClipboardData(CF_HDROP)) };

    if(!drop)
    {
        CloseClipboard();
        return ClipboardErrorCode::NoMatchingRequestedDatatype;
    }

    const UINT fileCount { DragQueryFileW(drop, 0xFFFFFFFF, nullptr, 0) };

    for(UINT index = 0; index < fileCount; ++index)
    {
        const UINT length { DragQueryFileW(drop, index, nullptr, 0) };
        std::wstring path(length, L'\0');
        DragQueryFileW(drop, index, path.data(), length + 1); //the string keeps room for its null
        pathsOut.emplace_back(std::move(path));
    }

    const HANDLE effect { GetClipboardData(preferredDropEffectFormat()) };
    const DWORD* effectValue { effect ? static_cast<const DWORD*>(GlobalLock(effect)) : nullptr };

    if(effectValue)
    {
        operationOut = 0 != (*effectValue & DROPEFFECT_MOVE) ? FileOperation::Cut : FileOperation::Copy;
        GlobalUnlock(effect);
    }

    if(!CloseClipboard())
        return ClipboardErrorCode::CloseClipboardFailed;

    return pathsOut.empty() ? ClipboardErrorCode::ClipboardDataEmpty : ClipboardErrorCode::Success;
}

bool PlatformBridge::Clipboard::HasFilesOnClipboard()
{
    return IsClipboardFormatAvailable(CF_HDROP);
}

void PlatformBridge::Clipboard::ClearCutFiles(const std::vector<std::filesystem::path>& paths)
{
    std::vector<std::filesystem::path> held {};
    FileOperation operation { FileOperation::Copy };

    if(ClipboardErrorCode::Success != PasteFilesFromClipboard(held, operation))
        return;

    //Cut something else meanwhile: that stays
    if(FileOperation::Cut != operation || held != paths)
        return;

    if(!OpenClipboard(nullptr))
        return;

    EmptyClipboard();
    CloseClipboard();
}
