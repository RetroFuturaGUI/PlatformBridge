#include "Input.hpp"
#include <Windows.h>
#include <print>
#include <thread>

namespace
{
    HHOOK
        _keyboardHook { nullptr },
        _mouseHook { nullptr };

    DWORD
        _keyboardThreadId { 0 },
        _mouseThreadId { 0 };

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

    if (_keyboardThreadId != 0)
        PostThreadMessageW(_keyboardThreadId, WM_QUIT, 0, 0);

    if (_mouseThreadId != 0)
        PostThreadMessageW(_mouseThreadId, WM_QUIT, 0, 0);

    if (_keyboardInputThread.joinable())
        _keyboardInputThread.join();

    if (_mouseInputThread.joinable())
        _mouseInputThread.join();

    std::scoped_lock lock(_inputMutex);
    _keyboardUseState = KeyboardUseState::KeyReleased;
    _lastKeySym = 0;
    _heldKeys.clear();
    _mouseButtonStateMask = 0;
}

void PlatformBridge::Input::initThread()
{
    if (_keyboardInputThread.get_id() == std::thread::id())
    {
        _running = true;
        _keyboardInputThread = std::thread(captureKeyStroke);
    }

    if (_mouseInputThread.get_id() == std::thread::id())
    {
        _running = true;
        _mouseInputThread = std::thread(captureMouseInput);
    }
}

PlatformBridge::Input::~Input()
{
    Stop();
}

void PlatformBridge::Input::captureKeyStroke()
{
    _keyboardThreadId = GetCurrentThreadId();
    _keyboardHook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandleW(nullptr), 0);

    if (_keyboardHook == nullptr)
    {
        std::println("ERROR::INPUT: Failed to install keyboard hook. Error={}", GetLastError());
        return;
    }

    MSG msg;

    while (GetMessageW(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);

        std::scoped_lock lock(_inputMutex);
        if (_currentWindow == nullptr)
            _currentWindow = GetForegroundWindow();
    }

    UnhookWindowsHookEx(_keyboardHook);
    _keyboardHook = nullptr;
}

void PlatformBridge::Input::captureMouseInput()
{
    _mouseThreadId = GetCurrentThreadId();
    _mouseHook = SetWindowsHookExW(WH_MOUSE_LL, MouseProc, GetModuleHandleW(nullptr), 0);

    if (_mouseHook == nullptr)
    {
        std::println("ERROR::INPUT: Failed to install mouse hook. Error={}", GetLastError());
        return;
    }

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    UnhookWindowsHookEx(_mouseHook);
    _mouseHook = nullptr;
}

LRESULT CALLBACK PlatformBridge::Input::KeyboardProc(const int nCode, const WPARAM wParam, const LPARAM lParam)
{
    if (nCode >= 0)
    {
        const auto* keyboardHook = reinterpret_cast<const KBDLLHOOKSTRUCT*>(lParam);

        if (_currentWindow != nullptr && GetForegroundWindow() != _currentWindow)
            return CallNextHookEx(nullptr, nCode, wParam, lParam);

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            BYTE keyboardState[256] = {};
            wchar_t buffer[8] = {};
            int charCount = 0;

            //Text conversion is best-effort - if it fails, the key is still tracked as held below, it just
            //won't contribute to _inputStringBuffer this frame.
            if (GetKeyboardState(keyboardState))
            {
                const HKL layout = GetKeyboardLayout(0);
                charCount = ToUnicodeEx(
                    keyboardHook->vkCode,
                    keyboardHook->scanCode,
                    keyboardState,
                    buffer,
                    static_cast<int>(std::size(buffer)),
                    0,
                    layout);
            }

            {
                std::scoped_lock lock(_inputMutex);

                if (charCount > 0)
                    _inputStringBuffer = utf8FromWide(std::wstring(buffer, charCount));
                else
                    _inputStringBuffer.clear();

                _keyboardUseState = (_lastKeySym == keyboardHook->vkCode)
                    ? KeyboardUseState::SameKeyPressed
                    : KeyboardUseState::KeyPressed;
                _lastKeySym = static_cast<uint32_t>(keyboardHook->vkCode);
                _heldKeys.insert(static_cast<uint32_t>(keyboardHook->vkCode));
            }
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            std::scoped_lock lock(_inputMutex);
            _keyboardUseState = KeyboardUseState::KeyReleased;
            _lastKeySym = 0;
            _heldKeys.erase(static_cast<uint32_t>(keyboardHook->vkCode));
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK PlatformBridge::Input::MouseProc(const int nCode, const WPARAM wParam, const LPARAM lParam)
{
    if (nCode >= 0)
    {
        const auto* mouseHook = reinterpret_cast<const MSLLHOOKSTRUCT*>(lParam);
        std::scoped_lock lock(_inputMutex);

        switch (wParam)
        {
            case WM_MOUSEMOVE:
                _mouseScreenX = mouseHook->pt.x;
                _mouseScreenY = mouseHook->pt.y;
                break;
            case WM_LBUTTONDOWN:
                _mouseButtonStateMask |= static_cast<uint32_t>(MouseButton::Left);
                break;
            case WM_LBUTTONUP:
                _mouseButtonStateMask &= ~static_cast<uint32_t>(MouseButton::Left);
                break;
            case WM_RBUTTONDOWN:
                _mouseButtonStateMask |= static_cast<uint32_t>(MouseButton::Right);
                break;
            case WM_RBUTTONUP:
                _mouseButtonStateMask &= ~static_cast<uint32_t>(MouseButton::Right);
                break;
            case WM_MBUTTONDOWN:
                _mouseButtonStateMask |= static_cast<uint32_t>(MouseButton::Middle);
                break;
            case WM_MBUTTONUP:
                _mouseButtonStateMask &= ~static_cast<uint32_t>(MouseButton::Middle);
                break;
            default:
                [[unlikely]];
                break;
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

bool PlatformBridge::Input::GetMouseWindowPosition(HWND window, int32_t& x, int32_t& y)
{
    if (window == nullptr)
        return false;

    POINT point;

    {
        std::scoped_lock lock(_inputMutex);
        point.x = _mouseScreenX;
        point.y = _mouseScreenY;
    }

    if (!ScreenToClient(window, &point))
        return false;

    x = point.x;
    y = point.y;
    return true;
}