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
    db << createProjectsTable();
    db << createChatsTable();
    db << createChatTableTrigger();
}

std::string createProjectsTable() {
    return "CREATE TABLE IF NOT EXISTS projects ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "project_id TEXT, "
           "context TEXT, "
           "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";
}

std::string createChatsTable() {
    return "CREATE TABLE IF NOT EXISTS chats ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "project_ref_id INTEGER, "
           "prompt TEXT, "
           "response TEXT, "
           "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
           "FOREIGN KEY(project_ref_id) REFERENCES projects(id) ON DELETE CASCADE);";
}

std::string createChatTableTrigger() {
    return "CREATE TRIGGER IF NOT EXISTS trim_old_conversations "
           "AFTER INSERT ON chats "
           "BEGIN "
               "DELETE FROM chats "
                   "WHERE id IN ( "
                       "SELECT id FROM ( "
                           "SELECT id, "
                           "ROW_NUMBER() OVER ( "
                               "PARTITION BY project_ref_id "
                               "ORDER BY id DESC "
                           ") AS rn "
                       "FROM chats "
                    ") "
                    "WHERE rn > 30 "
               "); "
           "END;";
}

} // namespace database
