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