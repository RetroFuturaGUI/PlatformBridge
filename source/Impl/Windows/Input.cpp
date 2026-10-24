#include "Input.hpp"
#include <Windows.h>
#include <CommCtrl.h>
#include <print>

#pragma comment(lib, "comctl32.lib")

namespace
{
    //WM_KEYDOWN/WM_KEYUP report Ctrl/Shift/Alt as the generic VK_CONTROL/VK_SHIFT/VK_MENU, not the
    //left/right-specific VK_LCONTROL/VK_RCONTROL/etc. that PB_KEY_CONTROL_L/R and friends map to - unlike
    //WH_KEYBOARD_LL's KBDLLHOOKSTRUCT::vkCode, which already gave the specific side directly. Resolve it the
    //same way the hook effectively did, from the message's scan code + extended-key bit.
    uint32_t resolveExtendedVirtualKey(const WPARAM vk, const LPARAM lParam)
    {
        if (vk != VK_SHIFT && vk != VK_CONTROL && vk != VK_MENU)
            return static_cast<uint32_t>(vk);

        UINT scanCode = static_cast<UINT>((lParam >> 16) & 0xFF);

        if (lParam & 0x01000000) //extended-key bit
            scanCode |= 0xE000;

        const UINT mappedVk = MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX);
        return mappedVk != 0 ? static_cast<uint32_t>(mappedVk) : static_cast<uint32_t>(vk);
    }
}

void PlatformBridge::Input::Refresh()
{
    std::scoped_lock lock(_inputMutex);
    _inputStringBuffer.reserve(64);
    removeSubclass();
    _currentWindow = nullptr;
    _display = nullptr;
    _rawDisplay = nullptr;
    _ownsDisplay = false;
    _keyboardUseState = KeyboardUseState::KeyReleased;
}

void PlatformBridge::Input::Stop()
{
    std::scoped_lock lock(_inputMutex);
    removeSubclass();
    _currentWindow = nullptr;
    _keyboardUseState = KeyboardUseState::KeyReleased;
    _lastKeySym = 0;
    _pendingHighSurrogate = 0;
    _heldKeys.clear();
    _keyPressCounts.clear();
    _previousMouseButtons = MouseButton::None;
}

PlatformBridge::Input::~Input()
{
    Stop();
}

void PlatformBridge::Input::installSubclass(HWND window)
{
    //The window's messages pass through SubclassProc(edure) before they reach GLFW. Windows keeps each subclass of a window apart, so Cursor's subclass shares the window
    SetWindowSubclass(window, SubclassProc, 0, 0);
    _subclassedWindow = window;
}

void PlatformBridge::Input::removeSubclass()
{
    if (!_subclassedWindow)
        return;

    RemoveWindowSubclass(_subclassedWindow, SubclassProc, 0);
    _subclassedWindow = nullptr;
}

void PlatformBridge::Input::forgetReleasedKeys()
{
    std::erase_if(_heldKeys, [](const uint32_t vkCode) { return 0 == (GetAsyncKeyState(static_cast<int>(vkCode)) & 0x8000); });

    if (_heldKeys.contains(_lastKeySym))
        return;

    _lastKeySym = 0;
    _keyboardUseState = KeyboardUseState::KeyReleased;
}

LRESULT CALLBACK PlatformBridge::Input::SubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassID, DWORD_PTR)
{
    switch (msg)
    {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            const uint32_t vkCode = resolveExtendedVirtualKey(wParam, lParam);
            std::scoped_lock lock(_inputMutex);

            //WM_CHAR (if this keystroke produces one) is dispatched separately, right after this message -
            //clear first so keys that produce no text (arrows, F-keys, ...) don't leave stale text behind
            //for a later WM_CHAR-less frame to re-consume via GetInputString().
            _inputStringBuffer.clear();

            _keyboardUseState = (_lastKeySym == vkCode)
                ? KeyboardUseState::SameKeyPressed
                : KeyboardUseState::KeyPressed;
            _lastKeySym = vkCode;

            //Only a genuine released->pressed edge counts as a new press - OS auto-repeat re-fires
            //WM_KEYDOWN for a key that's already held, and _heldKeys distinguishes the two.
            if (!_heldKeys.contains(vkCode))
                ++_keyPressCounts[vkCode];

            _heldKeys.insert(vkCode);
        } break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            const uint32_t vkCode = resolveExtendedVirtualKey(wParam, lParam);
            std::scoped_lock lock(_inputMutex);
            _keyboardUseState = KeyboardUseState::KeyReleased;
            _lastKeySym = 0;
            _heldKeys.erase(vkCode);
        } break;
        case WM_CHAR:
        case WM_SYSCHAR:
        {
            const wchar_t character { static_cast<wchar_t>(wParam) };
            std::scoped_lock lock(_inputMutex);

            //characters past U+FFFF (emoji and other stuff) come as two WM_CHARs. Each half alone converts to U+FFFD, so the first half requires second.
            if (IS_HIGH_SURROGATE(character))
            {
                _pendingHighSurrogate = character;
                break;
            }

            const wchar_t pair[2] { _pendingHighSurrogate, character };
            const bool isPair { 0 != _pendingHighSurrogate && IS_LOW_SURROGATE(character) };
            _pendingHighSurrogate = 0;

            char utf8[4];
            const int length { WideCharToMultiByte(CP_UTF8, 0, isPair ? pair : &character, isPair ? 2 : 1, utf8, static_cast<int>(sizeof(utf8)), nullptr, nullptr) };
            _inputStringBuffer.assign(utf8, length); //copies into the buffer's storage to prevent unnecessary copying
        } break;
        case WM_SETFOCUS:
        {
            //The window took the keys before Windows gave it the focus, so a key let go in between sent its key-up elsewhere
            std::scoped_lock lock(_inputMutex);
            forgetReleasedKeys();
        } break;
        case WM_NCDESTROY:
        {
            std::scoped_lock lock(_inputMutex);
            RemoveWindowSubclass(hwnd, SubclassProc, subclassID);
            _subclassedWindow = nullptr; //the window is gone, so a later removeSubclass has nothing to take out
        } break;
        default:
            break;
    }

    return DefSubclassProc(hwnd, msg, wParam, lParam);
}

PlatformBridge::KeyboardUseState PlatformBridge::Input::GetKeyboardUseState()
{
    return _keyboardUseState;
}

const std::string& PlatformBridge::Input::GetInputString()
{
    return _inputStringBuffer;
}

void PlatformBridge::Input::SetActiveWindow(HWND window)
{
    std::scoped_lock lock(_inputMutex);

    //The subclass is a capture mechanism, not a focus indicator - it stays installed on whichever window last
    //had it regardless of window being 0 here, so held-key/text tracking stays accurate no matter how often
    //SetActiveWindow(0) is called (e.g. every frame the mouse isn't hovering a given TextBox). _currentWindow
    //is the separate, freely-toggled "active window" id exposed via GetActiveWindowID().
    if (window != nullptr && window != _subclassedWindow)
    {
        removeSubclass();
        installSubclass(window);
        forgetReleasedKeys(); //key-ups that went to the window before
    }

    _currentWindow = window;
}

void PlatformBridge::Input::SetActiveDisplay(void* display)
{
    std::scoped_lock lock(_inputMutex);
    _display = display;
    _rawDisplay = display;
    _ownsDisplay = false;
}

void* PlatformBridge::Input::GetActiveDisplay()
{
    return _display;
}

uint64_t PlatformBridge::Input::GetActiveWindowID()
{
    return reinterpret_cast<uint64_t>(_currentWindow);
}

PlatformBridge::KeyPressState PlatformBridge::Input::GetKeyPressState(const uint32_t key)
{
    if (_lastKeySym == key && _keyboardUseState == KeyboardUseState::SameKeyPressed)
        return KeyPressState::Repeat;

    if (_lastKeySym == key)
        return KeyPressState::Press;

    return KeyPressState::Release;
}

uint32_t PlatformBridge::Input::GetLastKeySym()
{
    std::scoped_lock lock(_inputMutex);
    return _lastKeySym;
}

uint32_t PlatformBridge::Input::GetKeyPressCount(const uint32_t key)
{
    std::scoped_lock lock(_inputMutex);
    const auto it = _keyPressCounts.find(key);
    return it != _keyPressCounts.end() ? it->second : 0;
}

bool PlatformBridge::Input::IsKeyDown(const uint32_t key)
{
    std::scoped_lock lock(_inputMutex);
    return _heldKeys.contains(key);
}

bool PlatformBridge::Input::IsKeyPhysicallyDown(const uint32_t key)
{
    return 0 != (GetAsyncKeyState(static_cast<int>(key)) & 0x8000);
}

bool PlatformBridge::Input::IsMouseButtonDown(const MouseButton button)
{
    switch (button)
    {
        case MouseButton::Left:
            return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        case MouseButton::Right:
            return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
        case MouseButton::Middle:
            return (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
        default:
            return false;
    }
}

bool PlatformBridge::Input::GetMouseWindowPosition(HWND window, int32_t& x, int32_t& y)
{
    if (window == nullptr)
        return false;

    POINT point;

    if (!GetCursorPos(&point))
        return false;

    if (!ScreenToClient(window, &point))
        return false;

    x = point.x;
    y = point.y;
    return true;
}

bool PlatformBridge::Input::IsWindowUnderMouse(HWND window)
{
    if (!window)
        return false;

    POINT point;

    if (!GetCursorPos(&point))
        return false;

    const HWND underMouse { WindowFromPoint(point) }; //the window a click at the cursor would reach. Hidden windows are skipped

    if (!underMouse)
        return false;

    return GetAncestor(underMouse, GA_ROOT) == window; //that can be a child window, so its top-level window is compared
}

std::chrono::milliseconds PlatformBridge::Input::GetDoubleClickTime()
{
    return std::chrono::milliseconds(::GetDoubleClickTime()); //the Win32 function, not this one
}

PlatformBridge::MousePressState PlatformBridge::Input::GetMousePressState(const MouseButton button)
{
    std::scoped_lock lock(_inputMutex);

    const uint32_t mask = static_cast<uint32_t>(button);
    const bool isDown = IsMouseButtonDown(button);
    const bool wasDown = (static_cast<uint32_t>(_previousMouseButtons) & mask) != 0;

    _previousMouseButtons = isDown
        ? static_cast<MouseButton>(static_cast<uint32_t>(_previousMouseButtons) | mask)
        : static_cast<MouseButton>(static_cast<uint32_t>(_previousMouseButtons) & ~mask);

    if (!isDown)
        return MousePressState::Release;

    return wasDown ? MousePressState::Repeat : MousePressState::Press;
}