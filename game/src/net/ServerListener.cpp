#include "ServerListener.h"
#include <SFML/Network.hpp>

#include <SFML/Config.hpp>
#include <SFML/System.hpp>

#include <iostream>

#define SS sf::Socket::Status

// SERVER Listener
net::ServerListener::ServerListener()
    : m_running(false)
    , m_udpSocket(std::make_shared<sf::UdpSocket>(sf::UdpSocket()))
    , m_minKeepAliveTime(sf::seconds(3))
    , m_retryAmount(10)
    , m_maxPacketsPerLoop(200) {
    this->m_tcpListener.setBlocking(false);
    this->m_udpSocket->setBlocking(false);
}

void net::ServerListener::broadcastTCP(packet::BasePacket data) {
    for (std::map<std::pair<sf::IpAddress, uint16_t>, ServerConnection>::iterator it = this->m_connections.begin();
         it != this->m_connections.end(); it++) {
        it->second.sendTCP(data);
    }
}

void net::ServerListener::broadcastUDP(packet::BasePacket data) {
    for (std::map<std::pair<sf::IpAddress, uint16_t>, ServerConnection>::iterator it = this->m_connections.begin();
         it != this->m_connections.end(); it++) {
        it->second.sendUDP(data);
    }
}

bool net::ServerListener::start(uint16_t tcpPort, uint16_t udpPort) {
    if (this->m_running) {
        std::cout << "Server already running; Can't start\n";
        return false;
    }

    if (this->m_udpSocket->bind(udpPort) != SS::Done) {
        return false;
    }
    if (this->m_tcpListener.listen(tcpPort) != SS::Done) {
        return false;
    }
    this->m_running = true;
    return true;
}

void net::ServerListener::stop() {
    if (!m_running) {
        std::cout << "Server not running; Can't stop";
        return;
    }
    m_running = false;
    m_udpSocket->unbind();
    m_tcpListener.close();

    //    this->m_udpQueue.clear();

    // TODO: send goodbye packet to all clients

    m_connections.clear();
}

// client must send at least one KA packet within the specified interval
void net::ServerListener::setMinKeepAliveTime(sf::Time newT) {
    this->m_minKeepAliveTime = newT;
}

net::ServerConnection net::ServerListener::getConnection(sf::IpAddress ip, uint16_t tcp) {
    return this->m_connections.at(std::pair<sf::IpAddress, uint16_t>(ip, tcp));
}

size_t net::ServerListener::getConnectionCount() {
    return this->m_connections.size();
}

void net::ServerListener::setSendRetryAmount(uint32_t newR) {
    this->m_retryAmount = newR;
}

void net::ServerListener::setMaxPacketsPerLoop(uint32_t newM) {
    this->m_maxPacketsPerLoop = newM;
}

bool net::ServerListener::isRunning() const {
    return this->m_running;
}

static sf::Clock test;
static bool gotBack = true;

void net::ServerListener::update() {
    if (!this->m_running) {
        std::cout << "update() cannot be called when not running!\n";
        return;
    }

    std::shared_ptr<sf::TcpSocket> p_tcpSocket = std::make_shared<sf::TcpSocket>(sf::TcpSocket());
    if (this->m_tcpListener.accept(*p_tcpSocket) == SS::Done) {
        // get client's UDP port; Better ways exist though
        sf::Packet udpPortPacket;
        size_t recv = 0;

        SS status = p_tcpSocket->receive(udpPortPacket);
        if (status != SS::Done) {
            std::cout << "UDP initialisation error\n";
            p_tcpSocket->disconnect();
            return;
        }

        uint16_t remoteUdpPort;
        udpPortPacket >> remoteUdpPort;

        std::cout << "Got udp port: " << remoteUdpPort << "\n";

        ServerConnection connection = ServerConnection(p_tcpSocket, remoteUdpPort, this->m_udpSocket);

        connection.m_kaTCP = false;
        connection.m_kaUDP = false;
        connection.m_keepAliveKickTimer.restart();

        connection.m_retryAmount = this->m_retryAmount;

        this->onNewConnection((connection));
        this->m_connections.insert(std::pair<std::pair<sf::IpAddress, uint16_t>, ServerConnection>(
            std::pair<sf::IpAddress, uint16_t>(p_tcpSocket->getRemoteAddress().value(), remoteUdpPort), connection));
    }

    if (m_connections.empty()) {
        return;
    }

    // remove ALL dead connections before ever doing any network logic.
    bool allDeadRemoved;
    do {
        allDeadRemoved = true;
        for (std::map<std::pair<sf::IpAddress, uint16_t>, ServerConnection>::iterator it = this->m_connections.begin();
             it != this->m_connections.end(); it++) {
            if (!it->second.m_exists) {
                this->onConnectionDisconnect(it->second);
                this->m_connections.erase(it);
                allDeadRemoved = false;
                break;
            }
        }
    } while (!allDeadRemoved);

    {  // scope for vars | UDP Queueing
        sf::Packet udpPacket;
        std::optional<sf::IpAddress> optClientAddr;
        uint16_t udpPort;

        // TODO: Error check
        while (this->m_udpSocket->receive(udpPacket, optClientAddr, udpPort) == SS::Done) {
            if (optClientAddr.has_value()) {
                std::map<std::pair<sf::IpAddress, uint16_t>, ServerConnection>::iterator it =
                    this->m_connections.find(std::pair<sf::IpAddress, uint16_t>(optClientAddr.value(), udpPort));

                if (it == this->m_connections.end()) {
                    continue;
                }

                if (it->second.m_udpQueue.size() > this->m_maxPacketsPerLoop) {
                    if (!it->second.m_exists) {
                        continue;
                    }

                    std::cout << "Holy fuck this guy is spamming packets\n";
                    it->second.disconnect();
                    continue;
                }

                it->second.m_udpQueue.push_back(udpPacket);
            }
        }
    }

    for (std::map<std::pair<sf::IpAddress, uint16_t>, ServerConnection>::iterator it = this->m_connections.begin();
         it != this->m_connections.end(); it++) {
        {  // UDP Receive
            for (std::vector<sf::Packet>::iterator udpPacketIt = it->second.m_udpQueue.begin();
                 udpPacketIt != it->second.m_udpQueue.end(); udpPacketIt++) {
                if (udpPacketIt->getDataSize() == 0) {
                    it->second.m_kaUDP = true;
                    std::cout << "KA TCP echo\n";
                    it->second.sendUDP(packet::BasePacket());
                    continue;
                }
                this->onReceiveUDP(it->second, *udpPacketIt);
            }
            it->second.m_udpQueue.clear();
        }

        {  // TCP Receive
            sf::Packet tcpPacket;

            // TODO: Error check
            while (it->second.m_tcpSocket->receive(tcpPacket) == SS::Done) {
                if (tcpPacket.getDataSize() == 0) {
                    it->second.m_kaTCP = true;
                    std::cout << "KA UDP echo\n";
                    it->second.sendTCP(net::packet::BasePacket());
                    continue;
                }
                this->onReceiveTCP(it->second, tcpPacket);
            }
        }

        {
            // Keep Alive
            // Client will send an empty packet every x sec, server will respond with another empty packet
            // If server doesn't respond in the next x sec (should be immediate reply though), assumed dead
            // If client doesn't send in the next y sec (y > x), server will kick
            // x = client's KA interval | y = server's KA kick timer
            if (it->second.m_keepAliveKickTimer.getElapsedTime() > this->m_minKeepAliveTime) {
                if (!it->second.m_kaTCP || !it->second.m_kaUDP) {
                    std::cout << "Client not responding; Kicking\n";
                    it->second.disconnect();
                    continue;
                }

                it->second.m_kaTCP = false;
                it->second.m_kaUDP = false;
                it->second.m_keepAliveKickTimer.restart();
            }
        }
    }
}
