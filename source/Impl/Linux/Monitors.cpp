#include "Monitors.hpp"
#include "LinuxHeaders.hpp"
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include <X11/extensions/Xrandr.h>
#include <algorithm>
#include <cstdlib>

namespace
{
    struct WorkArea
    {
        long
            _X { 0 },
            _Y { 0 },
            _Width { 0 },
            _Height { 0 };
        bool _IsKnown { false };
    };

    //The desktop's work area from the window manager (_NET_WORKAREA), for the current desktop
    WorkArea readWorkArea(Display* display, ::Window root)
    {
        const Atom workAreaAtom { XInternAtom(display, "_NET_WORKAREA", True) };

        if(None == workAreaAtom)
            return {};

        const Atom currentDesktopAtom { XInternAtom(display, "_NET_CURRENT_DESKTOP", True) };
        long desktop { 0 };
        Atom type { None };
        int32_t format { 0 };
        unsigned long
            itemCount { 0 },
            bytesAfter { 0 };
        unsigned char* data { nullptr };

        if(None != currentDesktopAtom
           && Success == XGetWindowProperty(display, root, currentDesktopAtom, 0, 1, False, XA_CARDINAL, &type, &format, &itemCount, &bytesAfter, &data)
           && data && 0 < itemCount)
            desktop = reinterpret_cast<long*>(data)[0];

        if(data)
            XFree(data);

        data = nullptr;

        //Four values per desktop: x, y, width, height
        if(Success != XGetWindowProperty(display, root, workAreaAtom, desktop * 4, 4, False, XA_CARDINAL, &type, &format, &itemCount, &bytesAfter, &data))
            return {};

        if(!data)
            return {};

        WorkArea workArea {};

        if(4 <= itemCount)
        {
            const long* values { reinterpret_cast<long*>(data) };
            workArea = WorkArea { ._X = values[0], ._Y = values[1], ._Width = values[2], ._Height = values[3], ._IsKnown = true };
        }

        XFree(data);
        return workArea;
    }

    //The desktop's scale from the Xft.dpi resource desktops set, 96 dpi being 1
    float readScalingFactor(Display* display)
    {
        const char* resources { XResourceManagerString(display) };

        if(!resources)
            return 1.0f;

        XrmInitialize();
        XrmDatabase database { XrmGetStringDatabase(resources) };

        if(!database)
            return 1.0f;

        char* type { nullptr };
        XrmValue value {};
        float scalingFactor { 1.0f };

        if(XrmGetResource(database, "Xft.dpi", "Xft.Dpi", &type, &value) && value.addr)
        {
            const float dpi { static_cast<float>(std::atof(value.addr)) };

            if(0.0f < dpi)
                scalingFactor = dpi / 96.0f;
        }

        XrmDestroyDatabase(database);
        return scalingFactor;
    }

    float readRefreshRate(Display* display, XRRScreenResources* resources, const XRRMonitorInfo& monitor) //0 if it cannot be read
    {
        if(!resources)
            return 0.0f;

        if(0 >= monitor.noutput || !monitor.outputs)
            return 0.0f;

        XRROutputInfo* output { XRRGetOutputInfo(display, resources, monitor.outputs[0]) };

        if(!output)
            return 0.0f;

        XRRCrtcInfo* crtc { None != output->crtc ? XRRGetCrtcInfo(display, resources, output->crtc) : nullptr };
        XRRFreeOutputInfo(output);

        if(!crtc)
            return 0.0f;

        float refreshRate { 0.0f };

        for(size_t modeIndex { 0 }; modeIndex < resources->nmode; ++modeIndex)
        {
            const XRRModeInfo& mode { resources->modes[modeIndex] };

            if(mode.id != crtc->mode)
                continue;

            if(0 != mode.hTotal && 0 != mode.vTotal)
                refreshRate = static_cast<float>(static_cast<double>(mode.dotClock) / (static_cast<double>(mode.hTotal) * static_cast<double>(mode.vTotal)));

            break;
        }

        XRRFreeCrtcInfo(crtc);
        return refreshRate;
    }
}

void PlatformBridge::Monitors::Refresh()
{
    _monitors.clear();
    Display* display { XOpenDisplay(nullptr) };

    if(!display)
        return;

    const ::Window root { DefaultRootWindow(display) };
    const WorkArea workArea { readWorkArea(display, root) };
    const float scalingFactor { readScalingFactor(display) };
    XRRScreenResources* resources { XRRGetScreenResourcesCurrent(display, root) };
    int32_t monitorCount { 0 };
    XRRMonitorInfo* monitors { XRRGetMonitors(display, root, True, &monitorCount) };

    for(int monitorIndex { 0 }; monitors && monitorIndex < monitorCount; ++monitorIndex)
    {
        const XRRMonitorInfo& monitor { monitors[monitorIndex] };
        char* name { XGetAtomName(display, monitor.name) };

        //The window manager's work area spans the desktop, so each monitor takes its part of it
        long
            workLeft { monitor.x },
            workTop { monitor.y },
            workRight { static_cast<long>(monitor.x) + monitor.width },
            workBottom { static_cast<long>(monitor.y) + monitor.height };

        if(workArea._IsKnown)
        {
            workLeft = std::max(workLeft, workArea._X);
            workTop = std::max(workTop, workArea._Y);
            workRight = std::min(workRight, workArea._X + workArea._Width);
            workBottom = std::min(workBottom, workArea._Y + workArea._Height);
        }

        _monitors.push_back(MonitorInfo
        {
            ._Name = name ? name : "",
            ._X = monitor.x,
            ._Y = monitor.y,
            ._Width = monitor.width,
            ._Height = monitor.height,
            ._WorkX = static_cast<int32_t>(workLeft),
            ._WorkY = static_cast<int32_t>(workTop),
            ._WorkWidth = static_cast<int32_t>(std::max(workRight - workLeft, 0L)),
            ._WorkHeight = static_cast<int32_t>(std::max(workBottom - workTop, 0L)),
            ._ScalingFactor = scalingFactor,
            ._RefreshRate = readRefreshRate(display, resources, monitor),
            ._IsPrimary = 0 != monitor.primary
        });

        if(name)
            XFree(name);
    }

    if(monitors)
        XRRFreeMonitors(monitors);

    if(resources)
        XRRFreeScreenResources(resources);

    XCloseDisplay(display);
}

const std::vector<PlatformBridge::MonitorInfo>& PlatformBridge::Monitors::GetMonitors()
{
    if(_monitors.empty())
        Refresh();

    return _monitors;
}

const PlatformBridge::MonitorInfo* PlatformBridge::Monitors::GetPrimaryMonitor()
{
    const std::vector<MonitorInfo>& monitors { GetMonitors() };

    for(const MonitorInfo& monitor : monitors)
    {
        if(monitor._IsPrimary)
            return &monitor;
    }

    return monitors.empty() ? nullptr : &monitors.front();
}