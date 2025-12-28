#include "PacketNode.hpp"
#include "Client.hpp"
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;
std::atomic_int success;
void sendHello(int helloCnt) {
    try {
        Client client;
        ip::tcp::endpoint serverEP(ip::make_address("127.0.0.1"), 10086);
        client.connect(serverEP);
        SendNode node(0, "Hello server!");
        while (helloCnt--) {
            client.sendPacket(node);
            success++;
        }
    } catch (...) {
        return;
    }
}
int main() {
    const int helloPerThread = 200;
    const int threadCnt = 50;
    std::vector<std::thread> threads;
    threads.reserve(threadCnt);
    for (int i = 0; i < threadCnt; i++)
        threads.emplace_back(sendHello, helloPerThread);
    for (auto& t : threads) t.join();
    std::cout << "total " << success << " hello messages was sent." << std::endl;
    return 0;
}
