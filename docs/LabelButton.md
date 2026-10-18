# LabelButton

`LabelButton` derives from `Button`, draws the button rectangle, and places a text label in its center. Resizing or moving the button recenters the text.

## Create with a factory

```cpp
ButtonData buttonStyle(
    sf::FloatRect({120.f, 250.f}, {260.f, 80.f}),
    sf::Color(80, 100, 160), sf::Color::White, 3.f, 1, "Play");

LabelButtonData data(buttonStyle, font, "Play", 28, sf::Color::White);
LabelButtonFactory factory;
auto button = factory.create(data);
```

`LabelButtonData` contains the base `ButtonData`, a font reference, text, character size, and text color. The font must remain alive for the lifetime of the button.

Use `setLabel` to replace the text and `setTextColor` to change its color. Button appearance, bounds, name, and observer events are inherited from `Button`.
