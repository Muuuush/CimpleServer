#include "Client.hpp"

using namespace boost::asio;

Client::Client() : ioc(), socket(ioc) {}

void Client::connect(boost::asio::ip::tcp::endpoint serverEP) {
    socket.connect(serverEP);
}

void Client::connect(boost::asio::ip::tcp::endpoint serverEP, boost::system::error_code& ec) noexcept {
    socket.connect(serverEP, ec);
}

void Client::sendPacket(const SendNode& node) {
    write(
        socket,
        buffer(node.data, node.totalLength)
    );
}

void Client::sendPacket(const SendNode& node, boost::system::error_code& ec) {
    write(
        socket,
        buffer(node.data, node.totalLength),
        ec
    );
}

RecieveNode Client::recievePacket() {
    RecieveNode node;
    read(
        this->socket,
        buffer(node.data, PacketNode::HEADER_SECTION)
    );
    node.totalLength = detail::socket_ops::network_to_host_short(*reinterpret_cast<uint16_t*>(node.data + PacketNode::TYPE_SECTION));
    node.currLength = PacketNode::HEADER_SECTION;
    read(
        this->socket,
        buffer(node.data + node.currLength, node.totalLength - node.currLength)
    );
    node.currLength = node.totalLength;
    return node;
}

RecieveNode Client::recievePacket(boost::system::error_code& ec) {
    RecieveNode node;
    read(
        this->socket,
        buffer(node.data, PacketNode::TYPE_SECTION + PacketNode::LENGTH_SECTION),
        ec
    );
    if (ec.failed()) return RecieveNode();

    node.totalLength = detail::socket_ops::network_to_host_short(*reinterpret_cast<uint16_t*>(node.data + PacketNode::TYPE_SECTION));
    node.currLength = PacketNode::TYPE_SECTION + PacketNode::LENGTH_SECTION;
    read(
        this->socket,
        buffer(node.data + node.currLength, node.totalLength - node.currLength)
    );
    if (ec.failed()) return RecieveNode();

    node.currLength = node.totalLength;
    return node;
}
