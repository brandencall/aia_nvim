
#include "router.h"
#include "../ModelManager.h"
#include "tcp.h"

void routeRequest(int clientSocket, std::string prompt) {
    ModelManager modelManager = ModelManager::getInstance();
    std::shared_ptr<models::BaseModel> currentModel = modelManager.getCurrentAvailableModel();
    // TODO: need to validate currentModel limits. Need to handle failed responses as well
    std::string response = currentModel->processPrompt(prompt);
    if (!response.empty()) {
        sendMsg(clientSocket, response);
    } else {
        sendMsg(clientSocket, "Error while processing prompt");
    }
}
