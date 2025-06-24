#pragma once
#include "BaseModel.h"
#include <memory>
#include <nlohmann/json.hpp>

std::shared_ptr<models::BaseModel> createEndpointFromJson(const nlohmann::json& j); 
