# Installation

[Documentation](README.md) / Installation

## Requirements

- CMake 3.28 or newer
- A C++20 compiler
- SFML 3 (fetched by AutoMenu)

## Core library

Add AutoMenu to your project's `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
    AutoMenu
    GIT_REPOSITORY https://github.com/kinger-killer/AutoMenu.git
    GIT_TAG main
)
FetchContent_MakeAvailable(AutoMenu)

target_link_libraries(MyGame PRIVATE AutoMenu)
```

For reproducible dependency versions, replace `main` with a release tag or commit identifier. The core target brings in SFML Graphics and does not fetch nlohmann/json.

## Optional JSON module

Set `AUTOMENU_BUILD_JSON` before `FetchContent_MakeAvailable(AutoMenu)`, then link `AutoMenu::Json` instead of linking the core target directly:

```cmake
include(FetchContent)

set(AUTOMENU_BUILD_JSON ON)

FetchContent_Declare(
    AutoMenu
    GIT_REPOSITORY https://github.com/kinger-killer/AutoMenu.git
    GIT_TAG main
)
FetchContent_MakeAvailable(AutoMenu)

target_link_libraries(MyGame PRIVATE AutoMenu::Json)
```

`AutoMenu::Json` also links the core `AutoMenu` target. Enabling it makes CMake fetch nlohmann/json 3.12.0. The JSON module is in [`modules/AutoMenuJson`](../modules/AutoMenuJson) and is off by default.


## Next steps

- [Create your first interface](GettingStarted.md).
- [Bind values and load JSON widgets](JSON.md).
- [Build the showcase applications](Examples.md).
