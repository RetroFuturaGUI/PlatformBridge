#pragma once
#include <string>
#include <filesystem>
#include <windows.h>
#include <utility>
#include <print>

namespace PlatformBridge
{
    class Fonts
    {
    public:
        /// @brief Refreshes all data of installed fonts.
        static void Refresh();

        /// @brief Refreshes all data of installed fonts.
        /// @return Returns an std::vector<std::pair<std::string, std::string>> containing information of all installed fonts 
        static std::vector<std::pair<std::string, std::string>> GetFontsInformation();

    private:
        Fonts() = default;
        ~Fonts() = default;
        Fonts(const Fonts&) = delete;
        Fonts(Fonts&&) = delete;
        void operator=(const Fonts&) = delete;
        void operator=(Fonts&&) = delete;
        static Fonts& GetInstance()
        {
            static Fonts Instance;
            return Instance;
        }

        static inline std::vector<std::pair<std::string, std::string>> _fontInformation; 

        static void setFontsInformation();
    };
}