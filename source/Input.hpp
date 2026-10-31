#pragma once
#include <chrono>
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
        None = 0,
        Left = 1,
        Right = 1 << 1,
        Middle = 1 << 2
    };

    enum class MousePressState :uint32_t
    {
        Release,
        Press,
        Repeat
    };

    /// @brief How far the mouse wheel turned during a frame in notches: 1 per detent of a wheel, fractions from touchpads and free-spinning wheels.
    struct WheelDelta
    {
        float
            _X { 0.0f }, //to the right: tilting wheels, two-finger swipes, and a vertical wheel turned with Shift held
            _Y { 0.0f }; //up
    };

    class Input
    {
    public:
        /// @brief Refreshes the Input module.
        static void Refresh();

        /// @brief Stops the key scan thread.
        static void Stop();

        /// @brief Hands over what arrived since the last call: the typed text, the wheel, the pinch and the key strokes. Call it
        ///        once per frame on the windows' thread, before anything reads them; until the next call every read returns the same.
        static void NewFrame();

        //static int32_t GetKeyCode(const uint32_t window);

        /// @brief Returns the text typed during the last frame, in UTF-8, with the system's repeats of held keys. Backspace and Enter are in it as \b and \r, in the order they were typed.
        static const std::string& GetInputString();

        /// @brief Returns how far the wheel turned during the last frame, over any of the app's windows. Ctrl turns it into a pinch (GetPinchScale) instead.
        static WheelDelta GetWheelDelta();

        /// @brief Returns the factor two fingers spread apart by during the last frame: above 1 apart, below 1 together, 1 for none.
        ///        A touchscreen's pinch, a precision touchpad's (which Windows sends as Ctrl+wheel), and a wheel turned with Ctrl.
        static float GetPinchScale();

        /// @brief Returns how many lines one notch of the wheel scrolls, as the system is set: 3 unless changed, 0 for a page per notch.
        static uint32_t GetWheelScrollLines();

        /// @brief Returns how often a key went down during the last frame
        static uint32_t GetKeyStrokeCount(const uint32_t key);

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

        /// @brief Asks the system whether a key is down right now, whichever window or program has the keys. IsKeyDown only
        /// knows the keys that reached the app's key window; this one is for moments it can't see them, like a drop from
        /// another program.
        /// @return true if the key is currently pressed.
        static bool IsKeyPhysicallyDown(const uint32_t key);

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

        /// @brief Get the edge-detected press state of the given mouse button, advanced once per call. The
        /// first call observed after the button goes down returns Press; every subsequent call while it's
        /// still held returns Repeat; once the button is up it returns Release. Intended to be polled once
        /// per frame per button (mirroring how GetKeyPressState is driven by real repeat events for the
        /// keyboard) - polling the same button multiple times within one frame will consume the Press edge
        /// on the first call and report Repeat to the rest.
        /// @return A MousePressState (Release, Press, Repeat).
        static MousePressState GetMousePressState(const MouseButton button);

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

        /// @brief Get whether the given native window is the topmost one under the mouse cursor, the window a click there reaches.
        /// Windows of every application count, so any window lying over the given one at the cursor hides it.
        /// @return false when the cursor is outside the window, or another window lies over it there.
#ifdef __linux__
        static bool IsWindowUnderMouse(const uint64_t window);
#elif _WIN32
        static bool IsWindowUnderMouse(HWND window);
#endif

        /// @brief Get the longest time between two presses of a double-click: the system setting on Windows, 400 ms on Linux.
        static std::chrono::milliseconds GetDoubleClickTime();

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

        /// @brief Drops the held keys Windows reports as up. A key let go while another window had the keys sent its key-up
        ///        there, so without this it would stay held. Call with _inputMutex locked.
        static void forgetReleasedKeys();
        static LRESULT CALLBACK SubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassID, DWORD_PTR referenceData);

        /// @brief Installs messageHookProc on the calling thread, once.
        static void installMessageHook();

        /// @brief Sees every message the thread's windows take from the queue, so the wheel and the pinch reach PlatformBridge from any of them, not only from the window keys go to.
        static LRESULT CALLBACK messageHookProc(int code, WPARAM wParam, LPARAM lParam);
        static void readWheel(const MSG& message);
        static void readGesture(const MSG& message);
#endif
        static constexpr float _pinchScalePerNotch { 1.2f }; //for a pinch that arrives as Ctrl+wheel

        //Frame values hand over in NewFrame from the pending ones, which collect what arrives in between
        static inline std::string
            _inputStringBuffer,
            _pendingInputString;
        static inline std::unordered_map<uint32_t, uint32_t>
            _keyStrokeCounts,
            _pendingKeyStrokeCounts;
        static inline WheelDelta
            _wheelDelta,
            _pendingWheelDelta;
        static inline float
            _pinchScale { 1.0f },
            _pendingPinchScale { 1.0f };
        //static inline int32_t _inputLanguage;
#ifdef __linux__
        static inline uint64_t
            _rootWindow { 0 },
            _currentWindow { 0 },
            _rawWindow { 0 };
#elif _WIN32
        static inline HWND _currentWindow { nullptr };
        static inline HWND _subclassedWindow { nullptr };
        static inline wchar_t _pendingHighSurrogate { 0 };
        static inline HHOOK _messageHook { nullptr };
        static inline uint64_t _gestureDistance { 0 }; //between the fingers at the zoom gesture's last message, in pixels
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
        static inline MouseButton _previousMouseButtons { MouseButton::None };
        //static inline ModifierKey _modifierKeyState;
#ifdef __linux__
        static inline int32_t
            _mouseScreenX { 0 },
            _mouseScreenY { 0 };
        static inline uint32_t _mouseButtonStateMask { 0 };
#endif
    };
}