#include "Session.hpp"

#include "Server.hpp"
#include "LogicSystem.hpp"
#include <iostream>

using namespace boost::asio;

Session::Session(Server* server)
    : uuid(boost::uuids::random_generator()()),
    server(server), socket(server->ioc) {}

Session::~Session() {}

void Session::startRecieving() {
    auto node = std::make_shared<RecieveNode>();
    async_read(
        this->socket,
        buffer(node->data, PacketNode::TYPE_SECTION + PacketNode::LENGTH_SECTION),
        std::bind(
            &Session::HandleHead,
            shared_from_this(),
            std::placeholders::_1,
            node
        )
    );
}

void Session::send(SendNode node) {
    async_write(
        socket,
        buffer(node.data, node.totalLength),
        std::bind(
            &Session::HandleSend,
            shared_from_this(),
            std::placeholders::_1
        )
    );
}

void Session::close() {
    server->sessions.erase(uuid);
}

void Session::HandleHead(std::shared_ptr<Session> session, const boost::system::error_code& ec, std::shared_ptr<RecieveNode> node) {
    if (ec.failed()) {
        if (ec == error::eof)
            std::cout << "[Log]: Connection closed by peer. " << session->toString() << std::endl;
        else
            std::cerr << "[Error]: " << ec.what() << ", from " << session->toString() << std::endl;
        session->close();
    } else {
        node->totalLength = detail::socket_ops::network_to_host_short(*reinterpret_cast<uint16_t*>(node->data + PacketNode::TYPE_SECTION));
        node->currLength = PacketNode::TYPE_SECTION + PacketNode::LENGTH_SECTION;
        async_read(
            session->socket,
            buffer(node->data + node->currLength, node->totalLength - node->currLength),
            std::bind(
                &Session::HandleRecieve,
                session,
                std::placeholders::_1,
                node
            )
        );
    }
}

void Session::HandleRecieve(std::shared_ptr<Session> session, const boost::system::error_code& ec, std::shared_ptr<RecieveNode> node) {
    if (ec.failed()) {
        if (ec == error::eof)
            std::cout << "[Log]: Connection closed by peer. " << session->toString() << std::endl;
        else
            std::cerr << "[Error]: " << ec.what() << ", from " << session->toString() << std::endl;
        session->close();
    } else {
        LogicSystem::getInstance(session->server->logicQueueCapacity, session->server->logicWorkerNum)->registerNode(LogicNode(session, *node));
        session->startRecieving();
    }
}

void Session::HandleSend(std::shared_ptr<Session> session, const boost::system::error_code& ec) {
    if (ec.failed()) {
        std::cerr << "[Error]: " << ec.what() << ", from " << session->toString() << std::endl;
        session->close();
    } else {
        // send seccuessfully
    }
}
