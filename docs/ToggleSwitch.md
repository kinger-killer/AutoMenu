# ToggleSwitch

`ToggleSwitch` is an on/off control backed by a `bool` owned by the game. A left click inside its bounds toggles that value. The track color and knob position reflect the value.

## Create with a factory

```cpp
bool fullscreen = false;

ToggleSwitchData data(
    sf::FloatRect({40.f, 40.f}, {68.f, 32.f}), fullscreen);
ToggleSwitchFactory factory;
auto toggle = factory.create(data);
```

The referenced boolean and any supplied textures must outlive the toggle. The knob moves smoothly between the ends of the track. `ToggleSwitchData` configures the animation duration and knob scale, as well as colors, outline, and render layer.

To use custom artwork, pass optional off-track, on-track, and knob textures to `ToggleSwitchData`:

```cpp
ToggleSwitchData data(
    bounds, enabled, onColor, offColor, knobColor,
    outlineColor, outlineThickness, renderLayer,
    &offTrackTexture, &onTrackTexture, &knobTexture,
    sf::milliseconds(220), 0.8f);
```

Both track textures are required to use custom track artwork; if either is `nullptr`, the built-in colored track is used. The knob texture is independent: set it to `nullptr` to use the built-in circle. Track textures are stretched to the control bounds. The knob texture is scaled to a square based on the track height and `knobScale`, then moves along the track.
