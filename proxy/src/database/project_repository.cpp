#include "project_repository.h"
#include "db.h"
#include "models/project.h"
#include <iostream>
#include <sqlite_modern_cpp/errors.h>
#include <string>

namespace database {

bool insertProject(const network::ClientRequest &request) {
    if (!getProject(request)) {
        try {
            getDB() << "INSERT INTO projects (project_id, context) VALUES (?, ?);" << request.project_id
                    << request.content.prompt;
            return true;
        } catch (const sqlite::sqlite_exception &e) {
            std::cerr << "SQLite error: " << e.what();
            return false;
        }
    }
    std::cout << "This project already exists." << std::endl;
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
