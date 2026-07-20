#include "Input.hpp"
#include <Windows.h>
#include <chrono>
#include <print>
#include <thread>

namespace
{
    HHOOK _keyboardHook { nullptr };

    std::string utf8FromWide(const std::wstring& text)
    {
        if (text.empty())
            return {};

        const int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0, nullptr, nullptr);
        std::string result(sizeNeeded, '\0');
        WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), sizeNeeded, nullptr, nullptr);
        return result;
    }
}

void PlatformBridge::Input::Refresh()
{
    _inputStringBuffer.reserve(64);

    {
        std::scoped_lock lock(_inputMutex);
        _currentWindow = GetForegroundWindow();
        _display = nullptr;
        _rawDisplay = nullptr;
        _ownsDisplay = false;
        _keyboardUseState = KeyboardUseState::KeyReleased;
    }

    initThread();
}

void PlatformBridge::Input::Stop()
{
    _running = false;

    if (_inputThread.joinable())
        _inputThread.join();

    std::scoped_lock lock(_inputMutex);
    if (_keyboardHook != nullptr)
    {
        UnhookWindowsHookEx(_keyboardHook);
        _keyboardHook = nullptr;
    }

    _keyboardUseState = KeyboardUseState::KeyReleased;
    _lastKeySym = 0;
}

void PlatformBridge::Input::initThread()
{
    if (_inputThread.get_id() != std::thread::id())
        return;

    if (_keyboardHook == nullptr)
    {
        _keyboardHook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandleW(nullptr), 0);

        if (_keyboardHook == nullptr)
        {
            std::println("ERROR::INPUT: Failed to install keyboard hook. Error={}", GetLastError());
            return;
        }
    }

    _running = true;
    _inputThread = std::thread(captureKeyStroke);
}

PlatformBridge::Input::~Input()
{
    Stop();
}

void PlatformBridge::Input::captureKeyStroke()
{
    while (_running)
    {
        {
            std::scoped_lock lock(_inputMutex);
            
            if (_currentWindow == nullptr)
                _currentWindow = GetForegroundWindow();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

LRESULT CALLBACK PlatformBridge::Input::KeyboardProc(const int nCode, const WPARAM wParam, const LPARAM lParam)
{
    if (nCode >= 0)
    {
        const auto* keyboardHook = reinterpret_cast<const KBDLLHOOKSTRUCT*>(lParam);
        auto& instance = GetInstance();

        if (instance._currentWindow != nullptr && GetForegroundWindow() != instance._currentWindow)
            return CallNextHookEx(nullptr, nCode, wParam, lParam);

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            BYTE keyboardState[256] = {};
            if (GetKeyboardState(keyboardState))
            {
                wchar_t buffer[8] = {};
                const HKL layout = GetKeyboardLayout(0);
                const int charCount = ToUnicodeEx(
                    keyboardHook->vkCode,
                    keyboardHook->scanCode,
                    keyboardState,
                    buffer,
                    static_cast<int>(std::size(buffer)),
                    0,
                    layout);

                if (charCount > 0)
                {
                    std::scoped_lock lock(instance._inputMutex);
                    instance._inputStringBuffer = utf8FromWide(std::wstring(buffer, charCount));
                    instance._keyboardUseState = (instance._lastKeySym == keyboardHook->vkCode)
                        ? KeyboardUseState::SameKeyPressed
                        : KeyboardUseState::KeyPressed;
                    instance._lastKeySym = static_cast<uint32_t>(keyboardHook->vkCode);
                }
            }
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            std::scoped_lock lock(instance._inputMutex);
            instance._keyboardUseState = KeyboardUseState::KeyReleased;
            instance._lastKeySym = 0;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
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