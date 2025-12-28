#include "Server.hpp"

#include "Session.hpp"
#include <iostream>

using namespace boost::asio;

Server::Server(unsigned short port, int logicQueueCapacity, int logicWorkerNum)
    : ioc(), port(port), logicQueueCapacity(logicQueueCapacity), logicWorkerNum(logicWorkerNum),
    acceptor(ioc, ip::tcp::endpoint(ip::tcp::v4(), port)) {}

void Server::start() {
    acceptor.listen(BACKLOG);
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([this](auto, auto) {
        ioc.stop();
    });
    try {
        startAccept();
        ioc.run();
    } catch (const std::exception& e) {
        std::cout << "[Error]: " << e.what() << std::endl;
    }
}

void Server::startAccept() {
    auto newSession = std::make_shared<Session>(this);
    acceptor.async_accept(
        newSession->socket,
        [newSession, this](const boost::system::error_code& ec) {
            this->sessions.insert({newSession->uuid, newSession});
            auto ep = newSession->getRemoteEndpoint();
            std::cout << "[LOG]: " << "Connected to " << newSession->toString() << "." << std::endl;
            newSession->startRecieving();

            startAccept();
        }
    );
}
