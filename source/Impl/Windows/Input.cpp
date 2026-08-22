#include "Input.hpp"
#include <Windows.h>
#include <print>

namespace
{
    std::string utf8FromWide(const std::wstring& text)
    {
        if (text.empty())
            return {};

        const int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0, nullptr, nullptr);
        std::string result(sizeNeeded, '\0');
        WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), sizeNeeded, nullptr, nullptr);
        return result;
    }

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
    _inputStringBuffer.reserve(64);

    std::scoped_lock lock(_inputMutex);
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
    _originalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(SubclassProc)));
    _subclassedWindow = window;
}

void PlatformBridge::Input::removeSubclass()
{
    if (_subclassedWindow != nullptr && _originalWndProc != nullptr)
        SetWindowLongPtrW(_subclassedWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(_originalWndProc));

    _subclassedWindow = nullptr;
    _originalWndProc = nullptr;
}

LRESULT CALLBACK PlatformBridge::Input::SubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
            std::scoped_lock lock(_inputMutex);
            _inputStringBuffer = utf8FromWide(std::wstring(1, static_cast<wchar_t>(wParam)));
        } break;
        default:
            break;
    }

    return CallWindowProcW(_originalWndProc, hwnd, msg, wParam, lParam);
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