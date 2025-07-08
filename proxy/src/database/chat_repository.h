#pragma once

#include "db.h"
#include "models/chat.h"
#include "models/project.h"
#include "project_repository.h"
#include <iostream>
#include <vector>

namespace database {

bool insertChat(network::ClientRequest request, std::string response);
std::vector<Chat> getChatHistory(Project project);

} // namespace database
