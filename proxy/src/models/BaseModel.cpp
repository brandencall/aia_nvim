#include "BaseModel.h"
#include <iostream>

namespace models {

std::string BaseModel::processClientRequest(const network::ClientRequest &request) const {
    std::optional<database::Project> project = database::getProject(request);
    if (!project) {
        std::cout << "There is no project context" << std::endl;
        return request.content->prompt;
    }
    std::string projectContext = project->context;
    projectContext +=
        " Any and all code change recommendations should be done in code and not in a git diff unless said otherwise\n";
    projectContext += "Summary of previous converstion: " + database::getLastSummary(project->id).summary + '\n';
    std::string prompt = projectContext + processClientContent(request.content);
    std::cout << prompt << std::endl;
    return prompt;
}

std::string BaseModel::processClientContent(const std::optional<network::Content> &content) const {
    if (content) {
        std::string result = processHarpoonContent(content);
        result += *content->gitDiff + "\n\n";
        return result + content->prompt;
    }
    return "";
}

std::string BaseModel::processHarpoonContent(const std::optional<network::Content> &content) const {
    std::string result;
    for (const auto &file : *content->harpoonFiles) {
        result += file.file + ": {\n";
        for (const auto &function : file.functions) {
            result += function + "\n";
        }
        result += "}\n";
    }
    return result + "\n\n";
}

} // namespace models
