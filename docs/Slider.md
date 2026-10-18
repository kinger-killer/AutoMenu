# Slider

`Slider<Value>` is a horizontal control with a sprite track and sprite knob. Clicking within its bounds or dragging with the left mouse button writes a value into a variable owned by the game.

## Create with a factory

```cpp
float volume = 0.5f;

SliderData<float> data(
    sf::FloatRect({120.f, 430.f}, {360.f, 32.f}),
    volume, 0.f, 1.f, // bound value, minimum, maximum
    trackTexture, knobTexture,
    sf::Color(70, 70, 70), // track tint
    sf::Color(100, 160, 240), // knob tint
    1); // render layer

SliderFactory<float> factory;
auto slider = factory.create(data);
```

The bound value and both textures must outlive the slider. The bounds describe the full control area. AutoMenu stretches the track across its width and fits the knob to the bounds' height. Use `setTrackColor` and `setKnobColor` to change sprite tints.

Values are clamped to the configured range when changed by mouse input. Integral `Value` types are rounded to the nearest integer. If the maximum is not greater than the minimum, mouse input does not change the value and the knob stays at the minimum end. The slider cancels dragging when the window loses focus.

Read the bound variable in game code to use the selected value. The slider does not currently emit a value-changed event.
