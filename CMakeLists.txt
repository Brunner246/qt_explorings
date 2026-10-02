cmake_minimum_required(VERSION 3.28)
project(qt_explorings LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 6.8 REQUIRED COMPONENTS Widgets Quick QuickWidgets QuickControls2 Qml Network)
qt_standard_project_setup(REQUIRES 6.8)

if(MSVC)
    add_compile_options(/W4 /permissive- /utf-8 /Zc:__cplusplus)
endif()

include(cmake/QtExampleDeploy.cmake)

add_subdirectory(01-widgets-mvvm-dialog)
add_subdirectory(02-qml-mvvm-dialog)
add_subdirectory(03-widgets-drag-drop-table)
add_subdirectory(04-qml-drag-drop-table)
add_subdirectory(05-tree-view)
add_subdirectory(06-custom-delegate-listview)
add_subdirectory(07-network-access)
