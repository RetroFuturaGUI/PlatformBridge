#pragma once

#ifdef _WIN32
#include "Monitors.hpp" // implement for Linux once needed
#endif

#include "Paths.hpp"
#include "Fonts.hpp"

namespace PlatformBridge
{
    /// @brief Refreshes all platform bridge components.
    static inline void RefreshPlatformBridge()
    {
    #ifdef _WIN32
        PlatformBridge::Monitors::Refresh();
    #endif
        PlatformBridge::Paths::Refresh();
        PlatformBridge::Fonts::Refresh();
    }
}