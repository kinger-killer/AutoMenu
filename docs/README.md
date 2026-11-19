# Documentation

[Project overview](../README.md)

## Start here

1. [Installation](Installation.md) — requirements, CMake targets, and optional JSON support.
2. [First interface](GettingStarted.md) — create a widget and run the event/render loop.
3. [Examples](Examples.md) — build and explore the C++ and JSON showcases.

## Core concepts

| Topic | Guide |
| --- | --- |
| Widget ownership, input, frame timing, and drawing order | [UI](UI.md) |
| Base interface and custom widgets | [Widget](Widget.md) |
| Runtime bindings, JSON fields, and custom JSON factories | [JSON module](JSON.md) |
| Button observers and interaction events | [Button events](Button.md#button-events) |

## Widget reference

- [Widget](Widget.md) — abstract base class and factory interface.
- [Button](Button.md) — rectangle button and observer events.
- [LabelButton](LabelButton.md) — button with centered text.
- [SpriteButton](SpriteButton.md) — button drawn with a sprite.
- [Label](Label.md) — non-interactive text.
- [Bar](Bar.md) — animated value bar with a fast current-level indicator.
- [Slider](Slider.md) — horizontal sprite slider bound to a game value.
- [RadioButton](RadioButton.md) — one choice in a shared selection group.
- [DropdownList](DropdownList.md) — expandable list of options.
- [ToggleSwitch](ToggleSwitch.md) — on/off control.
- [CheckBox](CheckBox.md) — labeled boolean control.
- [TextInput](TextInput.md) — single-line text input with caret editing.
- [UI](UI.md) — widget ownership, updates, and render ordering.

## Choose a widget

| Need | Widgets |
| --- | --- |
| Trigger an action | [Button](Button.md), [LabelButton](LabelButton.md), [SpriteButton](SpriteButton.md) |
| Display text or progress | [Label](Label.md), [Bar](Bar.md) |
| Select a numeric value | [Slider](Slider.md) |
| Select one option | [RadioButton](RadioButton.md), [DropdownList](DropdownList.md) |
| Switch a boolean value | [ToggleSwitch](ToggleSwitch.md), [CheckBox](CheckBox.md) |
| Edit text | [TextInput](TextInput.md) |
