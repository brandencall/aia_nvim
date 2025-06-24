
#pragma once
#include "../models/BaseModel.h"
#include <memory>
#include <string>
#include <vector>

std::vector<std::shared_ptr<models::BaseModel>> loadModelsFromConfig(const std::string &filePath);
