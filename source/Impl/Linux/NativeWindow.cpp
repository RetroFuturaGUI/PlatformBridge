#include "NativeWindow.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <vector>

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
void PlatformBridge::NativeWindow::ShowInTaskbar(void* display, const uint64_t window, const bool show)
{
    if(!display)
        return;

    if(0 == window)
        return;

    Display* x11Display { static_cast<Display*>(display) };
    const ::Window x11Window { static_cast<::Window>(window) };
    const Atom
        stateAtom { XInternAtom(x11Display, "_NET_WM_STATE", False) },
        skipTaskbarAtom { XInternAtom(x11Display, "_NET_WM_STATE_SKIP_TASKBAR", False) };
    XWindowAttributes attributes {};
    XGetWindowAttributes(x11Display, x11Window, &attributes);

    //A mapped window's state changes through the window manager (EWMH): 1 adds the state, 0 removes it
    if(IsUnmapped != attributes.map_state)
    {
        XEvent event {};
        event.xclient.type = ClientMessage;
        event.xclient.window = x11Window;
        event.xclient.message_type = stateAtom;
        event.xclient.format = 32;
        event.xclient.data.l[0] = show ? 0 : 1;
        event.xclient.data.l[1] = static_cast<long>(skipTaskbarAtom);
        event.xclient.data.l[2] = 0;
        event.xclient.data.l[3] = 1; //from an application.
        XSendEvent(x11Display, DefaultRootWindow(x11Display), False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
        XFlush(x11Display);
        return;
    }

    //Before it is mapped the window keeps the list itself, which the window manager reads once it maps
    std::vector<Atom> states {};
    Atom actualType { None };
    int actualFormat { 0 };
    unsigned long
        itemCount { 0 },
        bytesAfter { 0 };
    unsigned char* data { nullptr };

    if(Success == XGetWindowProperty(x11Display, x11Window, stateAtom, 0, 1024, False, XA_ATOM, &actualType, &actualFormat, &itemCount, &bytesAfter, &data) && data)
    {
        const Atom* atoms { reinterpret_cast<const Atom*>(data) };

        for(unsigned long item = 0; item < itemCount; ++item)
        {
            if(skipTaskbarAtom != atoms[item])
                states.push_back(atoms[item]);
        }
    }

    if(data)
        XFree(data);

    if(!show)
        states.push_back(skipTaskbarAtom);

    XChangeProperty(x11Display, x11Window, stateAtom, XA_ATOM, 32, PropModeReplace, reinterpret_cast<const unsigned char*>(states.data()), static_cast<int>(states.size()));
    XFlush(x11Display);
}