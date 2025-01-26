#include <iostream>
#include <SFML/Graphics.hpp>
#include "Grid.hpp"
#include "Transform.hpp"

#include "net/ClientListener.h"
#include "net/ServerConnection.h"
#include "net/ServerListener.h"

#include <Windows.h>

class SimpleServer : public net::ServerListener {
    void onReceiveTCP(net::ServerConnection client, sf::Packet data) {
        std::cout << "Received TCP\n";
    }
    void onReceiveUDP(net::ServerConnection client, sf::Packet data) {
        std::cout << "Received UDP\n";
    }
    void onConnectionDisconnect(net::ServerConnection client) {
        std::cout << "Connection left\n";
    }
    void onNewConnection(net::ServerConnection client) {
        std::cout << "New connection\n";
    }
};

class SimpleClient : public net::ClientListener {
    void onConnected() {
        std::cout << "Connected\n";
    }
    void onDisconnect() {
        std::cout << "Disconnected\n";
    }
    void onReceiveTCP(sf::Packet data) {
        std::cout << "Received TCP\n";
    }
    void onReceiveUDP(sf::Packet data) {
        std::cout << "Received UDP\n";
    }
};

int main() {

    // netTest::testMain();
    /*
    auto window = sf::RenderWindow(sf::VideoMode({720, 480}), "Hello, SFML!");

    Grid grid(window, 24, 16);
    auto& unit = grid.createUnit({0, 0});
    unit.setTargetTile({5u, 5u});

    sf::Clock clock;
    float deltaTime = 0.0f;

    const sf::Font font("../../res/fonts/Roboto-Regular.ttf");
    sf::Text text(font);
    text.setFillColor(sf::Color::Green);
    // auto window = sf::RenderWindow(sf::VideoMode({720, 480}), "Hello, SFML!");

    */
    uint16_t port = 33615;

    const char* z = "ASDASDASDTA";
    const char* x = "1233423423436564566";
    bool g = true;

    bool c;
    std::cout << "1 - SERVER | 0 - CLIENT\n";
    std::cin >> c;
    if (c) {
        serverLabel:
        SimpleServer server;

        // should never return false (fail)
        server.start(port, port);

        while (1) {
            if (server.isRunning()) {
                server.update();
            }
            if (g) {
                SetConsoleTitle(z);
            } else {
                SetConsoleTitle(x);
            }
            g = !g;
        }

        server.stop();
        goto serverLabel;
    } else {
        clientLabel:
        SimpleClient client;

        if (!client.connect(sf::IpAddress::LocalHost, port, port, sf::seconds(5))) {
            goto clientLabel;
        }

        while (1) {
            if (client.isConnected()) {
                client.update();
            }
            if (g) {
                SetConsoleTitle(z);
            } else {
                SetConsoleTitle(x);
            }
            g = !g;
        }

        client.disconnect();
        goto clientLabel;
    }

    // while (1)
    // c.test();
    //  netTest::testMain();
    /*

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        unit.moveToTarget(deltaTime);

        window.clear();
        grid.render();
        window.draw(text);
        window.display();

        deltaTime = clock.getElapsedTime().asSeconds();
        text.setString(std::to_string(static_cast<int>(1.0f / deltaTime)));

        clock.restart();
    }*/
}
