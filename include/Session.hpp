#pragma once

#include "PacketNode.hpp"

#include <boost/asio.hpp>
#include <boost/uuid.hpp>
#include <sstream>

class Server;
class Session : public std::enable_shared_from_this<Session> {
    friend Server;
public:
    Session(Server* server);
    ~Session();

    void startRecieving();
    void send(SendNode node);

    boost::uuids::uuid getUUID() const noexcept { return uuid; }
    boost::asio::ip::tcp::endpoint getRemoteEndpoint() const noexcept { return socket.remote_endpoint(); }
    std::string toString() {
        std::stringstream ss;
        ss << socket.remote_endpoint().address().to_string() << ":" << socket.remote_endpoint().port()
            << " [" << uuid << "]";
        return ss.str();
    }

private:
    void close();

    static void HandleHead(std::shared_ptr<Session> session, const boost::system::error_code& ec, std::shared_ptr<RecieveNode> node);
    static void HandleRecieve(std::shared_ptr<Session> session, const boost::system::error_code& ec, std::shared_ptr<RecieveNode> node);
    static void HandleSend(std::shared_ptr<Session> session, const boost::system::error_code& ec);

private:
    boost::uuids::uuid uuid;
    Server* server;
    boost::asio::ip::tcp::socket socket;
};
