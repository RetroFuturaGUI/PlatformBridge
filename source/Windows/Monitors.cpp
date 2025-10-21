#include "Monitors.hpp"
#include <print>
#include <codecvt>

PlatformBridge::Monitors::Monitors()
{
    if (!enumerateMonitors())
    {
        std::println("Failed to enumerate monitors");
    }

    for(auto& monitorEx : _monitorInfoEx)
    {
        MonitorInfo monitorInfo;
        monitorInfo.name = reinterpret_cast<char*>(monitorEx.szDevice);
        monitorInfo.resWidth = monitorEx.rcMonitor.right - monitorEx.rcMonitor.left;
        monitorInfo.resHeight = monitorEx.rcMonitor.bottom - monitorEx.rcMonitor.top;
       
        //monitorInfo.scalingPercentage = ?
       // monitorInfo.refreshRate = ?
        //monitorInfo.handle = ?

        _monitors.push_back(monitorInfo);
        std::println("Found monitor: {0} ({1}x{2})", monitorInfo.name, monitorInfo.resWidth, monitorInfo.resHeight);
    }
}

bool PlatformBridge::Monitors::enumerateMonitors()
{
              HDC hdc = nullptr;
            LPCRECT lprcClip = nullptr;
            LPARAM dwData = 0;
    return EnumDisplayMonitors(hdc, lprcClip, MonitorEnumProc, dwData);
}