#include "client_request.h"
#include <optional>

namespace network {

void from_json(const json &j, HarpoonFile &harpoon) {
    j.at("file").get_to(harpoon.file);
    j.at("functions").get_to(harpoon.functions);
}

void from_json(const json &j, Content &content) {
    j.at("prompt").get_to(content.prompt);
    if (j.contains("harpoon_files") && !j.at("harpoon_files").is_null()) {
        content.harpoonFiles = j.at("harpoon_files").get<std::vector<network::HarpoonFile>>();
    } else {
        content.harpoonFiles = std::nullopt;
    }
    if (j.contains("git_diff") && !j.at("git_diff").is_null()) {
        content.gitDiff = j.at("git_diff").get<std::string>();
    } else {
        content.gitDiff = std::nullopt;
    }
    if (j.contains("file_structure") && !j.at("file_structure").is_null()) {
        content.fileStructure = j.at("file_structure").get<std::string>();
    } else {
        content.fileStructure = std::nullopt;
    }
}

void from_json(const json &j, ClientRequest &request) {
    j.at("request_type").get_to(request.request_type);
    j.at("project_id").get_to(request.project_id);
    if (j.contains("content") && !j.at("content").is_null()) {
        request.content = j.at("content").get<network::Content>();
    } else {
        request.content = std::nullopt;
    }
}
} // namespace network
