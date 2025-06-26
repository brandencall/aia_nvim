#pragma once

#include "../network/client_request.h"
#include "models/project.h"
#include <optional>

namespace database {

void insertProject(const network::ClientRequest &request);
std::optional<Project> getProject(const network::ClientRequest &request);

} // namespace database
