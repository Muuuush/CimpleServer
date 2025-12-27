#include <iostream>
#include <boost/asio.hpp>

#include "PacketNode.hpp"

using namespace boost::asio;

const std::string SERVER_IP = "127.0.0.1";
const int PORT = 10086;

int main() {
    try {
        io_context ioContext;
        ip::tcp::endpoint serverEP(ip::make_address(SERVER_IP), PORT);
        ip::tcp::socket socket(ioContext);
        socket.connect(serverEP);
        std::cout << "Connect success to " << serverEP.address().to_string() << ":" << serverEP.port() << std::endl;

        std::cout << ">> ";
        std::string input;
        std::getline(std::cin, input);
        SendNode node(input.data(), input.size());
        std::cout << "Node lenth: " << node.totalLength * 2 << std::endl;
        char* data = new char[node.totalLength * 2]();
        memcpy(data, node.data, node.totalLength);
        memcpy(data + node.totalLength, node.data, node.totalLength);
        write(socket, buffer(data, node.totalLength * 2));
        std::cout << "Message sent" << std::endl;

        sleep(3);

        // char reply[MAX_LENGTH] = {};
        // boost::system::error_code ec;
        // size_t length = socket.read_some(buffer(reply, MAX_LENGTH), ec);
        // if (ec.failed()) throw boost::system::system_error(ec);
        // std::cout << "[Receive]: " << std::string_view(reply, length) << std::endl;
        socket.close();
    } catch (const boost::system::system_error& e) {
        std::cerr << "[Error]: " << e.what() << std::endl;
    }
    return 0;
}
