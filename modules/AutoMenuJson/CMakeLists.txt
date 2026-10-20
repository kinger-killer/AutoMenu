include(FetchContent)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")

if(NOT TARGET nlohmann_json::nlohmann_json)
	FetchContent_Declare(json
		URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
		URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
		DOWNLOAD_EXTRACT_TIMESTAMP TRUE
	)
	FetchContent_MakeAvailable(json)
endif()

add_library(AutoMenuJson INTERFACE)
add_library(AutoMenu::Json ALIAS AutoMenuJson)

target_include_directories(AutoMenuJson INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(AutoMenuJson INTERFACE
	AutoMenu
	nlohmann_json::nlohmann_json
)
target_sources(AutoMenuJson INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/Factories/BarJsonWidgetFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/Factories/ButtonJsonWidgetFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/Factories/CheckBoxJsonWidgetFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/Factories/DropdownListJsonWidgetFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/Factories/LabelButtonJsonWidgetFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/Factories/LabelJsonWidgetFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/Factories/RadioButtonJsonWidgetFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/Factories/SliderJsonWidgetFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/Factories/SpriteButtonJsonWidgetFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/Factories/TextInputJsonWidgetFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/Factories/ToggleSwitchJsonWidgetFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/DefaultJsonWidgetFactoryRegistry.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/JsonWidgetLoader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/JsonWidgetFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/JsonWidgetFactoryHelpers.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AutoMenuJson/RuntimeBindings.hpp
)
