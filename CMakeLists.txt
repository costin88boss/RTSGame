cmake_minimum_required(VERSION 3.29.5)

project(RTSGame CXX)

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
    EXCLUDE_FROM_ALL
    SYSTEM
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_SHALLOW ON
)

FetchContent_Declare(
    SPDLOG
    EXCLUDE_FROM_ALL
    SYSTEM
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.15.0
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(SFML SPDLOG)

# BLABLHDEg DF AO

add_subdirectory(game)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics SFML::Audio SFML::Network
    spdlog::spdlog_header_only)