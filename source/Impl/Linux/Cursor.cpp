#include "Cursor.hpp"
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Xcursor/Xcursor.h>

void PlatformBridge::Cursor::SetIcon(void* display, const uint64_t window, const CursorIcon icon)
{
    if(!display)
        return;

    if(0 == window)
        return;

    const uint64_t cursor { loadCursor(display, icon) };

    if(0 == cursor)
        return;

    //X11 keeps the cursor with the window
    Display* x11Display { static_cast<Display*>(display) };
    XDefineCursor(x11Display, static_cast<::Window>(window), static_cast<::Cursor>(cursor));
    XFlush(x11Display);
}

uint64_t PlatformBridge::Cursor::loadCursor(void* display, const CursorIcon icon)
{
    //A cursor belongs to the display connection it was made on, so another display makes a new one
    if(display != _cursorDisplay)
    {
        _cursors.fill(0);
        _cursorDisplay = display;
    }

    uint64_t& cursor { _cursors[static_cast<std::size_t>(icon)] };

    if(0 != cursor)
        return cursor;

    Display* x11Display { static_cast<Display*>(display) };
    const char* theme { XcursorGetTheme(x11Display) };

    //consider the desktop's cursor theme first, so the icons match other programs
    if(theme)
    {
        XcursorImage* image { XcursorLibraryLoadImage(themeName(icon), theme, XcursorGetDefaultSize(x11Display)) };

        if(image)
        {
            cursor = XcursorImageLoadCursor(x11Display, image);
            XcursorImageDestroy(image);
        }
    }

    if(0 == cursor)
        cursor = XCreateFontCursor(x11Display, fontShape(icon));

    return cursor;
}

const char* PlatformBridge::Cursor::themeName(const CursorIcon icon)
{
    switch(icon)
    {
        case CursorIcon::Text:
            return "text";
        case CursorIcon::Hand:
            return "pointer";
        case CursorIcon::Crosshair:
            return "crosshair";
        case CursorIcon::ResizeHorizontal:
            return "ew-resize";
        case CursorIcon::ResizeVertical:
            return "ns-resize";
        case CursorIcon::ResizeTopLeftBottomRight:
            return "nwse-resize";
        case CursorIcon::ResizeTopRightBottomLeft:
            return "nesw-resize";
        case CursorIcon::Move:
            return "move";
        case CursorIcon::NotAllowed:
            return "not-allowed";
        case CursorIcon::Wait:
            return "wait";
        default: //Arrow
            return "default";
    }
}

uint32_t PlatformBridge::Cursor::fontShape(const CursorIcon icon)
{
    switch(icon)
    {
        case CursorIcon::Text:
            return XC_xterm;
        case CursorIcon::Hand:
            return XC_hand2;
        case CursorIcon::Crosshair:
            return XC_crosshair;
        case CursorIcon::ResizeHorizontal:
            return XC_sb_h_double_arrow;
        case CursorIcon::ResizeVertical:
            return XC_sb_v_double_arrow;
        case CursorIcon::ResizeTopLeftBottomRight:
            return XC_top_left_corner; //X11's own cursors have no diagonal double arrow >.>
        case CursorIcon::ResizeTopRightBottomLeft:
            return XC_top_right_corner;
        case CursorIcon::Move:
            return XC_fleur;
        case CursorIcon::NotAllowed:
            return XC_X_cursor;
        case CursorIcon::Wait:
            return XC_watch;
        default: //Arrow
            return XC_left_ptr;
    }
}