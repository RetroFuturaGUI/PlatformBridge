#include "Paths.hpp"

void PlatformBridge::Paths::Refresh()
{
    setExecutablePath();
}

std::string PlatformBridge::Paths::GetExecutablePath()
{
    if(_exePath.empty())
        Refresh();

    return _exePath.string();
}

const std::filesystem::path PlatformBridge::Paths::GetExecutablePathFSPath()
{
    if(_exePath.empty())
        Refresh();
        
    return _exePath;
}

void PlatformBridge::Paths::setExecutablePath()
{
    wchar_t buf[MAX_PATH];
    DWORD n = GetModuleFileNameW(NULL, buf, MAX_PATH);

    if (n == 0) 
        return;
        
    _exePath = std::filesystem::path(std::wstring(buf, buf + n));
}