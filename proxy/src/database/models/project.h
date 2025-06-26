#pragma once

#include <string>

namespace database {

struct Project {
    int id;
    std::string project_id;
    std::string context;
    std::string timestamp;
};

} // namespace database
