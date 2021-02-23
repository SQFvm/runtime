#pragma once
#include "../runtime/logging.h"
#include "../runtime/runtime.h"

#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <utility>

namespace TCLAP
{
    class CmdLine;
}

class cli
{
    StdOutLogger m_logger;
    sqf::runtime::runtime m_runtime;
    bool m_cli_file;
    bool m_parse_only;
    bool m_good;
    bool m_automated;
    std::unordered_map<std::string, std::vector<std::function<std::pair<std::filesystem::path, std::string>()>>> m_files;

    void handle_files();
    void mount_filesystem(const std::vector<std::string>& mappings);
    int cli_from_file(const char* arg0, std::filesystem::path path);
public:
    cli();
    int run(size_t argc, const char** argv);

    bool verbose() const { return m_logger.isEnabled(loglevel::verbose); }
};