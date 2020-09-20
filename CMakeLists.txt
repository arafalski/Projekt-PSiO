cmake_minimum_required(VERSION 3.16)
project(Projekt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FLAGS -Wall -Wextra -Werror -Wconversion -pedantic -O3)

set(SRC
    pacman.cpp
    map.cpp
    collidable.cpp
    leaderboard.cpp
    tile.cpp
    configuration.cpp
    screens.cpp
    light.cpp
    game.cpp
    wallTile.cpp
    startTile.cpp
    endTile.cpp
    pointTile.cpp)

add_subdirectory(SFML)

add_library(${PROJECT_NAME}-lib ${SRC})
target_link_libraries(${PROJECT_NAME}-lib PRIVATE sfml-graphics sfml-audio)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-lib sfml-graphics sfml-audio)
