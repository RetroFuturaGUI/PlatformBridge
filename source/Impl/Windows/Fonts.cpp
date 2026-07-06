#include "Fonts.hpp"
#include <Windows.h>
#include <gdiplus.h>
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <optional>
#include <string_view>
#include <vector>
#pragma comment(lib, "gdiplus.lib")

namespace
{
    std::string toLowerAscii(std::string_view text)
    {
        std::string lowered;
        lowered.reserve(text.size());

        for (const unsigned char ch : text)
            lowered.push_back(static_cast<char>(std::tolower(ch)));

        return lowered;
    }

    std::string decodeNameString(const std::vector<std::uint8_t>& bytes, std::size_t offset, std::uint16_t length, std::uint16_t platformId, std::uint16_t encodingId)
    {
        if (offset + length > bytes.size())
            return {};

        if (platformId == 3 && (encodingId == 1 || encodingId == 10))
        {
            std::string decoded;
            decoded.reserve(length / 2);

            for (std::size_t i = 0; i + 1 < length; i += 2)
            {
                const std::uint16_t codeUnit = static_cast<std::uint16_t>((bytes[offset + i] << 8) | bytes[offset + i + 1]);
                if (codeUnit == 0)
                    break;
                if (codeUnit <= 0x7F)
                    decoded.push_back(static_cast<char>(codeUnit));
            }

            return decoded;
        }

        return std::string(reinterpret_cast<const char*>(bytes.data() + offset), length);
    }

    std::optional<std::string> getFontSubfamilyName(const std::filesystem::path& fontPath)
    {
        std::ifstream input(fontPath, std::ios::binary);
        if (!input)
            return std::nullopt;

        std::vector<std::uint8_t> header(12);
        if (!input.read(reinterpret_cast<char*>(header.data()), static_cast<std::streamsize>(header.size())))
            return std::nullopt;

        const auto readU16Be = [&](const std::vector<std::uint8_t>& bytes, std::size_t offset) -> std::uint16_t
        {
            return static_cast<std::uint16_t>((bytes[offset] << 8) | bytes[offset + 1]);
        };

        const std::uint16_t numTables = static_cast<std::uint16_t>((header[4] << 8) | header[5]);

        for (std::uint16_t tableIndex = 0; tableIndex < numTables; ++tableIndex)
        {
            std::vector<std::uint8_t> tableEntry(16);
            if (!input.read(reinterpret_cast<char*>(tableEntry.data()), static_cast<std::streamsize>(tableEntry.size())))
                return std::nullopt;

            const std::string tag(reinterpret_cast<const char*>(tableEntry.data()), 4);
            if (tag != "name")
                continue;

            const std::uint32_t tableOffset = static_cast<std::uint32_t>((tableEntry[8] << 24) | (tableEntry[9] << 16) |
                (tableEntry[10] << 8) | tableEntry[11]);
            const std::uint32_t tableLength = static_cast<std::uint32_t>((tableEntry[12] << 24) | (tableEntry[13] << 16) |
                (tableEntry[14] << 8) | tableEntry[15]);

            std::vector<std::uint8_t> nameTableBytes(tableLength);
            input.clear();
            input.seekg(tableOffset, std::ios::beg);
            if (!input.read(reinterpret_cast<char*>(nameTableBytes.data()), nameTableBytes.size()))
                return std::nullopt;

            const std::uint16_t format = readU16Be(nameTableBytes, 0);
            const std::uint16_t count = readU16Be(nameTableBytes, 2);
            const std::uint16_t stringOffset = readU16Be(nameTableBytes, 4);

            for (std::uint16_t recordIndex = 0; recordIndex < count; ++recordIndex)
            {
                const std::size_t recordOffset = 6 + recordIndex * 12;
                const std::uint16_t platformId = readU16Be(nameTableBytes, recordOffset);
                const std::uint16_t encodingId = readU16Be(nameTableBytes, recordOffset + 2);
                const std::uint16_t languageId = readU16Be(nameTableBytes, recordOffset + 4);
                const std::uint16_t nameId = readU16Be(nameTableBytes, recordOffset + 6);
                const std::uint16_t length = readU16Be(nameTableBytes, recordOffset + 8);
                const std::uint16_t offset = readU16Be(nameTableBytes, recordOffset + 10);

                (void)languageId;
                if (nameId != 2)
                    continue;

                const std::size_t stringOffsetValue = stringOffset + offset;
                const std::string decoded = decodeNameString(nameTableBytes, stringOffsetValue, length, platformId, encodingId);
                if (!decoded.empty())
                    return decoded;
            }

            if (format == 1)
                return std::nullopt;

            break;
        }

        return std::nullopt;
    }

    PlatformBridge::Fonts::Slant getSlantValueFromFontFile(const std::filesystem::path& fontPath)
    {
        const auto subfamily = getFontSubfamilyName(fontPath);
        if (!subfamily)
            return PlatformBridge::Fonts::Slant::Roman;

        const std::string lowered = toLowerAscii(*subfamily);
        if (lowered.find("oblique") != std::string::npos)
            return PlatformBridge::Fonts::Slant::Oblique;

        if (lowered.find("italic") != std::string::npos)
            return PlatformBridge::Fonts::Slant::Italic;

        return PlatformBridge::Fonts::Slant::Roman;
    }
}

void PlatformBridge::Fonts::setFontsInformation()
{
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);





    _fontProperties.clear();

    HKEY hKey;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    {
        std::println("ERROR: Could not open Fonts registry key");
        return;
    }

    wchar_t valueName[256];
    wchar_t fontPath[MAX_PATH];
    DWORD valueNameSize = 0;
    DWORD fontPathSize = 0;
    DWORD index = 0;
    std::filesystem::path fullPath;
    const std::wstring fontFolderPath = L"C:\\Windows\\Fonts\\";
    
    while (RegEnumValueW(hKey, index, valueName, &(valueNameSize = sizeof(valueName) / sizeof(wchar_t)), NULL, NULL, (LPBYTE)fontPath, &(fontPathSize = sizeof(fontPath))) == ERROR_SUCCESS)
    {
        //std::wstring fontFile(fontPath);
        std::wstring test(fontPath);
        std::wstring fontFile = fontFolderPath + test;
        fullPath = fontFile;




        Gdiplus::PrivateFontCollection fontCollection;
        if (fontCollection.AddFontFile(fontFile.c_str()) != Gdiplus::Ok)
        {
            ++index;
            continue;
        }

        INT numFamilies = fontCollection.GetFamilyCount();
        if (numFamilies <= 0) {
            std::println("No font families found in {}", fullPath.string());
            ++index;
            continue;
        }

        std::wstring familyNameW(valueName, valueNameSize);
        std::string familyNameA(familyNameW.begin(), familyNameW.end());
        familyNameA.resize(familyNameA.find_last_of(" (")); // cut off "(TrueType)" or similar suffixes
        // Print results
        std::println("Font: {}", familyNameA);
        std::println("Path: {}", fullPath.string());
        //std::println("Weight: {}", weight);
        //std::println("Italic: {}", (isItalic ? "Yes" : "No"));
        //std::println("Bold: {}", (isBold ? "Yes" : "No"));

        std::println("--------------------------------------------------");







        const auto slant = getSlantValueFromFontFile(fullPath);

        if (std::filesystem::exists(fullPath))
            _fontProperties.push_back(
                {
                    ._Name = familyNameA,
                    ._Path = fullPath.string(),
                    ._Style = "",
                    ._Weight = Weight::Normal,
                    ._Slant = slant,
                    ._UnicodeRanges = {}
                }
            );

        ++index;
    }

    RegCloseKey(hKey);
    // Cleanup
    Gdiplus::GdiplusShutdown(gdiplusToken);
}