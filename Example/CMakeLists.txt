cmake_minimum_required(VERSION 3.28)
project(AutoMenuButtonExample LANGUAGES CXX)

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
