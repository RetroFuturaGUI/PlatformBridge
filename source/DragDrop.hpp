#pragma once
#include <cstdint>
#include <filesystem>
#include <vector>

namespace PlatformBridge
{
    /// @brief Drags files out of the app, onto the system's file manager or any program that takes dropped files.
    class DragDrop
    {
    public:
        /// @brief What the program the files were dropped on did with them, as it reports it.
        enum class DropEffect : uint32_t
        {
            None, //cancelled, dropped where nothing takes files, or moved by the target itself, which reports it so
            Copy,
            Move,
            Link
        };

        DragDrop() = delete;

        /// @brief Returns whether files can be dragged out on this platform: on Windows, not yet on Linux.
        static bool CanDragFilesOut();

        /// @brief Drags these files with the system's drag image until the mouse button comes up, or Escape cancels. Blocks
        ///        meanwhile: the system runs the drag in a loop of its own, so the app's windows don't draw until the drop. The
        ///        program dropped on copies or moves the files itself. All of them must lie in one folder. Call it on the thread
        ///        the windows run on. Windows only for now; elsewhere it returns None at once.
        /// @param allowsMove Whether the target may move the files away; copying and linking are always allowed.
        static DropEffect DragFilesOut(const std::vector<std::filesystem::path>& paths, const bool allowsMove);
    };
}