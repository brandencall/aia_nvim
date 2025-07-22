#pragma once

#include "../network/client_request.h"
#include "models/project.h"
#include <optional>

namespace database {

bool upsertProject(const network::ClientRequest &request);
std::optional<Project> getProject(const network::ClientRequest &request);

} // namespace database
