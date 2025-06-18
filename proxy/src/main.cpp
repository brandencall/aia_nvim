#include "websocket/websocket.h"

int main() {
    websocket::WebSocket ws;
    if (!ws.setup_ws_server(9002)){
        return 1;
    }
    return 0;
}
