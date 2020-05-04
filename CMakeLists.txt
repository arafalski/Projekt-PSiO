cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME Projekt)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pedantic -Wextra -Werror -Wconversion -O3")

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "SFML/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

add_executable(${PROJECT_NAME} main.cpp pacman.cpp pacman.hpp animation.cpp animation.hpp map.hpp consts.hpp map.cpp collider.cpp collider.hpp screens.hpp configuration.hpp leaderboard.cpp leaderboard.hpp)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)
set(CMAKE_EXE_LINKER_FLAGS "-static")