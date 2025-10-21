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
        static inline const std::vector<MonitorInfo>& GetMonitors();

    private:
        Monitors();
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

        static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
        bool enumerateMonitors();
        static inline std::vector<MonitorInfo> _monitors;
        static inline std::vector<MONITORINFOEX> _monitorInfoEx;
        int32_t _monitorCount = 0;
        int32_t _currentMonitor = 0;
    };
}