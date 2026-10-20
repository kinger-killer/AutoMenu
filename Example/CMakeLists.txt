cmake_minimum_required(VERSION 3.28)
project(AutoMenuButtonExample LANGUAGES CXX)

set(AUTOMENU_BUILD_JSON ON CACHE BOOL "Build AutoMenu's optional JSON module" FORCE)
add_subdirectory(.. AutoMenuLibrary)

if(WIN32)
    add_executable(AutoMenuButtonExample WIN32
        main.cpp
        ExampleAssets.cpp
        WidgetShowcase.cpp
    )
else()
    add_executable(AutoMenuButtonExample
        main.cpp
        ExampleAssets.cpp
        WidgetShowcase.cpp
    )
endif()
target_compile_features(AutoMenuButtonExample PRIVATE cxx_std_20)
target_link_libraries(AutoMenuButtonExample PRIVATE AutoMenu)

# Build the JSON-driven equivalent alongside the original C++ example.
add_subdirectory(../ExampleJSON ExampleJSONBuild)
