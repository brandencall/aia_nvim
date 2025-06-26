#pragma once

#include <sqlite_modern_cpp.h>

namespace database {

sqlite::database &getDB();
void initializeDB();

} // namespace database
