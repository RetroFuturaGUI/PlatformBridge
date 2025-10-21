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
        static const std::vector<MonitorInfo>& GetMonitors()
        {
            return GetInstance()._monitors;
        }

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

        static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
        {
            MONITORINFOEX info;
            info.cbSize = sizeof(MONITORINFOEX);
            GetMonitorInfo(hMonitor, &info);
            MonitorInfo monitorInfo;
            monitorInfo.name = reinterpret_cast<char*>(info.szDevice);
            monitorInfo.resWidth = info.rcMonitor.right - info.rcMonitor.left;
            monitorInfo.resHeight = info.rcMonitor.bottom - info.rcMonitor.top;
            monitorInfo.handle = hMonitor;

            DEVMODE devMode;
            devMode.dmSize = sizeof(DEVMODE);

            if (EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &devMode))
                monitorInfo.refreshRate = devMode.dmDisplayFrequency;

            devMode.dmSize = sizeof(devMode);
            devMode.dmDriverExtra = 0;
            EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &devMode);
            auto cxPhysical = devMode.dmPelsWidth;
            auto cyPhysical = devMode.dmPelsHeight;
            auto horizontalScale = ((double) cxPhysical / (double) monitorInfo.resWidth);
            auto verticalScale = ((double) cyPhysical / (double) monitorInfo.resHeight);
            monitorInfo.scalingPercentage = static_cast<int32_t>(min(horizontalScale, verticalScale) * 100);
            _monitors.push_back(monitorInfo);
            std::println("Found monitor: {0} ({1}x{2}), rate: {3}Hz, scaling: {4}%", monitorInfo.name, monitorInfo.resWidth, monitorInfo.resHeight, monitorInfo.refreshRate, monitorInfo.scalingPercentage);
            return TRUE;
        }



        bool enumerateMonitors();
        
        static inline std::vector<MonitorInfo> _monitors;
        static inline std::vector<MONITORINFOEX> _monitorInfoEx;
        int32_t _monitorCount = 0;
        int32_t _currentMonitor = 0;
    };
}