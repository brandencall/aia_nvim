#include <iostream>
#include <nlohmann/json.hpp>
#include <sqlite_modern_cpp.h>

int main() {
  try {
    sqlite::database db("test.db");
    db << "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name "
          "TEXT);";
    db << "INSERT INTO users (name) VALUES (?);" << "Alice";

    nlohmann::json j;
    db << "SELECT name FROM users;" >>
        [&](std::string name) { j.push_back({{"name", name}}); };
    std::cout << j.dump(4) << "\n";
  } catch (sqlite::sqlite_exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
  }
  return 0;
}
