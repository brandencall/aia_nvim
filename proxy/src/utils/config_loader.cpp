#include "../models/model_factory.h"
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

using json = nlohmann::json;

std::vector<std::shared_ptr<models::BaseModel>> loadModelsFromConfig(const std::string &filePath) {
    std::ifstream jsonFile(filePath);
    if (!jsonFile.is_open()) {
        throw std::runtime_error("Failed to open the endpoint config");
    }
    json j;
    jsonFile >> j;
    std::vector<std::shared_ptr<models::BaseModel>> models;
    for (const auto &model : j) {
        models.push_back(createEndpointFromJson(model));
    }
    return models;
}
