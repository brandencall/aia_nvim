#pragma once

#include <string>

int createServerSocket(const char* ip, int port);
int acceptClient(int serverSocket);
void clientSession(int clientSocket);
std::string handleClient(int clientSocket);
void sendMsg(int clientSocket, std::string msg);
