#include "Monitors.hpp"
#include <print>
#include <codecvt>

PlatformBridge::Monitors::Monitors()
{
    if (!enumerateMonitors())
    {
    #ifndef NDEBUG
        std::println("Failed to enumerate monitors");
    #endif
    }
}

bool PlatformBridge::Monitors::enumerateMonitors()
{
    HDC hdc = nullptr;
    LPCRECT lprcClip = nullptr;
    LPARAM dwData = 0;
    return EnumDisplayMonitors(hdc, lprcClip, MonitorEnumProc, dwData);
}

BOOL CALLBACK PlatformBridge::Monitors::MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
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
     monitorInfo.scalingFactor = monitorInfo.scalingPercentage / 100.0f;
    _monitors.push_back(monitorInfo);

#ifndef NDEBUG
    std::println("Found monitor: {0} ({1}x{2}), rate: {3}Hz, scaling: {4}%", monitorInfo.name, monitorInfo.resWidth, monitorInfo.resHeight, monitorInfo.refreshRate, monitorInfo.scalingPercentage);
#endif
    return TRUE;
}

const std::vector<PlatformBridge::MonitorInfo>& PlatformBridge::Monitors::GetMonitors()
{
    if(_monitors.empty())
        GetInstance().enumerateMonitors();

    return _monitors;
}