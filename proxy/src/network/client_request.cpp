#include "client_request.h"

namespace network {

void from_json(const json &j, HarpoonFile &harpoon){
    j.at("file").get_to(harpoon.file);
    j.at("functions").get_to(harpoon.functions);
} 

void from_json(const json &j, Content &content){
    j.at("prompt").get_to(content.prompt);
    j.at("harpoon_files").get_to(content.harpoonFiles);
    j.at("git_diff").get_to(content.gitDiff);
}

void from_json(const json &j, ClientRequest &request) {
    j.at("request_type").get_to(request.request_type);
    j.at("project_id").get_to(request.project_id);
    j.at("content").get_to(request.content);
}
} // namespace network
