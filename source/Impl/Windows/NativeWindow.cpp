#include "NativeWindow.hpp"
#include <Windows.h>

void PlatformBridge::NativeWindow::SetOwner(HWND window, HWND owner)
{
    if(!window)
        return;

    if(!owner)
        return;

    //GWLP_HWNDPARENT sets a top-level window owner (actually not a parent)
    SetWindowLongPtrW(window, GWLP_HWNDPARENT, reinterpret_cast<LONG_PTR>(owner));

    //Removes the taskbar button, which GLFW forces on every window through WS_EX_APPWINDOW
    const LONG_PTR exStyle { GetWindowLongPtrW(window, GWL_EXSTYLE) };
    SetWindowLongPtrW(window, GWL_EXSTYLE, exStyle & ~static_cast<LONG_PTR>(WS_EX_APPWINDOW));
    SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED); //applies the style change
}
void PlatformBridge::NativeWindow::ShowInTaskbar(HWND window, const bool show)
{
    if(!window)
        return;

    //A tool window has no taskbar button; GLFW gives every window WS_EX_APPWINDOW, which forces one
    const LONG_PTR
        exStyle { GetWindowLongPtrW(window, GWL_EXSTYLE) },
        shownStyle { show
            ? (exStyle | static_cast<LONG_PTR>(WS_EX_APPWINDOW)) & ~static_cast<LONG_PTR>(WS_EX_TOOLWINDOW)
            : (exStyle & ~static_cast<LONG_PTR>(WS_EX_APPWINDOW)) | static_cast<LONG_PTR>(WS_EX_TOOLWINDOW) };

    if(shownStyle == exStyle)
        return;

    //The taskbar reads the style when a window shows, so a shown one is hidden around the change
    const bool isShown { FALSE != IsWindowVisible(window) };

    if(isShown)
        ShowWindow(window, SW_HIDE);

    SetWindowLongPtrW(window, GWL_EXSTYLE, shownStyle);
    SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED); //applies the style change.

    if(isShown)
        ShowWindow(window, SW_SHOWNOACTIVATE);
}