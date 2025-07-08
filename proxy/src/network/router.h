#pragma once
#include "../ModelManager.h"
#include "../database/chat_repository.h"
#include "../database/project_repository.h"
#include "tcp.h"
#include <string>
#include <utility>

namespace network {

class Router {
  public:
    Router(int clientSocket, ModelManager &modelManager) : _clientSocket(clientSocket), _modelManager(modelManager) {}
    void routeRequest(const ClientRequest &request);
    void handleResponse(std::pair<long, std::string> response, const ClientRequest &request);
    void handlePromptRequest(const ClientRequest &request);

  private:
    int _clientSocket;
    ModelManager &_modelManager;
    int _requestsAttempted = 0;
    int _requestsLimit = 5;

    void handle404Response(const ClientRequest &request);
    void handle429Response(const ClientRequest &request);
    void handle509Response(const ClientRequest &request);
    void handleNewProjectRequest(const ClientRequest &request);
    void handleSuccessfulResponse(const ClientRequest request, std::string response);
};
} // namespace network
