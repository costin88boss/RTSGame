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
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_SHALLOW ON
)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.x
)

FetchContent_MakeAvailable(SFML spdlog)

if (MSVC AND TARGET spdlog)
  get_target_property(spdlog_includes spdlog INTERFACE_INCLUDE_DIRECTORIES)
  set_target_properties(spdlog PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "")
  target_include_directories(spdlog SYSTEM INTERFACE ${spdlog_includes})


  target_compile_options(spdlog PRIVATE /W0) # fuck my life
endif()

if (MSVC AND TARGET fmt)
  get_target_property(fmt_includes fmt INTERFACE_INCLUDE_DIRECTORIES)
  set_target_properties(fmt PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "")
  target_include_directories(fmt SYSTEM INTERFACE ${fmt_includes})


  target_compile_options(fmt PRIVATE /W0) # fuck my life
endif()

add_subdirectory(game)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    SFML::Graphics SFML::Audio SFML::Network
    spdlog::spdlog)