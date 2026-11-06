#include "Input.hpp"
#include <Windows.h>
#include <CommCtrl.h>
#include <cmath>
#include <print>
#include <utility>

#pragma comment(lib, "comctl32.lib")

namespace
{
    //WM_KEYDOWN/WM_KEYUP report Ctrl/Shift/Alt as the generic VK_CONTROL/VK_SHIFT/VK_MENU, not the
    //left/right-specific VK_LCONTROL/VK_RCONTROL/etc. that PB_KEY_CONTROL_L/R and friends map to - unlike
    //WH_KEYBOARD_LL's KBDLLHOOKSTRUCT::vkCode, which already gave the specific side directly. Resolve it the
    //same way the hook effectively did, from the message's scan code + extended-key bit
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
    _inputStringBuffer.clear();
    _pendingInputString.clear();
    _keyStrokeCounts.clear();
    _pendingKeyStrokeCounts.clear();
    _wheelDelta = WheelDelta {};
    _pendingWheelDelta = WheelDelta {};
    _pinchScale = 1.0f;
    _pendingPinchScale = 1.0f;
    _gestureDistance = 0;

    if (!_messageHook)
        return;

    UnhookWindowsHookEx(_messageHook);
    _messageHook = nullptr;
}

void PlatformBridge::Input::NewFrame()
{
    installMessageHook();

    std::scoped_lock lock(_inputMutex);
    _inputStringBuffer.swap(_pendingInputString);
    _pendingInputString.clear();
    _keyStrokeCounts.swap(_pendingKeyStrokeCounts);
    _pendingKeyStrokeCounts.clear();
    _wheelDelta = std::exchange(_pendingWheelDelta, WheelDelta {});
    _pinchScale = std::exchange(_pendingPinchScale, 1.0f);
}

void PlatformBridge::Input::installMessageHook()
{
    if (_messageHook)
        return;

    //One hook for the thread that calls NewFrame, which made the windows: the wheel goes to whichever of them lies under the
    //mouse (or has the focus), so none of them needs registering
    _messageHook = SetWindowsHookExW(WH_GETMESSAGE, messageHookProc, nullptr, GetCurrentThreadId());
}

LRESULT CALLBACK PlatformBridge::Input::messageHookProc(int code, WPARAM wParam, LPARAM lParam)
{
    //Only messages taken off the queue: a peek that leaves one there would count it twice
    if (HC_ACTION == code && PM_REMOVE == wParam)
    {
        const MSG& message { *reinterpret_cast<const MSG*>(lParam) };

        switch (message.message)
        {
            case WM_MOUSEWHEEL:
            case WM_MOUSEHWHEEL:
                readWheel(message);
                break;
            case WM_GESTURE:
                readGesture(message);
                break;
            default:
                break;
        }
    }

    return CallNextHookEx(nullptr, code, wParam, lParam);
}

void PlatformBridge::Input::readWheel(const MSG& message)
{
    const float notches { static_cast<float>(GET_WHEEL_DELTA_WPARAM(message.wParam)) / static_cast<float>(WHEEL_DELTA) };
    const WORD keys { GET_KEYSTATE_WPARAM(message.wParam) }; //as they were when the wheel turned
    std::scoped_lock lock(_inputMutex);

    if (WM_MOUSEHWHEEL == message.message)
    {
        _pendingWheelDelta._X += notches;
        return;
    }

    //A precision touchpad sends its pinch as Ctrl+wheel, and a wheel turned with Ctrl zooms the same way, as in browsers
    if (0 != (keys & MK_CONTROL))
    {
        _pendingPinchScale *= std::pow(_pinchScalePerNotch, notches);
        return;
    }

    //Shift turns the wheel sideways, as in most programs. Turning it toward the user scrolls right
    if (0 != (keys & MK_SHIFT))
    {
        _pendingWheelDelta._X -= notches;
        return;
    }

    _pendingWheelDelta._Y += notches;
}

void PlatformBridge::Input::readGesture(const MSG& message)
{
    GESTUREINFO gesture {};
    gesture.cbSize = sizeof(GESTUREINFO);

    //The handle stays open: the window procedure the message goes on to closes it
    if (!GetGestureInfo(reinterpret_cast<HGESTUREINFO>(message.lParam), &gesture))
        return;

    if (GID_ZOOM != gesture.dwID)
        return;

    const uint64_t distance { gesture.ullArguments }; //between the two fingers (pixels)
    std::scoped_lock lock(_inputMutex);

    if (0 != (gesture.dwFlags & GF_BEGIN) || 0 == _gestureDistance || 0 == distance)
    {
        _gestureDistance = distance;
        return;
    }

    _pendingPinchScale *= static_cast<float>(distance) / static_cast<float>(_gestureDistance);
    _gestureDistance = distance;
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

            _keyboardUseState = (_lastKeySym == vkCode)
                ? KeyboardUseState::SameKeyPressed
                : KeyboardUseState::KeyPressed;
            _lastKeySym = vkCode;

            //Only a genuine released->pressed edge counts as a new press - OS auto-repeat re-fires
            //WM_KEYDOWN for a key that's already held, and _heldKeys distinguishes the two.
            if (!_heldKeys.contains(vkCode))
                ++_keyPressCounts[vkCode];

            ++_pendingKeyStrokeCounts[vkCode]; //repeats included
            _heldKeys.insert(vkCode);
        } break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            const uint32_t vkCode = resolveExtendedVirtualKey(wParam, lParam);
            std::scoped_lock lock(_inputMutex);
            _heldKeys.erase(vkCode);

            //Letting go of another key, like Shift, leaves the last one pressed alone, which may still be held and repeating
            if (_lastKeySym == vkCode)
                _lastKeySym = 0;

            if (_heldKeys.empty())
                _keyboardUseState = KeyboardUseState::KeyReleased;
        } break;
        case WM_CHAR:
        case WM_SYSCHAR:
        {
            const wchar_t character { static_cast<wchar_t>(wParam) };
            std::scoped_lock lock(_inputMutex);

            //characters past U+FFFF (emoji and other stuff) come as two WM_CHARs. Each half alone converts to U+FFFD, so the first half requires second
            if (IS_HIGH_SURROGATE(character))
            {
                _pendingHighSurrogate = character;
                break;
            }

            const wchar_t pair[2] { _pendingHighSurrogate, character };
            const bool isPair { 0 != _pendingHighSurrogate && IS_LOW_SURROGATE(character) };
            _pendingHighSurrogate = 0;

            char utf8[4];
            const int32_t length { WideCharToMultiByte(CP_UTF8, 0, isPair ? pair : &character, isPair ? 2 : 1, utf8, static_cast<int>(sizeof(utf8)), nullptr, nullptr) };
            _pendingInputString.append(utf8, length); //everything typed until the next NewFrame
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
    return _inputStringBuffer; //only NewFrame writes it, on the same thread as the readers
}

PlatformBridge::WheelDelta PlatformBridge::Input::GetWheelDelta()
{
    std::scoped_lock lock(_inputMutex);
    return _wheelDelta;
}

float PlatformBridge::Input::GetPinchScale()
{
    std::scoped_lock lock(_inputMutex);
    return _pinchScale;
}

uint32_t PlatformBridge::Input::GetWheelScrollLines()
{
    UINT lines { 3 };

    if (!SystemParametersInfoW(SPI_GETWHEELSCROLLLINES, 0, &lines, 0))
        return 3;

    if (WHEEL_PAGESCROLL == lines)
        return 0;

    return static_cast<uint32_t>(lines);
}

uint32_t PlatformBridge::Input::GetKeyStrokeCount(const uint32_t key)
{
    std::scoped_lock lock(_inputMutex);
    const auto it = _keyStrokeCounts.find(key);
    return it != _keyStrokeCounts.end() ? it->second : 0;
}

void PlatformBridge::Input::SetActiveWindow(HWND window)
{
    std::scoped_lock lock(_inputMutex);

    //The subclass is a capture mechanism (not a focus indicator). It stays installed on whichever window last
    //had it regardless of the window being 0 here, so held-key/text tracking stays accurate no matter how often
    //SetActiveWindow(0) is called (e.g. every frame the mouse isn't hovering a given TextBox). _currentWindow
    //is the separate, freely toggled "active window" ID exposed via GetActiveWindowID()
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