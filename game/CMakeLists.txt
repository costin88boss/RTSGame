
# during cmake config process :(
execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar c ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}${CMAKE_BUILD_TYPE}/data.bin --format=zip .
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res"
)


# BLAME IDEs IF THIS IS INCONSISTENT
#set(SOURCES
#    src/Main.cpp
#    src/net/ServerConnection.hpp
#    src/net/ServerConnection.cpp
#    src/net/ServerListener.cpp
#    src/net/ServerListener.hpp
#    src/net/ClientListener.hpp
#    src/net/ClientListener.cpp
#    src/net/packet/Packets.hpp
#    src/net/packet/Packets.cpp
#    src/game/Game.cpp
#    src/game/Game.hpp
#    src/util/StreamLogger.hpp
#    
# "src/util/PhysFsStream.cpp"
# "src/util/PhysFsStream.hpp"
#src/scene/scene.hpp)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h src/*.hpp)

add_executable(${PROJECT_NAME} ${SOURCES})

if(MSVC)
#target_compile_options(${PROJECT_NAME}
        #PRIVATE /external:anglebrackets
        #PRIVATE /external:W0)
        #PRIVATE -Wunsafe-buffer-usage
       # PRIVATE -Wreserved-macro-identifier)

        #-Wreserved-macro-identifier -Wunsafe-buffer-usage
else()
#target_compile_options(${PROJECT_NAME}
#        PRIVATE -w)
endif()


