#include "ServerConnection.hpp"
#include <SFML/Network.hpp>

#pragma once
namespace net {

    class ServerListener {
    public:
        ServerListener();
        void update();

        void broadcastTCP(packet::BasePacket);
        void broadcastUDP(packet::BasePacket);

        bool start(uint16_t tcpPort, uint16_t udpPort);
        void stop();

        void setSendRetryAmount(uint32_t);
        void setMaxPacketsPerLoop(uint32_t);

        void setMinKeepAliveTime(sf::Time);

        ServerConnection getConnection(sf::IpAddress, uint16_t);
        size_t getConnectionCount();

        bool isRunning() const;

    protected:
        virtual void onReceiveTCP(ServerConnection, sf::Packet) = 0;
        virtual void onReceiveUDP(ServerConnection, sf::Packet) = 0;
        virtual void onNewConnection(ServerConnection) = 0;
        virtual void onConnectionDisconnect(ServerConnection) = 0;

    private:
        sf::TcpListener m_tcpListener;
        std::shared_ptr<sf::UdpSocket> m_udpSocket;
        bool m_running;

        // UDP Port; used to filter out queue while being inserted to
        // Not a very ideal approach but who cares
        std::map<std::pair<sf::IpAddress, uint16_t>, ServerConnection> m_connections;

        uint32_t m_retryAmount;
        uint32_t m_maxPacketsPerLoop;

        // client must send at least one KA packet within the specified interval
        // MUST BE HIGHER THAN CLIENT'S KA INTERVAL
        sf::Time m_minKeepAliveTime;
    };
}  // namespace net
