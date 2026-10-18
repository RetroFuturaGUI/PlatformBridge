#pragma once
#include <cstdint>
#ifdef _WIN32
    #include <Windows.h>
#endif

namespace PlatformBridge
{
    /// @brief This makes popup window behave like dialoges belonging to a parent window
    class NativeWindow
    {
    public:
        /// @brief Makes owner the owner of window. The window then stays above its owner and minimizes with it. On Windows it
        ///        also loses its taskbar button, and on Linux most window managers leave it out of the taskbar.
#ifdef __linux__
        static void SetOwner(void* display, const uint64_t window, const uint64_t owner);
#elif _WIN32
        static void SetOwner(HWND window, HWND owner);
#endif

    private:
        NativeWindow() = delete;
        NativeWindow(const NativeWindow&) = delete;
        NativeWindow(NativeWindow&&) = delete;
        auto operator =(const NativeWindow&) = delete;
        auto operator =(NativeWindow&&) = delete;
    };
}
