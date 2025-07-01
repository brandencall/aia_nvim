#include "BaseModel.h"
#include <iostream>

namespace models {

std::string BaseModel::processClientRequest(const network::ClientRequest &request) const {
    std::optional<database::Project> project = database::getProject(request);
    if (!project) {
        std::cout << "There is no project context" << std::endl;
        return request.content.prompt;
    }
    std::string projectContext = project->context;
    std::string prompt = processClientContent(request.content);
    std::cout << prompt << std::endl;
    return prompt;
}

std::string BaseModel::processClientContent(const network::Content &content) const {
    std::string result = "Below are relavent files with the function signatures:\n";
    for (const auto &file : content.harpoonFiles) {
        result += file.file + ": {\n";
        for (const auto &function : file.functions) {
            result += function + "\n";
        }
        result += "}\n";
    }
    result += "Below is the git diff:\n" + content.gitDiff;
    return result + content.prompt;
}

} // namespace models
