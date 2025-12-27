#pragma once

#include <cstddef>
#include <cstdint>

class PacketNode {
public:
    ~PacketNode();

public:
    static const uint16_t MAX_LENGTH = 0xffff;
    static const size_t LENGTH_SECTION = sizeof(uint16_t);
public:
    char* data;
    uint16_t currLength;
    uint16_t totalLength;
};

class SendNode final : public PacketNode {
public:
    SendNode(char* data, uint16_t length);
};

class RecieveNode final : public PacketNode {
public:
    RecieveNode();
};
