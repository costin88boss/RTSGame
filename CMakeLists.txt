cmake_minimum_required(VERSION 3.29.5)

project(RTSGame)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/deps/)

include(FetchContent)


FetchContent_Declare(
    SFML
    SYSTEM
    EXCLUDE_FROM_ALL
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_SHALLOW ON
)

FetchContent_Declare(
    spdlog
    SYSTEM
    EXCLUDE_FROM_ALL
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.x
)

FetchContent_Declare(
    physfs
    SYSTEM
    EXCLUDE_FROM_ALL
    GIT_REPOSITORY https://github.com/icculus/physfs.git
    GIT_TAG release-3.2.0
)


FetchContent_MakeAvailable(SFML spdlog physfs)

add_subdirectory(game)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    SFML::Graphics SFML::Audio SFML::Network
    spdlog::spdlog PhysFS::PhysFS-static)