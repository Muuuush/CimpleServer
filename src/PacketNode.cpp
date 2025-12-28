#include "PacketNode.hpp"

#include <boost/asio.hpp>
#include <cstring>
#include <iostream>

PacketNode::~PacketNode() noexcept {
    delete data;
}

PacketNode::PacketNode(const PacketNode& other) noexcept {
    currLength = other.currLength;
    totalLength = other.totalLength;
    data = new char[totalLength];
    std::memcpy(data, other.data, totalLength);
}

PacketNode::PacketNode(PacketNode&& rvalue) noexcept {
    currLength = rvalue.currLength;
    totalLength = rvalue.totalLength;
    data = rvalue.data;
    rvalue.data = nullptr;
}

PacketNode& PacketNode::operator=(const PacketNode& other) noexcept {
    if (&other == this) return *this;
    currLength = other.currLength;
    totalLength = other.totalLength;
    data = new char[totalLength];
    std::memcpy(data, other.data, totalLength);
    return *this;
}

PacketNode& PacketNode::operator=(PacketNode&& rvalue) noexcept {
    if (&rvalue == this) return *this;
    currLength = rvalue.currLength;
    totalLength = rvalue.totalLength;
    data = rvalue.data;
    rvalue.data = nullptr;
    return *this;
}

SendNode::SendNode(uint16_t type, const char* data, uint16_t data_length) noexcept {
    if (data_length > MAX_DATA_LENGTH) {
        std::cerr << "[WARN]: node is too long to send. It will be turncated.";
        data_length = MAX_DATA_LENGTH;
    }

    this->data = new char[TYPE_SECTION + LENGTH_SECTION + data_length];
    this->currLength = 0;
    this->totalLength = TYPE_SECTION + LENGTH_SECTION + data_length;
    *reinterpret_cast<uint16_t*>(this->data) = boost::asio::detail::socket_ops::host_to_network_short(type);
    *reinterpret_cast<uint16_t*>(this->data + TYPE_SECTION) = boost::asio::detail::socket_ops::host_to_network_short(totalLength);
    std::memcpy(this->data + TYPE_SECTION + LENGTH_SECTION, data, data_length);
}

SendNode::SendNode(uint16_t type, std::string_view data) noexcept : SendNode(type, data.data(), data.length()) {}

RecieveNode::RecieveNode() noexcept {
    data = new char[MAX_LENGTH]{};
    currLength = 0;
    totalLength = 0;
}
