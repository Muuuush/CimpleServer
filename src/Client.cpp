#include "Client.hpp"

using namespace boost::asio;

Client::Client() : ioc(), socket(ioc) {}

void Client::connect(boost::asio::ip::tcp::endpoint serverEP) {
    socket.connect(serverEP);
}

void Client::sendPacket(SendNode node) {
    write(
        socket,
        buffer(node.data, node.totalLength)
    );
}

RecieveNode Client::recievePacket() {
    RecieveNode node;
    read(
        this->socket,
        buffer(node.data, PacketNode::TYPE_SECTION + PacketNode::LENGTH_SECTION)
    );
    node.totalLength = detail::socket_ops::network_to_host_short(*reinterpret_cast<uint16_t*>(node.data + PacketNode::TYPE_SECTION));
    node.currLength = PacketNode::TYPE_SECTION + PacketNode::LENGTH_SECTION;
    read(
        this->socket,
        buffer(node.data + node.currLength, node.totalLength - node.currLength)
    );
    node.currLength = node.totalLength;
    return node;
}
