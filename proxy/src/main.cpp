#include "tcp/tcp.h"
#include <unistd.h>

int main() {
    const char *ip = "10.0.0.234";
    int port = 22222;
    int serverSocket = createServerSocket(ip, port);
    if (serverSocket == -1)
        return -1;
    while (true) {
        int clientSocket = acceptClient(serverSocket);
        if (clientSocket != -1) {
            handleClient(clientSocket);
        }
    }
    close(serverSocket);
    return 0;
}
