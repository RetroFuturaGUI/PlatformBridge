#include "NativeWindow.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void PlatformBridge::NativeWindow::SetOwner(void* display, const uint64_t window, const uint64_t owner)
{
    if(!display)
        return;

    if(0 == window || 0 == owner)
        return;

    Display* x11Display { static_cast<Display*>(display) };

    //The window manager keeps the transient window above
    XSetTransientForHint(x11Display, static_cast<::Window>(window), static_cast<::Window>(owner));
    XFlush(x11Display);
}
