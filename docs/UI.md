# UI

`UI` owns a collection of widgets and forwards drawing and updates to them. A game can keep one `UI` per menu screen or HUD and select which one to update and render.

## Add widgets

Factories return `std::unique_ptr` objects. Transfer ownership to the UI:

```cpp
UI menu;
ButtonFactory factory;
auto playButton = factory.create(buttonData);
menu.addWidget(std::move(playButton));
```

`addWidget` ignores null pointers. Widgets are sorted by render layer; smaller layer numbers draw first. Equal layers retain their insertion order.

## Update and render

`update` accepts one SFML event and a `sf::Time` delta. Call it for each event so interactive widgets receive input. Call `render` during the draw part of each frame:

```cpp
while (const auto event = window.pollEvent())
{
    if (event->is<sf::Event::Closed>())
        window.close();
    ui.update(*event, window, sf::Time::Zero);
}

// Apply elapsed frame time once for animations and blinking cursors.
ui.update(sf::Event{sf::Event::FocusGained{}}, window, frameTime);

ui.render(window);
```

Passing zero during event dispatch prevents animations from advancing once for every queued event. The extra focus-gained event provides one time update per frame; widgets use its elapsed time for effects such as the `Bar` and `ToggleSwitch` animations and the `TextInput` caret blink. These widgets ignore the event itself.

`UI` does not own fonts, textures, or game state referenced by widgets. Keep those resources alive until the widgets that use them are destroyed.
