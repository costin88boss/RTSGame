#include <SFML/Network.hpp>
#include "packet/Packets.h"

#pragma once
namespace net {
    class ClientListener {
    public:
        ClientListener();
        void update();

        bool sendTCP(packet::BasePacket);
        bool sendUDP(packet::BasePacket);

        // BLOCKING function!
        bool connect(sf::IpAddress serverIp, uint16_t tcpPort, uint16_t udpPort, sf::Time timeout);
        void disconnect();

        void setKeepAliveInterval(sf::Time);
        void setSendRetryAmount(uint32_t);

        sf::IpAddress getServerIp() const;
        uint16_t getServerUdpPort() const;
        uint16_t getServerTcpPort() const;

        bool isConnected() const;

    protected:
        virtual void onConnected() = 0;
        virtual void onDisconnect() = 0;
        virtual void onReceiveTCP(sf::Packet) = 0;
        virtual void onReceiveUDP(sf::Packet) = 0;

    private:
        sf::TcpSocket m_tcpSocket;
        sf::UdpSocket m_udpSocket;
        sf::IpAddress m_serverIp = sf::IpAddress::Any;
        uint16_t m_serverUdpPort;
        bool m_connected;

        uint32_t m_retryAmount;

        // MUST BE LOWER THAN SERVER'S KA INTERVAL
        sf::Time m_keepAliveInterval;

        bool m_kaTCP, m_kaUDP;
        sf::Clock m_keepAliveTimer;
    };
}  // namespace net
