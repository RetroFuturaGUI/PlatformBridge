#pragma once
#include <cstdint>
#include <filesystem>
#include <functional>
#include <vector>

namespace PlatformBridge
{
    /// @brief Drags files out of the app, onto the system's file manager or any program that takes dropped files, and takes
    ///        files dragged onto the app's windows
    class DragDrop
    {
    public:
        /// @brief What the program the files were dropped on did with them, as it reports it
        enum class DropEffect : uint32_t
        {
            None, //cancelled, dropped where nothing takes files, or moved by the target itself, which reports it so
            Copy,
            Move,
            Link
        };

        /// @brief The keys held while files are dragged over a drop target, as a mask
        enum class DropKeys : uint32_t
        {
            None = 0,
            Control = 1,
            Shift = 1 << 1,
            Alt = 1 << 2
        };

        /// @brief The effects a drag's source allows, as a mask
        enum class DropEffects : uint32_t
        {
            None = 0,
            Copy = 1,
            Move = 1 << 1,
            Link = 1 << 2
        };

        /// @brief Called with the dragged files' paths, the point in the window's client pixels (y running downward), the keys
        ///        held and the effects the source allows. Returns the effect the target would perform there, one the source
        ///        allows, or None to refuse
        using DropHandler = std::function<DropEffect(const std::vector<std::filesystem::path>& paths, const int x, const int y, const DropKeys keys, const DropEffects allowed)>;

        /// @brief What a drop target is told while files are dragged over its window
        struct DropTargetEvents
        {
            DropHandler _OnOver {}; //when the drag enters the window, and each time it moves or the keys change. Its result shows as the cursor
            std::function<void()> _OnLeave {}; //the drag left the window or was cancelled
            DropHandler _OnDrop {}; //the files were let go. Move means the target moves them itself, so the source never deletes them
        };

        DragDrop() = delete;

        /// @brief Returns whether files can be dragged out on this platform (Windows only so far)
        static bool CanDragFilesOut();

        /// @brief Starts a file drag with the system's drag image. The call blocks until the mouse button is released or
        ///        Escape is pressed. The system handles the drag in its own loop, so the app's windows do not redraw until
        ///        it ends. The drop target copies or moves the files. All files must be in the same folder. Call this on the
        ///        window thread. Supported on Windows only; on other platforms it immediately returns None
        /// @param allowsMove Whether the target may move the files away; copying and linking are always allowed
        static DropEffect DragFilesOut(const std::vector<std::filesystem::path>& paths, const bool allowsMove);

        /// @brief Returns whether a window can take files dropped on it from other programs while they are dragged, with
        ///        feedback and the effect negotiated (Windows only so far). Elsewhere drops arrive only once let go, through GLFW
        static bool CanTakeDrops();

        /// @brief Has a window take files dragged onto it from any program, or from this one: the events tell it where they
        ///        are and when they are let go. The system's drag image keeps showing over the window. A Move the drop handler
        ///        returns is an optimized move. The target moves the files itself, possibly later, and the source is told not
        ///        to delete them. Meanwhile the window gets no WM_DROPFILES (GLFW's drop callback stays quiet), so a drop arrives
        ///        once. Call it on the window thread. Returns false where unsupported or when the window already has one
        /// @param nativeWindow The window's handle (an HWND on Windows)
        static bool RegisterDropTarget(void* nativeWindow, DropTargetEvents events);

        /// @brief Stops a window taking drops, before the window is destroyed
        static void RevokeDropTarget(void* nativeWindow);
    };

    constexpr DragDrop::DropKeys operator |(const DragDrop::DropKeys left, const DragDrop::DropKeys right)
    {
        return static_cast<DragDrop::DropKeys>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
    }

    constexpr DragDrop::DropEffects operator |(const DragDrop::DropEffects left, const DragDrop::DropEffects right)
    {
        return static_cast<DragDrop::DropEffects>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
    }

    constexpr bool HasDropKey(const DragDrop::DropKeys keys, const DragDrop::DropKeys key)
    {
        return 0 != (static_cast<uint32_t>(keys) & static_cast<uint32_t>(key));
    }

    constexpr bool HasDropEffect(const DragDrop::DropEffects effects, const DragDrop::DropEffects effect)
    {
        return 0 != (static_cast<uint32_t>(effects) & static_cast<uint32_t>(effect));
    }
}