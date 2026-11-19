# Examples

[Documentation](README.md) / Examples

## Available showcases

| Application | Layout | Runtime behavior |
| --- | --- | --- |
| `AutoMenuButtonExample` | [C++ widget showcase](../Example/WidgetShowcase.cpp) | C++ values, input handling, and observers |
| `AutoMenuJsonExample` | [menu.json](../ExampleJSON/menu.json) | [C++ bindings and observers](../ExampleJSON/WidgetShowcase.cpp) |

Both showcases demonstrate the same interface. JSON describes widget construction, layout, and style; game input and observer behavior remain in C++.

## Build both applications

Run from the repository root:

```sh
cmake -S Example -B build/examples
cmake --build build/examples --config Release
```

The `Example` project enables the optional JSON module and builds both executables. The first configuration downloads SFML and nlohmann/json, so it requires network access.

## Build only the JSON application

```sh
cmake -S ExampleJSON -B build/json-example
cmake --build build/json-example --config Release
```

Run the executable from the chosen build directory; generators such as Visual Studio place it in a configuration subdirectory such as `Release`. When both examples are built together, the JSON executable is under `ExampleJSONBuild` in that build tree.

CMake copies `menu.json` into the JSON application's build directory and supplies its path to the executable. For your own application, `JsonWidgetLoader::loadFile` resolves relative paths from the process's current working directory.

## Explore the code

- [C++ event loop](../Example/main.cpp) — event dispatch, animation timing, and rendering.
- [C++ widget construction](../Example/WidgetShowcase.cpp) — data structures and factories.
- [JSON runtime setup](../ExampleJSON/WidgetShowcase.cpp) — shared values, SFML resources, and observers.
- [Shared assets](../Example/ExampleAssets.cpp) — resource loading and generated artwork.

See [Installation](Installation.md) to integrate the library into your own application and the [JSON module guide](JSON.md) for the document format.
