#include "Paths.hpp"

void PlatformBridge::Paths::Refresh()
{
    setExecutablePath();
    setWorkingDir();
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

std::string PlatformBridge::Paths::GetWorkingDir()
{
    return _workingDir;
}

const char* PlatformBridge::Paths::GetWorkingDirCstr()
{
    return _workingDir.c_str();
}

void PlatformBridge::Paths::SetWorkingDir(const char* dir)
{
    _workingDir = dir;
    SetCurrentDirectoryA(dir);
}

void PlatformBridge::Paths::setExecutablePath()
{
    wchar_t buf[MAX_PATH];
    DWORD n = GetModuleFileNameW(NULL, buf, MAX_PATH);

    if (n == 0) 
        return;
        
    _exePath = std::filesystem::path(std::wstring(buf, buf + n));
}

void PlatformBridge::Paths::setWorkingDir()
{
    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    _workingDir = buffer;
}
