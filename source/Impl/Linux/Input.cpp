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

    if(_inputThread.joinable())
        _inputThread.join();

    std::scoped_lock lock(_inputMutex);

    if (_display && _currentWindow != 0)
        XSelectInput(static_cast<Display*>(_display), static_cast<Window>(_currentWindow), NoEventMask);
}

void PlatformBridge::Input::initThread()
{
    if(_inputThread.get_id() != std::thread::id())
        return;

    _running = true;
    _inputThread = std::thread(captureKeyStroke);
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
    uint64_t currentWindow;

    while (_running)
    {
        {
            std::scoped_lock lock(_inputMutex);
            display = _display;
            currentWindow = _currentWindow;
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
