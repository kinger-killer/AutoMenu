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

// Apply elapsed frame time once for time-based widgets such as Bar.
ui.update(sf::Event{sf::Event::FocusGained{}}, window, frameTime);

ui.render(window);
```

Passing zero during event dispatch prevents time-based widgets from advancing once for every queued event. The extra focus-gained event provides one time update per frame; current input widgets ignore it.

`UI` does not own fonts, textures, or game state referenced by widgets. Keep those resources alive until the widgets that use them are destroyed.
