#include <iostream>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/error.hpp>
#include <websocketpp/logger/levels.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg, server *s) {
    try {
        std::string received = msg->get_payload();
        std::cout << "Received: " << received << std::endl;
        std::string response = "Echo: " + received;
        s->send(hdl, response, msg->get_opcode());
    } catch (const websocketpp::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    try {
        server wsServer;
        wsServer.clear_access_channels(websocketpp::log::alevel::all);
        wsServer.init_asio();
        wsServer.set_message_handler(std::bind(on_message, std::placeholders::_1, std::placeholders::_2, &wsServer));
        wsServer.listen(9002);
        wsServer.start_accept();
        std::cout << "Websocket server running on port 9002" << std::endl;
        wsServer.run();
    } catch (const websocketpp::exception &e) {
        std::cerr << "websocket err " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
