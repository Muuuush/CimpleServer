#pragma once

#include <boost/asio.hpp>
#include <boost/uuid.hpp>
#include <unordered_map>

class Session;

class Server {
    friend Session;
public:
    Server(unsigned short port, int logicQueueCapacity = 1024, int logicWorkerNum = 1);
    void start();
private:
    void startAccept();

private:
    inline static int BACKLOG = 30;
    boost::asio::io_context ioc;
    const unsigned short port;
    const int logicQueueCapacity;
    const int logicWorkerNum;
    boost::asio::ip::tcp::acceptor acceptor;
    std::unordered_map<boost::uuids::uuid, std::shared_ptr<Session>> sessions;
};
