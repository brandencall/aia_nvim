#include "router.h"
#include "tcp.h"
#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace network {
void Router::routeRequest(const ClientRequest &request) {
    if (request.request_type == "upsert_project") {
        std::cout << "Project request" << std::endl;
        handleNewProjectRequest(request);
    } else if (request.request_type == "get_project") {
        handleGetProjectRequest(request);
    } else if (request.request_type == "prompt") {
        handlePromptRequest(request);
    }
}

void Router::handleNewProjectRequest(const ClientRequest &request) {
    bool upsertedProject = database::upsertProject(request);
    if (upsertedProject) {
        sendMsg(_clientSocket, "Successfully upserted new Project: " + request.project_id);
    } else {
        sendMsg(_clientSocket, "Failed to insert project...");
    }
}

void Router::handleGetProjectRequest(const ClientRequest &request) {
    std::optional<database::Project> project = database::getProject(request);
    if (project == std::nullopt) {
        sendMsg(_clientSocket, "No project exists for " + request.project_id);
    } else {
        std::cout << "The project context is: " << project->context << '\n';
        sendMsg(_clientSocket, project->context);
    }
}

void Router::handlePromptRequest(const ClientRequest &request) {
    if (_requestsAttempted < _requestsLimit) {
        std::shared_ptr<models::BaseModel> currentModel = _modelManager.getCurrentAvailableModel();
        // TODO: need to validate currentModel limits.
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
    if (response.first == 200) {
        handleSuccessfulResponse(request, response.second);
    } else if (response.first == 404) {
        handle404Response(request);
    } else if (response.first == 429) {
        handle429Response(request);
    } else if (response.first == 509) {
        handle509Response(request);
    } else if (response.second.empty()) {
        std::cout << "The response is empty for some reason" << std::endl;
        sendMsg(_clientSocket, "ERROR: The response is empty for some reason");
    } else {
        std::cout << "There was an error with the response: " << response.first << std::endl;
        sendMsg(_clientSocket, "There was an error with the response: " + std::to_string(response.first));
    }
}

void Router::handleSuccessfulResponse(const ClientRequest &request, const std::string &response) {
    sendMsg(_clientSocket, response);
    bool insertedChat = database::insertChat(request, response);
    if (insertedChat) {
        std::vector<std::string> latestConvos = database::getRecentConversations(request);
        std::string summary = utils::summarizeText(latestConvos);
        database::insertSummary(request, summary);
    }

    _requestsAttempted = 0;
}

void Router::handle404Response(const ClientRequest &request) {
    _modelManager.setNextModel();
    handlePromptRequest(request);
}

void Router::handle429Response(const ClientRequest &request) {
    std::cout << "TOO MANY REQUESTS! Switching models" << std::endl;
    _modelManager.setNextModel();
    handlePromptRequest(request);
}

void Router::handle509Response(const ClientRequest &request) {
    std::shared_ptr<models::BaseModel> currentModel = _modelManager.getCurrentAvailableModel();
    std::cout << "Bandwith exceeded. Retrying same model: " << currentModel->getKey() << std::endl;
    for (int i = 0; i < 2; i++) {
        auto response = currentModel->processPrompt(request);
        if (response.first != 200) {
            std::cout << "Retrying failed again for model: " << currentModel->getKey() << std::endl;
        } else {
            handleSuccessfulResponse(request, response.second);
            break;
        }
    }
    _modelManager.setNextModel();
    handlePromptRequest(request);
}

} // namespace network
