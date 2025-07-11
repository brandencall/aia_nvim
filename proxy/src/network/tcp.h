#pragma once

#include "router.h"
#include <algorithm>
#include <string>

namespace network {
int createServerSocket(const char *ip, int port);
int acceptClient(int serverSocket);
void clientSession(int clientSocket, ModelManager &modelManager);
std::optional<ClientRequest> handleClient(int clientSocket);
std::string removeNuls(const std::string &input);
void sendMsg(int clientSocket, std::string msg);
} // namespace network
