#include "tcp.h"
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <limits.h>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <thread>

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

int acceptClient(int serverSocket) {
    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientLen);
    if (clientSocket < 0) {
        std::cerr << "Accept failed\n";
        return -1;
    }

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

    std::string allowedIP = "10.0.0.209";
    if (std::string(clientIP) != allowedIP) {
        std::cout << "Rejected connection " << clientIP << std::endl;
        close(clientSocket);
        return -1;
    }

    std::cout << "Accepted connection from " << clientIP << std::endl;
    return clientSocket;
}

void handleClient(int clientSocket) {
    char buffer[1024];
    while (true) {
        std::memset(buffer, 0, 1024);
        ssize_t bytesReceived = recv(clientSocket, buffer, 1024 - 1, 0);

        if (bytesReceived == 0) {
            std::cout << "Client disconnected gracefully" << std::endl;
            break;
        } else if (bytesReceived < 0) {
            perror("recv failed");
            break;
        } else {
            std::cout << "Client said: " << std::string(buffer) << std::endl;
            std::string confirmation = "Processing...";
            send(clientSocket, confirmation.c_str(), confirmation.size(), 0);
            std::cout << confirmation << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            // std::string result = process_request(std::string(buffer));
            // send(clientSocket, result.c_str, result.size(), 0);
            std::string placeholder ="This will be the LLM response!"; 
            std::cout << placeholder << std::endl;
            send(clientSocket, placeholder.c_str(), placeholder.size(), 0);
        }
    }
    close(clientSocket);
}
