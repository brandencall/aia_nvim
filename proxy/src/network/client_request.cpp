#include "client_request.h"

namespace network {

void from_json(const json &j, ClientRequest &request) {
    j.at("request_type").get_to(request.request_type);
    j.at("project_id").get_to(request.project_id);
    j.at("content").get_to(request.content);
}
} // namespace network
