#include "ClientListener.h"
#include <SFML/Network.hpp>

#include <iostream>

#define SS sf::Socket::Status

// CLIENT Listener
net::ClientListener::ClientListener()
    : m_connected(false)
    , m_serverUdpPort(0)
    , m_retryAmount(10)
    , m_keepAliveInterval(sf::seconds(1))
    , m_kaTCP(false)
    , m_kaUDP(false) {
    this->m_tcpSocket.setBlocking(false);
    this->m_udpSocket.setBlocking(false);
    m_keepAliveTimer.reset();
}

bool net::ClientListener::sendTCP(packet::BasePacket packet) {
    if (!this->m_connected) {
        return false;
    }

    for (uint8_t i = 0; i < this->m_retryAmount; i++) {
        SS status = this->m_tcpSocket.send(packet);
        switch (status) {
            case sf::Socket::Status::Done:
                return true;
            case sf::Socket::Status::NotReady:
                std::cout << "TCP send error: Not ready\n";
                return false;
            case sf::Socket::Status::Disconnected:
                std::cout << "TCP send error: Disconnected\n";
                return false;
            case sf::Socket::Status::Error:
                std::cout << "TCP send error: Unknown\n";
                return false;
        }
        std::cout << "TCP send fail; Retrying\n";
    }
    std::cout << "TCP send error: Retry limit\n";
    return false;
}

bool net::ClientListener::sendUDP(packet::BasePacket packet) {
    if (!this->m_connected) {
        return false;
    }

    for (uint8_t i = 0; i < this->m_retryAmount; i++) {
        SS status = this->m_udpSocket.send(packet, this->m_serverIp, this->m_serverUdpPort);
        switch (status) {
            case sf::Socket::Status::Done:
                return true;
            case sf::Socket::Status::NotReady:
                std::cout << "UDP Send error: Not ready\n";
                return false;
            case sf::Socket::Status::Error:
                std::cout << "UDP Send error: Unknown\n";
                return false;
        }
        std::cout << "UDP send fail; Retrying\n";
    }
    std::cout << "UDP send error: Retry limit\n";
    return false;
}

bool net::ClientListener::connect(sf::IpAddress serverIp, uint16_t tcpPort, uint16_t udpPort, sf::Time timeout) {
    m_tcpSocket.setBlocking(true);
    m_udpSocket.setBlocking(true);

    if (this->m_tcpSocket.connect(serverIp, tcpPort, timeout) != SS::Done) {
        std::cout << "Can't connect TCP\n";
        return false;
    }
    if (this->m_udpSocket.bind(sf::Socket::AnyPort) != SS::Done) {
        std::cout << "System refused to bind UDP port\n";
        return false;
    }

    uint16_t localUdpPort = m_udpSocket.getLocalPort();

    this->m_connected = true;

    packet::BasePacket udpPortPacket;
    udpPortPacket << localUdpPort;

    if (!this->sendTCP(udpPortPacket)) {
        std::cout << "UDP connect error\n";
        m_tcpSocket.disconnect();
        m_udpSocket.unbind();
        return false;
    }

    std::cout << "Sent udp port: " << localUdpPort << "\n";

    this->m_serverUdpPort = udpPort;
    this->m_serverIp = serverIp;

    m_tcpSocket.setBlocking(false);
    m_udpSocket.setBlocking(false);
    m_keepAliveTimer.restart();

    this->m_kaTCP = true;
    this->m_kaUDP = true;

    this->onConnected();

    return true;
}

void net::ClientListener::disconnect() {
    if (!m_connected) {
        std::cout << "Client not connected; can't disconnect";
        return;
    }

    // TODO: send goodbye packet to server
    // currently the server will just time out the client

    this->m_connected = false;
    this->m_tcpSocket.disconnect();
    this->m_udpSocket.unbind();
    this->m_keepAliveTimer.reset();

    this->m_kaTCP = false;
    this->m_kaUDP = false;

    this->onDisconnect();
}

void net::ClientListener::setKeepAliveInterval(sf::Time newT) {
    this->m_keepAliveInterval = newT;
}

void net::ClientListener::setSendRetryAmount(uint32_t newR) {
    this->m_retryAmount = newR;
}

sf::IpAddress net::ClientListener::getServerIp() const {
    return this->m_serverIp;
}

uint16_t net::ClientListener::getServerUdpPort() const {
    return this->m_serverUdpPort;
}

uint16_t net::ClientListener::getServerTcpPort() const {
    return this->m_tcpSocket.getRemotePort();
}

bool net::ClientListener::isConnected() const {
    return m_connected;
}

void net::ClientListener::update() {
    if (!m_connected) {
        std::cout << "update() cannot be called when not connected to a server!\n";
        return;
    }

    {  // UDP Receive
        sf::Packet udpPacket;
        std::optional<sf::IpAddress> optIpAddr;
        uint16_t udpPort;
        while (this->m_udpSocket.receive(udpPacket, optIpAddr, udpPort) == SS::Done) {
            if (!optIpAddr.has_value() || udpPort == 0) {
                std::cout << "UDP receive error; No IP/Port\n";
                return;
            }
            if (optIpAddr.value() != this->m_serverIp || udpPort != this->m_serverUdpPort) {
                return;
            }

            this->m_kaUDP = true;

            if (udpPacket.getDataSize() == 0) {  // Keep Alive packet
                continue;
            }

            this->onReceiveUDP(udpPacket);
        }
    }

    {  // TCP Receive
        sf::Packet tcpPacket;
        while (this->m_tcpSocket.receive(tcpPacket) == SS::Done) {
            this->m_kaTCP = true;

            if (tcpPacket.getDataSize() == 0) {  // Keep Alive packet
                continue;
            }
            this->onReceiveTCP(tcpPacket);
        }
    }

    {
        // Keep Alive
        // Client will send an empty packet every x sec, server will respond with another empty packet
        // If server doesn't respond in the next x sec (should be immediate reply though), assumed dead
        // If client doesn't send in the next y sec (y > x), server will kick
        // x = client's KA interval | y = server's KA kick timer
        if (m_keepAliveTimer.getElapsedTime() > m_keepAliveInterval) {
            if (!this->m_kaTCP || !this->m_kaUDP) {
                std::cout << "Server not sending keep alive; Disconnecting\n";
                this->disconnect();
                return;
            }

            this->m_kaTCP = false;
            this->m_kaUDP = false;
            this->m_keepAliveTimer.restart();

            if (!this->sendTCP(packet::BasePacket())) {
                std::cout << "TCP keep Alive fail; Disconnecting\n";
                this->disconnect();
                return;
            }
            if (!this->sendUDP(packet::BasePacket())) {
                std::cout << "UDP keep Alive fail; Disconnecting\n";
                this->disconnect();
                return;
            }
            std::cout << "Sent KA\n";
        }
    }
}
