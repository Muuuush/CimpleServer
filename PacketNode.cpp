#include "PacketNode.hpp"

#include <boost/asio.hpp>
#include <cstring>

PacketNode::~PacketNode() {
    delete data;
}

SendNode::SendNode(char* data, uint16_t length) {
    this->data = new char[length + LENGTH_SECTION];
    *reinterpret_cast<uint16_t*>(this->data) = boost::asio::detail::socket_ops::host_to_network_short(length + LENGTH_SECTION);
    this->currLength = 0;
    this->totalLength = length + LENGTH_SECTION;
    std::memcpy(this->data + LENGTH_SECTION, data, length);
}

RecieveNode::RecieveNode() {
    data = new char[MAX_LENGTH]{};
    currLength = 0;
    totalLength = 0;
}
