# RadioButton

`RadioButton<Value>` derives from `CheckBox` and reuses its label, hover handling, and indicator rendering. Its indicator uses the radio style. Radio buttons are grouped by sharing the same selected-value variable. Clicking one assigns its option value to that variable, which selects it and clears the visual selection from buttons with other option values. The click and selection behavior is implemented by `RadioButton`; the base class does not need extra virtual hooks.

## Create a group with a factory

```cpp
int quality = 0;

RadioButtonData<int> lowData(
    sf::FloatRect({40.f, 40.f}, {200.f, 30.f}), font,
    "Low", quality, 0);
RadioButtonData<int> highData(
    sf::FloatRect({40.f, 80.f}, {200.f, 30.f}), font,
    "High", quality, 1);

RadioButtonFactory<int> factory;
ui.addWidget(factory.create(lowData));
ui.addWidget(factory.create(highData));
```

Both buttons reference the same `quality` variable. The selected value and font must outlive the buttons. `Value` can be an integer or enum; it must support equality comparison.

`RadioButtonData<Value>` configures bounds, font, label, shared selection, this button's option value, selected-dot color, outline color, text color, character size, and render layer. To use custom artwork, pass a `CheckBoxDesign` to the data constructor and set its `indicatorTexture` and `checkedMarkTexture`; those texture objects must outlive the radio buttons. The factory applies the design when creating the widget.
