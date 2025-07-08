#pragma once

#include <string>

namespace database {

struct Chat {
    int id;
    int projectRefId;
    std::string prompt;
    std::string response;
    std::string timestamp;
};

} // namespace database
