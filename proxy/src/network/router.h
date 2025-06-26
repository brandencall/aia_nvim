#pragma once
#include "../ModelManager.h"
#include "tcp.h"
#include <string>
#include <utility>

namespace network {

class Router {
  public:
    Router(int clientSocket) : _clientSocket(clientSocket) {}
    void routeRequest(const ClientRequest &request);

  private:
    int _clientSocket;
    int _requestsAttempted;
    int _requestsLimit = 5;
    ModelManager _modelManager = ModelManager::getInstance();

    void handleResponse(std::pair<long, std::string> response, const ClientRequest &request);
};
} // namespace network
