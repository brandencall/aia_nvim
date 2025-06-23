#include "tcp/tcp.h"
#include <cstdlib>
#include <iostream>
#include <thread>
#include <unistd.h>
#include "utils/env_loader.h"


int main() {
    loadEnvFile(".env");

    const char *ip = "10.0.0.234";
    int port = 22222;
    int serverSocket = createServerSocket(ip, port);
    if (serverSocket == -1)
        return -1;
    while (true) {
        int clientSocket = acceptClient(serverSocket);
        if (clientSocket == -1) break;
        std::thread(clientSession, clientSocket).detach();
    }
    close(serverSocket);
    return 0;
}
