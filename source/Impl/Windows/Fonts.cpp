#include "Fonts.hpp"
#include <Windows.h>
#include <algorithm>
#include <atomic>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <optional>
#include <string_view>
#include <thread>
#include <vector>

namespace
{
    struct FontFace
    {
        std::string
            family {},
            legacyFamily {}, //the older name some programs know the font family by ("Segoe UI Semibold" for Segoe UI at 600)
            subfamily {};
        PlatformBridge::Fonts::Slant slant { PlatformBridge::Fonts::Slant::Roman };
        PlatformBridge::Fonts::Weight weight { PlatformBridge::Fonts::Weight::Normal };
        uint32_t faceIndex { 0 };
    };

    std::string toLowerAscii(std::string_view text)
    {
        std::string lowered;
        lowered.reserve(text.size());

        for (const unsigned char ch : text)
            lowered.push_back(static_cast<char>(std::tolower(ch)));

        return lowered;
    }

    uint16_t readU16Be(const std::vector<uint8_t>& bytes, const size_t offset)
    {
        if (offset + 2 > bytes.size())
            return 0;

        return static_cast<uint16_t>((bytes[offset] << 8) | bytes[offset + 1]);
    }

    uint32_t readU32Be(const std::vector<uint8_t>& bytes, const size_t offset)
    {
        if (offset + 4 > bytes.size())
            return 0;

        return (static_cast<uint32_t>(bytes[offset]) << 24) |
            (static_cast<uint32_t>(bytes[offset + 1]) << 16) |
            (static_cast<uint32_t>(bytes[offset + 2]) << 8) |
            static_cast<uint32_t>(bytes[offset + 3]);
    }

    //A name record as UTF-8: Windows names are UTF-16 big endian, Mac Roman ones are kept as they are
    std::string decodeNameString(const std::vector<uint8_t>& bytes, const size_t offset, const size_t length, const uint16_t platformId)
    {
        if (offset + length > bytes.size())
            return {};

        if (3 != platformId)
            return std::string(reinterpret_cast<const char*>(bytes.data() + offset), length);

        std::wstring wide {};
        wide.reserve(length / 2);

        for (size_t i { 0 }; i + 1 < length; i += 2)
            wide.push_back(static_cast<wchar_t>((bytes[offset + i] << 8) | bytes[offset + i + 1]));

        const int32_t utf8Length { WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), nullptr, 0, nullptr, nullptr) };

        if (0 >= utf8Length)
            return {};

        std::string utf8(static_cast<size_t>(utf8Length), '\0');
        WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), utf8.data(), utf8Length, nullptr, nullptr);
        return utf8;
    }

    //The name with the given ID, in US English if the font has it, else in the first Windows language, else the Mac name
    std::string readNameString(const std::vector<uint8_t>& nameTable, const uint16_t wantedNameId)
    {
        const size_t
            count { readU16Be(nameTable, 2) },
            stringOffset { readU16Be(nameTable, 4) };
        std::string
            english {},
            otherWindows {},
            mac {};

        for (size_t recordIndex { 0 }; recordIndex < count; ++recordIndex)
        {
            const size_t recordOffset { 6 + recordIndex * 12 };
            const uint16_t
                platformId { readU16Be(nameTable, recordOffset) },
                encodingId { readU16Be(nameTable, recordOffset + 2) },
                languageId { readU16Be(nameTable, recordOffset + 4) },
                nameId { readU16Be(nameTable, recordOffset + 6) },
                length { readU16Be(nameTable, recordOffset + 8) },
                offset { readU16Be(nameTable, recordOffset + 10) };

            if (wantedNameId != nameId)
                continue;

            const size_t start { stringOffset + offset };

            if (3 == platformId && (1 == encodingId || 10 == encodingId))
            {
                if (0x0409 == languageId)
                    english = decodeNameString(nameTable, start, length, platformId);
                else if (otherWindows.empty())
                    otherWindows = decodeNameString(nameTable, start, length, platformId);
            }
            else if (1 == platformId && 0 == encodingId && mac.empty())
                mac = decodeNameString(nameTable, start, length, platformId);
        }

        if (!english.empty())
            return english;

        return otherWindows.empty() ? mac : otherWindows;
    }

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

        if (lowered.find("extra light") != std::string::npos || lowered.find("extralight") != std::string::npos || lowered.find("ultralight") != std::string::npos)
            return PlatformBridge::Fonts::Weight::ExtraLight;

        if (lowered.find("light") != std::string::npos)
            return PlatformBridge::Fonts::Weight::Light;

        if (lowered.find("medium") != std::string::npos)
            return PlatformBridge::Fonts::Weight::Medium;

        if (lowered.find("semibold") != std::string::npos || lowered.find("demibold") != std::string::npos)
            return PlatformBridge::Fonts::Weight::SemiBold;

        if (lowered.find("extrabold") != std::string::npos || lowered.find("extra bold") != std::string::npos || lowered.find("ultrabold") != std::string::npos)
            return PlatformBridge::Fonts::Weight::ExtraBold;

        if (lowered.find("bold") != std::string::npos)
            return PlatformBridge::Fonts::Weight::Bold;

        if (lowered.find("black") != std::string::npos || lowered.find("heavy") != std::string::npos)
            return PlatformBridge::Fonts::Weight::Black;

        return PlatformBridge::Fonts::Weight::Normal;
    }

    //The OS/2 table's weight class, to the nearest hundred. Some old-ass fonts count 1 to 9 instead!
    PlatformBridge::Fonts::Weight getWeightFromClass(uint16_t weightClass)
    {
        if (10 > weightClass)
            weightClass = static_cast<uint16_t>(weightClass * 100);

        const int32_t rounded { std::clamp((static_cast<int32_t>(weightClass) + 50) / 100 * 100, 100, 900) };
        return static_cast<PlatformBridge::Fonts::Weight>(rounded);
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

    //The code-points a Windows Unicode cmap covers: format 4 for the first 65536, format 12 for all of them (emoji included)
    std::vector<std::pair<uint32_t, uint32_t>> getCodepointCoverage(const std::vector<uint8_t>& cmapTableBytes)
    {
        if (cmapTableBytes.size() < 8)
            return {};

        const uint16_t numSubtables { readU16Be(cmapTableBytes, 2) };
        std::vector<std::pair<uint32_t, uint32_t>> ranges {};

        for (size_t subtableIndex { 0 }; subtableIndex < numSubtables; ++subtableIndex)
        {
            const size_t subtableOffset { 4 + subtableIndex * 8 };
            const uint16_t
                platformId { readU16Be(cmapTableBytes, subtableOffset) },
                encodingId { readU16Be(cmapTableBytes, subtableOffset + 2) };
            const size_t dataOffset { readU32Be(cmapTableBytes, subtableOffset + 4) };

            if (3 != platformId || 10 != encodingId || 12 != readU16Be(cmapTableBytes, dataOffset))
                continue;

            const uint32_t groupCount { readU32Be(cmapTableBytes, dataOffset + 12) };
            ranges.reserve(std::min<size_t>(groupCount, cmapTableBytes.size() / 12));

            for (size_t groupIndex { 0 }; groupIndex < groupCount; ++groupIndex)
            {
                const size_t groupOffset { dataOffset + 16 + groupIndex * 12 };
                const uint32_t
                    first { readU32Be(cmapTableBytes, groupOffset) },
                    last { readU32Be(cmapTableBytes, groupOffset + 4) };

                if (!ranges.empty() && ranges.back().second + 1 == first)
                {
                    ranges.back().second = last; //continues the previous group: the glyphs are only numbered apart
                    continue;
                }

                ranges.emplace_back(first, last);
            }

            return ranges;
        }

        for (size_t subtableIndex { 0 }; subtableIndex < numSubtables; ++subtableIndex)
        {
            const size_t subtableOffset { 4 + subtableIndex * 8 };
            const uint16_t
                platformId { readU16Be(cmapTableBytes, subtableOffset) },
                encodingId { readU16Be(cmapTableBytes, subtableOffset + 2) };
            const size_t dataOffset { readU32Be(cmapTableBytes, subtableOffset + 4) };

            if (3 != platformId || 1 != encodingId || 4 != readU16Be(cmapTableBytes, dataOffset))
                continue;

            const size_t
                segCountX2 { readU16Be(cmapTableBytes, dataOffset + 6) },
                segCount { segCountX2 / 2 },
                endCodeOffset { dataOffset + 14 },
                startCodeOffset { endCodeOffset + segCount * 2 + 2 };

            for (size_t segIndex { 0 }; segIndex < segCount; ++segIndex)
            {
                const uint16_t
                    endCode { readU16Be(cmapTableBytes, endCodeOffset + segIndex * 2) },
                    startCode { readU16Be(cmapTableBytes, startCodeOffset + segIndex * 2) };

                if (endCode == 0xFFFF || startCode == 0)
                    continue;

                if (!ranges.empty() && ranges.back().second + 1 == startCode)
                {
                    ranges.back().second = endCode;
                    continue;
                }

                ranges.emplace_back(startCode, endCode);
            }

            return ranges;
        }

        return {};
    }

    //One face of a font file, from its table directory at faceOffset: its names and style
    std::optional<FontFace> readFontFace(std::ifstream& input, const uint32_t faceOffset, const uint32_t faceIndex)
    {
        const std::optional<std::vector<uint8_t>> header { readTableBytes(input, faceOffset, 12) };

        if (!header)
            return std::nullopt;

        const size_t numTables { readU16Be(*header, 4) };
        const std::optional<std::vector<uint8_t>> tableDirectoryBytes { readTableBytes(input, faceOffset + 12, static_cast<uint32_t>(numTables * 16)) };

        if (!tableDirectoryBytes)
            return std::nullopt;

        std::optional<std::vector<uint8_t>>
            nameTableBytes {},
            os2TableBytes {};

        for (size_t tableIndex { 0 }; tableIndex < numTables; ++tableIndex)
        {
            const size_t tableEntryOffset { tableIndex * 16 };
            const std::string tag(reinterpret_cast<const char*>(tableDirectoryBytes->data() + tableEntryOffset), 4);
            const uint32_t
                tableOffset { readU32Be(*tableDirectoryBytes, tableEntryOffset + 8) },
                tableLength { readU32Be(*tableDirectoryBytes, tableEntryOffset + 12) };

            if (tag == "name")
                nameTableBytes = readTableBytes(input, tableOffset, tableLength);
            else if (tag == "OS/2")
                os2TableBytes = readTableBytes(input, tableOffset, tableLength);
        }

        if (!nameTableBytes)
            return std::nullopt;

        FontFace face { .faceIndex = faceIndex };
        face.legacyFamily = readNameString(*nameTableBytes, 1);
        face.family = readNameString(*nameTableBytes, 16); //the typographic family, which groups every weight under one name
        face.subfamily = readNameString(*nameTableBytes, 17);

        if (face.family.empty())
            face.family = face.legacyFamily;

        if (face.subfamily.empty())
            face.subfamily = readNameString(*nameTableBytes, 2);

        if (face.family.empty())
            return std::nullopt;

        face.slant = getSlantFromSubfamily(face.subfamily);
        face.weight = getWeightFromSubfamily(face.subfamily);

        //The style bits and weight class, where the font has them, over what its subfamily's name suggests
        if (os2TableBytes && 64 <= os2TableBytes->size())
        {
            const uint16_t
                weightClass { readU16Be(*os2TableBytes, 4) },
                selection { readU16Be(*os2TableBytes, 62) };

            if (0 != weightClass)
                face.weight = getWeightFromClass(weightClass);

            if (0 != (selection & (1 << 9)))
                face.slant = PlatformBridge::Fonts::Slant::Oblique;
            else if (0 != (selection & 1))
                face.slant = PlatformBridge::Fonts::Slant::Italic;
        }

        return face;
    }

    //Where the face's table directory starts: 0 for a file with one face, its entry in a collection's (.ttc) header otherwise
    std::optional<uint32_t> getFaceOffset(std::ifstream& input, const uint32_t faceIndex)
    {
        const std::optional<std::vector<uint8_t>> header { readTableBytes(input, 0, 12) };

        if (!header)
            return std::nullopt;

        if (0 != std::memcmp(header->data(), "ttcf", 4))
            return 0 == faceIndex ? std::optional<uint32_t> { 0 } : std::nullopt;

        if (faceIndex >= readU32Be(*header, 8))
            return std::nullopt;

        const std::optional<std::vector<uint8_t>> offset { readTableBytes(input, 12 + faceIndex * 4, 4) };

        if (!offset)
            return std::nullopt;

        return readU32Be(*offset, 0);
    }

    //The code-points one face of a font file covers, from its character map
    std::vector<std::pair<uint32_t, uint32_t>> readUnicodeRanges(const std::filesystem::path& fontPath, const uint32_t faceIndex)
    {
        std::ifstream input(fontPath, std::ios::binary);

        if (!input)
            return {};

        const std::optional<uint32_t> faceOffset { getFaceOffset(input, faceIndex) };

        if (!faceOffset)
            return {};

        const std::optional<std::vector<uint8_t>> header { readTableBytes(input, *faceOffset, 12) };

        if (!header)
            return {};

        const size_t numTables { readU16Be(*header, 4) };
        const std::optional<std::vector<uint8_t>> tableDirectoryBytes { readTableBytes(input, *faceOffset + 12, static_cast<uint32_t>(numTables * 16)) };

        if (!tableDirectoryBytes)
            return {};

        for (size_t tableIndex { 0 }; tableIndex < numTables; ++tableIndex)
        {
            const size_t tableEntryOffset { tableIndex * 16 };

            if (0 != std::memcmp(tableDirectoryBytes->data() + tableEntryOffset, "cmap", 4))
                continue;

            const std::optional<std::vector<uint8_t>> cmapTableBytes { readTableBytes(input, readU32Be(*tableDirectoryBytes, tableEntryOffset + 8), readU32Be(*tableDirectoryBytes, tableEntryOffset + 12)) };

            if (!cmapTableBytes)
                return {};

            return getCodepointCoverage(*cmapTableBytes);
        }

        return {};
    }

    //Every face of a font file: one, or several in a collection (.ttc)
    std::vector<FontFace> getFontFaces(const std::filesystem::path& fontPath)
    {
        std::ifstream input(fontPath, std::ios::binary);

        if (!input)
            return {};

        const std::optional<std::vector<uint8_t>> header { readTableBytes(input, 0, 12) };

        if (!header)
            return {};

        std::vector<uint32_t> faceOffsets { 0 };

        if (0 == std::memcmp(header->data(), "ttcf", 4))
        {
            const uint32_t faceCount { readU32Be(*header, 8) };

            if (0 == faceCount || 1024 < faceCount) //no real collection holds that many, so the file is damaged
                return {};

            const std::optional<std::vector<uint8_t>> offsets { readTableBytes(input, 12, faceCount * 4) };

            if (!offsets)
                return {};

            faceOffsets.resize(faceCount);

            for (uint32_t faceIndex { 0 }; faceIndex < faceCount; ++faceIndex)
                faceOffsets[faceIndex] = readU32Be(*offsets, faceIndex * 4);
        }

        std::vector<FontFace> faces {};

        for (uint32_t faceIndex { 0 }; faceIndex < faceOffsets.size(); ++faceIndex)
        {
            std::optional<FontFace> face { readFontFace(input, faceOffsets[faceIndex], faceIndex) };

            if (face)
                faces.push_back(std::move(*face));
        }

        return faces;
    }

    //The font files a registry key lists: the machine's by file name in the Windows font folder, a user's by full path
    std::vector<std::filesystem::path> getRegisteredFontFiles(const HKEY root, const std::filesystem::path& fontFolder)
    {
        HKEY key { nullptr };

        if (ERROR_SUCCESS != RegOpenKeyExW(root, L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &key))
            return {};

        std::vector<std::filesystem::path> files {};
        wchar_t
            valueName[256],
            fontPath[MAX_PATH];
        DWORD
            valueNameSize { 0 },
            fontPathSize { 0 },
            type { 0 },
            index { 0 };

        while (ERROR_SUCCESS == RegEnumValueW(key, index++, valueName, &(valueNameSize = sizeof(valueName) / sizeof(wchar_t)), nullptr, &type, reinterpret_cast<LPBYTE>(fontPath), &(fontPathSize = sizeof(fontPath))))
        {
            if (REG_SZ != type)
                continue;

            const std::filesystem::path file { std::wstring(fontPath, wcsnlen(fontPath, fontPathSize / sizeof(wchar_t))) };
            const std::string extension { toLowerAscii(file.extension().string()) };

            if (".ttf" != extension && ".otf" != extension && ".ttc" != extension)
                continue;

            files.push_back(file.is_absolute() ? file : fontFolder / file);
        }

        RegCloseKey(key);
        return files;
    }

    //Every file's faces, read on a few threads at once. Each file's faces land in its own slot, so they keep the registry's order
    std::vector<std::vector<FontFace>> getAllFontFaces(const std::vector<std::filesystem::path>& files)
    {
        std::vector<std::vector<FontFace>> faces(files.size());
        std::atomic<size_t> nextFile { 0 };
        const size_t workerCount { std::clamp<size_t>(std::thread::hardware_concurrency(), 1, 8) }; //more only queue up for the disk
        std::vector<std::jthread> workers {};
        workers.reserve(workerCount);

        for (size_t worker { 0 }; worker < workerCount; ++worker)
        {
            workers.emplace_back([&files, &faces, &nextFile]()
            {
                for (size_t fileIndex { nextFile++ }; fileIndex < files.size(); fileIndex = nextFile++)
                {
                    std::error_code error {};

                    if (!std::filesystem::exists(files[fileIndex], error))
                        continue;

                    faces[fileIndex] = getFontFaces(files[fileIndex]);
                }
            });
        }

        workers.clear(); //joins them
        return faces;
    }
}

void PlatformBridge::Fonts::setFontsInformation()
{
    _fontProperties.clear();

    {
        const std::lock_guard lock { _unicodeRangesMutex };
        _unicodeRanges.clear(); //the fonts may have changed
    }

    wchar_t windowsFolder[MAX_PATH] {};

    if (0 == GetWindowsDirectoryW(windowsFolder, MAX_PATH))
    {
        std::println("ERROR: Could not find the Windows folder");
        return;
    }

    const std::filesystem::path fontFolder { std::filesystem::path(windowsFolder) / "Fonts" };
    std::vector<std::filesystem::path> files { getRegisteredFontFiles(HKEY_LOCAL_MACHINE, fontFolder) };
    const std::vector<std::filesystem::path> userFiles { getRegisteredFontFiles(HKEY_CURRENT_USER, fontFolder) }; //installed for this user only
    files.insert(files.end(), userFiles.begin(), userFiles.end());

    std::vector<std::vector<FontFace>> faces { getAllFontFaces(files) };

    for (size_t fileIndex { 0 }; fileIndex < files.size(); ++fileIndex)
    {
        const std::filesystem::path& file { files[fileIndex] };

        for (FontFace& face : faces[fileIndex])
        {
            FontProperty property
            {
                ._Name = face.family,
                ._Path = file.string(),
                ._Style = face.subfamily,
                ._Weight = face.weight,
                ._Slant = face.slant,
                ._FaceIndex = face.faceIndex
            };

            //Found by its older name too, so a program asking for "Arial Black" or "Segoe UI Semibold" still gets it
            if (!face.legacyFamily.empty() && face.legacyFamily != face.family)
            {
                _fontProperties.push_back(property);
                _fontProperties.back()._Name = face.legacyFamily;
            }

            _fontProperties.push_back(std::move(property));
        }
    }
}

const std::vector<std::pair<uint32_t, uint32_t>>& PlatformBridge::Fonts::GetUnicodeRanges(const FontProperty& font)
{
    const std::lock_guard lock { _unicodeRangesMutex };
    const std::pair<std::string, uint32_t> key { font._Path, font._FaceIndex };
    const auto found { _unicodeRanges.find(key) };

    if (_unicodeRanges.end() != found)
        return found->second;

    return _unicodeRanges.emplace(key, readUnicodeRanges(std::filesystem::path(font._Path), font._FaceIndex)).first->second;
}
