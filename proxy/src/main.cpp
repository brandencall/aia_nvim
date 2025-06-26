#include "ModelManager.h"
#include "models/BaseModel.h"
#include "network/tcp.h"
#include "utils/config_loader.h"
#include <filesystem>
#include <memory>
#include <thread>
#include <unistd.h>
#include <vector>

int main() {
    std::vector<std::shared_ptr<models::BaseModel>> models = loadModelsFromConfig("endpoints.json");
    ModelManager::init(models);
    std::filesystem::create_directories("data");

    const char *ip = "10.0.0.234";
    int port = 22222;
    int serverSocket = network::createServerSocket(ip, port);
    if (serverSocket == -1)
        return -1;
    while (true) {
        int clientSocket = network::acceptClient(serverSocket);
        if (clientSocket == -1)
            break;
        std::thread(network::clientSession, clientSocket).detach();
    }
    close(serverSocket);
    return 0;
}
