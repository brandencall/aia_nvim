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
    std::string result = processHarpoonContent(content);
    result += content.gitDiff + "\n\n";
    result += content.fileStructure + "\n\n";
    return result + content.prompt;
}

std::string BaseModel::processHarpoonContent(const network::Content &content) const {
    std::string result;
    for (const auto &file : content.harpoonFiles) {
        result += file.file + ": {\n";
        for (const auto &function : file.functions) {
            result += function + "\n";
        }
        result += "}\n";
    }
    return result + "\n\n";
}

} // namespace models
