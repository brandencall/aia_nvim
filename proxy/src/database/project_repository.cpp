#include "project_repository.h"
#include "db.h"
#include "models/project.h"
#include <iostream>
#include <optional>
#include <sqlite_modern_cpp.h>
#include <sqlite_modern_cpp/errors.h>
#include <string>

namespace database {

bool upsertProject(const network::ClientRequest &request) {
    std::optional<Project> project = getProject(request);
    sqlite::database db = getDB();
    if (project == std::nullopt) {
        try {
            db << "INSERT INTO projects (project_id, context) VALUES (?, ?);" << request.project_id
               << request.content->prompt;
            return true;
        } catch (const sqlite::sqlite_exception &e) {
            std::cerr << "SQLite error: " << e.what() << std::endl;
            return false;
        }
    } else if (request.content) {
        db << "UPDATE projects SET context = ? WHERE project_id = ? " << request.content->prompt << request.project_id;
        return true;
    }

    return false;
}

std::optional<Project> getProject(const network::ClientRequest &request) {
    std::optional<Project> result;
    getDB() << "SELECT id, project_id, context, timestamp FROM projects WHERE project_id = ?;" << request.project_id >>
        [&](int id, std::string project_id, std::string context, std::string timestamp) {
            result = Project{id, std::move(project_id), std::move(context), std::move(timestamp)};
        };
    return result;
}

} // namespace database
