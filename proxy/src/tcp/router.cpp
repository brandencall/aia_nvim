
#include "router.h"
#include "tcp.h"
#include <iostream>
#include <string>
#include <utility>

void Router::routeRequest(std::string prompt) {
    if (_requestsAttempted < _requestsLimit) {
        std::shared_ptr<models::BaseModel> currentModel = _modelManager.getCurrentAvailableModel();
        // TODO: need to validate currentModel limits.
        std::pair<long, std::string> response = currentModel->processPrompt(prompt);
        _requestsAttempted++;
        handleResponse(response, prompt);
    } else {
        sendMsg(_clientSocket, "Number of requests attempted exceeded the limit. Try again later. This is not "
                               "including the bandwith retry's");
        _requestsAttempted = 0;
        return;
    }
}

void Router::handleResponse(std::pair<long, std::string> response, std::string prompt) {
    if (response.first == 404) {
        std::cerr << "The model endpoint doesn't exist" << std::endl;
        // switch to another model
        _modelManager.setNextModel();
        routeRequest(prompt);
    } else if (response.first == 429) {
        std::cout << "TOO MANY REQUESTS! Switching models" << std::endl;
        // Need to switch models because we are at limit.
        routeRequest(prompt);
    } else if (response.first == 509) {
        std::shared_ptr<models::BaseModel> currentModel = _modelManager.getCurrentAvailableModel();
        std::cout << "Bandwith exceeded. Retrying same model: " << currentModel->getKey() << std::endl;
        for (int i = 0; i < 3; i++) {
            response = currentModel->processPrompt(prompt);
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
