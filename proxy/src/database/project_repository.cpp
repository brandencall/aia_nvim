#include "project_repository.h"
#include "db.h"
#include "models/project.h"
#include <iostream>
#include <string>

namespace database {

void insertProject(const network::ClientRequest &request) {
    if (!getProject(request)) {
        getDB() << "INSERT INTO projects (project_id, context) VALUES (?, ?);" << request.project_id << request.content;
    } else {
        std::cout << "this project already exists. Should probably notify the client" << std::endl;
    }
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
