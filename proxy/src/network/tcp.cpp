#include "tcp.h"
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <limits.h>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace network {

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

void clientSession(int clientSocket, ModelManager &modelManager) {
    Router router{clientSocket, modelManager};
    while (true) {
        auto prompt = handleClient(clientSocket);
        if (!prompt)
            break;
        router.routeRequest(*prompt);
    }
    close(clientSocket);
}

std::optional<ClientRequest> handleClient(int clientSocket) {
    uint32_t len_network = 0;
    ssize_t headerBytes = recv(clientSocket, &len_network, sizeof(len_network), MSG_WAITALL);
    if (headerBytes == 0) {
        std::cout << "Client disconnected gracefully\n";
        return std::nullopt;
    } else if (headerBytes < 0) {
        perror("recv failed (header)");
        return std::nullopt;
    }

    uint32_t messageLength = ntohl(len_network);

    std::string message;
    message.resize(messageLength);

    size_t totalReceived = 0;
    while (totalReceived < messageLength) {
        ssize_t bytes = recv(clientSocket, &message[totalReceived], messageLength - totalReceived, 0);
        if (bytes <= 0) {
            perror("recv failed (payload)");
            return std::nullopt;
        }
        totalReceived += bytes;
    }

    std::cout << "Client sent: " << message << std::endl;

    std::string confirmation = "Processing...";
    sendMsg(clientSocket, confirmation);
    std::cout << confirmation << std::endl;

    try {
        std::string cleanMessage = message;
        std::cout << cleanMessage << '\n';
        json j = json::parse(cleanMessage);
        ClientRequest request = j.get<ClientRequest>();
        return request;
    } catch (const std::exception &e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return std::nullopt;
    }
}

std::string removeNuls(const std::string &input) {
    std::string clean = input;
    clean.erase(std::remove(clean.begin(), clean.end(), '\0'), clean.end());
    return clean;
}

void sendMsg(int clientSocket, std::string msg) { send(clientSocket, msg.c_str(), msg.size(), 0); }

} // namespace network
