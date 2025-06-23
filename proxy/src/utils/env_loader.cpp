#include <fstream>
#include <iostream>
#include <string>

void loadEnvFile(const std::string &filePath) {
    std::ifstream envFile(filePath);
    if (!envFile.is_open()){
        std::cout << "couldn't open up the file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(envFile, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        size_t delimPos = line.find('=');
        if (delimPos == std::string::npos)
            continue;

        std::string key = line.substr(0, delimPos);
        std::string value = line.substr(delimPos + 1);

        setenv(key.c_str(), value.c_str(), 1);
    }
}
