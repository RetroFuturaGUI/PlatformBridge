#pragma once
#include "Monitors.hpp"
#include "Paths.hpp"
#include "Fonts.hpp"

namespace PlatformBridge
{
    /// @brief Refreshes all platform bridge components.
    static inline void RefreshPlatformBridge()
    {
        PlatformBridge::Monitors::Refresh();
        PlatformBridge::Paths::Refresh();
        PlatformBridge::Fonts::Refresh();
    }
}