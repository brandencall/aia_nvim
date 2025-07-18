#pragma once

#include <string>

namespace database {

struct Summary {
    int id;
    int projectRefId;
    std::string summary;
    std::string timestamp;
};

} // namespace database
