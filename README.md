# AutoMenu

AutoMenu is a header-only C++20 UI library built on SFML 3. It provides widgets for game menus and in-game interfaces. The game owns its values and assets; widgets display or modify the values they are given.

The consuming project needs CMake 3.28 or newer and C++20 support.

## Add AutoMenu with FetchContent

In your project's `CMakeLists.txt`, declare AutoMenu and link its interface target:

```cmake
include(FetchContent)

FetchContent_Declare(
    AutoMenu
    GIT_REPOSITORY https://github.com/kinger-killer/AutoMenu.git
    GIT_TAG 1.0.0
)
FetchContent_MakeAvailable(AutoMenu)

target_link_libraries(MyGame PRIVATE AutoMenu)
```

AutoMenu supplies the `src` include directory and its SFML graphics dependency. For reproducible dependency versions, replace `main` with a release tag or commit identifier.

## Widgets

Each component has its own reference page:

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
- [TextInput](docs/TextInput.md) — basic single-line text input.
- [UI](docs/UI.md) — widget ownership, updates, and render ordering.

## Typical use

Create a widget with its `Data` structure and factory, then transfer the returned `std::unique_ptr` to `UI::addWidget`. Forward SFML events to `UI::update` and draw the UI with `UI::render`.

Widget resources are borrowed: keep referenced fonts, textures, and values alive for as long as the widget uses them. See each widget's page for its specific requirements.
