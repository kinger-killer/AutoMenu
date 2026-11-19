# Bar

[Documentation](README.md) / Bar

`Bar<Value>` displays the fraction of a current value relative to a maximum value. It is useful for health, progress, or resource meters. The bar observes both values by reference, so game code can update them directly.

## Create with a factory

```cpp
using namespace AutoMenu;
using namespace AutoMenu::widgets;
float health = 75.f;
const float maxHealth = 100.f;

BarData<float> data(
    sf::FloatRect({120.f, 190.f}, {500.f, 24.f}),
    health, maxHealth,
    sf::Color(70, 70, 70), // background
    sf::Color(70, 200, 100), // fill
    sf::Color::White, 2.f, // outline color and thickness
    1, // render layer
    0.5f); // animation speed

BarFactory<float> factory;
auto bar = factory.create(data);
```

## Ownership and lifetime

With this reference-based constructor, both values must outlive the bar. `BarData` retains references to them; the data object itself may be temporary after `create` returns. To share ownership with the bar, pass `std::shared_ptr` values instead:

```cpp
using namespace AutoMenu;
using namespace AutoMenu::widgets;
auto health = std::make_shared<float>(75.f);
auto maxHealth = std::make_shared<const float>(100.f);

BarData<float> data(
    sf::FloatRect({120.f, 190.f}, {500.f, 24.f}),
    health, maxHealth);
```

The bar keeps these shared values alive and reads their current contents during updates.

The optional JSON module creates bars through the same `BarData` and `BarFactory` path. Its `value` and `maximum` fields name shared values registered in `ValueRegistry`; see [JSON module guide](JSON.md).

## Behavior and styling

The fill fraction is clamped to `[0, 1]`. A maximum at or below zero produces an empty bar. A contrasting indicator moves quickly toward the current value; the main fill follows it more slowly and is capped at the indicator's edge. This leaves a visible strip while the main fill catches up. `responseSpeed` controls the indicator; `animationSpeed` controls the main fill. Both are measured in fractions of the full bar per second. Configure the colors with `setIndicatorColor` and the speeds with `setResponseSpeed` and `setAnimationSpeed` or through `BarData`.

Other style methods are `setBackgroundColor`, `setFillColor`, `setOutline`, `setPosition`, and `setSize`.

## See also

- [UI: ownership, updates, and rendering](UI.md)
- [JSON widget descriptions](JSON.md#widget-descriptions)
- [All widgets](README.md#widget-reference)
