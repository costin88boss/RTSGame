cmake_minimum_required(VERSION 3.20)

project(RTSGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER clang++)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG master
)

FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

add_executable(RTSGame ${SOURCES})

target_link_libraries(RTSGame PRIVATE sfml-graphics sfml-window sfml-system)