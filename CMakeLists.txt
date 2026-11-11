cmake_minimum_required(VERSION 3.16)
project(Media_Player_Lab2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets REQUIRED)

add_executable(Media_Player_Lab2 main.cpp MediaPlayer.cpp MediaPlayer.h
        MediaPlayer.cpp
        MediaPlayer.h)

target_link_libraries(Media_Player_Lab2
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Multimedia
        Qt6::MultimediaWidgets
)