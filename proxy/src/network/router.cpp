#include "router.h"
#include <iostream>
#include <ostream>
#include <string>
#include <utility>

namespace network {
void Router::routeRequest(const ClientRequest &request) {
    if (request.request_type == "new_project") {
        std::cout << "New Project request" << std::endl;
        handleNewProjectRequest(request);
    } else if (request.request_type == "prompt") {
        handlePromptRequest(request);
    }
}

void Router::handleNewProjectRequest(const ClientRequest &request) {
    bool insertedProject = database::insertProject(request);
    if (insertedProject) {
        sendMsg(_clientSocket, "Successfully inserted new Project: " + request.project_id);
    } else {
        sendMsg(_clientSocket, "Failed to insert project. Project may already exist.");
    }
}

void Router::handlePromptRequest(const ClientRequest &request) {
    if (_requestsAttempted < _requestsLimit) {
        std::shared_ptr<models::BaseModel> currentModel = _modelManager.getCurrentAvailableModel();
        // TODO: need to validate currentModel limits.
        // TODO: Need to pass the ClientRequest so that it can be project specific
        std::pair<long, std::string> response = currentModel->processPrompt(request);
        _requestsAttempted++;
        handleResponse(response, request);
    } else {
        sendMsg(_clientSocket, "Number of requests attempted exceeded the limit. Try again later. This is not "
                               "including the bandwith retry's");
        _requestsAttempted = 0;
        return;
    }
}

void Router::handleResponse(std::pair<long, std::string> response, const ClientRequest &request) {
    if (response.first == 404) {
        std::cerr << "The model endpoint doesn't exist" << std::endl;
        // switch to another model
        _modelManager.setNextModel();
        handlePromptRequest(request);
    } else if (response.first == 429) {
        std::cout << "TOO MANY REQUESTS! Switching models" << std::endl;
        // Need to switch models because we are at limit.
        _modelManager.setNextModel();
        handlePromptRequest(request);
    } else if (response.first == 509) {
        std::shared_ptr<models::BaseModel> currentModel = _modelManager.getCurrentAvailableModel();
        std::cout << "Bandwith exceeded. Retrying same model: " << currentModel->getKey() << std::endl;
        for (int i = 0; i < 3; i++) {
            response = currentModel->processPrompt(request);
            if (response.first == 509) {
                std::cout << "Retrying failed again for model: " << currentModel->getKey() << std::endl;
            } else if (response.second.empty()) {
                std::cout << "The response is empty for some reason" << std::endl;
                sendMsg(_clientSocket, "Error while processing prompt");
                break;
            } else {
                sendMsg(_clientSocket, response.second);
                break;
            }
        }
    } else if (response.second.empty()) {
        std::cout << "The response is empty for some reason" << std::endl;
        sendMsg(_clientSocket, "Error while processing prompt");
    } else {
        sendMsg(_clientSocket, response.second);
    }
}

} // namespace network
