cmake_minimum_required(VERSION 3.16)

project(PathfinderWidgets VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Gui)

include_directories(${CMAKE_CURRENT_LIST_DIR}/PathView)

set(PROJECT_SOURCES
        main.cpp

        MainWindow.cpp
        MainWindow.h
        MainWindow.ui

        PathView/PathView.h
        PathView/PathView.cpp

        PathView/FieldGenerator.h
        PathView/FieldGenerator.cpp

        PathView/BFSProcessor.h
        PathView/BFSProcessor.cpp

        PathView/Common.h
)

qt_add_executable(PathfinderWidgets
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(PathfinderWidgets PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                        PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
