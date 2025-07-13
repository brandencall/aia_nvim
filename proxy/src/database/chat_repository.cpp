#include "chat_repository.h"
#include "db.h"
#include "models/chat.h"
#include "models/project.h"
#include "project_repository.h"
#include <fstream>
#include <optional>
#include <string>
#include <utility>

namespace database {

bool insertChat(const network::ClientRequest &request, const std::string &response) {
    try {
        std::optional<Project> project = getProject(request);
        if (project == std::nullopt) {
            std::cout << "There is not project to insert chart history for." << std::endl;
            return false;
        }
        getDB() << "INSERT INTO chats (project_ref_id, prompt, response) VALUES (?, ?, ?);" << project->id
                << request.content.prompt << response;
        return true;
    } catch (const sqlite::sqlite_exception &e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
        return false;
    }
    return false;
}

std::vector<Chat> getChatHistory(const Project &project) {
    std::vector<Chat> result;
    getDB() << "SELECT id, project_ref_id, prompt, response, timestamp FROM chats WHERE project_ref_id = ?;"
            << project.id >>
        [&](int id, int project_ref_id, std::string prompt, std::string response, std::string timestamp) {
            result.push_back(Chat{id, project_ref_id, std::move(prompt), std::move(response), std::move(timestamp)});
        };
    return result;
}

std::vector<std::string> getRecentConversations(const Project &project) {
    std::vector<std::string> result;
    getDB() << "SELECT prompt || ' ' || response "
               "FROM chats "
               "WHERE project_ref_id = ( "
               "SELECT id FROM projects WHERE project_id = ? "
               ") "
               "ORDER BY timestamp DESC "
               "LIMIT 10; "
            << project.id >>
        [&](std::string combined) { result.push_back(combined); };
    return result;
}

} // namespace database
