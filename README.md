# AutoMenu

AutoMenu is a header-only C++20 UI library for SFML 3. It provides widgets for game menus and in-game interfaces, including buttons, labels, bars, sliders, selection controls, and text input.

The core library and the JSON loader are separate CMake targets. Use only `AutoMenu` if you build your interface in C++; enable `AutoMenu::Json` when you want to describe supported widgets in JSON.

## Requirements

- CMake 3.28 or newer
- A C++20 compiler
- SFML 3 (fetched by AutoMenu)

## Add the core library with FetchContent

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

## Enable the optional JSON module

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

`AutoMenu::Json` also links the core `AutoMenu` target. Enabling it makes CMake fetch nlohmann/json 3.12.0. The JSON module is in [`modules/AutoMenuJson`](modules/AutoMenuJson) and is off by default.

### Bind game values to JSON widgets

The JSON description stores keys for live game values; it does not own or initialize those values. Bind shared values in C++, then give the registry to `JsonWidgetLoader`:

```cpp
#include <memory>
#include <AutoMenuJson/DefaultJsonWidgetFactoryRegistry.hpp>
#include <AutoMenuJson/JsonWidgetLoader.hpp>
#include <AutoMenuJson/RuntimeBindings.hpp>

auto health = std::make_shared<float>(75.f);
auto maxHealth = std::make_shared<const float>(100.f);

ValueRegistry values;
values.bind("player.health", health);
values.bind("player.max_health", maxHealth);

UI ui;
JsonWidgetLoader loader(values, getDefaultJsonWidgetFactoryRegistry());
const auto loadedWidgets = loader.loadFile("menu.json", ui);
```

Widgets created by the JSON loader retain the shared values and SFML resources they use. Runtime values remain connected to the game: for example, changing `*health` updates the bar.

The default registry supports `button`, `label_button`, `sprite_button`, `label`, `bar`, `slider`, `radio_button`, `dropdown_list`, `toggle_switch`, `check_box`, and `text_input`. Each widget is created through its existing C++ `Data` structure and factory. Shared values, fonts, and textures referenced by JSON are resolved from `ValueRegistry` and kept alive by the created widget.

For example, a `bar` description looks like this:

```json
{
  "widgets": [
    {
      "type": "bar",
      "position": [70, 185],
      "size": [460, 28],
      "value": "player.health",
      "maximum": "player.max_health",
      "background": [70, 70, 70, 255],
      "fill": [70, 200, 100, 255],
      "outline": [255, 255, 255, 255],
      "outlineThickness": 2,
      "renderLayer": 0,
      "animationSpeed": 0.5
    }
  ]
}
```

Colors are RGBA arrays with channel values from 0 to 255. `position` and `size` are arrays of two numbers. Value and resource fields contain keys previously registered in `ValueRegistry`; common fields use the names `value`, `font`, and `texture`, while controls with multiple textures use names such as `trackTexture` and `knobTexture`. Required widget-specific fields are checked during loading. `loadFile` interprets relative paths from the process's current working directory. See the [JSON module guide](docs/JSON.md) for each widget's required fields and a complete binding example.

To add a custom JSON widget type, implement `IJsonWidgetFactory` and register it in a `JsonWidgetFactoryRegistry`; the JSON parser does not need to change.

## Typical use without JSON

Create widgets with their `Data` structures and factories, transfer the returned `std::unique_ptr` to `UI::addWidget`, forward SFML events to `UI::update`, and draw the interface with `UI::render`.

Fonts and textures are borrowed by widgets and must outlive them. Reference-based widget values must also outlive the widget; `Bar` additionally accepts `std::shared_ptr` values when shared ownership is needed. See each widget's page for its specific requirements.

## Examples

`Example` is the C++-configured showcase. `ExampleJSON` renders the same interface from [`menu.json`](ExampleJSON/menu.json), while C++ binds the live game values and SFML assets by key. The JSON example's widget layout is in `ExampleJSON/menu.json`; game input and button observer behavior remain in C++.

## Widget reference

- [Widget](docs/Widget.md) — abstract base class and factory interface.
- [Button](docs/Button.md) — rectangle button and observer events.
- [LabelButton](docs/LabelButton.md) — button with centered text.
- [SpriteButton](docs/SpriteButton.md) — button drawn with a sprite.
- [Label](docs/Label.md) — non-interactive text.
- [Bar](docs/Bar.md) — animated value bar.
- [Slider](docs/Slider.md) — horizontal sprite slider bound to a game value.
- [RadioButton](docs/RadioButton.md) — one choice in a shared selection group.
- [DropdownList](docs/DropdownList.md) — expandable list of options.
- [ToggleSwitch](docs/ToggleSwitch.md) — on/off control.
- [CheckBox](docs/CheckBox.md) — labeled boolean control.
- [TextInput](docs/TextInput.md) — single-line text input with caret editing.
- [UI](docs/UI.md) — widget ownership, updates, and render ordering.
