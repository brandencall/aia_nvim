#pragma once
#include <iostream>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/error.hpp>
#include <websocketpp/logger/levels.hpp>
#include <websocketpp/server.hpp>

namespace websocket {

class WebSocket {
  public:
    using server = websocketpp::server<websocketpp::config::asio>;
    using connection_hdl = websocketpp::connection_hdl;
    using message_ptr = server::message_ptr;

    WebSocket();
    ~WebSocket();

    bool setup_ws_server(int port);
    static void on_message(connection_hdl hdl, message_ptr msg, server *s);

  private:
    server wsServer;
};
} // namespace websocket
