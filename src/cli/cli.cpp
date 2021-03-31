#include "cli.hpp"
#include "main.h"
#include "interactive_helper.h"
#include "../runtime/git_sha1.h"
#include "../operators/ops.h"
#include "../operators/object.h"


// Runtime core-components
#include "../parser/config/config_parser.hpp"
#include "../parser/preprocessor/default.h"
#include "../fileio/default.h"
#include "../parser/sqf/sqf_parser.hpp"
#if defined(SQF_SQC_SUPPORT)
#include "../sqc/sqc_parser.h"
#endif

#include <tclap/CmdLine.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <cstring>
#else
#include <limits.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <execinfo.h>
#endif

#ifdef _WIN32
#define RELPATHHINT "Supports absolute and relative pathing using '.\\path\\to\\file' or 'C:\\path\\to\\file'."
#else
#define RELPATHHINT "Supports absolute and relative pathing using './path/to/file' or '/path/to/file'."
#endif
#define CMDADD(TYPE, NAME, ...) TYPE NAME(__VA_ARGS__); cmd.add(NAME)


void cli::handle_files()
{
    for (auto& [key, generators] : m_files)
    {
        for (auto& generator : generators)
        {
            auto [path, contents] = generator();
            if (key == "sqf")
            {
                if (verbose()) { std::cout << "Preprocessing file '" << path << std::endl; }
                auto ppedStr = m_runtime.parser_preprocessor().preprocess(m_runtime, contents, { path.string(), {} });
                if (ppedStr.has_value())
                {
                    if (verbose()) { std::cout << "Parsing file '" << path << std::endl; }
                    if (m_parse_only)
                    {
                        auto success = m_runtime.parser_sqf().check_syntax(m_runtime, *ppedStr, { path.string(), {} });
                        m_good = m_good && !success;
                    }
                    else
                    {
                        auto set = m_runtime.parser_sqf().parse(m_runtime, *ppedStr, { path.string(), {} });
                        if (set.has_value())
                        {
                            auto context = m_runtime.context_create().lock();
                            sqf::runtime::frame f(m_runtime.default_value_scope(), *set);
                            context->push_frame(f);
                            context->name(path.string());
                            if (verbose()) { std::cout << "Created Context '" << path << "'" << std::endl; }
                        }
                        else
                        {
                            m_good = false;
                            std::cout << "Failed to parse file '" << path << "'" << std::endl;
                        }
                    }
                }
                else
                {
                    m_good = false;
                    std::cout << "Failed to preprocess file '" << path << "'" << std::endl;
                }
            }
            else if (key == "config")
            {
                if (verbose()) { std::cout << "Preprocessing file '" << path << std::endl; }
                auto ppedStr = m_runtime.parser_preprocessor().preprocess(m_runtime, contents, { path.string(), {} });
                if (ppedStr.has_value())
                {
                    if (verbose()) { std::cout << "Parsing file '" << path << std::endl; }
                    if (m_parse_only)
                    {
                        auto success = !m_runtime.parser_config().check_syntax(*ppedStr, { path.string(), {} });
                        m_good = m_good && !success;
                    }
                    else
                    {
                        auto success = m_runtime.parser_config().parse(m_runtime.confighost(), *ppedStr, { path.string(), {} });
                        if (!success)
                        {
                            m_good = false;
                            std::cout << "Failed to parse file '" << path << "'" << std::endl;
                        }
                    }
                }
                else
                {
                    m_good = false;
                    std::cout << "Failed to preprocess file '" << path << "'" << std::endl;
                }
            }
#if defined(SQF_SQC_SUPPORT)
            else if (key == "sqc")
            {
                sqf::sqc::parser sqc_parser(m_logger);
                if (verbose()) { std::cout << "Preprocessing file '" << path << std::endl; }
                auto ppedStr = m_runtime.parser_preprocessor().preprocess(m_runtime, contents, { path.string(), {} });
                if (ppedStr.has_value())
                {
                    if (verbose()) { std::cout << "Parsing file '" << path << std::endl; }
                    if (m_parse_only)
                    {
                        auto success = sqc_parser.check_syntax(m_runtime, *ppedStr, { path.string(), {} });
                        m_good = m_good && !success;
                    }
                    else
                    {
                        auto set = sqc_parser.parse(m_runtime, *ppedStr, { path.string(), {} });
                        if (set.has_value())
                        {
                            auto context = m_runtime.context_create().lock();
                            sqf::runtime::frame f(m_runtime.default_value_scope(), *set);
                            context->push_frame(f);
                            context->name(path.string());
                            if (verbose()) { std::cout << "Created Context '" << path << "'" << std::endl; }
                        }
                        else
                        {
                            m_good = false;
                            std::cout << "Failed to parse file '" << path << "'" << std::endl;
                        }
                    }
                }
                else
                {
                    m_good = false;
                    std::cout << "Failed to preprocess file '" << path << "'" << std::endl;
                }
            }
            else if (key == "sqf2sqc")
            {
                if (verbose()) { std::cout << "Preprocessing file '" << path << std::endl; }
                auto ppedStr = m_runtime.parser_preprocessor().preprocess(m_runtime, contents, { path.string(), {} });
                if (ppedStr.has_value())
                {
                    if (verbose()) { std::cout << "Parsing file '" << path << std::endl; }
                    if (m_parse_only)
                    {
                        auto success = m_runtime.parser_sqf().check_syntax(m_runtime, *ppedStr, { path.string(), {} });
                        m_good = m_good && !success;
                    }
                    else
                    {
                        auto set = m_runtime.parser_sqf().parse(m_runtime, *ppedStr, { path.string(), {} });
                        if (set.has_value())
                        {
                            auto path_sqc = path.replace_extension(".sqc");
                            std::ofstream output_file(path_sqc, std::ios::out | std::ios::trunc);
                            if (!output_file.is_open() || !output_file.good())
                            {
                                std::cout << "Failed to open output file '" << path_sqc << "'" << std::endl;
                            }
                            else
                            {
                                auto transpiled = ::sqf::sqc::parser::to_sqc(set.value());
                                output_file << transpiled;

                                if (verbose()) { std::cout << "Written out transpiled '" << path << "' to '" << path_sqc << "'." << std::endl; }
                            }
                        }
                        else
                        {
                            m_good = false;
                            std::cout << "Failed to parse file '" << path << "'" << std::endl;
                        }
                    }
                }
                else
                {
                    m_good = false;
                    std::cout << "Failed to preprocess file '" << path << "'" << std::endl;
                }
            }
            else if (key == "sqc2sqf")
            {
                sqf::sqc::parser sqc_parser(m_logger);
                if (verbose()) { std::cout << "Preprocessing file '" << path << std::endl; }
                auto ppedStr = m_runtime.parser_preprocessor().preprocess(m_runtime, contents, { path.string(), {} });
                if (ppedStr.has_value())
                {
                    if (verbose()) { std::cout << "Parsing file '" << path << std::endl; }
                    if (m_parse_only)
                    {
                        auto success = sqc_parser.check_syntax(m_runtime, *ppedStr, { path.string(), {} });
                        m_good = m_good && !success;
                    }
                    else
                    {
                        auto set = sqc_parser.parse(m_runtime, *ppedStr, { path.string(), {} });
                        if (set.has_value())
                        {
                            auto path_sqc = path.replace_extension(".sqf");
                            std::ofstream output_file(path_sqc, std::ios::out | std::ios::trunc);
                            if (!output_file.is_open() || !output_file.good())
                            {
                                std::cout << "Failed to open output file '" << path_sqc << "'" << std::endl;
                            }
                            else
                            {
                                auto transpiled = sqf::types::d_code(set.value()).to_string_sqf();
                                if (transpiled.length() > 2)
                                {
                                    output_file << std::string_view(transpiled.data() + 1, transpiled.length() - 2);
                                }

                                if (verbose()) { std::cout << "Written out transpiled '" << path << "' to '" << path_sqc << "'." << std::endl; }
                            }
                        }
                        else
                        {
                            m_good = false;
                            std::cout << "Failed to parse file '" << path << "'" << std::endl;
                        }
                    }
                }
                else
                {
                    m_good = false;
                    std::cout << "Failed to preprocess file '" << path << "'" << std::endl;
                }
            }
#endif
        }
    }
}

void cli::mount_filesystem(const std::vector<std::string>& mappings)
{
    m_runtime.fileio(std::make_unique<sqf::fileio::impl_default>(m_logger));
    for (auto& mapping : mappings)
    {
        auto split_index = mapping.find('|');
        if (split_index == std::string::npos)
        {
            m_good = false;
            std::cerr << "Failed find splitter '|' for mapping '" << mapping << "'." << std::endl;
            continue;
        }
        std::filesystem::path phys = mapping.substr(0, split_index);
        std::filesystem::path virt = mapping.substr(split_index + 1);
        if (phys.empty() || virt.empty())
        {
            continue;
        }
        m_runtime.fileio().add_mapping(phys.string(), virt.string());
        if (verbose())
        {
            std::cout << "Mapped '" << virt << "' onto '" << phys << "'." << std::endl;
        }
    }
}

int cli::cli_from_file(const char* arg0, std::filesystem::path path)
{
    if (m_cli_file)
    {
        std::cout << "'" << path << "' is not allowed inside of the actual file!." << std::endl;
        return -1;
    }
    m_cli_file = true;
    if (path.empty())
    {
        std::cout << "'" << path << "' is no valid file path after sanitize." << std::endl;
        return -1;
    }
    path = std::filesystem::absolute(path.lexically_normal());

    try
    {
        auto file = sqf::runtime::fileio::read_file_from_disk(path.string());
        if (!file.has_value())
        {
            std::cout << "Failed to load file '" << path << "'" << std::endl;
            return -1;
        }
        auto str = *file;
        std::vector<const char*> args;
        args.push_back(arg0);
        size_t index = 0, last_index = 0;

        bool dobreak = false;
        while (!dobreak)
        {
            if ((index = str.find('\n', last_index)) == std::string::npos)
            {
                index = str.length();
                dobreak = true;
            }
            auto line = str.substr(last_index, index - last_index);
            line.erase(std::find_if(line.rbegin(), line.rend(), [](char c) -> bool {
                return c != '\r';
            }).base(), line.end());
            last_index = index + 1;
            if ((index = line.find(' ')) != std::string::npos)
            {
                auto arg = line.substr(index + 1);
                line = line.substr(0, index);
                args.push_back(copy_str(arg));
                args.push_back(copy_str(line));
            }
            else
            {
                args.push_back(copy_str(line));
            }
        }

        auto result = run(static_cast<int>(args.size()), args.data());

        if (args.size() > 1)
        {
            for (auto it = args.begin() + 1; it != args.end(); it++)
            {
                delete[] * it;
            }
        }
        return result;
    }
    catch (std::runtime_error& err)
    {
        std::cout << err.what() << std::endl;
        return -1;
    }
}

cli::cli() : m_logger(), m_runtime(m_logger, {}), m_cli_file(false), m_parse_only(false), m_good(false), m_automated(false)
{
}

int cli::run(size_t argc, const char** argv)
{
    std::string executable_path;
    {
#if defined(_WIN32) || defined(_WIN64)
        char buffer[MAX_PATH];
        _getcwd(buffer, MAX_PATH);
        executable_path = buffer;
#elif defined(__GNUC__)
        char buffer[PATH_MAX];
        if (getcwd(buffer, PATH_MAX))
        {
            executable_path = buffer;
        }
#else
#error "NO IMPLEMENTATION AVAILABLE"
#endif
}

    m_good = true;
#ifdef DF__CLI_PRINT_INPUT_ARGS
    std::cout << "\x1B[95m[CLI-INARG-PRINT]\033[0m" << "Got arguments:" << std::endl;
    for (int i = 0; i < argc; i++)
    {
        std::cout << "\x1B[95m[CLI-INARG-PRINT]\033[0m" << "    " << std::setw(3) << i << ": `" << argv[i] << "`" << std::endl;
    }
#endif // DF__CLI_PRINT_INPUT_ARGS
    TCLAP::CmdLine cmd("Emulates the ArmA-Series SQF environment.", ' ', std::string{ VERSION_FULL } + " (" + g_GIT_SHA1 + ")\n");

    // Logswitches
    CMDADD(TCLAP::SwitchArg, verboseArg, "V", "verbose", "Enables additional output.", false);
    CMDADD(TCLAP::SwitchArg, traceArg, "T", "trace", "Enables trace output.", false);

    // CLI Configuration
    CMDADD(TCLAP::ValueArg<std::string>,    cliFileArg,                 "",     "cli-file",                 "Allows to provide a file from which to load arguments from. If passed, all other arguments will be ignored! Each argument needs to be separated by line-feed. " RELPATHHINT, false, "", "PATH");
    CMDADD(TCLAP::SwitchArg,                interactiveArg,             "",     "interactive",              "Starts into the interactive mode. Interactive mode will run the VM in a separate thread, allowing you to control the behavior via basic commands.", false);
    CMDADD(TCLAP::SwitchArg,                automatedArg,               "a",    "automated",                "Disables CLI prompts.", false);
    CMDADD(TCLAP::SwitchArg,                suppressWelcomeArg,         "",     "suppress-welcome",         "Suppresses the welcome message during execution.", false);
    CMDADD(TCLAP::SwitchArg,                parseOnlyArg,               "",     "parse-only",               "Disables code execution and performs only parsing.", false);
    CMDADD(TCLAP::SwitchArg,                noExecutePrintArg,          "",     "no-execute-print",         "Disables the `Executing...` and two horizontal lines hint printing.", false);
    CMDADD(TCLAP::SwitchArg,                noSpawnPlayerArg,           "",     "no-spawn-player",          "Prevents automatic player creation.", false);
    CMDADD(TCLAP::SwitchArg,                noLoadExecDirArg,           "",     "no-load-executable-dir",   "Does not adds the executable path to the virtual file system.", false);

    // Runtime configuration
    CMDADD(TCLAP::ValueArg<long>,           maxRuntimeArg,              "m",    "max-runtime",              "Sets the maximum allowed runtime for the VM. 0 means no restriction in place.", false, 0, "MILLISECONDS");
    CMDADD(TCLAP::SwitchArg,                enableClassnameCheckArg,    "c",    "check-classnames",         "Enables the config checking for eg. createVehicle.", false);
    CMDADD(TCLAP::SwitchArg,                noOperatorsArg,             "",     "no-operators",             "If provided, SQF-VM will not be loaded using the default set of operators it comes with (except for SQF-VM specific operators).", false);
    CMDADD(TCLAP::SwitchArg,                noWorkPrintArg,             "",     "no-work-print",            "Prevents the results printing of contexts that reached an empty state.", false);
    CMDADD(TCLAP::MultiArg<std::string>,    virtualArg,                 "v",    "virtual",                  "Creates a mapping for a virtual and a physical path. Mapping is separated by a '|', with the left side being the physical, and the right argument the virtual path. " RELPATHHINT, false, "PATH|VIRTUAL");

    // Input - File
    CMDADD(TCLAP::MultiArg<std::string>,    inputArg,                   "i",    "input",                    "Loads provided file from disk. File-Type is determined using default file extensions (sqf, cpp, hpp, pbo). " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");
    CMDADD(TCLAP::MultiArg<std::string>,    inputSqfArg,                "",     "input-sqf",                "Loads provided SQF file from disk. Will be executed as if it was spawned. " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");
    CMDADD(TCLAP::MultiArg<std::string>,    inputConfigArg,             "",     "input-config",             "Loads provided config file from disk. Will be parsed before files, added using '--input'. " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");
    CMDADD(TCLAP::MultiArg<std::string>,    inputPboArg,                "",     "input-pbo",                "Loads provided PBO file from disk. Will be parsed before files, added using '--input'. " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");

    // Input - Raw
    CMDADD(TCLAP::MultiArg<std::string>,    sqfArg,                     "",     "sqf",                      "Loads provided sqf-code directly into the VM. Input is getting preprocessed! Will be executed as if it was spawned.", false, "CODE");
    CMDADD(TCLAP::MultiArg<std::string>,    configArg,                  "",     "config",                   "Loads provided config-code directly into the VM. Input is getting preprocessed!", false, "CODE");

    // Preprocessing
    CMDADD(TCLAP::MultiArg<std::string>,    preprocessFileArg,          "E",    "preprocess-file",          "Runs the preprocessor on provided file and prints it to stdout. " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");
    CMDADD(TCLAP::MultiArg<std::string>,    defineArg,                  "D",    "define",                   "Allows to add PreProcessor definitions. Note that file-based definitions may override and/or conflict with theese.", false, "NAME|NAME=VALUE");

    // Dummy Operators
    CMDADD(TCLAP::MultiArg<std::string>,    commandDummyNular,          "",     "command-dummy-nular",      "Adds the provided command as dummy.", false, "NAME");
    CMDADD(TCLAP::MultiArg<std::string>,    commandDummyUnary,          "",     "command-dummy-unary",      "Adds the provided command as dummy.", false, "NAME");
    CMDADD(TCLAP::MultiArg<std::string>,    commandDummyBinary,         "",     "command-dummy-binary",     "Adds the provided command as dummy. Note that you need to also provide a precedence. Example: 4|commandname", false, "PRECEDENCE|NAME");

#if defined(SQF_SQC_SUPPORT)
    CMDADD(TCLAP::MultiArg<std::string>,    sqcToSqfArg,                "",     "sqc-to-sqf",               "Transforms the provided SQC files to valid SQF syntax. New file will be placed next to existing .sqc file, with .sqf extension. " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");
    CMDADD(TCLAP::MultiArg<std::string>,    sqfToSqcArg,                "",     "sqf-to-sqc",               "Transforms the provided SQF files to valid SQC syntax. SQC created may not be as clean as hand written options. New file will be placed next to existing .sqf file, with .sqc extension. " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");
    CMDADD(TCLAP::MultiArg<std::string>,    inputSqcArg,                "",     "input-sqc",                "Loads provided SQC file from disk. Will be executed as if it was spawned. " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");
    CMDADD(TCLAP::MultiArg<std::string>,    sqcArg,                     "",     "sqc",                      "Loads provided sqc-code directly into the VM. Input is getting preprocessed! Will be executed as if it was spawned.", false, "CODE");
    CMDADD(TCLAP::SwitchArg,                parseSqcArg,                "",     "parse-sqc",                "Replaces the default (SQF) parser with the SQC one.", false);
#endif

    cmd.getArgList().reverse();
    cmd.parse(argc, argv);

    /* Logswitches */
    {
        m_logger.setEnabled(loglevel::verbose, verboseArg.getValue());
        m_logger.setEnabled(loglevel::trace, traceArg.getValue());
    }



    if (!cliFileArg.getValue().empty() && !m_cli_file)
    {
        m_cli_file = true;
        return cli_from_file(argv[0], std::filesystem::path(cliFileArg.getValue()));
    }
    m_automated = automatedArg.getValue();

    /* Basic setup */ {
        m_runtime.parser_config(std::make_unique<sqf::parser::config::parser>(m_logger));
        m_runtime.parser_preprocessor(std::make_unique<sqf::parser::preprocessor::impl_default>(m_logger));
#if defined(SQF_SQC_SUPPORT)
        if (parseSqcArg.getValue())
        {
            m_runtime.parser_sqf(std::make_unique<sqf::sqc::parser>(m_logger));
        }
        else
        {
            m_runtime.parser_sqf(std::make_unique<sqf::parser::sqf::parser>(m_logger));
        }
#else
        m_runtime.parser_sqf(std::make_unique<sqf::parser::sqf::parser>(m_logger));
#endif
        m_parse_only = parseOnlyArg.getValue();
    }

    /* Runtime configuration */ {
        m_runtime.configuration().max_runtime = std::chrono::milliseconds(maxRuntimeArg.getValue());
        m_runtime.configuration().enable_classname_check = enableClassnameCheckArg.getValue();
        if (!noOperatorsArg.getValue()) { sqf::operators::ops(m_runtime); }
        m_runtime.configuration().print_context_work_to_log_on_exit = !noWorkPrintArg.getValue();
        mount_filesystem(virtualArg.getValue());
        if (!noLoadExecDirArg.getValue())
        {
            m_runtime.fileio().add_mapping(executable_path, "/");
        }
    }

    /* Input */ {
        std::vector<std::string> sqf_files = inputSqfArg.getValue();
        std::vector<std::string> config_files = inputConfigArg.getValue();
        std::vector<std::string> pbo_files = inputPboArg.getValue();
#if defined(SQF_SQC_SUPPORT)
        std::vector<std::string> sqc_files = inputSqcArg.getValue();
#endif
        bool errflag = false;
        bool automated = automatedArg.getValue();
        // bool noAssemblyCreation = noAssemblyCreationArg.getValue();
        bool parseOnly = parseOnlyArg.getValue(); // || noAssemblyCreation;
        for (auto& f : inputArg.getValue())
        {
            std::filesystem::path path(f);
            if (path.extension() == ".sqf")
            {
                sqf_files.push_back(f);
            }
#if defined(SQF_SQC_SUPPORT)

            else if (path.extension() == ".sqc")
            {
                sqc_files.push_back(f);
            }
#endif
            else if (path.extension() == ".cpp" || path.extension() == ".hpp" || path.extension() == ".ext")
            {
                config_files.push_back(f);
            }
            else if (path.extension() == ".pbo")
            {
                pbo_files.push_back(f);
            }
            else
            {
                errflag = true;
                std::cerr << "The file extension '" << path.extension() << "' is not mapped for auto-detection. Consider using '--input-sqf' for example, to explicitly add your file." << std::endl;
            }
        }

        for (auto rit = sqf_files.rbegin(); rit != sqf_files.rend(); ++rit)
        {
            std::filesystem::path path(*rit);
            path = path.lexically_normal();
            m_files["sqf"].push_back([path]() -> std::pair<std::filesystem::path, std::string> {
                auto res = sqf::fileio::disabled::read_file_from_disk(path.string());
                if (res.has_value())
                {
                    return std::make_pair(path, res.value());
                }
                else
                {
                    throw std::runtime_error("Failed to load file '" + path.string() + "'.");
                }
            });
        }
        for (auto rit = config_files.rbegin(); rit != config_files.rend(); ++rit)
        {
            std::filesystem::path path(*rit);
            path = path.lexically_normal();
            m_files["config"].push_back([path]() -> std::pair<std::filesystem::path, std::string> {
                auto res = sqf::fileio::disabled::read_file_from_disk(path.string());
                if (res.has_value())
                {
                    return std::make_pair(path, res.value());
                }
                else
                {
                    throw std::runtime_error("Failed to load file '" + path.string() + "'.");
                }
            });
        }
        for (auto rit = pbo_files.rbegin(); rit != pbo_files.rend(); ++rit)
        {
            std::filesystem::path path(*rit);
            path = path.lexically_normal();
            rvutils::pbo::pbofile pbo(path);
            if (!pbo.good())
            {
                std::cout << "Failed to parse PBO '" << path << "'.";
                continue;
            }
            static_cast<sqf::fileio::impl_default&>(m_runtime.fileio()).add_pbo_mapping(pbo);
            rvutils::pbo::pbofile::reader reader;
            if (pbo.read("config.cpp", reader))
            {
                if (reader.descriptor().size > 0)
                {
                    std::string conf;
                    conf.reserve(reader.descriptor().size);
                    reader.read(conf.data(), conf.capacity());

                    m_files["config"].push_back([path, conf]() -> std::pair<std::filesystem::path, std::string> { return std::make_pair(path, conf); });
                }
            }
        }

        // Load all sqf-code provided via arg.
        for (auto raw = sqfArg.getValue().rbegin(); raw != sqfArg.getValue().rend(); raw++)
        {
            auto contents = *raw;
            m_files["sqf"].push_back([contents]() -> std::pair<std::filesystem::path, std::string> {
                return std::make_pair(std::filesystem::path("__commandline"), contents);
            });
        }

        // Load & merge all config-code provided via arg.
        for (auto& raw : configArg.getValue())
        {
            m_files["config"].push_back([raw]() -> std::pair<std::filesystem::path, std::string> {
                return std::make_pair(std::filesystem::path("__commandline"), raw);
            });
        }

#if defined(SQF_SQC_SUPPORT)
        for (auto rit = sqc_files.rbegin(); rit != sqc_files.rend(); ++rit)
        {
            std::filesystem::path path(*rit);
            path = path.lexically_normal();
            m_files["sqc"].push_back([path]() -> std::pair<std::filesystem::path, std::string> {
                auto res = sqf::fileio::disabled::read_file_from_disk(path.string());
                if (res.has_value())
                {
                    return std::make_pair(path, res.value());
                }
                else
                {
                    throw std::runtime_error("Failed to load file '" + path.string() + "'.");
                }
            });
        }
        for (auto& raw : sqcArg.getValue())
        {
            m_files["sqc"].push_back([raw]() -> std::pair<std::filesystem::path, std::string> {
                return std::make_pair(std::filesystem::path("__commandline"), raw);
            });
        }
        for (auto rit = sqfToSqcArg.getValue().rbegin(); rit != sqfToSqcArg.getValue().rend(); ++rit)
        {
            std::filesystem::path path(*rit);
            path = path.lexically_normal();
            m_files["sqf2sqc"].push_back([path]() -> std::pair<std::filesystem::path, std::string> {
                auto res = sqf::fileio::disabled::read_file_from_disk(path.string());
                if (res.has_value())
                {
                    return std::make_pair(path, res.value());
                }
                else
                {
                    throw std::runtime_error("Failed to load file '" + path.string() + "'.");
                }
            });
        }
        for (auto rit = sqcToSqfArg.getValue().rbegin(); rit != sqcToSqfArg.getValue().rend(); ++rit)
        {
            std::filesystem::path path(*rit);
            path = path.lexically_normal();
            m_files["sqc2sqf"].push_back([path]() -> std::pair<std::filesystem::path, std::string> {
                auto res = sqf::fileio::disabled::read_file_from_disk(path.string());
                if (res.has_value())
                {
                    return std::make_pair(path, res.value());
                }
                else
                {
                    throw std::runtime_error("Failed to load file '" + path.string() + "'.");
                }
            });
        }
#endif
    }

    /* Preprocessing */ {

        // Prepare Macros
        for (auto& d : defineArg.getValue())
        {
            auto equal_sign_index = d.find('=');
            if (equal_sign_index == std::string::npos)
            {
                m_runtime.parser_preprocessor().push_back({ d });
            }
            else
            {
                m_runtime.parser_preprocessor().push_back({ d.substr(0, equal_sign_index), d.substr(equal_sign_index + 1) });
            }
        }

        // Preprocess the files
        for (auto& f : preprocessFileArg.getValue())
        {
            auto path = std::filesystem::absolute(std::filesystem::path(f).lexically_normal());
            if (path.empty()) { continue; }
            try
            {
                if (verbose()) { std::cout << "Loading file '" << path << "' for preprocessing ..." << std::endl; }
                auto file = sqf::runtime::fileio::read_file_from_disk(path.string());
                if (!file.has_value())
                {
                    m_good = false;
                    std::cout << "Failed to load file '" << path << "'" << std::endl;
                    continue;
                }
                auto str = *file;
                if (verbose()) { std::cout << "Preprocessing file '" << path << std::endl; }
                auto ppedStr = m_runtime.parser_preprocessor().preprocess(m_runtime, str, { path.string(), {} });
                if (ppedStr.has_value())
                {
                    std::cout << *ppedStr << std::endl;
                }
                else
                {
                    m_good = false;
                    std::cout << "Failed to preprocess file '" << path << "'" << std::endl;
                }
            }
            catch (const std::runtime_error& ex)
            {
                m_good = false;
                std::cout << "Failed to load file " << ex.what() << std::endl;
            }
        }
    }

    /* Dummy Operators */ {
        for (auto& f : commandDummyNular.getValue())
        {
            m_runtime.register_sqfop(sqf::runtime::sqfop::nular(f, "DUMMY", [](sqf::runtime::runtime& runtime) -> sqf::runtime::value {
                runtime.__logmsg(logmessage::runtime::ErrorMessage(runtime.context_active().current_frame().diag_info_from_position(), "DUMMY", "DUMMY")); return {};
            }));
        }
        for (auto& f : commandDummyUnary.getValue())
        {
            m_runtime.register_sqfop(sqf::runtime::sqfop::unary(f, sqf::types::t_any(), "DUMMY", [](sqf::runtime::runtime& runtime, sqf::runtime::value::cref r) -> sqf::runtime::value {
                runtime.__logmsg(logmessage::runtime::ErrorMessage(runtime.context_active().current_frame().diag_info_from_position(), "DUMMY", "DUMMY")); return {};
            }));
        }
        for (auto& f : commandDummyBinary.getValue())
        {
            auto split_index = f.find('|');
            if (split_index == std::string::npos)
            {
                m_good = false;
                std::cerr << "Failed find splitter '|' for precedence '" << f << "'." << std::endl;
                continue;
            }
            auto precedence = f.substr(0, split_index);
            auto name = f.substr(split_index + 1);
            m_runtime.register_sqfop(sqf::runtime::sqfop::binary(static_cast<short>(std::stoi(precedence)), name, sqf::types::t_any(), sqf::types::t_any(), "DUMMY", [](sqf::runtime::runtime& runtime, sqf::runtime::value::cref l, sqf::runtime::value::cref r) -> sqf::runtime::value {
                runtime.__logmsg(logmessage::runtime::ErrorMessage(runtime.context_active().current_frame().diag_info_from_position(), "DUMMY", "DUMMY")); return {};
            }));
        }
    }

    handle_files();

    if (!m_good || m_parse_only)
    {
        if (verbose() && !m_good) { std::cout << "Exiting due to error." << std::endl; }
        if (!m_automated)
        {
            std::string line;
            std::cout << std::endl << "Press [ENTER] to continue...";
            std::getline(std::cin, line);
        }
        return m_good ? 0 : -1;
    }

    if (!noSpawnPlayerArg.getValue())
    {
        auto player_object = sqf::types::object::create(m_runtime, {}, false);;
        m_runtime.storage<sqf::types::object::object_storage>().player(player_object);
    }

    // END
    if (interactiveArg.getValue())
    {
        interactive_helper helper(m_runtime);
        helper.init();
        if (!suppressWelcomeArg.getValue())
        {
            std::cout << "You can disable this message using `--suppress-welcome`." << std::endl;
            helper.print_welcome();
        }
        helper.run();
    }
    else
    { // Default Mode
        do
        {
            if (!m_automated)
            {
                //Prompt user to type in code.
                int i = 1;
                if (!suppressWelcomeArg.getValue())
                {
                    printf("You can disable this message using `--suppress-welcome`.\n"
                        "Please enter your SQF code.\n"
                        "To get info about a command, use the `help__` operator.\n"
                        "For a list of all implemented commands, use the `cmds__` operator.\n"
                        "For a list of all SQF-VM internal commands, use the `cmdsvm__` operator.\n"
                        "To run the code, Press [ENTER] twice.\n"
                        "To exit, use the `exit__` command.\n"
                        "If you enjoy this tool, consider donating: https://paypal.me/X39\n");
                }
                std::string line;
                std::stringstream sstream;
                do
                {
                    printf("%d:\t", i++);
                    std::getline(std::cin, line);
                    sstream << line << std::endl;
                } while (!line.empty());

                std::cout << std::endl;


                auto input = sstream.str();

                std::string commandlinefeed = "__commandlinefeed.sqf";
                auto ppedStr = m_runtime.parser_preprocessor().preprocess(m_runtime, input, { commandlinefeed, {} });
                if (ppedStr.has_value())
                {
                    if (parseOnlyArg.getValue())
                    {
                        m_runtime.parser_sqf().check_syntax(m_runtime, *ppedStr, { commandlinefeed, {} });
                    }
                    else
                    {
                        auto set = m_runtime.parser_sqf().parse(m_runtime, *ppedStr, { commandlinefeed, {} });
                        if (set.has_value())
                        {
                            if (set->empty())
                            {
                                std::cout << "Compilation yielded no assembly." << std::endl;
                            }
                            else
                            {
                                sqf::runtime::frame f(m_runtime.default_value_scope(), *set);
                                m_runtime.context_active().push_frame(f);
                            }
                        }
                        else
                        {
                            std::cout << "Failed to parse commandline feed." << std::endl;
                        }
                    }
                }
                else
                {
                    std::cout << "Failed to preprocess file '" << commandlinefeed << "'" << std::endl;
                }
            }


            sqf::runtime::runtime::result result;
            if (!noExecutePrintArg.getValue())
            {
                std::cout << "Executing..." << std::endl;
                std::cout << std::string(console_width(), '-') << std::endl;
            }
            result = m_runtime.execute(sqf::runtime::runtime::action::start);
            if (!noExecutePrintArg.getValue())
            {
                std::cout << std::string(console_width(), '-') << std::endl;
            }
            if (result != sqf::runtime::runtime::result::ok)
            {
                m_runtime.execute(sqf::runtime::runtime::action::abort);
            }
            switch (result)
            {
                case sqf::runtime::runtime::result::invalid:
                if (verbose())
                {
                    std::cout << "Invalid result. Please raise a bug at github concerning this." << std::endl;
                }
                break;
                case sqf::runtime::runtime::result::empty:
                if (verbose())
                {
                    std::cout << "Ran to completion." << std::endl;
                }
                break;
                case sqf::runtime::runtime::result::ok:
                break;
                case sqf::runtime::runtime::result::action_error:
                std::cout << "Performing the action failed." << std::endl;
                break;
                case sqf::runtime::runtime::result::runtime_error:
                if (verbose())
                {
                    std::cout << "Runtime Error occured." << std::endl;
                }
                break;
                default:
                break;
            }
        } while (!m_automated && !m_runtime.is_exit_requested());
    }
    auto exitcode = m_runtime.exit_code();
    if (exitcode.has_value())
    {
        return exitcode.value();
    }
    return 0;
}
