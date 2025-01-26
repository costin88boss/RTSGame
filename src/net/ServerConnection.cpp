#include "ServerConnection.h"
#include "ServerListener.h"

#include <iostream>

#define SS sf::Socket::Status

net::ServerConnection::ServerConnection(
    std::shared_ptr<sf::TcpSocket> tcpSocket,
    uint16_t udpPort,
    std::shared_ptr<sf::UdpSocket> serverUdpSocket)
    : m_kaTCP(false)
    , m_kaUDP(false)
    , m_tcpPort(0)
    , m_udpPort(udpPort)
    , m_retryAmount(10)
    , m_serverUdpSocket(serverUdpSocket)
    , m_exists(true) {
    this->m_tcpSocket = tcpSocket;
    std::optional<sf::IpAddress> opt = this->m_tcpSocket->getRemoteAddress();
    if (!opt.has_value()) {
        return;
    }

    this->m_tcpSocket->setBlocking(false);
    this->m_tcpPort = tcpSocket->getRemotePort();
    this->m_address = opt.value();
}

bool net::ServerConnection::sendTCP(packet::BasePacket packet) {
    if (!this->m_exists) {
        return false;
    }

    for (uint8_t i = 0; i < this->m_retryAmount; i++) {
        SS status = this->m_tcpSocket->send(packet);
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

bool net::ServerConnection::sendUDP(packet::BasePacket packet) {
    if (!this->m_exists) {
        return false;
    }

    for (uint8_t i = 0; i < this->m_retryAmount; i++) {
        SS status = this->m_serverUdpSocket->send(packet, this->m_address, this->m_udpPort);
        switch (status) {
            case sf::Socket::Status::Done:
                std::cout << "SENT\n";
                return true;
            case sf::Socket::Status::NotReady:
                std::cout << "UDP Send error: Not ready\n";
                return false;
            case sf::Socket::Status::Disconnected:
                std::cout << "UDP Send error: Disconnected\n";
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

sf::IpAddress net::ServerConnection::getAddress() const {
    return this->m_address;
}

uint16_t net::ServerConnection::getUDPPort() const {
    return this->m_udpPort;
}

uint16_t net::ServerConnection::getTCPPort() const {
    return this->m_tcpPort;
}

void net::ServerConnection::updateRoundTripTime() {
    if (!this->m_exists) {
        return;
    }

    // Placed to prevent 'const' msg
    this->m_kaTCP = this->m_kaTCP;
    // TODO
}

sf::Time net::ServerConnection::getRoundTripTime() const {
    return this->m_lastRoundTripTime;
}

void net::ServerConnection::disconnect() {
    m_tcpSocket->disconnect();
    this->m_exists = false;
    m_keepAliveKickTimer.stop();

    // m_connections in ServerListener will be auto-handled
}
