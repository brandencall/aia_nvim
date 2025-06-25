#pragma once

#include <string>
#include <utility>

std::pair<long, std::string> sendRequest(const std::string &endpoint, const std::string &prompt);
