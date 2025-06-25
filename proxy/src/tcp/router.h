#pragma once
#include "../ModelManager.h"
#include <string>
#include <utility>

class Router {
  public:
    Router(int clientSocket) : _clientSocket(clientSocket) {}
    void routeRequest(std::string prompt);

  private:
    int _clientSocket;
    int _requestsAttempted;
    int _requestsLimit = 5;
    ModelManager _modelManager = ModelManager::getInstance();

    void handleResponse(std::pair<long, std::string> response, std::string prompt);
};
