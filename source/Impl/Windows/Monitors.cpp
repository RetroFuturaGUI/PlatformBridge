#include "Monitors.hpp"
#include <algorithm>
#include <print>
#include <string>

namespace
{
    //A monitor's device name, from the UTF-16 the system gives
    std::string toUtf8(const wchar_t* text)
    {
        const int length { WideCharToMultiByte(CP_UTF8, 0, text, -1, nullptr, 0, nullptr, nullptr) };

        if(1 >= length)
            return {};

        std::string utf8(static_cast<size_t>(length - 1), '\0');
        WideCharToMultiByte(CP_UTF8, 0, text, -1, utf8.data(), length, nullptr, nullptr);
        return utf8;
    }
}

void PlatformBridge::Monitors::Refresh()
{
    _monitors.clear(); //once per refresh: the callback runs once per monitor

    if(EnumDisplayMonitors(nullptr, nullptr, monitorEnumProc, 0))
        return;

#ifndef NDEBUG
    std::println("Failed to enumerate monitors");
#endif
}

BOOL CALLBACK PlatformBridge::Monitors::monitorEnumProc(HMONITOR monitor, [[maybe_unused]] HDC hdcMonitor, [[maybe_unused]] LPRECT monitorRect, [[maybe_unused]] LPARAM data)
{
    MONITORINFOEXW info {};
    info.cbSize = sizeof(MONITORINFOEXW);

    if(!GetMonitorInfoW(monitor, &info))
        return TRUE; //the next one may still answer

    MonitorInfo monitorInfo
    {
        ._Name = toUtf8(info.szDevice),
        ._X = info.rcMonitor.left,
        ._Y = info.rcMonitor.top,
        ._Width = info.rcMonitor.right - info.rcMonitor.left,
        ._Height = info.rcMonitor.bottom - info.rcMonitor.top,
        ._WorkX = info.rcWork.left,
        ._WorkY = info.rcWork.top,
        ._WorkWidth = info.rcWork.right - info.rcWork.left,
        ._WorkHeight = info.rcWork.bottom - info.rcWork.top,
        ._ScalingFactor = 1.0f,
        ._RefreshRate = 0.0f,
        ._IsPrimary = 0 != (info.dwFlags & MONITORINFOF_PRIMARY)
    };

    DEVMODEW devMode {};
    devMode.dmSize = sizeof(DEVMODEW);

    //The physical mode against the size the process sees: above 1 while a process unaware of DPI is scaled
    if(EnumDisplaySettingsW(info.szDevice, ENUM_CURRENT_SETTINGS, &devMode) && 0 < monitorInfo._Width && 0 < monitorInfo._Height)
    {
        monitorInfo._RefreshRate = static_cast<float>(devMode.dmDisplayFrequency);
        monitorInfo._ScalingFactor = (std::min)(static_cast<float>(devMode.dmPelsWidth) / static_cast<float>(monitorInfo._Width),
                                                static_cast<float>(devMode.dmPelsHeight) / static_cast<float>(monitorInfo._Height));
    }

    _monitors.push_back(monitorInfo);

#ifndef NDEBUG
    std::println("Found monitor: {0} ({1}x{2} at {3},{4}{5}), rate: {6}Hz, scaling: {7}%", monitorInfo._Name, monitorInfo._Width, monitorInfo._Height,
                 monitorInfo._X, monitorInfo._Y, monitorInfo._IsPrimary ? ", primary" : "", monitorInfo._RefreshRate,
                 static_cast<int32_t>(monitorInfo._ScalingFactor * 100.0f));
#endif
    return TRUE;
}

const std::vector<PlatformBridge::MonitorInfo>& PlatformBridge::Monitors::GetMonitors()
{
    if(_monitors.empty())
        Refresh();

    return _monitors;
}

const PlatformBridge::MonitorInfo* PlatformBridge::Monitors::GetPrimaryMonitor()
{
    const std::vector<MonitorInfo>& monitors { GetMonitors() };

    for(const MonitorInfo& monitor : monitors)
    {
        if(monitor._IsPrimary)
            return &monitor;
    }

    return monitors.empty() ? nullptr : &monitors.front();
}