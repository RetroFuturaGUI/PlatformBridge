#pragma once
#include <filesystem>

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

        /// @brief Returns the working directory
        /// @return An std::string representing the working directory.
        static std::string GetWorkingDir();

        /// @brief Returns the working directory as C-String
        /// @return A char* representing the working directory.
        static const char* GetWorkingDirCstr();

        /// @brief Sets the Working Directory
        static void SetWorkingDir(const char* dir);

    private:
        static inline std::filesystem::path _exePath;
        static inline std::string _workingDir;

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
        static void setWorkingDir();
    };
}