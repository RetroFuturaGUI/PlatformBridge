#pragma once
#include "Definitions.hpp"
#ifdef _WIN32
#include "Monitors.hpp" // implement for Linux once needed
#endif

#include "Paths.hpp"
#include "Fonts.hpp"
#include "Input.hpp"

namespace PlatformBridge
{
    /// @brief Refreshes all platform bridge components.
    static inline void RefreshPlatformBridge()
    {
    #ifdef _WIN32
        PlatformBridge::Monitors::Refresh();
    #else
        PlatformBridge::Input::Refresh();
    #endif
        PlatformBridge::Paths::Refresh();
        PlatformBridge::Fonts::Refresh();
    }
}