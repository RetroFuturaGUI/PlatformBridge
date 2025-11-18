#pragma once
#include <string>
#include <print>
#include <algorithm>
#include <Windows.h>
#include <vector>

namespace PlatformBridge
{
    struct MonitorInfo
    {
        std::string name;
        int32_t resWidth;
        int32_t resHeight;
        int32_t scalingPercentage;
        float scalingFactor;
        float refreshRate;
        HMONITOR handle;
    };

    class Monitors
    {
    public:
        /// @brief Refreshes the monitor list.
        static const void Refresh();

        /// @brief Returns information about all connected monitors.
        /// @return A constant reference to a vector of MonitorInfo structures.
        static const std::vector<MonitorInfo>& GetMonitors();

    private:
        Monitors() = default;
        ~Monitors() = default;
        Monitors(const Monitors&) = delete;
        Monitors(Monitors&&) = delete;
        void operator=(const Monitors&) = delete;
        void operator=(Monitors&&) = delete;
        static Monitors& GetInstance()
        {
            static Monitors Instance;
            return Instance;
        }

        static BOOL CALLBACK monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
        bool enumerateMonitors();
        static inline std::vector<MonitorInfo> _monitors;
        static inline std::vector<MONITORINFOEX> _monitorInfoEx;
        int32_t _monitorCount = 0;
        int32_t _currentMonitor = 0;
    };
}