#pragma once

#include "Server.hpp"

#include "PacketNode.hpp"

#include <boost/asio.hpp>
#include <boost/uuid.hpp>

class Session : public std::enable_shared_from_this<Session> {
    friend Server;
public:
    Session(Server* server);
    ~Session();

    void startRecieving();
    void unregister();

    static void HandleHead(std::shared_ptr<Session> session, const boost::system::error_code& ec, std::shared_ptr<RecieveNode> node);
    static void HandleRecieve(std::shared_ptr<Session> session, const boost::system::error_code& ec, std::shared_ptr<RecieveNode> node);
    static void HandleSend(std::shared_ptr<Session> session, const boost::system::error_code& ec);

private:
    boost::uuids::uuid uuid;
    Server* server;
    boost::asio::ip::tcp::socket socket;
};
