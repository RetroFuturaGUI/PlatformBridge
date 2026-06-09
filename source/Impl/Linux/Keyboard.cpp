#include "Keyboard.hpp"
#include "LinuxHeaders.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <print>
#include <chrono>
#include <thread>
#include <format>

void PlatformBridge::Keyboard::Refresh()
{
    _inputStringBuffer.reserve(64);
    _display = XOpenDisplay(nullptr);

    if (!_display) 
    {
        std::println("Cannot open display");
        return;
    }

    _rootWindow = DefaultRootWindow(_display);
    XEvent event {};
    _currentWindow = _rootWindow;
    XGrabKeyboard((Display*)_display, _rootWindow, False, GrabModeAsync, GrabModeAsync, CurrentTime);
    initThread();
}

void PlatformBridge::Keyboard::Stop()
{
    _running = false;

    if(_inputThread.joinable())
        _inputThread.join();

    if (_display)
        XUngrabKeyboard((Display*)_display, CurrentTime);
}

void PlatformBridge::Keyboard::initThread()
{
    if(_inputThread.get_id() != std::thread::id())
        return;

    _running = true;
    _inputThread = std::thread(captureKeyStroke);
}

PlatformBridge::Keyboard::~Keyboard()
{
    Stop();

    if (!_display)
        return;

    XCloseDisplay(static_cast<Display*>(_display));
    _display = nullptr;
}

void PlatformBridge::Keyboard::captureKeyStroke()
{
    XEvent event {};
    char buffer[64];

    while (_running)
    {
        if (!_display)
            break;

        if (XPending((Display*)_display) == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }

        XNextEvent((Display*)_display, &event);

        if (event.type != KeyPress)
        {
            _keyboardUseState = KeyboardUseState::KeyReleased;
            _lastKeySym = 0;
            continue;
        }
        
        _keyboardUseState = KeyboardUseState::KeyPressed;
        KeySym keysym;
        XLookupString(&event.xkey, buffer, sizeof(buffer), &keysym, nullptr);
        _inputStringBuffer = buffer;
        /*std::println("Key pressed:\n    Window ID: {}\n    Key String: {}\n    Key Code: {}", 
                        std::format("0x{:X}", _currentWindow), 
                        _inputStringBuffer, 
                        std::format("0x{:X}", keysym)
                    );*/
    
        if(_lastKeySym == keysym)
        {
            _keyboardUseState = KeyboardUseState::SameKeyPressed;
            return;
        }
                    
        _lastKeySym = keysym;
    }
}


PlatformBridge::KeyboardUseState PlatformBridge::Keyboard::GetKeyboardUseState()
{
    return _keyboardUseState;
}

const std::string& PlatformBridge::Keyboard::GetInputString()
{
    return _inputStringBuffer;
}

void PlatformBridge::Keyboard::SetActiveWindow(const uint32_t window)
{
    if (!_display)
        return;

    if (window == 0)
    {
        XUngrabKeyboard((Display*)_display, CurrentTime);
        _currentWindow = 0;
        return;
    }

    if (_currentWindow == window)
        return;

    XUngrabKeyboard((Display*)_display, CurrentTime);
    _currentWindow = window;
    XGrabKeyboard((Display*)_display, _currentWindow, False, GrabModeAsync, GrabModeAsync, CurrentTime);
}

PlatformBridge::KeyPressState PlatformBridge::Keyboard::GetKeyPressState(const uint32_t key)
{
    if(_lastKeySym == key && _keyboardUseState == KeyboardUseState::SameKeyPressed)
        return KeyPressState::Repeat;

    if(_lastKeySym == key)
        return KeyPressState::Press;
        
    return KeyPressState::Release;
}
