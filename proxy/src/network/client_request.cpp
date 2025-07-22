#include "client_request.h"
#include <optional>

namespace network {

void from_json(const json &j, HarpoonFile &harpoon) {
    j.at("file").get_to(harpoon.file);
    j.at("functions").get_to(harpoon.functions);
}

void from_json(const json &j, Content &content) {
    j.at("prompt").get_to(content.prompt);
    j.at("harpoon_files").get_to(content.harpoonFiles);
    j.at("git_diff").get_to(content.gitDiff);
    j.at("file_structure").get_to(content.fileStructure);
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
