# PlatformBridge
A cross-platform API wrapper to be compiled to static libraries

## Usage

### Initialization
- `PlatformBridge::Input::Refresh()` — Refreshes the input capture routine.  
  **Returns:** `void`  
  **Notes:** Must be called before using the module.

### Monitors
- `std::vector<MonitorInfo>& PlatformBridge::Monitors::GetMonitors()` — Returns a vector containing information about all connected monitors.

### Fonts
- `PlatformBridge::Fonts` — Provides access to installed font metadata such as family name, style, weight, slant, path, and Unicode coverage.

- `PlatformBridge::Fonts::FontProperty` — Represents one discovered font entry with its name, path, style, and formatting attributes.

- `PlatformBridge::Fonts::Refresh()` — Refreshes the cached font information from the current platform.

- `PlatformBridge::Fonts::GetFontProperties()` — Returns the cached list of discovered font properties.

### Clipboard
#### Enums:
ClipboardDatatype (int32_t):
- Unknown
- Text
- Bitmap

ClipboardErrorCode (uint32_t):
- Success
- OpenClipboardFailed
- EmptyClipboardFailed
- SetClipboardDataFailed
- GetClipboardDataFailed
- CloseClipboardFailed
- NoMatchingRequestedDatatype
- NoDataAvailable
- UnsupportedDatatype
- ClipboardDataEmpty

FileOperation (uint32_t):
- Copy
- Cut

Functions

- `PlatformBridge::Clipboard` — Provides read/write access to the system clipboard. **Notes:** Linux has no system clipboard here yet: text and files are copied and pasted inside the app only (untested).

- `PlatformBridge::Clipboard::CopyToClipboard(const PlatformBridge::Clipboard::ClipboardDatatype type, const void* dataIn, const size_t size)` — Writes data of the given type to the system clipboard. **Parameters:** `type` is the datatype of `dataIn` (currently only `Text` is supported, expected as UTF-32/`char32_t`); `dataIn` is a pointer to the raw data; `size` is the size of `dataIn` in bytes. **Returns:** A `ClipboardErrorCode` (`Success` on success).

- `PlatformBridge::Clipboard::PasteFromClipboard(const PlatformBridge::Clipboard::ClipboardDatatype type, void*& dataOut, size_t* sizeOut)` — Reads data of the given type from the system clipboard. **Parameters:** `type` is the requested datatype (currently only `Text` is supported, returned as UTF-32/`char32_t`); `dataOut` is set to point at the internal data buffer; `sizeOut` is set to the size of the returned data in bytes. **Notes:** `dataOut` remains valid until the next call on `PasteFromClipboard`, call on`ClearClipboardDataBuffer()`, or on termination of the your program. **Returns:** A `ClipboardErrorCode` (`Success` on success).

- `PlatformBridge::Clipboard::ClearClipboardDataBuffer()` — Clears the internal buffer used to hold data retrieved via `PasteFromClipboard()`.

- `PlatformBridge::Clipboard::CopyFilesToClipboard(const std::vector<std::filesystem::path>& paths, const FileOperation operation)` — Puts files and folders on the clipboard to be pasted, copied or moved. **Notes:** On Windows as `CF_HDROP` with a "Preferred DropEffect", the way Windows Explorer puts them there, so either side pastes what the other copied or cut. **Returns:** A `ClipboardErrorCode` (`Success` on success).

- `PlatformBridge::Clipboard::PasteFilesFromClipboard(std::vector<std::filesystem::path>& pathsOut, FileOperation& operationOut)` — Reads the files and folders on the clipboard, and whether they were copied or cut; without an effect they count as copied. **Returns:** `NoMatchingRequestedDatatype` when the clipboard holds no files, `Success` otherwise.

- `PlatformBridge::Clipboard::HasFilesOnClipboard()` — Returns whether the clipboard holds files or folders. Cheap enough to poll once per frame.

- `PlatformBridge::Clipboard::ClearCutFiles(const std::vector<std::filesystem::path>& paths)` — Empties the clipboard if it still holds exactly these files, cut, so a cut pasted and moved can't be pasted again; anything cut or copied since stays.

### DragDrop
#### Enums:
DropEffect (uint32_t):
- None (cancelled, dropped where nothing takes files, or moved by the target itself, which reports it so)
- Copy
- Move
- Link

Functions

- `PlatformBridge::DragDrop` — Drags files out of the app, onto the system's file manager or any program that takes dropped files. Dropping files *onto* a window is GLFW's part (`glfwSetDropCallback`).

- `PlatformBridge::DragDrop::CanDragFilesOut()` — Returns whether files can be dragged out on this platform: `true` on Windows, `false` on Linux for now.

- `PlatformBridge::DragDrop::DragFilesOut(const std::vector<std::filesystem::path>& paths, const bool allowsMove)` — Drags the files with the system's drag image until the mouse button comes up or Escape cancels; the program dropped on copies or moves them itself. **Parameters:** `allowsMove` lets the target move the files away; copying and linking are always allowed. **Notes:** Blocks meanwhile, since the system runs the drag in a loop of its own, so the app's windows don't draw until the drop; call it on the windows' thread, while the button is held. All files must lie in one folder. On Windows the files become Shell items whose data object carries every format a target may ask for, and `SHDoDragDrop` runs the drag with the Shell's drop source; OLE is initialized on the calling thread the first time. **Returns:** What the target reports doing, `None` on Linux.

### Paths
- `PlatformBridge::Paths` — Exposes the executable and working-directory locations in a cross-platform way.

- `PlatformBridge::Paths::Refresh()` — Refreshes the cached executable and working-directory values.

- `PlatformBridge::Paths::GetExecutablePath()` — Returns the executable path as a string.

- `PlatformBridge::Paths::GetExecutablePathFSPath()` — Returns the executable path as a filesystem path object.

- `PlatformBridge::Paths::GetWorkingDir()` — Returns the current working directory as a string.

- `PlatformBridge::Paths::GetWorkingDirCstr()` — Returns the current working directory as a C-string.

- `PlatformBridge::Paths::SetWorkingDir()` — Updates the working directory used by the application.

### FileSystem
#### Enums:
EntryKind (uint32_t):
- File
- Directory
- Other

RootKind (uint32_t):
- Fixed
- Removable
- Optical
- Network
- Other

KnownFolder (uint32_t):
- Home
- Desktop
- Documents
- Downloads
- Music
- Pictures
- Videos

TrashResult (uint32_t):
- Moved
- NotTrashable (the bin can't take the item, which stays where it is)
- Failed

Functions

- `PlatformBridge::FileSystem` — Lists folders and finds the places browsing starts from: drives or mounted volumes, and the user's known folders; moves items to the recycle bin and back.

- `PlatformBridge::FileSystem::Entry` — One file or folder in a listed folder: its path, kind, size, last write time and whether it is hidden (the hidden attribute on Windows, a leading `.` on Linux). `_IsOnlineOnly` marks a cloud file whose content isn't on this computer - a Cloud Files placeholder of OneDrive or kDrive, or a file moved to slow storage (`FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS`, `RECALL_ON_OPEN`, `OFFLINE`) - which downloads in full once read, so a thumbnail had better not read it. Always false on Linux.

- `PlatformBridge::FileSystem::Root` — A drive on Windows (`C:\`), or on Linux the file system root and the volumes mounted under `/media`, `/run/media` and `/mnt`, plus network shares. **Notes:** On Windows only fixed drives get a label, because asking an empty card reader or disc drive errors and a network drive can stall.

- `PlatformBridge::FileSystem::Refresh()` — Re-reads the roots and known folders. Call it again to pick up drives plugged in since.

- `PlatformBridge::FileSystem::GetRoots()` — Returns the roots found by the last `Refresh()`.

- `PlatformBridge::FileSystem::GetKnownFolder(const PlatformBridge::FileSystem::KnownFolder folder)` — Returns the given known folder: `SHGetKnownFolderPath` on Windows, `$HOME` and the user-dirs file on Linux. **Returns:** An empty path when the platform has no such folder.

- `PlatformBridge::FileSystem::ListDirectory(const std::filesystem::path& directory, std::vector<Entry>& entriesOut, std::error_code& errorOut)` — Lists the files and folders in `directory`, without `.` and `..`, and never throws. **Parameters:** `entriesOut` is cleared, then filled in the order the platform returns the entries; `errorOut` is set when the folder can't be listed. **Notes:** On Windows a drive without a disc or card fails with an error instead of showing a system message box. **Returns:** `false` when the folder can't be listed.

- `PlatformBridge::FileSystem::DirectoryWatcher` — Watches one folder for changes to what it holds: entries added, removed or renamed, their sizes, write times and attributes, not the contents of its subfolders. `Watch(directory, errorOut)` starts on a folder instead of the one before, `Stop()` ends it, and `HasChanged()`, polled once per frame, never waits and reports any number of changes since the last call as one. **Notes:** `FindFirstChangeNotificationW` on Windows, `inotify` on Linux (untested). When the folder itself is deleted or moved, `HasChanged()` returns true once more and the watch ends (`IsWatching()` turns false).

- `PlatformBridge::FileSystem::MoveToTrash(const std::filesystem::path& path, std::error_code& errorOut)` — Moves a file or folder to the recycle bin on Windows, or to the trash in the home folder on Linux. Never shows a dialog and never deletes permanently: an item the bin can't take is left alone (`NotTrashable`), so the caller can ask first. **Notes:** On Windows through the Shell (`IFileOperation`, COM initialized on the calling thread for the call). The Shell would delete an item bigger than its volume's bin without a word, so the size is checked against the bin's limit first (`MaxCapacity` under the volume's `BitBucket` key, or Windows' default); a drive without a bin is caught by the Shell's own check. On Linux only items on the home folder's file system go (untested). Blocks until done, so call it off the main thread. **Returns:** `Moved`, `NotTrashable`, or `Failed` with `errorOut` set.

- `PlatformBridge::FileSystem::RestoreFromTrash(const std::filesystem::path& originalPath, std::filesystem::path& restoredPathOut, std::error_code& errorOut)` — Puts back the item most recently moved to the bin from this path. A name taken there meanwhile gets a free one (`FreeName`), and a folder it was in that went too is made again. **Notes:** On Windows it finds the item by the bin's "Original Location" and file name through the Shell, moves it back with `IFileOperation`, and removes the record the move leaves behind; on Linux by the `.trashinfo` files (untested). **Returns:** `false`, with `no_such_file_or_directory` when the bin holds nothing from this path.

- `PlatformBridge::FileSystem::FreeName(const std::filesystem::path& wanted, const bool isFolder)` — Returns the path as it is when nothing is there, otherwise "Name (2).ext", "Name (3).ext" and so on; a folder's number goes at the end of its name.

- `PlatformBridge::FileSystem::IsSameVolume(const std::filesystem::path& first, const std::filesystem::path& second)` — Returns whether two existing files or folders lie on the same volume, by the volume's serial number on Windows and the device on Linux, so mount points and subst drives count as the volume they show. **Notes:** Windows reports renaming a folder onto another volume as access denied, not as a different device, so ask this instead of reading the error. **Returns:** `true` when it can't tell.

### Input
#### Enums:
KeyPressState (uint32_t):
- Release
- Press
- Repeat

KeyboardUseState (uint32_t):
- KeyReleased,
- KeyPressed,
- SameKEyPressed

Functions

- `void PlatformBridge::Input::Refresh()` — Refreshes and restarts the input capture routine. This is only needed after a refresh; the initial setup is handled by `PlatformBridge::RefreshPlatformBridge()`.

- `void PlatformBridge::Input::Stop()` — Stops the input capture routine. This is recommended when PlatformBridge is no longer needed.

- `const std::string& PlatformBridge::Input::GetInputString()` — Returns the UTF-8 encoded input string.

- Linux: `void SetActiveWindow(const uint64_t window)` — Sets the active window to capture input from. **Parameters:** `window` is the X11 window ID.

- Windows: `void SetActiveWindow(HWND window)` — Sets the active window to capture input from. **Parameters:** `window` is the Win32 window handle.

- Linux: `void SetActiveDisplay(void* display)` — Sets the active display where the active window is located. **Parameters:** `display` is the X11 display pointer.

- Linux: `void* GetActiveDisplay()` — Returns the active display pointer.

- Linux: `uint64_t GetActiveWindowID()` — Returns the active window ID.

- `KeyPressState PlatformBridge::Input::GetKeyPressState()` — Returns the current keyboard press state.

- `KeyboardUseState PlatformBridge::Input::GetKeyboardUseState()` — Returns the current keyboard usage state.

- `bool PlatformBridge::Input::IsKeyDown(const uint32_t key)` — Returns whether a key is held, of all the keys held at once, not only the last one pressed, so modifier combinations like Ctrl+C can be checked. Knows the keys that reached the app's key window.

- `uint32_t PlatformBridge::Input::GetKeyPressCount(const uint32_t key)` — Returns how many times a key went from up to down since input capture started. Comparing it with the count seen before tells every separate press apart, even when it's polled too rarely to see the release in between; a held key's repeats don't count.

- `bool PlatformBridge::Input::IsKeyPhysicallyDown(const uint32_t key)` — Asks the system whether a key is down right now, whichever window or program has the keys (`GetAsyncKeyState` on Windows, `XQueryKeymap` on Linux, untested). The held keys PlatformBridge tracks only include those that reached the app's key window; this is for moments they can't, like a drop from another program. When the keys move to another of the app's windows, or it gets the focus, held keys the system reports up are let go, so a key released in between doesn't stay held.

- `std::chrono::milliseconds PlatformBridge::Input::GetDoubleClickTime()` — Returns the longest time between the two presses of a double-click: the system setting on Windows, 400 ms on Linux.


ToDo: Window: get active window HWND