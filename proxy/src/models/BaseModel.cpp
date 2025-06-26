
#include "BaseModel.h"
#include <iostream>

namespace models {

std::string BaseModel::processClientRequest(const network::ClientRequest &request) const {
    std::optional<database::Project> project = database::getProject(request);
    if (!project) {
        std::cout << "There is no project context" << std::endl;
        return request.content;
    } 
    std::string context = project->context;
    std::string prompt = request.content;
    return context + "\n" + prompt;
}

} // namespace models
