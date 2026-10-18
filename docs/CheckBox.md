# CheckBox

`CheckBox` is a labeled boolean control derived from `Button`. A left click inside the full widget bounds toggles the referenced `bool`; it also emits the standard `ButtonEvent` events and can be subscribed to with `subscribe()`. By default, it draws a circle that changes from outlined to filled.

## Create with a factory

```cpp
bool subtitles = true;

CheckBoxData data(
    sf::FloatRect({40.f, 40.f}, {220.f, 32.f}), font,
    "Subtitles", subtitles);
CheckBoxFactory factory;
auto box = factory.create(data);
```

The font and boolean must outlive the checkbox. The bounds height determines the indicator size; the label is drawn to its right. The default circle indicator uses one `sf::CircleShape` for both states.

## Use custom artwork

`CheckBoxDesign` accepts two optional textures: `indicatorTexture` for the outline/base artwork and `checkedMarkTexture` for the checked mark. They can contain any shape or artwork (for example, a square, triangle, or custom icon). Use transparent backgrounds so only the artwork is drawn. The textures are scaled proportionally to fit the indicator area.

```cpp
sf::Texture outlineTexture;
sf::Texture checkMarkTexture;
outlineTexture.loadFromFile("assets/triangle-outline.png");
checkMarkTexture.loadFromFile("assets/star-mark.png");

CheckBoxDesign design(&outlineTexture, &checkMarkTexture);
design.checkedColor = sf::Color(80, 190, 120); // Optional per-field customization.

CheckBoxData data(bounds, font, "Subtitles", subtitles, design);
```

Keep both textures alive for as long as the checkbox uses them. The design also contains `outlineColor`, `hoverColor`, `checkedColor`, `hoverFillColor`, `uncheckedFillColor`, `textColor`, `outlineThickness`, `indicatorSizeRatio`, and `characterSize`. The design can be replaced at runtime with `checkBox->setDesign(newDesign)` or inspected with `getDesign()`.
