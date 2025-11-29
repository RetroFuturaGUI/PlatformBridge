#include "Fonts.hpp"

void PlatformBridge::Fonts::Refresh()
{
    setFontsInformation();
}

std::vector<std::pair<std::string, std::string>> PlatformBridge::Fonts::GetFontsInformation()
{
    if(_fontInformation.empty() || _fontInformation.size() == 0)
        Refresh();
        
    return _fontInformation;
}

void PlatformBridge::Fonts::setFontsInformation()
{
    _fontInformation.clear();

    HKEY hKey;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    {
        std::println("ERROR: Could not open Fonts registry key");
        return;
    }

    wchar_t valueName[256];
    wchar_t fontPath[MAX_PATH];
    DWORD valueNameSize;
    DWORD fontPathSize;
    DWORD index = 0;

    while (RegEnumValueW(hKey, index, valueName, &(valueNameSize = sizeof(valueName) / sizeof(wchar_t)), NULL, NULL, (LPBYTE)fontPath, &(fontPathSize = sizeof(fontPath))) == ERROR_SUCCESS)
    {
        std::wstring fontName(valueName);
        std::wstring fontFile(fontPath);

        std::filesystem::path fullPath = std::filesystem::path(L"C:\\Windows\\Fonts") / fontFile;

        if (std::filesystem::exists(fullPath))
            _fontInformation.emplace_back(std::string(fontName.begin(), fontName.end()), fullPath.string());

        ++index;
    }

    RegCloseKey(hKey);
}