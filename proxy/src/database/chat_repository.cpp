#include "chat_repository.h"
#include "models/chat.h"
#include "models/project.h"
#include "project_repository.h"
#include <optional>
#include <utility>

namespace database {

bool insertChat(network::ClientRequest request, std::string response) {
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
        std::cerr << "SQLite error: " << e.what();
        return false;
    }
    return false;
}
std::vector<Chat> getChatHistory(Project project) {
    std::vector<Chat> result;
    getDB() << "SELECT id, project_ref_id, prompt, response, timestamp FROM chats WHERE project_ref_id = ?;"
            << project.id >>
        [&](int id, int project_ref_id, std::string prompt, std::string response, std::string timestamp) {
            result.push_back(Chat{id, project_ref_id, std::move(prompt), std::move(response), std::move(timestamp)});
        };
    return result;
}

} // namespace database
