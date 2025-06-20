#include "websocket.h"
#include <functional>
#include <iostream>

namespace websocket {

WebSocket::WebSocket() {
    wsServer.clear_access_channels(websocketpp::log::alevel::all);
    wsServer.init_asio();
}

WebSocket::~WebSocket() { wsServer.stop(); }

void WebSocket::on_open(connection_hdl hdl){
    std::cout << "Connection was established." << std::endl;
}

void WebSocket::on_message(connection_hdl hdl, message_ptr msg) {
    try {
        std::string received = msg->get_payload();
        std::cout << "Received: " << received << std::endl;
        std::string response = "Echo: " + received;
        this->wsServer.send(hdl, response, msg->get_opcode());
    } catch (const websocketpp::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

bool WebSocket::setup_ws_server(int port) {
    try {
        wsServer.set_open_handler(std::bind(&WebSocket::on_open, this, std::placeholders::_1));
        wsServer.set_message_handler(
            std::bind(&WebSocket::on_message, this, std::placeholders::_1, std::placeholders::_2));
        wsServer.listen(port);
        wsServer.start_accept();
        std::cout << "Websocket server running on port 9002" << std::endl;
        wsServer.run();
        return true;
    } catch (const websocketpp::exception &e) {
        std::cerr << "websocket err " << e.what() << std::endl;
        return false;
    }
}
} // namespace websocket
