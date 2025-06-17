#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <string>

typedef websocketpp::client<websocketpp::config::asio> client;

void on_open(client* c, websocketpp::connection_hdl hdl) {
    std::cout << "Connected to server" << std::endl;
    try {
        c->send(hdl, "Hello, server!", websocketpp::frame::opcode::text);
        std::cout << "Sent: Hello, server!" << std::endl;
    } catch (const websocketpp::exception& e) {
        std::cerr << "Error sending message: " << e.what() << std::endl;
    }
}

void on_message(client* c, websocketpp::connection_hdl hdl, client::message_ptr msg) {
    std::cout << "Received: " << msg->get_payload() << std::endl;
}

void on_close(client* c, websocketpp::connection_hdl hdl) {
    std::cout << "Disconnected from server" << std::endl;
}

int main() {
    try {
        client ws_client;
        ws_client.clear_access_channels(websocketpp::log::alevel::all); // Disable logging
        ws_client.init_asio();

        ws_client.set_open_handler(std::bind(on_open, &ws_client, std::placeholders::_1));
        ws_client.set_message_handler(std::bind(on_message, &ws_client, std::placeholders::_1, std::placeholders::_2));
        ws_client.set_close_handler(std::bind(on_close, &ws_client, std::placeholders::_1));

        websocketpp::lib::error_code ec;
        client::connection_ptr con = ws_client.get_connection("ws://server.brabs:9002", ec);
        if (ec) {
            std::cerr << "Connection error: " << ec.message() << std::endl;
            return 1;
        }

        ws_client.connect(con);
        std::cout << "Connecting to ws://localhost:9002" << std::endl;
        ws_client.run();
    } catch (const websocketpp::exception& e) {
        std::cerr << "WebSocket error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
