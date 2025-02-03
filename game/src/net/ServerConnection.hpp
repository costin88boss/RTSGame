#pragma once
#include <SFML/Network.hpp>
#include "packet/Packets.hpp"

namespace net {

    class ServerConnection {
    public:
        bool sendTCP(packet::BasePacket);
        bool sendUDP(packet::BasePacket);

        sf::IpAddress getAddress() const;
        uint16_t getUDPPort() const;
        uint16_t getTCPPort() const;

        void updateRoundTripTime();

        // Returns last RTT since updateRoundTripTime was called
        sf::Time getRoundTripTime() const;

        void disconnect();

    private:
        ServerConnection(std::shared_ptr<sf::TcpSocket>, uint16_t, std::shared_ptr<sf::UdpSocket>);

        std::shared_ptr<sf::TcpSocket> m_tcpSocket = nullptr;
        // UDP socket only for sending; reading is managed by ServerListener
        // Also this is ServerListener's udpSocket
        std::shared_ptr<sf::UdpSocket> m_serverUdpSocket = nullptr;
        sf::Packet m_packetBuffer;

        std::vector<sf::Packet> m_udpQueue;

        sf::IpAddress m_address = sf::IpAddress::Any;
        uint16_t m_udpPort, m_tcpPort;
        bool m_exists;

        sf::Time m_lastRoundTripTime;

        bool m_kaTCP, m_kaUDP;
        sf::Clock m_keepAliveKickTimer;

        uint32_t m_retryAmount;

        // Linker magic
        friend class ServerListener;
    };

}  // namespace net
