cmake_minimum_required(VERSION 3.28)
project(AutoMenu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

option(AUTOMENU_BUILD_JSON "Build the optional AutoMenu JSON module" OFF)

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW TRUE
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(SFML)

add_library(AutoMenu INTERFACE)
target_include_directories(AutoMenu INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(AutoMenu INTERFACE
    SFML::Graphics
)

target_sources(AutoMenu INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/src/UI.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets/Widget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets/Button.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets/LabelButton.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets/SpriteButton.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets/Bar.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets/Label.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets/Slider.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets/RadioButton.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets/DropdownList.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets/ToggleSwitch.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets/CheckBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets/TextInput.hpp
)

if(AUTOMENU_BUILD_JSON)
	add_subdirectory(modules/AutoMenuJson)
endif()
