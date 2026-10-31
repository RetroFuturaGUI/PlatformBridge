#pragma once
#include "Definitions.hpp"
#include "Monitors.hpp"
#include "Clipboard.hpp"
#include "Cursor.hpp"
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
        PlatformBridge::Monitors::Refresh();
        PlatformBridge::Input::Refresh();
        PlatformBridge::Paths::Refresh();
        PlatformBridge::FileSystem::Refresh();
        PlatformBridge::Fonts::Refresh();
    }
}