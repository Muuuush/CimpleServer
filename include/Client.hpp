#pragma once

#include "PacketNode.hpp"
#include <boost/asio.hpp>

class Client {
public:
    Client();
    void connect(boost::asio::ip::tcp::endpoint serverEP);
    void sendPacket(SendNode node);
    RecieveNode recievePacket();

private:
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::socket socket;
};
