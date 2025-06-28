
#include "db.h"
#include <sqlite_modern_cpp.h>

namespace database {

sqlite::database &getDB() {
    static sqlite::database db("data/aia.db");
    return db;
}

void initializeDB() {
    getDB() << "CREATE TABLE IF NOT EXISTS projects ("
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "project_id TEXT, "
          "context TEXT, "
          "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";
}

} // namespace database
