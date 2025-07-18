#pragma once

#include "db.h"
#include "models/chat.h"
#include "models/project.h"
#include "project_repository.h"
#include <iostream>
#include <string>
#include <vector>

namespace database {

bool insertChat(const network::ClientRequest &request, const std::string &response);
std::vector<Chat> getChatHistory(const Project &project);
std::vector<std::string> getRecentConversations(const network::ClientRequest &request);

} // namespace database
