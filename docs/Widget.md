# Widget

`Widget` is the abstract base class for all AutoMenu components. It defines the interface that `UI` uses for drawing, input updates, and render-layer ordering.

## Interface

```cpp
class Widget
{
public:
    virtual ~Widget() = default;
    virtual void setRenderLayer(std::uint8_t layer);
    virtual std::uint8_t getRenderLayer() const;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void update(const sf::Event& event,
                        const sf::RenderWindow& window,
                        sf::Time deltaTime) = 0;
};
```

Smaller render-layer values are rendered first. The default layer is `0`.

`update` receives an SFML event, a read-only reference to the window, and elapsed frame time. A custom widget can use the event for input and `deltaTime` for time-based behavior.

## Creating a custom widget

Derive from `Widget` and implement `render` and `update`:

```cpp
class CustomWidget : public Widget
{
public:
    void render(sf::RenderWindow& window) override;
    void update(const sf::Event& event,
                const sf::RenderWindow& window,
                sf::Time deltaTime) override;
};
```

Use `WidgetFactory<Product, Data>` as the common factory interface. See [UI](UI.md) for adding custom widgets to a UI.

Widgets created by the optional JSON module keep their bound values, fonts, and textures alive through shared ownership. Widgets created directly with core C++ factories follow the lifetime rules documented on each widget's page.
