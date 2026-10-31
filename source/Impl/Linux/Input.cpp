#include "Input.hpp"
#include "LinuxHeaders.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <print>
#include <chrono>
#include <cmath>
#include <thread>
#include <format>
#include <string_view>
#include <utility>

void PlatformBridge::Input::Refresh()
{
    {
        std::scoped_lock lock(_inputMutex);
        _inputStringBuffer.reserve(64);

        if (!_display)
        {
            _display = XOpenDisplay(nullptr);
            _rawDisplay = _display;
            _ownsDisplay = (_display != nullptr);
        }

        _currentWindow = 0;
        _rawWindow = 0;
    }

    if (!_display)
    {
        std::println("Cannot open display");
        return;
    }

    {
        std::scoped_lock lock(_inputMutex);
        _rootWindow = DefaultRootWindow(static_cast<Display*>(_display));
    }

    initThread();
}

void PlatformBridge::Input::Stop()
{
    _running = false;

    if(_keyboardInputThread.joinable())
        _keyboardInputThread.join();

    std::scoped_lock lock(_inputMutex);

    if (_display && _currentWindow != 0)
        XSelectInput(static_cast<Display*>(_display), static_cast<Window>(_currentWindow), NoEventMask);

    _heldKeys.clear();
    _keyPressCounts.clear();
    _inputStringBuffer.clear();
    _pendingInputString.clear();
    _keyStrokeCounts.clear();
    _pendingKeyStrokeCounts.clear();
    _wheelDelta = WheelDelta {};
    _pendingWheelDelta = WheelDelta {};
    _pinchScale = 1.0f;
    _pendingPinchScale = 1.0f;
}

void PlatformBridge::Input::NewFrame()
{
    //The key thread keeps writing the pending values; the frame's values are the GUI thread's alone, so GetInputString can hand out its buffer
    std::scoped_lock lock(_inputMutex);
    _inputStringBuffer.swap(_pendingInputString);
    _pendingInputString.clear();
    _keyStrokeCounts.swap(_pendingKeyStrokeCounts);
    _pendingKeyStrokeCounts.clear();
    _wheelDelta = std::exchange(_pendingWheelDelta, WheelDelta {});
    _pinchScale = std::exchange(_pendingPinchScale, 1.0f);
}

void PlatformBridge::Input::initThread()
{
    if(_keyboardInputThread.get_id() != std::thread::id())
        return;

    _running = true;
    _keyboardInputThread = std::thread(captureKeyStroke);
}

PlatformBridge::Input::~Input()
{
    Stop();

    if (_display && _ownsDisplay)
        XCloseDisplay(static_cast<Display*>(_display));

    _display = nullptr;
    _rawDisplay = nullptr;
    _ownsDisplay = false;
}

void PlatformBridge::Input::captureKeyStroke()
{
    XEvent event {};
    char buffer[64];
    void* display;
    uint64_t currentWindow, rootWindow;

    while (_running)
    {
        {
            std::scoped_lock lock(_inputMutex);
            display = _display;
            currentWindow = _currentWindow;
            rootWindow = _rootWindow;
        }

        if (display && rootWindow != 0)
        {
            Window rootReturn, childReturn;
            int rootX, rootY, windowX, windowY;
            unsigned int maskReturn;

            if (XQueryPointer(static_cast<Display*>(display), static_cast<Window>(rootWindow),
                &rootReturn, &childReturn, &rootX, &rootY, &windowX, &windowY, &maskReturn))
            {
                std::scoped_lock lock(_inputMutex);
                _mouseScreenX = rootX;
                _mouseScreenY = rootY;
                _mouseButtonStateMask = 0;

                if (maskReturn & Button1Mask)
                    _mouseButtonStateMask |= static_cast<uint32_t>(MouseButton::Left);

                if (maskReturn & Button2Mask)
                    _mouseButtonStateMask |= static_cast<uint32_t>(MouseButton::Middle);

                if (maskReturn & Button3Mask)
                    _mouseButtonStateMask |= static_cast<uint32_t>(MouseButton::Right);
            }
        }

        if (!display || currentWindow == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }

        if (XPending(static_cast<Display*>(display)) == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }

        XNextEvent(static_cast<Display*>(display), &event);

        //The GUI thread reads all of this, so every write below is locked
        std::scoped_lock lock(_inputMutex);

        switch(event.type)
        {
            case KeyPress:
            {
                KeySym keysym;
                const int length { XLookupString(&event.xkey, buffer, sizeof(buffer), &keysym, nullptr) }; //0 for keys without text; \b and \r for Backspace and Enter
                _pendingInputString.append(buffer, length); //buffer isn't null-terminated
                std::println("Key pressed: {} (keysym=0x{:X})", std::string_view(buffer, length), static_cast<unsigned long>(keysym));

                //Only a genuine released->pressed edge counts as a new press - X11 auto-repeat re-fires
                //KeyPress for a key that's already held, and _heldKeys distinguishes the two.
                if (!_heldKeys.contains(static_cast<uint32_t>(keysym)))
                    ++_keyPressCounts[static_cast<uint32_t>(keysym)];

                ++_pendingKeyStrokeCounts[static_cast<uint32_t>(keysym)]; //repeats included
                _heldKeys.insert(static_cast<uint32_t>(keysym));
                _keyboardUseState = _lastKeySym == keysym ? KeyboardUseState::SameKeyPressed : KeyboardUseState::KeyPressed;
                _lastKeySym = keysym;
            } break;
            case KeyRelease:
            {
                //XLookupString isn't valid here (no key text on release) - XLookupKeysym gives us just enough
                //to know which key this release belongs to, so it can be removed from _heldKeys.
                const KeySym releasedKeysym = XLookupKeysym(&event.xkey, 0);
                _heldKeys.erase(static_cast<uint32_t>(releasedKeysym));

                // releasing another key, like Shift, leaves the last one pressed alone, which may still be held and repeating
                if (_lastKeySym == releasedKeysym)
                    _lastKeySym = 0;

                if (_heldKeys.empty())
                    _keyboardUseState = KeyboardUseState::KeyReleased;

                std::println("Key released");
            } break;
            case ButtonPress:
            {
                //The wheel comes as buttons: 4 and 5 turn it up and down, 6 and 7 tilt it left and right, one notch per press
                switch (event.xbutton.button)
                {
                    case 4:
                    case 5:
                    {
                        const float notches { 4 == event.xbutton.button ? 1.0f : -1.0f };

                        //Ctrl makes it a pinch and Shift turns it sideways, as on Windows
                        if (0 != (event.xbutton.state & ControlMask))
                            _pendingPinchScale *= std::pow(_pinchScalePerNotch, notches);
                        else if (0 != (event.xbutton.state & ShiftMask))
                            _pendingWheelDelta._X -= notches;
                        else
                            _pendingWheelDelta._Y += notches;
                    } break;
                    case 6:
                        _pendingWheelDelta._X -= 1.0f;
                        break;
                    case 7:
                        _pendingWheelDelta._X += 1.0f;
                        break;
                    default:
                        std::println("Mouse button press: {}", event.xbutton.button);
                        break;
                }
            } break;
            case ButtonRelease:
            {
                std::println("Mouse button release: {}", event.xbutton.button);
            } break;
            case MotionNotify:
            {
                std::println("Mouse move: x={}, y={}", event.xmotion.x, event.xmotion.y);
            } break;
            default:
                break;
        }
    }
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
    return 3; //X11 has no system setting, so desktops default it is
}

uint32_t PlatformBridge::Input::GetKeyStrokeCount(const uint32_t key)
{
    std::scoped_lock lock(_inputMutex);
    const auto it = _keyStrokeCounts.find(key);
    return it != _keyStrokeCounts.end() ? it->second : 0;
}

void PlatformBridge::Input::SetActiveWindow(const uint64_t window, const uint64_t rawWindow)
{
    std::scoped_lock lock(_inputMutex);

    if (!_display)
    {
        _currentWindow = 0;
        _rawWindow = 0;
        return;
    }

    if (window == 0)
    {
        if (_currentWindow != 0)
        {
            XSelectInput(static_cast<Display*>(_display), static_cast<Window>(_currentWindow), NoEventMask);
        }

        _currentWindow = 0;
        _rawWindow = 0;
        return;
    }

    if (_currentWindow == window && _rawWindow == rawWindow)
        return;

    _currentWindow = window;
    _rawWindow = rawWindow;

    XSelectInput(
        static_cast<Display*>(_display),
        static_cast<Window>(_currentWindow),
        KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask
    );
    XFlush(static_cast<Display*>(_display));
    std::println("X11 active window set to 0x{:X}", static_cast<unsigned long>(window));
}

void PlatformBridge::Input::SetActiveDisplay(void* display)
{
    std::scoped_lock lock(_inputMutex);

    if (display == nullptr)
    {
        if (_display && _ownsDisplay)
        {
            XCloseDisplay(static_cast<Display*>(_display));
        }

        _display = nullptr;
        _rawDisplay = nullptr;
        _ownsDisplay = false;
        _currentWindow = 0;
        _rawWindow = 0;
        return;
    }

    if (_display && _ownsDisplay && _display != display)
    {
        XCloseDisplay(static_cast<Display*>(_display));
        _display = nullptr;
    }

    _display = display;
    _rawDisplay = display;
    _ownsDisplay = false;
    std::println("X11 active display set");
}

void* PlatformBridge::Input::GetActiveDisplay()
{
    return _display;
}

uint64_t PlatformBridge::Input::GetActiveWindowID()
{
    return _currentWindow;
}

PlatformBridge::KeyPressState PlatformBridge::Input::GetKeyPressState(const uint32_t key)
{
    if(_lastKeySym == key && _keyboardUseState == KeyboardUseState::SameKeyPressed)
        return KeyPressState::Repeat;

    if(_lastKeySym == key)
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
    std::scoped_lock lock(_inputMutex);

    if (!_display)
        return false;

    Display* display { static_cast<Display*>(_display) };
    const KeyCode code { XKeysymToKeycode(display, static_cast<KeySym>(key)) };

    if (0 == code)
        return false;

    //One bit per key code, as the server sees the keyboard now
    char keys[32] {};
    XQueryKeymap(display, keys);
    return 0 != (keys[code / 8] & (1 << (code % 8)));
}

bool PlatformBridge::Input::IsMouseButtonDown(const MouseButton button)
{
    std::scoped_lock lock(_inputMutex);
    return (_mouseButtonStateMask & static_cast<uint32_t>(button)) != 0;
}

PlatformBridge::MousePressState PlatformBridge::Input::GetMousePressState(const MouseButton button)
{
    std::scoped_lock lock(_inputMutex);

    const uint32_t mask = static_cast<uint32_t>(button);
    const bool isDown = (_mouseButtonStateMask & mask) != 0;
    const bool wasDown = (static_cast<uint32_t>(_previousMouseButtons) & mask) != 0;

    _previousMouseButtons = isDown
        ? static_cast<MouseButton>(static_cast<uint32_t>(_previousMouseButtons) | mask)
        : static_cast<MouseButton>(static_cast<uint32_t>(_previousMouseButtons) & ~mask);

    if (!isDown)
        return MousePressState::Release;

    return wasDown ? MousePressState::Repeat : MousePressState::Press;
}

bool PlatformBridge::Input::GetMouseWindowPosition(const uint64_t window, int32_t& x, int32_t& y)
{
    std::scoped_lock lock(_inputMutex);

    if (!_display || window == 0)
        return false;

    Display* display = static_cast<Display*>(_display);
    Window childReturn;
    int relativeX = 0, relativeY = 0;

    if (!XTranslateCoordinates(display, DefaultRootWindow(display), static_cast<Window>(window),
        _mouseScreenX, _mouseScreenY, &relativeX, &relativeY, &childReturn))
        return false;

    x = relativeX;
    y = relativeY;
    return true;
}

bool PlatformBridge::Input::IsWindowUnderMouse(const uint64_t window)
{
    std::scoped_lock lock(_inputMutex);

    if (!_display || 0 == window)
        return false;

    Display* display = static_cast<Display*>(_display);

    //From the root down through the windows under the pointer, a window manager's frame included: the window is the one under the mouse when it lies on that path
    Window current = DefaultRootWindow(display);

    while (0 != current)
    {
        if (static_cast<Window>(window) == current)
            return true;

        Window
            rootReturn = 0,
            childReturn = 0;
        int
            rootX = 0,
            rootY = 0,
            windowX = 0,
            windowY = 0;
        unsigned int mask = 0;

        if (!XQueryPointer(display, current, &rootReturn, &childReturn, &rootX, &rootY, &windowX, &windowY, &mask))
            return false;

        current = childReturn;
    }

    return false;
}

std::chrono::milliseconds PlatformBridge::Input::GetDoubleClickTime()
{
    return std::chrono::milliseconds(400); //X11 has no system setting; desktops default to about this
}