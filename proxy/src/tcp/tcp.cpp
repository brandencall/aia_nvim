#include "tcp.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <limits.h>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int createServerSocket(const char *ip, int port) {
    int serverSocket;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cout << "Error creating socket" << std::endl;
        return -1;
    }
    sockaddr_in service{};
    service.sin_family = AF_INET;
    service.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &service.sin_addr) <= 0) {
        std::cerr << "Invalid IP address\n";
        return -1;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(serverSocket, (sockaddr *)&service, sizeof(service)) == -1) {
        perror("bind failed");
        close(serverSocket);
        return -1;
    }
    if (listen(serverSocket, 5) == -1) {
        std::cout << "Error on listen" << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "Server listening on " << ip << ":" << port << std::endl;
    return serverSocket;
}

std::string getPrompt(int serverSocket) {
    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientLen);
    if (clientSocket < 0) {
        std::cerr << "Accept failed\n";
        return "";
    }

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

    std::string allowedIP= "10.0.0.209";
    if (std::string(clientIP) != allowedIP) {
        std::cout << "Rejected connection " << clientIP << std::endl;
        close(clientSocket);
        return "";
    }

    std::cout << "Accepted connection from " << clientIP << std::endl;

    char buffer[1024];
    std::memset(buffer, 0, 1024);
    ssize_t bytesReceived = recv(clientSocket, buffer, 1024 - 1, 0);
    std::string prompt = (bytesReceived > 0) ? std::string(buffer) : "";

    send(clientSocket, "OK\n", 3, 0);

    return prompt;
}
