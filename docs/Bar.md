# Bar

`Bar<Value>` displays the fraction of a current value relative to a maximum value. It is useful for health, progress, or resource meters. The bar observes both values by reference, so game code can update them directly.

## Create with a factory

```cpp
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

The two values must outlive the bar. `BarData` retains references to them; the data object itself may be temporary after `create` returns.

The fill fraction is clamped to `[0, 1]`. A maximum at or below zero produces an empty bar. When the observed value changes, the displayed fill approaches the new fraction. Animation speed is measured in fractions of the full bar per second; `0.5f` traverses half the bar per second. Set it with `setAnimationSpeed` or through `BarData`.

Other style methods are `setBackgroundColor`, `setFillColor`, `setOutline`, `setPosition`, and `setSize`.
