#pragma once

#include <cstdint>
#include <string_view>

class PacketNode {
public:
    ~PacketNode() noexcept;
    PacketNode() noexcept = default;
    PacketNode(const PacketNode& other) noexcept;
    PacketNode(PacketNode&& rvalue) noexcept;
    PacketNode& operator=(const PacketNode& other) noexcept;
    PacketNode& operator=(PacketNode&& rvalue) noexcept;

public:
    static constexpr int TYPE_SECTION = sizeof(uint16_t);
    static constexpr int LENGTH_SECTION = sizeof(uint16_t);
    static constexpr uint16_t MAX_LENGTH = 0xffff;
    static constexpr uint16_t MAX_DATA_LENGTH = MAX_LENGTH - (TYPE_SECTION + LENGTH_SECTION);
public:
    char* data;
    uint16_t currLength;
    uint16_t totalLength;
};

class SendNode final : public PacketNode {
public:
    SendNode(uint16_t type, const char* data, uint16_t length) noexcept;
    SendNode(uint16_t type, std::string_view data) noexcept;
};

class RecieveNode final : public PacketNode {
public:
    RecieveNode() noexcept;
};
