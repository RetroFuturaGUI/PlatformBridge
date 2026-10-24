#pragma once
#include "CursorIcon.hpp"
#include <cstdint>
#ifdef __linux__
    #include <array>
    #include <cstddef>
#elif _WIN32
    #include <Windows.h>
#endif

namespace PlatformBridge
{
    /// @brief Sets the icon the mouse cursor shows over the app's own windows
    class Cursor
    {
    public:
        /// @brief Meant to be called when the cursor icon should change
#ifdef __linux__
        static void SetIcon(void* display, const uint64_t window, const CursorIcon icon);
#elif _WIN32
        static void SetIcon(HWND window, const CursorIcon icon);
#endif

    private:
        Cursor() = delete;
        Cursor(const Cursor&) = delete;
        Cursor(Cursor&&) = delete;
        auto operator =(const Cursor&) = delete;
        auto operator =(Cursor&&) = delete;

#ifdef __linux__
        /// @brief Returns icon's cursor on display, made the first time it's asked for. 0 when neither the cursor theme nor X11's own cursors have it.
        static uint64_t loadCursor(void* display, const CursorIcon icon);

        /// @brief Returns icon's name in cursor themes, as the freedesktop cursor names have it.
        static const char* themeName(const CursorIcon icon);

        /// @brief Returns X11's own cursor shape for icon, for when the theme lacks it.
        static uint32_t fontShape(const CursorIcon icon);

        static constexpr std::size_t _iconCount { static_cast<std::size_t>(CursorIcon::Wait) + 1 };
        static inline void* _cursorDisplay { nullptr }; //the display the cursors below were made on
        static inline std::array<uint64_t, _iconCount> _cursors {}; //0 for one not made yet
#elif _WIN32
        /// @brief Returns the system's shared cursor for icon. Shared cursors are never destroyed.
        static HCURSOR systemCursor(const CursorIcon icon);

        /// @brief Returns whether the mouse is over window's client area, with no other window lying over it there.
        static bool isMouseOverClientArea(HWND window);

        /// @brief Answers WM_SETCURSOR with the window's icon, which the subclass holds as its reference data. GLFW answers it
        ///        too, and would put its own cursor back on every mouse move.
        static LRESULT CALLBACK SubclassProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR subclassID, DWORD_PTR icon);
#endif
    };
}