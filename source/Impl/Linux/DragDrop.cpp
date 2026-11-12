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

//Taking drops while they hover needs the X drag-and-drop protocol too. Until then GLFW hands over finished drops

bool PlatformBridge::DragDrop::CanTakeDrops()
{
    return false;
}

bool PlatformBridge::DragDrop::RegisterDropTarget(void*, DropTargetEvents)
{
    return false;
}

void PlatformBridge::DragDrop::RevokeDropTarget(void*)
{
}