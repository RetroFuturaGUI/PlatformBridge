#include "Input.hpp"
#include "LinuxHeaders.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <print>
#include <chrono>
#include <thread>
#include <format>

void PlatformBridge::Input::Refresh()
{
    _inputStringBuffer.reserve(64);

    {
        std::scoped_lock lock(_inputMutex);
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

        switch(event.type)
        {
            case KeyPress:
            {
                _keyboardUseState = KeyboardUseState::KeyPressed;
                KeySym keysym;
                XLookupString(&event.xkey, buffer, sizeof(buffer), &keysym, nullptr);
                _inputStringBuffer = buffer;
                std::println("Key pressed: {} (keysym=0x{:X})", _inputStringBuffer, static_cast<unsigned long>(keysym));

                _heldKeys.insert(static_cast<uint32_t>(keysym));

                if(_lastKeySym == keysym)
                {
                    _keyboardUseState = KeyboardUseState::SameKeyPressed;
                    continue;
                }

                _lastKeySym = keysym;
            } break;
            case KeyRelease:
            {
                _keyboardUseState = KeyboardUseState::KeyReleased;
                _lastKeySym = 0;

                //XLookupString isn't valid here (no key text on release) - XLookupKeysym gives us just enough
                //to know which key this release belongs to, so it can be removed from _heldKeys.
                const KeySym releasedKeysym = XLookupKeysym(&event.xkey, 0);
                _heldKeys.erase(static_cast<uint32_t>(releasedKeysym));

                std::println("Key released");
            } break;
            case ButtonPress:
            {
                std::println("Mouse button press: {}", event.xbutton.button);
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
    return _inputStringBuffer;
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

bool PlatformBridge::Input::IsKeyDown(const uint32_t key)
{
    std::scoped_lock lock(_inputMutex);
    return _heldKeys.contains(key);
}

bool PlatformBridge::Input::IsMouseButtonDown(const MouseButton button)
{
    std::scoped_lock lock(_inputMutex);
    return (_mouseButtonStateMask & static_cast<uint32_t>(button)) != 0;
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
