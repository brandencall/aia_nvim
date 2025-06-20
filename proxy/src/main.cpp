#include "tcp/tcp.h"
#include <iostream>
#include <ostream>
#include <string>

int main() {
    const char* ip = "10.0.0.234";
    int port = 22222;
    int serverSocket = createServerSocket(ip, port);
    if (serverSocket == -1) return -1;
    while (true) {
        std::string prompt = getPrompt(serverSocket);
        if (!prompt.empty()){
            std::cout << "Client said: " << prompt << std::endl;
        }
    }
    return 0;
}
