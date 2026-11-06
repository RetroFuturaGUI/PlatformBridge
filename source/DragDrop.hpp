#pragma once
#include <cstdint>
#include <filesystem>
#include <vector>

namespace PlatformBridge
{
    /// @brief Drags files out of the app, onto the system's file manager or any program that takes dropped files
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

        DragDrop() = delete;

        /// @brief Returns whether files can be dragged out on this platform (Windows only so far)
        static bool CanDragFilesOut();

        /// @brief Starts a file drag with the system's drag image. The call blocks until the mouse button is released or
        ///        Escape is pressed. The system handles the drag in its own loop, so the app's windows do not redraw until
        ///        it ends. The drop target copies or moves the files. All files must be in the same folder. Call this on the
        ///        window thread. Supported on Windows only; on other platforms it immediately returns None
        /// @param allowsMove Whether the target may move the files away; copying and linking are always allowed
        static DropEffect DragFilesOut(const std::vector<std::filesystem::path>& paths, const bool allowsMove);
    };
}