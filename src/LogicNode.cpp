#include "LogicNode.hpp"

#include <boost/asio.hpp>

LogicNode::LogicNode(std::shared_ptr<Session> session, uint16_t type, std::string_view message) noexcept
    : session(session), type(type), message(message) {}

LogicNode::LogicNode(std::shared_ptr<Session> session, const RecieveNode& node) noexcept
    : session(session), type(boost::asio::detail::socket_ops::network_to_host_short(*reinterpret_cast<uint16_t*>(node.data))),
    message(node.data + PacketNode::TYPE_SECTION + PacketNode::LENGTH_SECTION, node.totalLength - (PacketNode::TYPE_SECTION + PacketNode::LENGTH_SECTION)) {}

LogicNode::LogicNode(LogicNode&& rvalue) noexcept
    : session(rvalue.session), type(rvalue.type), message(std::move(rvalue.message)) {}

LogicNode& LogicNode::operator=(LogicNode&& rvalue) noexcept {
    this->session = rvalue.session;
    this->type = rvalue.type;
    this->message = std::move(rvalue.message);
    return *this;
}
