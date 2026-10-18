# TextInput

`TextInput` is a single-line text field backed by an `sf::String` owned by the game. Click inside the field to focus it and place the caret. While focused, typed characters are inserted at the caret; Left/Right move it, Home/End jump to the start or end, Backspace removes the previous character, and Delete removes the next one. When text is wider than the field, it scrolls horizontally to keep the caret visible and is clipped to the field's inner bounds. Clicking outside or losing window focus removes keyboard focus.

## Create with a factory

```cpp
sf::String playerName;

TextInputData data(
    sf::FloatRect({40.f, 40.f}, {300.f, 38.f}), font,
    playerName, "Player name", 32);
TextInputFactory factory;
auto input = factory.create(data);
```

The font and string must outlive the text input. The optional maximum length defaults to 256 characters; character size defaults to 22. The placeholder is shown while the string is empty and the field is not focused.

`TextInputData` configures normal and focused backgrounds, text and placeholder colors, outline, and render layer. Selection and clipboard operations are not implemented.
