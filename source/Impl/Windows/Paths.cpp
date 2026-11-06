#include "Paths.hpp"
#include <Windows.h>

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

std::filesystem::path PlatformBridge::Paths::GetModuleFolder()
{
    // The module holding this very function: the program, or the library PlatformBridge is linked into
    HMODULE module { nullptr };

    if(!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCWSTR>(&PlatformBridge::Paths::GetModuleFolder), &module))
        return GetExecutablePathFSPath().parent_path();

    std::wstring path(MAX_PATH, L'\0');

    while(true)
    {
        const DWORD length { GetModuleFileNameW(module, path.data(), static_cast<DWORD>(path.size())) };

        if(0 == length)
            return GetExecutablePathFSPath().parent_path();

        if(length < path.size())
        {
            path.resize(length);
            break;
        }

        path.resize(path.size() * 2); //a long path doesn't fit MAX_PATH
    }

    return std::filesystem::path(path).parent_path();
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