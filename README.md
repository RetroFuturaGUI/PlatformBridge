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

### Paths
- `PlatformBridge::Paths` — Exposes the executable and working-directory locations in a cross-platform way.

- `PlatformBridge::Paths::Refresh()` — Refreshes the cached executable and working-directory values.

- `PlatformBridge::Paths::GetExecutablePath()` — Returns the executable path as a string.

- `PlatformBridge::Paths::GetExecutablePathFSPath()` — Returns the executable path as a filesystem path object.

- `PlatformBridge::Paths::GetWorkingDir()` — Returns the current working directory as a string.

- `PlatformBridge::Paths::GetWorkingDirCstr()` — Returns the current working directory as a C-string.

- `PlatformBridge::Paths::SetWorkingDir()` — Updates the working directory used by the application.

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


ToDo: Window: get active window HWND