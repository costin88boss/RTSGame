cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
project(RTSGame)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/bin)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/deps/)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(SFML)

set(SOURCES
    "src/NetTest.cpp"
    "src/RTSGame.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Audio SFML::Network)