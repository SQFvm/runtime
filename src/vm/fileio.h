#pragma once

#include <string>
#include <vector>

int get_bom_skip(const std::vector<char>& buff);
std::string load_file(const std::string& filename);
std::vector<char> readFile(const std::string& filename);

bool file_exists(const std::string& filename);