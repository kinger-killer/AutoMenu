cmake_minimum_required(VERSION 3.28)
project(AutoMenuJsonExample LANGUAGES CXX)

if(NOT TARGET AutoMenu)
	set(AUTOMENU_BUILD_JSON ON CACHE BOOL "Build AutoMenu's optional JSON module" FORCE)
	add_subdirectory(.. AutoMenuLibrary)
endif()

if(WIN32)
	add_executable(AutoMenuJsonExample WIN32
		main.cpp
		WidgetShowcase.cpp
		../Example/ExampleAssets.cpp
	)
else()
	add_executable(AutoMenuJsonExample
		main.cpp
		WidgetShowcase.cpp
		../Example/ExampleAssets.cpp
	)
endif()

target_compile_features(AutoMenuJsonExample PRIVATE cxx_std_20)
target_link_libraries(AutoMenuJsonExample PRIVATE AutoMenu::Json)
configure_file(menu.json ${CMAKE_CURRENT_BINARY_DIR}/menu.json COPYONLY)
target_compile_definitions(AutoMenuJsonExample PRIVATE
	AUTOMENU_JSON_EXAMPLE_MENU_FILE="${CMAKE_CURRENT_BINARY_DIR}/menu.json"
)
