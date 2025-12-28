#pragma once

#include "PacketNode.hpp"
#include "Session.hpp"
#include <cstdint>
#include <string>

class LogicNode {
    friend class LogicSystem;
public:
    LogicNode() noexcept = default;
    LogicNode(const LogicNode& other) noexcept = default;
    LogicNode(std::shared_ptr<Session> session, uint16_t type, std::string_view message) noexcept;
    LogicNode(std::shared_ptr<Session> session, const RecieveNode& node) noexcept;
    LogicNode(LogicNode&& rvalue) noexcept;
    LogicNode& operator=(const LogicNode& lvalue) noexcept = default;
    LogicNode& operator=(LogicNode&& rvalue) noexcept;

private:
    std::shared_ptr<Session> session;
    uint16_t type;
    std::string message;
};
