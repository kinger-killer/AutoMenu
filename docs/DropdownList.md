# DropdownList

`DropdownList` shows the currently selected option in a box. Clicking the box opens the options below it; clicking an option updates a selected index supplied by the game. Clicking elsewhere closes the list. Expanded options are `DropdownOptionButton` objects derived from `Button`: they highlight on hover and emit the usual button events.

## Create with a factory

```cpp
std::size_t resolution = 0;

DropdownListData data(
    sf::FloatRect({40.f, 40.f}, {240.f, 36.f}), font,
    {"800 x 600", "1280 x 720", "1920 x 1080"}, resolution);

DropdownListFactory factory;
auto list = factory.create(data);
```

The font and selected-index variable must outlive the dropdown. The options are copied into `DropdownListData` and then into the widget. Initialize the index to an existing option; an out-of-range index displays an empty value until a valid option is selected.

`DropdownListData` also configures character size, background, option, hover, and selected-option colors, text and outline colors, outline thickness, and render layer. Each expanded row has the same height as the closed box.

Get an option button by index to subscribe to its events or customize it:

```cpp
if (Button* option = list->getOptionButton(0))
    option->subscribe(observer);
```

Button event names are set to the option labels.
