# SpriteButton

`SpriteButton` derives from `Button` and draws a sprite fitted to the button bounds. The button's rectangular bounds remain its mouse hit area.

## Create with a factory

```cpp
ButtonData buttonStyle(
    sf::FloatRect({500.f, 240.f}, {120.f, 120.f}),
    sf::Color::White, sf::Color::White, 0.f, 1, "Inventory");

SpriteButtonData data(buttonStyle, iconTexture);
SpriteButtonFactory factory;
auto button = factory.create(data);
```

`SpriteButtonData` contains the base button style, a texture reference, and an optional tint. The texture must remain alive for the lifetime of the button. The sprite scales to fill the bounds and can therefore change aspect ratio.

Use `setTint` to change the sprite color. Input events, name, and bounds come from `Button`.
