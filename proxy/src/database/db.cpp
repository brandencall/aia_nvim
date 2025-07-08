
#include "db.h"
#include <sqlite_modern_cpp.h>

namespace database {

sqlite::database &getDB() {
    static sqlite::database db("data/aia.db");
    db << "PRAGMA foreign_keys = ON;";
    return db;
}

void initializeDB() {
    auto &db = getDB();
    db << "CREATE TABLE IF NOT EXISTS projects ("
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "project_id TEXT, "
          "context TEXT, "
          "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";

    db << "CREATE TABLE IF NOT EXISTS chats ("
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "project_ref_id INTEGER, "
          "prompt TEXT, "
          "response TEXT, "
          "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
          "FOREIGN KEY(project_ref_id) REFERENCES projects(id) ON DELETE CASCADE);";
}

} // namespace database
