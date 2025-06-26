#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace network {

struct ClientRequest {
    std::string request_type;
    std::string project_id;
    std::string content;
};

void from_json(const json &j, ClientRequest &request);

} // namespace network
