#include "summary_repository.h"

namespace database {

bool insertSummary(const network::ClientRequest &request, const std::string &summary) {
    try {
        std::optional<Project> project = getProject(request);
        if (project == std::nullopt) {
            std::cout << "There is not project to insert chart history for." << std::endl;
            return false;
        }
        getDB() << "INSERT INTO summaries (project_ref_id, summary) VALUES (?, ?);" << project->id << summary;
        return true;
    } catch (const sqlite::sqlite_exception &e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
        return false;
    }
    return false;
}

Summary getLastSummary(const Project &project) {
    Summary result;
    getDB() << "SELECT id, project_ref_id, summary, timestamp FROM summaries WHERE project_ref_id = ? ORDER BY "
               "timestamp DESC LIMIT 1;"
            << project.id >>
        [&](int id, int project_ref_id, std::string summary, std::string timestamp) {
            result = Summary{id, project_ref_id, std::move(summary), std::move(timestamp)};
        };
    return result;
}

} // namespace database
