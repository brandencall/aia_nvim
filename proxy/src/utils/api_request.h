#pragma once

#include <string>
#include <utility>

namespace utils {

std::pair<long, std::string> sendRequest(const std::string &endpoint, const std::string &prompt);

}
