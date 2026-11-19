# First interface

[Documentation](README.md) / First interface

## 1. Link the library

Follow [Installation](Installation.md) and link your application to `AutoMenu`. The example below needs no fonts or textures.

## 2. Create a button and run the UI

Save this as your application's `main.cpp`:

```cpp
#include <utility>
#include <AutoMenu.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "AutoMenu");
    window.setFramerateLimit(60);

    AutoMenu::UI ui;
    AutoMenu::widgets::ButtonData data(
        sf::FloatRect({120.f, 250.f}, {260.f, 80.f}),
        sf::Color(80, 100, 160), sf::Color::White, 3.f, 0, "Play");
    AutoMenu::widgets::ButtonFactory factory;
    ui.addWidget(factory.create(data));

    sf::Clock frameClock;
    while (window.isOpen())
    {
        const sf::Time frameTime = frameClock.restart();
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            ui.update(*event, window, sf::Time::Zero);
        }
        if (!window.isOpen())
            break;

        // Advance animations once per frame, including frames without input.
        ui.update(sf::Event{sf::Event::FocusGained{}}, window, frameTime);
        window.clear(sf::Color(35, 35, 35));
        ui.render(window);
        window.display();
    }
}
```

The factory returns a `std::unique_ptr`; `addWidget` transfers it to the UI. The button responds visually to hover and presses. To trigger application actions, [subscribe an observer to its events](Button.md#button-events).

The current API advances time through `update`, which also requires an event. The focus-gained event above follows the showcase's convention for a once-per-frame time update. See [UI](UI.md#update-and-render) for the timing details.

## 3. Keep dependencies alive

Core widgets usually borrow fonts, textures, and game values. Create those dependencies before the UI and keep them alive until their widgets are destroyed. `Bar` also supports shared values. Widgets loaded through JSON retain shared ownership of their bound dependencies.

See each [widget reference](README.md#widget-reference) for its exact lifetime requirements.

## Next steps

- Add text with [Label](Label.md) or [LabelButton](LabelButton.md).
- Bind a game value with [Bar](Bar.md), [Slider](Slider.md), or [ToggleSwitch](ToggleSwitch.md).
- Move layout into JSON with the [JSON module](JSON.md).
- Explore complete interfaces in the [examples](Examples.md).
