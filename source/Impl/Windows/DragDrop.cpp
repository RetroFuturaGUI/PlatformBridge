#include "DragDrop.hpp"
#include <Windows.h>
#include <ShlObj.h>
#include <wrl/client.h>

bool PlatformBridge::DragDrop::CanDragFilesOut()
{
    return true;
}

PlatformBridge::DragDrop::DropEffect PlatformBridge::DragDrop::DragFilesOut(const std::vector<std::filesystem::path>& paths, const bool allowsMove)
{
    if(paths.empty())
        return DropEffect::None;

    //OLE on the windows' thread, once: dragging needs it, and it stays until the thread ends
    thread_local const HRESULT oleInitialized { OleInitialize(nullptr) };

    if(FAILED(oleInitialized))
        return DropEffect::None;

    //As Shell items, whose data object carries every format a drop target may ask for: file names, item lists, contents
    std::vector<PIDLIST_ABSOLUTE> itemLists {};
    itemLists.reserve(paths.size());

    for(const std::filesystem::path& path : paths)
    {
        std::error_code error {};
        const std::filesystem::path fullPath { std::filesystem::absolute(path, error).make_preferred() };
        PIDLIST_ABSOLUTE itemList { nullptr };

        if(error || FAILED(SHParseDisplayName(fullPath.c_str(), nullptr, &itemList, 0, nullptr)))
            continue;

        itemLists.push_back(itemList);
    }

    DropEffect effect { DropEffect::None };

    if(!itemLists.empty())
    {
        Microsoft::WRL::ComPtr<IShellItemArray> items {};
        Microsoft::WRL::ComPtr<IDataObject> data {};
        std::vector<PCIDLIST_ABSOLUTE> readOnlyLists { itemLists.begin(), itemLists.end() };
        HRESULT step { SHCreateShellItemArrayFromIDLists(static_cast<UINT>(readOnlyLists.size()), readOnlyLists.data(), &items) };

        if(SUCCEEDED(step))
            step = items->BindToHandler(nullptr, BHID_DataObject, IID_PPV_ARGS(&data));

        //Without a drop source of its own, the Shell's follows the mouse button and Escape, and draws the drag image over the app's active window
        DWORD performed { DROPEFFECT_NONE };

        if(SUCCEEDED(step))
            step = SHDoDragDrop(GetActiveWindow(), data.Get(), nullptr, DROPEFFECT_COPY | DROPEFFECT_LINK | (allowsMove ? DROPEFFECT_MOVE : 0), &performed);

        if(DRAGDROP_S_DROP == step)
        {
            if(0 != (performed & DROPEFFECT_MOVE))
                effect = DropEffect::Move;
            else if(0 != (performed & DROPEFFECT_COPY))
                effect = DropEffect::Copy;
            else if(0 != (performed & DROPEFFECT_LINK))
                effect = DropEffect::Link;
        }
    }

    for(PIDLIST_ABSOLUTE itemList : itemLists)
        CoTaskMemFree(itemList);

    return effect;
}