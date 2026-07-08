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

    std::string normalizeFontFamilyName(std::string_view familyName)
    {
        std::string trimmed { familyName };
        const auto openParentheses = trimmed.find_first_of("(");
        if (openParentheses != std::string::npos)
            trimmed.resize(openParentheses);

        while (!trimmed.empty() && std::isspace(static_cast<unsigned char>(trimmed.back())))
            trimmed.pop_back();

        return trimmed;
    }

    std::string decodeNameString(const std::vector<uint8_t>& bytes, const size_t offset, const size_t length, const uint16_t platformId, const uint16_t encodingId)
    {
        if (offset + length > bytes.size())
            return {};

        if (platformId == 3 && (encodingId == 1 || encodingId == 10))
        {
            std::string decoded {};
            decoded.reserve(length / 2);

            for (size_t i = 0; i + 1 < length; i += 2)
            {
                const uint16_t codeUnit { static_cast<uint16_t>((bytes[offset + i] << 8) | bytes[offset + i + 1]) };

                if (codeUnit == 0)
                    break;

                if (codeUnit <= 0x7F)
                    decoded.push_back(static_cast<char>(codeUnit));
            }

            return decoded;
        }

        return std::string(reinterpret_cast<const char*>(bytes.data() + offset), length);
    }

    struct FontMetadata
    {
        std::string subfamily {};
        PlatformBridge::Fonts::Slant slant { PlatformBridge::Fonts::Slant::Roman };
        PlatformBridge::Fonts::Weight weight { PlatformBridge::Fonts::Weight::Normal };
        std::vector<std::pair<uint32_t, uint32_t>> unicodeRanges {};
    };

    PlatformBridge::Fonts::Slant getSlantFromSubfamily(std::string_view subfamily)
    {
        const std::string lowered = toLowerAscii(subfamily);

        if (lowered.find("oblique") != std::string::npos)
            return PlatformBridge::Fonts::Slant::Oblique;

        if (lowered.find("italic") != std::string::npos)
            return PlatformBridge::Fonts::Slant::Italic;

        return PlatformBridge::Fonts::Slant::Roman;
    }

    PlatformBridge::Fonts::Weight getWeightFromSubfamily(std::string_view subfamily)
    {
        const std::string lowered = toLowerAscii(subfamily);

        if (lowered.find("thin") != std::string::npos)
            return PlatformBridge::Fonts::Weight::Thin;

        if (lowered.find("extra light") != std::string::npos || lowered.find("ultralight") != std::string::npos)
            return PlatformBridge::Fonts::Weight::ExtraLight;

        if (lowered.find("light") != std::string::npos)
            return PlatformBridge::Fonts::Weight::Light;

        if (lowered.find("medium") != std::string::npos)
            return PlatformBridge::Fonts::Weight::Medium;

        if (lowered.find("semibold") != std::string::npos || lowered.find("demibold") != std::string::npos)
            return PlatformBridge::Fonts::Weight::SemiBold;

        if (lowered.find("bold") != std::string::npos)
            return PlatformBridge::Fonts::Weight::Bold;

        if (lowered.find("extrabold") != std::string::npos || lowered.find("ultrabold") != std::string::npos)
            return PlatformBridge::Fonts::Weight::ExtraBold;

        if (lowered.find("black") != std::string::npos || lowered.find("heavy") != std::string::npos)
            return PlatformBridge::Fonts::Weight::Black;

        return PlatformBridge::Fonts::Weight::Normal;
    }

    std::optional<std::vector<uint8_t>> readTableBytes(std::ifstream& input, const uint32_t offset, const uint32_t length)
    {
        std::vector<uint8_t> bytes(length);
        input.clear();
        input.seekg(offset, std::ios::beg);

        if (!input.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(length)))
            return std::nullopt;

        return bytes;
    }

    std::vector<std::pair<uint32_t, uint32_t>> getCodepointCoverage(const std::vector<uint8_t>& cmapTableBytes)
    {
        if (cmapTableBytes.size() < 8)
            return {};

        const auto readU16Be = [&](const std::vector<uint8_t>& bytes, size_t offset) -> uint16_t
        {
            if (offset + 2 > bytes.size())
                return 0;

            return static_cast<uint16_t>((bytes[offset] << 8) | bytes[offset + 1]);
        };

        const auto readU32Be = [&](const std::vector<uint8_t>& bytes, size_t offset) -> uint32_t
        {
            if (offset + 4 > bytes.size())
                return 0;

            return (static_cast<uint32_t>(bytes[offset]) << 24) |
                (static_cast<uint32_t>(bytes[offset + 1]) << 16) |
                (static_cast<uint32_t>(bytes[offset + 2]) << 8) |
                static_cast<uint32_t>(bytes[offset + 3]);
        };

        const uint16_t numSubtables { readU16Be(cmapTableBytes, 2) };
        std::vector<std::pair<uint32_t, uint32_t>> ranges {};

        for (size_t subtableIndex { 0 }; subtableIndex < numSubtables; ++subtableIndex)
        {
            const size_t subtableOffset { 4 + subtableIndex * 8 };
            const uint16_t
                platformId { readU16Be(cmapTableBytes, subtableOffset) },
                encodingId { readU16Be(cmapTableBytes, subtableOffset + 2) };
            const uint32_t subtableOffsetValue { readU32Be(cmapTableBytes, subtableOffset + 4) };

            if (platformId == 3 && (encodingId == 1 || encodingId == 10))
            {
                const size_t dataOffset { subtableOffsetValue };
                const uint16_t format { readU16Be(cmapTableBytes, dataOffset) };

                if (format == 4)
                {
                    const size_t 
                        segCountX2 { readU16Be(cmapTableBytes, dataOffset + 6) },
                        segCount { segCountX2 / 2 },
                        endCodeOffset { dataOffset + 14 },
                        startCodeOffset { endCodeOffset + segCount * 2 + 2 };

                    for (size_t segIndex = 0; segIndex < segCount; ++segIndex)
                    {
                        const uint16_t
                            endCode { readU16Be(cmapTableBytes, endCodeOffset + segIndex * 2) },
                            startCode { readU16Be(cmapTableBytes, startCodeOffset + segIndex * 2) };

                        if (endCode == 0xFFFF || startCode == 0)
                            continue;

                        ranges.emplace_back(startCode, endCode);
                    }
                    return ranges;
                }
            }
        }

        return {};
    }

    std::optional<FontMetadata> getFontMetadata(const std::filesystem::path& fontPath)
    {
        std::ifstream input(fontPath, std::ios::binary);

        if (!input)
            return std::nullopt;

        std::vector<uint8_t> header(12);

        if (!input.read(reinterpret_cast<char*>(header.data()), static_cast<std::streamsize>(header.size())))
            return std::nullopt;

        const auto readU16Be = [&](const std::vector<uint8_t>& bytes, size_t offset) -> uint16_t
        {
            if (offset + 2 > bytes.size())
                return 0;

            return static_cast<uint16_t>((bytes[offset] << 8) | bytes[offset + 1]);
        };

        const auto readU32Be = [&](const std::vector<uint8_t>& bytes, size_t offset) -> uint32_t
        {
            if (offset + 4 > bytes.size())
                return 0;

            return (static_cast<uint32_t>(bytes[offset]) << 24) |
                (static_cast<uint32_t>(bytes[offset + 1]) << 16) |
                (static_cast<uint32_t>(bytes[offset + 2]) << 8) |
                static_cast<uint32_t>(bytes[offset + 3]);
        };

        const size_t numTables { static_cast<size_t>((header[4] << 8) | header[5]) };
        std::vector<uint8_t> tableDirectoryBytes(numTables * 16);

        if (!input.read(reinterpret_cast<char*>(tableDirectoryBytes.data()), static_cast<std::streamsize>(tableDirectoryBytes.size())))
            return std::nullopt;

        std::optional<std::vector<uint8_t>>
            nameTableBytes {},
            cmapTableBytes {};

        for (size_t tableIndex { 0 }; tableIndex < numTables; ++tableIndex)
        {
            const size_t tableEntryOffset { tableIndex * 16 };
            const std::string tag(reinterpret_cast<const char*>(tableDirectoryBytes.data() + tableEntryOffset), 4);

            if (tag == "name")
            {
                const uint32_t tableOffset { readU32Be(tableDirectoryBytes, tableEntryOffset + 8) };
                const uint32_t tableLength { readU32Be(tableDirectoryBytes, tableEntryOffset + 12) };
                nameTableBytes = readTableBytes(input, tableOffset, tableLength);
                if (!nameTableBytes)
                    return std::nullopt;
            }
            else if (tag == "cmap")
            {
                const uint32_t tableOffset { readU32Be(tableDirectoryBytes, tableEntryOffset + 8) };
                const uint32_t tableLength { readU32Be(tableDirectoryBytes, tableEntryOffset + 12) };
                cmapTableBytes = readTableBytes(input, tableOffset, tableLength);
                if (!cmapTableBytes)
                    return std::nullopt;
            }
        }

        if (!nameTableBytes)
            return std::nullopt;

        const uint16_t format { readU16Be(*nameTableBytes, 0) };
        const size_t 
            count { readU16Be(*nameTableBytes, 2) },
            stringOffset { readU16Be(*nameTableBytes, 4) };

        for (size_t recordIndex { 0 }; recordIndex < count; ++recordIndex)
        {
            const size_t 
                recordOffset { 6 + recordIndex * 12 },
                length { readU16Be(*nameTableBytes, recordOffset + 8) };
            const uint16_t
                platformId { readU16Be(*nameTableBytes, recordOffset) },
                encodingId { readU16Be(*nameTableBytes, recordOffset + 2) },
                nameId { readU16Be(*nameTableBytes, recordOffset + 6) },
                offset { readU16Be(*nameTableBytes, recordOffset + 10) };

            if (nameId != 2)
                continue;

            const size_t stringOffsetValue { stringOffset + static_cast<size_t>(offset) };
            const std::string decoded { decodeNameString(*nameTableBytes, stringOffsetValue, length, platformId, encodingId) };

            if (!decoded.empty())
            {
                FontMetadata metadata;
                metadata.subfamily = decoded;
                metadata.slant = getSlantFromSubfamily(metadata.subfamily);
                metadata.weight = getWeightFromSubfamily(metadata.subfamily);
                metadata.unicodeRanges = cmapTableBytes ? getCodepointCoverage(*cmapTableBytes) : std::vector<std::pair<uint32_t, uint32_t>>{};
                return metadata;
            }
        }

        if (format == 1)
            return std::nullopt;

        return std::nullopt;
    }
}

void PlatformBridge::Fonts::setFontsInformation()
{
    _fontProperties.clear();
    ULONG_PTR gdiplusToken { 0 };
    Gdiplus::GdiplusStartupInput gdiplusStartupInput {};
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    HKEY hKey { nullptr };

    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    {
        std::println("ERROR: Could not open Fonts registry key");
        return;
    }

    wchar_t 
        valueName[256],
        fontPath[MAX_PATH];
    DWORD
        valueNameSize { 0 },
        fontPathSize { 0 },
        index { 0 };
    std::filesystem::path fullPath {};
    const std::wstring fontFolderPath { L"C:\\Windows\\Fonts\\" };
    
    while (RegEnumValueW(hKey, index, valueName, &(valueNameSize = sizeof(valueName) / sizeof(wchar_t)), NULL, NULL, (LPBYTE)fontPath, &(fontPathSize = sizeof(fontPath))) == ERROR_SUCCESS)
    {
        std::wstring fontFile { fontFolderPath + fontPath };
        fullPath = fontFile;
        Gdiplus::PrivateFontCollection fontCollection;

        if(fontFile.find(L".ttf") == std::wstring::npos && fontFile.find(L".otf") == std::wstring::npos && fontFile.find(L".ttc") == std::wstring::npos)
        {
            ++index;
            continue;
        }

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

        auto metadata { getFontMetadata(fullPath) };
        const auto slant { metadata ? metadata->slant : PlatformBridge::Fonts::Slant::Roman };
        const auto weight { metadata ? metadata->weight : PlatformBridge::Fonts::Weight::Normal };

        std::wstring familyNameW(valueName, valueNameSize);
__pragma(warning(push)) \
__pragma(warning(disable: 4244)) \
        std::string familyNameA(familyNameW.begin(), familyNameW.end());
__pragma(warning(pop))
        familyNameA = normalizeFontFamilyName(familyNameA);
        // Print results
       /* std::println("Font: {}", familyNameA);
        std::println("Path: {}", fullPath.string());
        std::println("Weight: {}", (uint32_t)weight);
        std::println("Slant: {}", (uint32_t)slant);
        std::println("Subfamily: {}", metadata ? metadata->subfamily : "Unknown");
        std::println("Unicode Ranges:");
        for (const auto& range : metadata ? metadata->unicodeRanges : std::vector<std::pair<uint32_t, uint32_t>>())
        {
            std::println("  - U+{:04X}-U+{:04X}", range.first, range.second);
        }
        std::println("--------------------------------------------------");*/

        if (std::filesystem::exists(fullPath))
            _fontProperties.push_back(
                {
                    ._Name = familyNameA,
                    ._Path = fullPath.string(),
                    ._Style = metadata ? metadata->subfamily : "Unknown",
                    ._Weight = weight,
                    ._Slant = slant,
                    ._UnicodeRanges = metadata ? metadata->unicodeRanges : std::vector<std::pair<uint32_t, uint32_t>>()
                }
            );

        ++index;
    }

    RegCloseKey(hKey);
    Gdiplus::GdiplusShutdown(gdiplusToken);
}