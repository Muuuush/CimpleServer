#include "Session.hpp"

#include <iostream>

using namespace boost::asio;

Session::Session(Server* server)
    : uuid(boost::uuids::random_generator()()),
    server(server), socket(server->ioc) {
    // std::cout << "Session " << boost::uuids::to_string(uuid) << " created" << std::endl;
}

void Session::startRecieving() {
    auto node = std::make_shared<RecieveNode>();
    async_read(
        this->socket,
        buffer(node->data, PacketNode::LENGTH_SECTION),
        std::bind(
            &Session::HandleHead,
            shared_from_this(),
            std::placeholders::_1,
            node
        )
    );
}

Session::~Session() {
    // std::cout << "Session " << boost::uuids::to_string(uuid) << " deleted" << std::endl;
}

void Session::unregister() {
    server->sessions.erase(uuid);
}

void Session::HandleHead(std::shared_ptr<Session> session, const boost::system::error_code& ec, std::shared_ptr<RecieveNode> node) {
    if (ec.failed()) {
        if (ec == error::eof)
            std::cout << "Connection closed by peer" << std::endl;
        else
            std::cerr << "[Error]: " << ec.what() << std::endl;
        session->unregister();
    } else {
        node->totalLength = detail::socket_ops::network_to_host_short(*reinterpret_cast<uint16_t*>(node->data));
        node->currLength = PacketNode::LENGTH_SECTION;
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

void Session::HandleRecieve(std::shared_ptr<Session> session, const boost::system::error_code& ec, std::shared_ptr<RecieveNode> node)
{
    if (ec.failed()) {
        if (ec == error::eof)
            std::cout << "Connection closed by peer" << std::endl;
        else
            std::cerr << "[Error]: " << ec.what() << std::endl;
        session->unregister();
    } else {
        // node is recieved
        // TODO: get handled
        std::cout << "[Recieve]: " << string_view(node->data + PacketNode::LENGTH_SECTION, node->totalLength - PacketNode::LENGTH_SECTION) << std::endl;

        session->startRecieving();
    }
}

void Session::HandleSend(std::shared_ptr<Session> session, const boost::system::error_code& ec) {
    if (ec.failed()) {
        std::cerr << "[Error]: " << ec.what() << std::endl;
        session->unregister();
    } else {
        // send seccuess
    }
}
