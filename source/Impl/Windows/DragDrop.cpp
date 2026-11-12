#include "DragDrop.hpp"
#include <Windows.h>
#include <ShlObj.h>
#include <shellapi.h>
#include <wrl/client.h>
#include <string>
#include <unordered_map>
#include <utility>

namespace
{
    //Object linking and embedding (OLE) on the windows' thread once. Dragging and taking drops need it, and it stays until the thread ends
    bool isOleReady()
    {
        thread_local const HRESULT oleInitialized { OleInitialize(nullptr) };
        return SUCCEEDED(oleInitialized);
    }

    PlatformBridge::DragDrop::DropKeys keysOf(const DWORD keyState)
    {
        using DropKeys = PlatformBridge::DragDrop::DropKeys;
        DropKeys keys { DropKeys::None };

        if(0 != (keyState & MK_CONTROL))
            keys = keys | DropKeys::Control;

        if(0 != (keyState & MK_SHIFT))
            keys = keys | DropKeys::Shift;

        if(0 != (keyState & MK_ALT))
            keys = keys | DropKeys::Alt;

        return keys;
    }

    PlatformBridge::DragDrop::DropEffects effectsOf(const DWORD effect)
    {
        using DropEffects = PlatformBridge::DragDrop::DropEffects;
        DropEffects effects { DropEffects::None };

        if(0 != (effect & DROPEFFECT_COPY))
            effects = effects | DropEffects::Copy;

        if(0 != (effect & DROPEFFECT_MOVE))
            effects = effects | DropEffects::Move;

        if(0 != (effect & DROPEFFECT_LINK))
            effects = effects | DropEffects::Link;

        return effects;
    }

    DWORD dropEffectOf(const PlatformBridge::DragDrop::DropEffect effect)
    {
        switch(effect)
        {
            case PlatformBridge::DragDrop::DropEffect::Copy:
                return DROPEFFECT_COPY;
            case PlatformBridge::DragDrop::DropEffect::Move:
                return DROPEFFECT_MOVE;
            case PlatformBridge::DragDrop::DropEffect::Link:
                return DROPEFFECT_LINK;
            default:
                return DROPEFFECT_NONE;
        }
    }

    //The files a drag carries, from its CF_HDROP format. Nothing when it carries no files.
    std::vector<std::filesystem::path> pathsOf(IDataObject* data)
    {
        if(!data)
            return {};

        FORMATETC format { .cfFormat = CF_HDROP, .ptd = nullptr, .dwAspect = DVASPECT_CONTENT, .lindex = -1, .tymed = TYMED_HGLOBAL };
        STGMEDIUM medium {};

        if(FAILED(data->GetData(&format, &medium)))
            return {};

        std::vector<std::filesystem::path> paths {};
        const HDROP drop { static_cast<HDROP>(medium.hGlobal) };
        const UINT count { DragQueryFileW(drop, 0xFFFFFFFF, nullptr, 0) };

        for(UINT index = 0; index < count; ++index)
        {
            const UINT length { DragQueryFileW(drop, index, nullptr, 0) };
            std::wstring name(length, L'\0');
            DragQueryFileW(drop, index, name.data(), length + 1);
            paths.emplace_back(std::move(name));
        }

        ReleaseStgMedium(&medium);
        return paths;
    }

    //Tells the source what the target did, the reliable way some sources read it (CFSTR_PERFORMEDDROPEFFECT).
    void tellPerformedEffect(IDataObject* data, const DWORD performed)
    {
        if(!data)
            return;

        HGLOBAL memory { GlobalAlloc(GMEM_MOVEABLE, sizeof(DWORD)) };

        if(!memory)
            return;

        DWORD* value { static_cast<DWORD*>(GlobalLock(memory)) };

        if(!value)
        {
            GlobalFree(memory);
            return;
        }

        *value = performed;
        GlobalUnlock(memory);
        FORMATETC format { .cfFormat = static_cast<CLIPFORMAT>(RegisterClipboardFormatA(CFSTR_PERFORMEDDROPEFFECT)), .ptd = nullptr, .dwAspect = DVASPECT_CONTENT, .lindex = -1, .tymed = TYMED_HGLOBAL };
        STGMEDIUM medium {};
        medium.tymed = TYMED_HGLOBAL;
        medium.hGlobal = memory;
        medium.pUnkForRelease = nullptr;

        if(FAILED(data->SetData(&format, &medium, TRUE)))
            GlobalFree(memory); //the data object took it only on success.
    }

    //A window's drop target: asks its events what a drop at the point would do, and keeps the system's drag image showing.
    class FileDropTarget final : public IDropTarget
    {
    public:
        FileDropTarget(HWND window, PlatformBridge::DragDrop::DropTargetEvents events)
            : _window(window), _events(std::move(events))
        {
            //Without the helper the source's drag image vanishes over the window
            CoCreateInstance(CLSID_DragDropHelper, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_helper));
        }

        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** object) override
        {
            if(!object)
                return E_POINTER;

            if(IID_IUnknown != riid && IID_IDropTarget != riid)
            {
                *object = nullptr;
                return E_NOINTERFACE;
            }

            *object = static_cast<IDropTarget*>(this);
            AddRef();
            return S_OK;
        }

        ULONG STDMETHODCALLTYPE AddRef() override
        {
            return static_cast<ULONG>(InterlockedIncrement(&_references));
        }

        ULONG STDMETHODCALLTYPE Release() override
        {
            const ULONG left { static_cast<ULONG>(InterlockedDecrement(&_references)) };

            if(0 == left)
                delete this;

            return left;
        }

        HRESULT STDMETHODCALLTYPE DragEnter(IDataObject* data, DWORD keyState, POINTL point, DWORD* effect) override
        {
            if(!effect)
                return E_INVALIDARG;

            _paths = pathsOf(data);
            *effect = ask(_events._OnOver, keyState, point, *effect);
            POINT screenPoint { .x = point.x, .y = point.y };

            if(_helper)
                _helper->DragEnter(_window, data, &screenPoint, *effect);

            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE DragOver(DWORD keyState, POINTL point, DWORD* effect) override
        {
            if(!effect)
                return E_INVALIDARG;

            *effect = ask(_events._OnOver, keyState, point, *effect);
            POINT screenPoint { .x = point.x, .y = point.y };

            if(_helper)
                _helper->DragOver(&screenPoint, *effect);

            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE DragLeave() override
        {
            if(_helper)
                _helper->DragLeave();

            _paths.clear();

            if(_events._OnLeave)
                _events._OnLeave();

            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE Drop(IDataObject* data, DWORD keyState, POINTL point, DWORD* effect) override
        {
            if(!effect)
                return E_INVALIDARG;

            _paths = pathsOf(data);
            const DWORD performed { ask(_events._OnDrop, keyState, point, *effect) };
            POINT screenPoint { .x = point.x, .y = point.y };

            if(_helper)
                _helper->Drop(data, &screenPoint, performed);

            _paths.clear();

            //A move the target does itself is an optimized move: the source is told no move took place, so it deletes nothing
            if(DROPEFFECT_MOVE == performed)
            {
                *effect = DROPEFFECT_NONE;
                tellPerformedEffect(data, DROPEFFECT_NONE);
                return S_OK;
            }

            *effect = performed;
            return S_OK;
        }

    private:
        //What the handler would do at the point, kept to what the source allows
        DWORD ask(const PlatformBridge::DragDrop::DropHandler& handler, const DWORD keyState, const POINTL point, const DWORD allowed) const
        {
            if(_paths.empty() || !handler)
                return DROPEFFECT_NONE;

            POINT clientPoint { .x = point.x, .y = point.y };
            ScreenToClient(_window, &clientPoint);
            const DWORD wanted { dropEffectOf(handler(_paths, clientPoint.x, clientPoint.y, keysOf(keyState), effectsOf(allowed))) };
            return 0 != (wanted & allowed) ? wanted : DROPEFFECT_NONE;
        }

        HWND _window { nullptr };
        PlatformBridge::DragDrop::DropTargetEvents _events {};
        Microsoft::WRL::ComPtr<IDropTargetHelper> _helper {};
        std::vector<std::filesystem::path> _paths {}; //what the drag over the window carries
        LONG _references { 1 };
    };

    struct RegisteredTarget
    {
        Microsoft::WRL::ComPtr<FileDropTarget> _Target {};
        bool _HasAcceptedFiles { false }; //took WM_DROPFILES before, as GLFW's windows do.
    };

    //The windows taking drops, used on the window thread only.
    std::unordered_map<HWND, RegisteredTarget>& registeredTargets()
    {
        static std::unordered_map<HWND, RegisteredTarget> targets {};
        return targets;
    }
}

bool PlatformBridge::DragDrop::CanDragFilesOut()
{
    return true;
}

PlatformBridge::DragDrop::DropEffect PlatformBridge::DragDrop::DragFilesOut(const std::vector<std::filesystem::path>& paths, const bool allowsMove)
{
    if(paths.empty())
        return DropEffect::None;

    if(!isOleReady())
        return DropEffect::None;

    //As Shell items whose data object carries every format a drop target may ask for (file names, item lists, contents)
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
bool PlatformBridge::DragDrop::CanTakeDrops()
{
    return true;
}

bool PlatformBridge::DragDrop::RegisterDropTarget(void* nativeWindow, DropTargetEvents events)
{
    HWND window { static_cast<HWND>(nativeWindow) };

    if(!window)
        return false;

    if(!isOleReady())
        return false;

    if(registeredTargets().contains(window))
        return false;

    Microsoft::WRL::ComPtr<FileDropTarget> target {};
    target.Attach(new FileDropTarget(window, std::move(events)));

    if(FAILED(RegisterDragDrop(window, target.Get())))
        return false;

    //The system would also send WM_DROPFILES, which GLFW listens for, so the same drop would arrive twice
    const bool hasAcceptedFiles { 0 != (GetWindowLongPtrW(window, GWL_EXSTYLE) & WS_EX_ACCEPTFILES) };
    DragAcceptFiles(window, FALSE);
    registeredTargets().emplace(window, RegisteredTarget { ._Target = std::move(target), ._HasAcceptedFiles = hasAcceptedFiles });
    return true;
}

void PlatformBridge::DragDrop::RevokeDropTarget(void* nativeWindow)
{
    HWND window { static_cast<HWND>(nativeWindow) };
    const auto found { registeredTargets().find(window) };

    if(registeredTargets().end() == found)
        return;

    RevokeDragDrop(window);

    if(found->second._HasAcceptedFiles)
        DragAcceptFiles(window, TRUE);

    registeredTargets().erase(found);
}