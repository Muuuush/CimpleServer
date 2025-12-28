#include "PacketNode.hpp"

#include <boost/asio.hpp>
#include <cstring>
#include <spdlog/spdlog.h>

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

std::string PacketNode::getMessage() {
    return std::string(data + HEADER_SECTION, totalLength - HEADER_SECTION);
}

SendNode::SendNode(uint16_t type, const char* data, size_t data_length) noexcept {
    if (data_length > MAX_DATA_LENGTH) {
        spdlog::warn("Node is too long to send. It will be truncated.");
        data_length = MAX_DATA_LENGTH;
    }

    this->data = new char[HEADER_SECTION + data_length];
    this->currLength = 0;
    this->totalLength = HEADER_SECTION + data_length;
    *reinterpret_cast<uint16_t*>(this->data) = boost::asio::detail::socket_ops::host_to_network_short(type);
    *reinterpret_cast<uint16_t*>(this->data + TYPE_SECTION) = boost::asio::detail::socket_ops::host_to_network_short(totalLength);
    std::memcpy(this->data + HEADER_SECTION, data, data_length);
}

SendNode::SendNode(uint16_t type, std::string_view data) noexcept : SendNode(type, data.data(), data.length()) {}

RecieveNode::RecieveNode() noexcept {
    data = new char[MAX_LENGTH]{};
    currLength = 0;
    totalLength = 0;
}
