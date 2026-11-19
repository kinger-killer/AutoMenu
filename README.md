# AutoMenu

AutoMenu is a header-only C++20 UI library for SFML 3. It provides buttons, labels, progress bars, sliders, selection controls, and text input for game menus and in-game interfaces.

Build interfaces in C++ with the core `AutoMenu` target, or enable `AutoMenu::Json` to describe layout and style in JSON while binding game values and resources in C++.

## Quick start

Requires CMake 3.28 or newer and a C++20 compiler. CMake fetches SFML 3.1.0 automatically.

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

Replace `main` with a release tag or commit identifier for reproducible builds. Include `<AutoMenu.hpp>` in your C++ code; `AutoMenu::UI` manages widgets, and widget classes and factories live in `AutoMenu::widgets`.

Continue with [your first interface](docs/GettingStarted.md), or see [installation](docs/Installation.md) for JSON support and dependency details.

## Documentation

- [Documentation index](docs/README.md) — learning path, core concepts, and widget reference.
- [UI lifecycle](docs/UI.md) — ownership, updates, frame timing, and rendering.
- [JSON module](docs/JSON.md) — runtime bindings and supported widget fields.
- [Examples](docs/Examples.md) — build the C++ and JSON showcases.
- [Custom widgets](docs/Widget.md#creating-a-custom-widget) — extend the base widget interface.

## Project layout

| Directory | Contents |
| --- | --- |
| [`src`](src) | Header-only core library and widget factories |
| [`modules/AutoMenuJson`](modules/AutoMenuJson) | Optional JSON loader, registry, and factories |
| [`Example`](Example) | Interface configured in C++ |
| [`ExampleJSON`](ExampleJSON) | Interface loaded from JSON with C++ runtime bindings |
| [`docs`](docs/README.md) | Guides and widget reference |

## License

AutoMenu is available under the [MIT license](LICENSE).
