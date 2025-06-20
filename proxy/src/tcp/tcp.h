#pragma once

#include <string>

int createServerSocket(const char* ip, int port);
int acceptClient(int serverSocket);
void handleClient(int clientSocket);
