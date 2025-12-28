#pragma once

#include "PacketNode.hpp"
#include <boost/asio.hpp>

class Client {
public:
    Client();
    void connect(boost::asio::ip::tcp::endpoint serverEP);
    void connect(boost::asio::ip::tcp::endpoint serverEP, boost::system::error_code& ec) noexcept;
    void sendPacket(const SendNode& node);
    void sendPacket(const SendNode& node, boost::system::error_code& ec);
    RecieveNode recievePacket();
    RecieveNode recievePacket(boost::system::error_code& ec);

private:
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::socket socket;
};
