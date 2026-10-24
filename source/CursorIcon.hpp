#pragma once
#include <cstdint>
/*Kept apart from Cursor.hpp, which needs windows.h, so a header can name an
  icon without bringing in windows.h and its min/max macros n stuff.*/

namespace PlatformBridge
{
    /// @brief Icons for PlatformBridge::Cursor.
    enum class CursorIcon : uint32_t
    {
        Arrow,
        Text,
        Hand,
        Crosshair,
        ResizeHorizontal,
        ResizeVertical,
        ResizeTopLeftBottomRight,
        ResizeTopRightBottomLeft,
        Move,
        NotAllowed,
        Wait
    };
}