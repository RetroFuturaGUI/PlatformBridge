#pragma once
#include <cstdint>
#include <string>
#include <vector>
#ifdef _WIN32
    #include <Windows.h>
#endif

namespace PlatformBridge
{
    /// @brief One connected monitor, in desktop coordinates: pixels, the origin at the primary monitor's top-left, y running down.
    struct MonitorInfo
    {
        std::string _Name {}; //the system's name for it, such as \\.\DISPLAY1 on Windows or DP-1 on X11
        int32_t
            _X { 0 },
            _Y { 0 },
            _Width { 0 },
            _Height { 0 },
            _WorkX { 0 }, //the work area: the monitor without the taskbar or panels
            _WorkY { 0 },
            _WorkWidth { 0 },
            _WorkHeight { 0 };
        float
            _ScalingFactor { 1.0f },
            _RefreshRate { 0.0f }; //in Hz, 0 when unknown
        bool _IsPrimary { false };
    };

    class Monitors
    {
    public:
        /// @brief Reads the connected monitors anew. Call it after one was added, removed or rearranged.
        static void Refresh();

        /// @brief Returns the connected monitors, reading them the first time.
        static const std::vector<MonitorInfo>& GetMonitors();

        /// @brief Returns the primary monitor, or the first one when none is marked primary. nullptr when no monitor was found.
        static const MonitorInfo* GetPrimaryMonitor();

    private:
        Monitors() = default;
        ~Monitors() = default;
        Monitors(const Monitors&) = delete;
        Monitors(Monitors&&) = delete;
        void operator=(const Monitors&) = delete;
        void operator=(Monitors&&) = delete;

#ifdef _WIN32
        static BOOL CALLBACK monitorEnumProc(HMONITOR monitor, HDC hdcMonitor, LPRECT monitorRect, LPARAM data);
#endif
        static inline std::vector<MonitorInfo> _monitors {};
    };
}