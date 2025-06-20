#pragma once

#include <string>
int createServerSocket(const char* ip, int port);
std::string getPrompt(int serverSocket);
