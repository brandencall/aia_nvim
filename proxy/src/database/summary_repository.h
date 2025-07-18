#pragma once

#include "db.h"
#include "models/project.h"
#include "models/summary.h"
#include "project_repository.h"
#include <iostream>
#include <string>
#include <vector>

namespace database {

bool insertSummary(const network::ClientRequest &request, const std::string &summary);
Summary getLastSummary(int id);

} // namespace database
