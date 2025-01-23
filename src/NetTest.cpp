#include <iostream>
#include <SFML/Network.hpp>
#include <thread>

using namespace std;

constexpr uint16_t port = 33560;

namespace netTest {
    static void serverTest() {
        cout << "SERVER" << endl;

        sf::TcpListener serverListener;
        if (serverListener.listen(port) != sf::Socket::Status::Done) {
            cout << "error";
            return;
        }

        serverListener.setBlocking(false);

        // Could've used multi-threading but non-blocking is more interesting
        sf::TcpSocket socket;
        while (1) {
            // "server-side logic code"
            // ....

            // client new connections
            sf::Socket::Status connectStatus = serverListener.accept(socket);
            switch (connectStatus) {
                case sf::Socket::Status::Done:
                    cout << "Some mf connected\n";
                    break;
                case sf::Socket::Status::NotReady:
                    break;
                default:
                    cout << "new client connect error\n";
            }
        }

        serverListener.close();
    }

    static void clientTest() {
        cout << "CLIENT" << endl;

        cin;

        sf::TcpSocket client;

        client.setBlocking(false);

        sf::Socket::Status status = sf::Socket::Status::NotReady;
        while (1) {
            // "game loop"

            // "graphics/logic code"
            // ....

            // networking code
            if (status == sf::Socket::Status::Done)
                continue;
            cout << (int) status;

            switch (status) {
                case sf::Socket::Status::Done:
                    cout << "Client connected\n";
                    break;
                case sf::Socket::Status::NotReady:
                    status = client.connect(sf::IpAddress::LocalHost, port, sf::seconds(1));
                    break;
                default:
                    cout << "ERROR\n";
                    return;
            }
        };

        client.disconnect();
    }

    void testMain() {
        bool a;
        cout << "CLIENT - 0 | SERVER - 1, INPUT: ";
        cin >> a;
        cout << "\n";

        if (a)
            serverTest();
        else
            clientTest();
    }
}  // namespace netTest
