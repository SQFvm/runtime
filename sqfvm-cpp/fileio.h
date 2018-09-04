#pragma once

#include <string>
#include <vector>

static int get_bom_skip(const std::vector<char>& buff);
static std::string load_file(const std::string& filename);
static std::vector<char> readFile(const std::string& filename);