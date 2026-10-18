# Button

`Button` is a rectangular interactive widget. It emits events for left mouse presses and releases inside its bounds, and for mouse hover beginning or ending.

## Create with a factory

```cpp
ButtonData data(
    sf::FloatRect({120.f, 250.f}, {260.f, 80.f}),
    sf::Color(80, 100, 160), // fill
    sf::Color::White,        // outline
    3.f,                     // outline thickness
    1,                       // render layer
    "Play");                 // name

ButtonFactory factory;
auto button = factory.create(data);
```

`ButtonData` defaults to a 200×80 button at (0, 0), a blue fill, white outline, 3-pixel outline thickness, layer 0, and an empty name.

The button can also be styled later with `setFillColor`, `setHoverFillColor`, `setHoverBrightness`, `setPressedBrightness`, `setOutline`, `setName`, `setPosition`, and `setSize`. By default, it lightens on hover and darkens while pressed. Brightness values are in the range `[-1, 1]`: positive values lighten, negative values darken, and zero disables the effect. An optional hover fill color becomes the base color for the hover effect. `Button::update` handles these visual changes internally and still emits hover events for observers.

## Button events

`Button` implements `ISubject`. Subscribe a shared observer to receive `ButtonEvent`:

```cpp
auto observer = std::make_shared<MyObserver>();
button->subscribe(observer);
```

`ButtonEvent` contains:

- `source`: reference to the button that emitted the event.
- `type`: one of `Pressed`, `Released`, `OnHoverBegin`, or `OnHoverEnd`.
- `buttonName`: a copy of the button name at the time of the event.

Events are delivered synchronously during `UI::update`. The observer must remain alive; the subject stores only a weak reference. `Pressed` and `Released` are emitted when the corresponding left mouse action occurs inside the button bounds. A release outside the bounds does not emit `Released`.
