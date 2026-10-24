#include "Cursor.hpp"
#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

void PlatformBridge::Cursor::SetIcon(HWND window, const CursorIcon icon)
{
    if(!window)
        return;

    //Installs the subclass on the first call and swaps the icon it holds after that
    SetWindowSubclass(window, SubclassProc, 0, static_cast<DWORD_PTR>(icon));

    if(isMouseOverClientArea(window))
        SetCursor(systemCursor(icon)); //a resting mouse would keep the old icon until it moves
}

HCURSOR PlatformBridge::Cursor::systemCursor(const CursorIcon icon)
{
    switch(icon)
    {
        case CursorIcon::Text:
            return LoadCursor(nullptr, IDC_IBEAM);
        case CursorIcon::Hand:
            return LoadCursor(nullptr, IDC_HAND);
        case CursorIcon::Crosshair:
            return LoadCursor(nullptr, IDC_CROSS);
        case CursorIcon::ResizeHorizontal:
            return LoadCursor(nullptr, IDC_SIZEWE);
        case CursorIcon::ResizeVertical:
            return LoadCursor(nullptr, IDC_SIZENS);
        case CursorIcon::ResizeTopLeftBottomRight:
            return LoadCursor(nullptr, IDC_SIZENWSE);
        case CursorIcon::ResizeTopRightBottomLeft:
            return LoadCursor(nullptr, IDC_SIZENESW);
        case CursorIcon::Move:
            return LoadCursor(nullptr, IDC_SIZEALL);
        case CursorIcon::NotAllowed:
            return LoadCursor(nullptr, IDC_NO);
        case CursorIcon::Wait:
            return LoadCursor(nullptr, IDC_WAIT);
        default: //Arrow
            return LoadCursor(nullptr, IDC_ARROW);
    }
}

bool PlatformBridge::Cursor::isMouseOverClientArea(HWND window)
{
    POINT point;

    if(!GetCursorPos(&point))
        return false;

    if(WindowFromPoint(point) != window)
        return false;

    if(!ScreenToClient(window, &point))
        return false;

    RECT clientArea;

    if(!GetClientRect(window, &clientArea))
        return false;

    return PtInRect(&clientArea, point);
}

LRESULT CALLBACK PlatformBridge::Cursor::SubclassProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR subclassID, DWORD_PTR icon)
{
    switch(message)
    {
        case WM_SETCURSOR:
        {
            if(HTCLIENT != LOWORD(lParam))
                break; //even pick cursors when on window borders

            SetCursor(systemCursor(static_cast<CursorIcon>(icon)));
            return TRUE;
        }
        case WM_NCDESTROY:
            RemoveWindowSubclass(window, SubclassProc, subclassID);
        break;
        default:
            [[unlikely]]
        break;
    }

    return DefSubclassProc(window, message, wParam, lParam);
}