#include "Paths.hpp"

PlatformBridge::Paths::Paths()
{
    setExecutablePath();
}

std::string PlatformBridge::Paths::GetExecutablePath()
{
    if(_exePath.empty())
        setExecutablePath();

    return _exePath.string();
}

const char* PlatformBridge::Paths::GetExecutablePathCStr()
{
    if(_exePath.empty())
        setExecutablePath();
        
    return _exePath.string().c_str();
}

void PlatformBridge::Paths::setExecutablePath()
{
    wchar_t buf[MAX_PATH];
    DWORD n = GetModuleFileNameW(NULL, buf, MAX_PATH);

    if (n == 0) 
        return;
        
    _exePath = std::filesystem::path(std::wstring(buf, buf + n));
}
