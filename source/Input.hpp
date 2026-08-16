/* ToDo:
- ensure _inputStringBuffer is never reallocated



*/
#pragma once
#include <string>
#include <string_view>
#include <cstdint>
#include <thread>
#include <atomic>
#include <mutex>
#include <unordered_set>
#include <unordered_map>
#ifdef _WIN32
    #include <Windows.h>
#endif

namespace PlatformBridge
{
    enum class KeyPressState : uint32_t
    {
        Release,
        Press,
        Repeat
    };

    enum class KeyboardUseState : uint32_t
    {
        KeyReleased,
        KeyPressed,
        SameKeyPressed
    };

    enum class MouseButton : uint32_t
    {
        Left = 1,
        Right = 1 << 1,
        Middle = 1 << 2
    };

    /*enum class ModifierKey : uint32_t
    {
        Released = 0,
        LeftCRTL = 1,
        RightCTRL = 1 << 1,
        LeftShift = 1 << 2,
        RightShift = 1 << 3,
        CapsLock = 1 << 4,
        Alt = 1 << 5,
        AltGr = 1 << 6,
        Tab = 1 << 7,
        Enter = 1 << 8,
        Backspace = 1 << 9,
        ArrowLeft = 1 << 10,
        ArrowRight = 1 << 11,
        ArrowUp = 1 << 12,
        ArrowDown = 1 << 13,
        Escape = 1 << 14,
        Delete = 1 << 15,
        Insert = 1 << 16,
        F1 = 1 << 17,
        F2 = 1 << 18,
        F3 = 1 << 19,
        F4 = 1 << 18,
        F5 = 1 << 19,
        F6 = 1 << 20,
        F7 = 1 << 21,
        F8 = 1 << 22,
        F9 = 1 << 21,
        F10 = 1 << 22,
        F11 = 1 << 23,
        F12 = 1 << 24,
        Super = 1 << 25,
        NumpadEnter = 1 << 26
    };*/

    class Input
    {
    public:
        /// @brief Refreshes the Input module.
        static void Refresh();

        /// @brief Stops the key scan thread.
        static void Stop();

        //static int32_t GetKeyCode(const uint32_t window);

        /// @brief Returns the utf8-formatted input string.
        /// @return An std::string representing the eutf-8 formatted input string.
        static const std::string& GetInputString();

        //static int32_t GetInputLanguage();

        /// @brief Sets the current window to capture key strokes from..
#ifdef __linux__
        static void SetActiveWindow(const uint64_t window, const uint64_t rawWindow = 0);
#elif _WIN32
        static void SetActiveWindow(HWND window);
#endif

        /// @brief Sets the current display to capture key strokes from..
        static void SetActiveDisplay(void* display);

        /// @brief Get KeyPressState of passed key symbol.
        /// @return Returns a KeyPressState (Release, Press, Repeat).
        static KeyPressState GetKeyPressState(const uint32_t key);

        /// @brief Get whether the given key is currently held down, independent of whatever other key was
        /// pressed most recently. Unlike GetKeyPressState (which only ever tracks a single "last" key and
        /// so can't represent two keys held at once), this lets modifier + key combos (e.g. Ctrl+C) be
        /// checked correctly.
        /// @return true if the key is currently pressed.
        static bool IsKeyDown(const uint32_t key);

        /// @brief Get the virtual key symbol of whichever key most recently produced a KeyPressState/text
        /// update (i.e. the key GetKeyPressState/GetInputString currently reflect).
        /// @return The last key's symbol, or 0 if no key is currently tracked as "last".
        static uint32_t GetLastKeySym();

        /// @brief Get how many times the given key has transitioned from released to pressed since input
        /// capture started. Unlike GetKeyPressState (a per-frame snapshot that stays "Press" for as long as
        /// the key remains the most recently pressed one, and so can't distinguish "still holding" from "was
        /// released and pressed again before this was polled"), this counter only advances on a genuine
        /// release-to-press edge, so comparing it against a previously-seen value reliably detects every
        /// distinct press even if polling is too infrequent to observe the release in between.
        /// @return The number of press edges recorded for this key so far.
        static uint32_t GetKeyPressCount(const uint32_t key);

        /// @brief Get KeyboardUseState.
        /// @return Returns a KeyboardUseState (KeyReleased, KeyPressed, KeyRepeated).
        static KeyboardUseState GetKeyboardUseState();

        /// @brief Get the active display.
        /// @return Returns the void pointer of the active display.
        static void* GetActiveDisplay();

        /// @brief Get the active window ID.
        /// @return Returns the active window ID as a pointer-sized integer.
        static uint64_t GetActiveWindowID();

        /// @brief Get whether the given mouse button is currently held down.
        /// @return true if the button is currently pressed.
        static bool IsMouseButtonDown(const MouseButton button);

        /// @brief Get the mouse cursor position relative to the given native window's client area, in the native
        /// windowing coordinate convention (origin top-left, Y increasing downward). Callers using a bottom-up
        /// coordinate space (e.g. OpenGL) are responsible for flipping Y themselves - PlatformBridge stays
        /// agnostic to whichever graphics API is consuming the coordinates.
        /// @return false if the position could not be determined (e.g. an invalid window).
#ifdef __linux__
        static bool GetMouseWindowPosition(const uint64_t window, int32_t& x, int32_t& y);
#elif _WIN32
        static bool GetMouseWindowPosition(HWND window, int32_t& x, int32_t& y);
#endif

    private:
        Input() = default;
        Input(const Input&) = delete;
        Input& operator=(const Input&) = delete;
        Input(const Input&&) = delete;
        Input& operator=(const Input&&) = delete;
        ~Input();
        static Input& GetInstance()
        {
            static Input Instance;
            return Instance;
        }
#ifdef __linux__
        static void initThread();
        static void captureKeyStroke();
#elif _WIN32
        static void installSubclass(HWND window);
        static void removeSubclass();
        static LRESULT CALLBACK SubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
        static inline std::string _inputStringBuffer;
        //static inline int32_t _inputLanguage;
#ifdef __linux__
        static inline uint64_t
            _rootWindow { 0 },
            _currentWindow { 0 },
            _rawWindow { 0 };
#elif _WIN32
        static inline HWND _currentWindow { nullptr };
        static inline HWND _subclassedWindow { nullptr };
        static inline WNDPROC _originalWndProc { nullptr };
#endif
        static inline uint32_t _lastKeySym { 0 };
        static inline std::unordered_set<uint32_t> _heldKeys;
        static inline std::unordered_map<uint32_t, uint32_t> _keyPressCounts;
        static inline void
            * _display { nullptr },
            * _rawDisplay { nullptr };
        static inline bool _ownsDisplay { false };
#ifdef __linux__
        static inline std::thread _keyboardInputThread;
        static inline std::atomic<bool> _running { false };
#endif
        static inline std::mutex _inputMutex;
        static inline KeyPressState _commonKeyState;
        static inline KeyboardUseState _keyboardUseState;
        //static inline ModifierKey _modifierKeyState;
#ifdef __linux__
        static inline int32_t
            _mouseScreenX { 0 },
            _mouseScreenY { 0 };
        static inline uint32_t _mouseButtonStateMask { 0 };
#endif
    };
}