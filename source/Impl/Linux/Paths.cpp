#include "Paths.hpp"
#include "LinuxHeaders.hpp"
#include <dlfcn.h>

void PlatformBridge::Paths::Refresh()
{
    setExecutablePath();
    setWorkingDir();
}

std::string PlatformBridge::Paths::GetExecutablePath()
{
    return _exePath.string();
}

const std::filesystem::path PlatformBridge::Paths::GetExecutablePathFSPath()
{
    return _exePath;
}

std::filesystem::path PlatformBridge::Paths::GetModuleFolder()
{
    // The shared object holding this very function. The program, or the library PlatformBridge is linked into
    Dl_info info {};

    if(0 == dladdr(reinterpret_cast<void*>(&PlatformBridge::Paths::GetModuleFolder), &info) || !info.dli_fname)
        return GetExecutablePathFSPath().parent_path();

    return std::filesystem::path(info.dli_fname).parent_path();
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
    chdir(dir);
    setWorkingDir();
}

void PlatformBridge::Paths::setExecutablePath()
{
    char buffer[1024];
    ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (count != -1)
    {
        buffer[count] = '\0';
        _exePath = std::filesystem::path(buffer);
    }
}

void PlatformBridge::Paths::setWorkingDir()
{
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != nullptr)
    {
        _workingDir = std::string(buffer);
    }
}