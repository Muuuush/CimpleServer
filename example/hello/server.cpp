#include "Server.hpp"
#include "LogicSystem.hpp"
#include <iostream>

std::atomic_int helloCnt;
void recieveHello(std::shared_ptr<Session> session, uint16_t type, const std::string& message) {
    auto ep = session->getRemoteEndpoint();
    // std::cout << "Recieve: \"" << message << "\" from " << ep.address().to_string() << ":" << ep.port() << std::endl;
    helloCnt++;
}

int main() {
    try {
        Server server(10086, ServerSetting());
        std::unordered_map<uint16_t, LogicSystem::callbackFunction> callbacks;
        callbacks.insert({0, recieveHello});
        LogicSystem::setCallbacks(callbacks);
        server.start();
        std:: cout << "Total recieved hello count: " << helloCnt << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[Error]: " << e.what() << std::endl;
    }
    return 0;
}
