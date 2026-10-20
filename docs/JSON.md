# JSON module

The optional `AutoMenu::Json` target reads a JSON document and creates widgets through the same C++ `Data` structures and factories used by the core library. The document describes widget type, layout, and style. C++ supplies live game values and SFML resources through `ValueRegistry`.

## Bind values and resources

Bind each named value as a `std::shared_ptr`. The registered C++ type must match the type expected by the widget factory.

```cpp
#include <memory>

#include <AutoMenuJson/DefaultJsonWidgetFactoryRegistry.hpp>
#include <AutoMenuJson/JsonWidgetLoader.hpp>
#include <AutoMenuJson/RuntimeBindings.hpp>

auto health = std::make_shared<float>(75.f);
auto maximumHealth = std::make_shared<const float>(100.f);
auto font = std::make_shared<sf::Font>();
font->openFromFile("assets/arial.ttf");

ValueRegistry values;
values.bind("player.health", health);
values.bind("player.max_health", maximumHealth);
values.bind("assets.font", font);

UI ui;
JsonWidgetLoader loader(values, getDefaultJsonWidgetFactoryRegistry());
const auto widgets = loader.loadFile("menu.json", ui);
```

The widget keeps its own `shared_ptr` to each bound dependency it uses. It can therefore continue using a value or resource if the corresponding entry is later erased from `ValueRegistry`.

The default registry supports these live-value types:

| Widget | JSON key | Registered C++ value type |
| --- | --- | --- |
| `bar` | `value`, `maximum` | `float`, `const float` |
| `slider` | `value` | `float` |
| `radio_button` | `value` | `int` |
| `dropdown_list` | `value` | `std::size_t` |
| `toggle_switch` | `value` | `bool` |
| `check_box` | `value` | `bool` |
| `text_input` | `value` | `sf::String` |

Font and texture fields also refer to keys in `ValueRegistry`. Bind them as `std::shared_ptr<sf::Font>` or `std::shared_ptr<sf::Texture>`. A widget that uses a font or texture retains it for its lifetime.

## Widget descriptions

The document has one top-level `widgets` array. Each entry has a `type` field. Every widget except `label` requires `position` and `size`, each an `[x, y]` pair. A label needs `position`, `text`, and `font`.

| `type` | Required fields in addition to layout | Optional resource/value fields |
| --- | --- | --- |
| `button` | — | `name`, `fill`, `outline`, `outlineThickness`, `hoverBrightness`, `pressedBrightness`, `renderLayer` |
| `label` | `text`, `font` | `characterSize`, `color`, `renderLayer` |
| `label_button` | `text`, `font` | Button style fields, `characterSize`, `textColor` |
| `sprite_button` | `texture` | Button style fields, `tint` |
| `bar` | `value`, `maximum` | `background`, `fill`, `outline`, `outlineThickness`, `animationSpeed`, `renderLayer` |
| `slider` | `value`, `minimum`, `maximum`, `trackTexture`, `knobTexture` | `trackColor`, `knobColor`, `renderLayer` |
| `radio_button` | `text`, `font`, `value`, `option` | `selectedColor`, `outlineColor`, `textColor`, `characterSize`, `renderLayer` |
| `dropdown_list` | `font`, `value`, `options` | `characterSize`, `background`, `optionColor`, `hoverOptionColor`, `selectedOptionColor`, `textColor`, `outlineColor`, `outlineThickness`, `renderLayer` |
| `toggle_switch` | `value` | `offTrackTexture`, `onTrackTexture`, `knobTexture`, colors, `animationMilliseconds`, `knobScale`, `renderLayer` |
| `check_box` | `font`, `value` | `text`, `indicatorTexture`, `checkedMarkTexture`, design colors, `outlineThickness`, `indicatorSizeRatio`, `characterSize`, `renderLayer` |
| `text_input` | `font`, `value` | `placeholder`, `maxLength`, `characterSize`, background/text/outline colors, `outlineThickness`, `renderLayer` |

All color values are RGBA arrays with integer channels from 0 to 255. Resource and live-value fields contain registry keys, not file paths. Load fonts and textures in C++ before loading the menu and bind them in the registry.

For example, this bar description uses the keys bound above:

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
      "animationSpeed": 0.5
    }
  ]
}
```

`JsonWidgetLoader::load` and `loadFile` return pointers to the newly created widgets in document order. Use them to connect C++ behavior such as `Button::subscribe`; visual layout and widget construction can remain in JSON while application behavior remains in C++.

The default JSON factories live in `modules/AutoMenuJson/include/AutoMenuJson/Factories`, one class per file. To add a widget type, implement `IJsonWidgetFactory` and register it in `JsonWidgetFactoryRegistry`. The JSON parser itself does not need to change.

The JSON example demonstrates the complete interface and runtime bindings in [`ExampleJSON`](../ExampleJSON/menu.json). Its optional `id` fields are application metadata; the generic loader ignores them.
