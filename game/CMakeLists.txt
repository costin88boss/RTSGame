file(COPY res/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(SOURCES
    src/RTSGame.cpp
    src/net/ServerConnection.h
    src/net/ServerConnection.cpp
    src/net/ServerListener.cpp
    src/net/ServerListener.h
    src/net/ClientListener.h
    src/net/ClientListener.cpp
    src/net/packet/Packets.h
    src/net/packet/Packets.cpp
    src/Game.cpp
    src/Game.h
)

add_executable(${PROJECT_NAME} ${SOURCES})