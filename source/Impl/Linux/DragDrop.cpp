#include "DragDrop.hpp"

//Dragging out needs the X drag-and-drop protocol, which isn't built yet

bool PlatformBridge::DragDrop::CanDragFilesOut()
{
    return false;
}

PlatformBridge::DragDrop::DropEffect PlatformBridge::DragDrop::DragFilesOut(const std::vector<std::filesystem::path>&, const bool)
{
    return DropEffect::None;
}