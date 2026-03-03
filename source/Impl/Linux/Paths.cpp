#include "Paths.hpp"
#include "LinuxHeaders.hpp"

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