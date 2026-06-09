/* ToDo:
- ensure _inputStringBuffer is never reallocated



*/
#pragma once
#include <string>
#include <string_view>
#include <cstdint>
#include <thread>
#include <atomic>

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

    class Keyboard
    {
    public:
        /// @brief Refreshes the Keyboard module.
        static void Refresh();

        /// @brief Stops the key scan thread.
        static void Stop();

        //static int32_t GetKeyCode(const uint32_t window);

        /// @brief Returns the utf8-formatted input string.
        /// @return An std::string representing the eutf-8 formatted input string.
        static const std::string& GetInputString();

        //static int32_t GetInputLanguage();

        /// @brief Sets the current window to capture key strokes from..
        static void SetActiveWindow(const uint32_t window);

        /// @brief Get KeyPressState of passed key symbol.
        /// @return Returns a KeyPressState (Release, Press, Repeat).
        static KeyPressState GetKeyPressState(const uint32_t key);

        /// @brief Get KeyboardUseState.
        /// @return Returns a KeyboardUseState (KeyReleased, KeyPressed, KeyRepeated).
        static KeyboardUseState GetKeyboardUseState();

    private:
        Keyboard() = default;
        Keyboard(const Keyboard&) = delete;
        Keyboard& operator=(const Keyboard&) = delete;
        Keyboard(const Keyboard&&) = delete;
        Keyboard& operator=(const Keyboard&&) = delete;
        ~Keyboard();
        static Keyboard& GetInstance()
        {
            static Keyboard Instance;
            return Instance;
        }
        static void initThread();
        static void captureKeyStroke();

        static inline std::string _inputStringBuffer;
        //static inline int32_t _inputLanguage;
        static inline uint32_t
            _rootWindow,
            _currentWindow,
            _lastKeySym;
        static inline void* _display;
        static inline std::thread _inputThread;
        static inline std::atomic<bool> _running { false };
        static inline KeyPressState _commonKeyState;
        static inline KeyboardUseState _keyboardUseState;
        //static inline ModifierKey _modifierKeyState;
    };
}