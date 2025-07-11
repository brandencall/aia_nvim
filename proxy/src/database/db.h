#pragma once

#include <sqlite_modern_cpp.h>

namespace database {

sqlite::database &getDB();
void initializeDB();
std::string createProjectsTable();
std::string createChatsTable();
std::string createChatTableTrigger();

} // namespace database
