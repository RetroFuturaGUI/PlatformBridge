#pragma once
#include "Definitions.hpp"
#ifdef _WIN32
#include "Monitors.hpp" // implement for Linux once needed
#endif
#include "Clipboard.hpp"
#include "DragDrop.hpp"
#include "FileSystem.hpp"
#include "Paths.hpp"
#include "Fonts.hpp"
#include "Input.hpp"
#include "NativeWindow.hpp"

namespace PlatformBridge
{
    /// @brief Refreshes all platform bridge components.
    static inline void RefreshPlatformBridge()
    {
    #ifdef _WIN32
        PlatformBridge::Monitors::Refresh();
    #endif
        PlatformBridge::Input::Refresh();
        PlatformBridge::Paths::Refresh();
        PlatformBridge::FileSystem::Refresh();
        PlatformBridge::Fonts::Refresh();
    }
}