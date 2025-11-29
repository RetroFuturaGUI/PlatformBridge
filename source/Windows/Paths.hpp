#pragma once
#include <filesystem>
#include <Windows.h>

namespace PlatformBridge
{
    class Paths
    {
    public:
        /// @brief Refreshes all paths.
        static void Refresh();

        /// @brief Returns the path of the executable.
        /// @return An std::string representing the executable path.
        static std::string GetExecutablePath();

        /// @brief Returns all path data of the executable.
        /// @return An std::filesystem::path representing the executable path.
        static const std::filesystem::path GetExecutablePathFSPath();

    private:
        Paths() = default;
        ~Paths() = default;
        Paths(const Paths&) = delete;
        Paths(Paths&&) = delete;
        void operator=(const Paths&) = delete;
        void operator=(Paths&&) = delete;
        static Paths& GetInstance()
        {
            static Paths Instance;
            return Instance;
        }

        static void setExecutablePath();

        static inline std::filesystem::path _exePath;
    };
}