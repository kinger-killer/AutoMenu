# Label

`Label` displays non-interactive SFML text.

## Create with a factory

```cpp
LabelData data(font, "Score: 0", {24.f, 24.f}, 26, sf::Color::White, 2);
LabelFactory factory;
auto label = factory.create(data);
```

`LabelData` takes a font reference, text, position, character size, color, and render layer. The font must remain alive for the lifetime of the label.

The position is the SFML text object's position. Use `setText`, `setPosition`, `setColor`, and `setCharacterSize` to update the label after creation. `Label` ignores input events and elapsed time.
