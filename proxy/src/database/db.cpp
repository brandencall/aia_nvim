
#include "db.h"
#include <sqlite_modern_cpp.h>

namespace database {

static sqlite::database db("data/aia.db");

sqlite::database &getDB() { return db; }

void initializeDB() {
    db << "CREATE TABLE IF NOT EXISTS projects ("
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "project_id TEXT, "
          "context TEXT, "
          "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";
}

} // namespace database
