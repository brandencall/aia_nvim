#pragma once

#include "client_request.h"
#include "router.h"
#include <string>

namespace network {
int createServerSocket(const char *ip, int port);
int acceptClient(int serverSocket);
void clientSession(int clientSocket);
std::optional<ClientRequest> handleClient(int clientSocket);
void sendMsg(int clientSocket, std::string msg);
} // namespace network
