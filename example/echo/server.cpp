#include "CimpleServer.hpp"
#include <iostream>

void echo(std::shared_ptr<Session> session, uint16_t type, const std::string& message) {
    std::cout << "Receive: \"" << message << "\" from " << session->getRemoteEndpoint() << std::endl;
    session->send(SendNode(type, message));
}

int main() {
    Server server(10086);
    std::unordered_map<uint16_t, LogicSystem::CallbackFunction> callbacks;
    callbacks.insert({0, echo});
    LogicSystem::setCallbacks(callbacks);
    server.start();
    return 0;
}
