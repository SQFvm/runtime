#include "main.h"
#include "../runtime/runtime.h"
#include "../runtime/git_sha1.h"

#include "../operators/object.h"
#include "../operators/ops_config.h"
#include "../operators/ops_diag.h"
#include "../operators/ops_generic.h"
#include "../operators/ops_group.h"
#include "../operators/ops_logic.h"
#include "../operators/ops_markers.h"
#include "../operators/ops_math.h"
#include "../operators/ops_namespace.h"
#include "../operators/ops_object.h"
#include "../operators/ops_sqfvm.h"
#include "../operators/ops_string.h"

#include "../parser/config/default.h"
#include "../parser/sqf/default.h"
#include "../parser/preprocessor/default.h"

#include "../fileio/default.h"

#include "../sqc/tokenizer.h"

#include "interactive_helper.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <tclap/CmdLine.h>
#include <algorithm>
#include <string_view>

#include <csignal>
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

void strcpy_safe(char* const dest, size_t len, const char* const src)
{
#ifdef WIN32
    strcpy_s(dest, len, src);
#else
    std::strcpy(dest, src);
#endif
}

char* const copy_str(const std::string& str)
{
    auto dest = new char[str.length() + 1];
    strcpy_safe(dest, str.length() + 1, str.c_str());
    return dest;
}

#ifdef WIN32
// ToDo: Implement StackTrace on error
#else
void handle_SIGSEGV(int val)
{
    void *arr[20];
    size_t size;
    size = backtrace(arr, 20);
    fprintf(stderr, "Error: signal %d:\n", val);
    backtrace_symbols_fd(arr, size, STDERR_FILENO);
    exit(-1);
}
#endif



int console_width()
{
#if _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        return 80;
    }
    columns = csbi.srWindow.Right - csbi.srWindow.Left;
    return columns;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
#endif
}


bool isInLoadFileCliMode = false;

std::string arg_file_actual_path(std::string executable_path, std::string f)
{
    if (f.empty())
    {
        return "";
    }
    if (f.length() > 2 && f[0] == '.' && (f[1] == '/' || f[1] == '\\'))
    {
        f = std::filesystem::absolute((std::filesystem::path(executable_path) / f).lexically_normal()).string();
    }
    return f;
}
std::string extension(std::string input)
{
    auto last_index = input.find_last_of('.');
    if (last_index == std::string::npos)
    {
        return "";
    }
    return input.substr(last_index + 1);
}

template<size_t TL> constexpr auto concat_size() { return TL; }
template<size_t TL, size_t... TVar> constexpr auto concat_size() { return concat_size<TVar...>() + TL; }
template<typename T, size_t TL, size_t TR>
constexpr auto concat_(std::array<T, TL> l, std::array<T, TR> r)
{
    std::array<T, TL + TR> arr;
    for (size_t i = 0; i < TL; i++)
    {
        arr[i] = l[i];
    }
    for (size_t i = TL; i < TL + TR; i++)
    {
        arr[i] = r[i - TL];
    }
    return arr;
}
template<typename T, size_t TL>
constexpr auto concat_(std::array<T, TL> l, T r)
{
    std::array<T, TL + 1> arr;
    for (size_t i = 0; i < TL; i++)
    {
        arr[i] = l[i];
    }
    arr[TR] = r;
    return arr;
}
template<typename T, size_t TR>
constexpr auto concat_(T l, std::array<T, TR> r)
{
    std::array<T, 1 + TR> arr;
    arr[0] = l;
    for (size_t i = 1; i < 1 + TR; i++)
    {
        arr[i] = r[i - 1];
    }
    return arr;
}
template<typename T>
constexpr auto concat(T t)
{
    return t;
}
template<typename T, typename... TVar>
constexpr auto concat(T t, TVar... args)
{
    auto arrb = concat<TVar...>(args...);
    auto res = concat_(t, arrb);
    return res;
}

int main(int argc, char** argv)
{
    {
        std::array<int, 3> arra = { 1, 2, 3 };
        std::array<int, 5> arrb = { 1, 2, 3, 4, 5 };
        std::array<int, 2> arrc = { 1, 2 };
        std::array<int, 7> arrd = { 1, 2, 3, 4, 5, 6, 7 };
        auto arr_res = concat(arra, arrb, arrc, arrd, 0, 0, 0, arra);


        auto arr_conc_a = std::array{ concat(arra, arrb), concat(arrc, arrd) };
        auto arr_conc_a = std::array{ concat(arrc, arrd), concat(arra, arrb) };

        for (auto it : arr_res)
        {
            std::cout << it << ", ";
        }
        std::cout << std::endl << std::endl << std::endl;
        std::string str = 
            "function test(a, b)" "\n"
            "{" "\n"
            "    let a be 12;" "\n"
            "    a = a + 12;" "\n"
            "}" "\n";
        sqf::sqc::tokenizer tokenizer(str.begin(), str.end());
        sqf::sqc::tokenizer::token t;
        while ((t = tokenizer.next()).type != sqf::sqc::tokenizer::etoken::eof)
        {
            if (t.type == sqf::sqc::tokenizer::etoken::i_whitespace) continue;
            std::cout << tokenizer.to_string(t.type) << " ";
        }
        std::cout << std::endl << std::endl << std::endl;
    }
#ifdef WIN32
    // ToDo: Implement StackTrace on error
#else
    struct sigaction action_SIGSEGV;

    memset(&action_SIGSEGV, 0, sizeof(struct sigaction));
    action_SIGSEGV.sa_handler = handle_SIGSEGV;
    sigaction(SIGSEGV, &action_SIGSEGV, NULL);
#endif

    std::string executable_path; 
    {
#if defined(_WIN32) || defined(_WIN64)
        char buffer[MAX_PATH];
        _getcwd(buffer, MAX_PATH);
        executable_path = buffer;
#elif defined(__GNUC__)
        char buffer[PATH_MAX];
        getcwd(buffer, PATH_MAX);
        executable_path = buffer;
#else
#error "NO IMPLEMENTATION AVAILABLE"
#endif
    }
    TCLAP::CmdLine cmd("Emulates the ArmA-Series SQF environment.", ' ', std::string{VERSION_FULL} + " (" + g_GIT_SHA1 + ")\n");

    TCLAP::ValueArg<std::string> cliFileArg("", "cli-file", "Allows to provide a file from which to load arguments from. If passed, all other arguments will be ignored! Each argument needs to be separated by line-feed. " RELPATHHINT, false, "", "PATH");
    cmd.add(cliFileArg);

    TCLAP::ValueArg<long> maxRuntimeArg("m", "max-runtime", "Sets the maximum allowed runtime for the VM. 0 means no restriction in place.", false, 0, "MILLISECONDS");
    cmd.add(maxRuntimeArg);

    TCLAP::MultiArg<std::string> inputArg("i", "input", "Loads provided file from disk. File-Type is determined using default file extensions (sqf, cpp, hpp, pbo). " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");
    cmd.add(inputArg);

    TCLAP::MultiArg<std::string> inputSqfArg("", "input-sqf", "Loads provided SQF file from disk. Will be executed as if it was spawned. Executed from left to right. " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");
    cmd.add(inputSqfArg);

    TCLAP::MultiArg<std::string> inputConfigArg("", "input-config", "Loads provided config file from disk. Will be parsed before files, added using '--input'. " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");
    cmd.add(inputConfigArg);

    TCLAP::MultiArg<std::string> inputPboArg("", "input-pbo", "Loads provided PBO file from disk. Will be parsed before files, added using '--input'. " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");
    cmd.add(inputPboArg);

    TCLAP::MultiArg<std::string> sqfArg("", "sqf", "Loads provided sqf-code directly into the VM. Input is not getting preprocessed! Will be executed as if it was spawned.", false, "CODE");
    cmd.add(sqfArg);

    TCLAP::MultiArg<std::string> configArg("", "config", "Loads provided config-code directly into the VM. Input is not getting preprocessed!", false, "CODE");
    cmd.add(configArg);

    TCLAP::MultiArg<std::string> preprocessFileArg("E", "preprocess-file", "Runs the preprocessor on provided file and prints it to stdout. " RELPATHHINT "!BE AWARE! This is case-sensitive!", false, "PATH");
    cmd.add(preprocessFileArg);

    TCLAP::MultiArg<std::string> defineArg("D", "define", "Allows to add PreProcessor definitions. Note that file-based definitions may override and/or conflict with theese.", false, "NAME|NAME=VALUE");
    cmd.add(defineArg);


    TCLAP::MultiArg<std::string> commandDummyNular("", "command-dummy-nular", "Adds the provided command as dummy.", false, "NAME");
    cmd.add(commandDummyNular);

    TCLAP::MultiArg<std::string> commandDummyUnary("", "command-dummy-unary", "Adds the provided command as dummy.", false, "NAME");
    cmd.add(commandDummyUnary);

    TCLAP::MultiArg<std::string> commandDummyBinary("", "command-dummy-binary", "Adds the provided command as dummy. Note that you need to also provide a precedence. Example: 4|commandname", false, "PRECEDENCE|NAME");
    cmd.add(commandDummyBinary);

    TCLAP::SwitchArg automatedArg("a", "automated", "Disables all possible prompts.", false);
    cmd.add(automatedArg);

    // TCLAP::ValueArg<int> serverArg("s", "server", "Causes the SQF-VM to start a network server that allows other SQF-VM instances to connecto to it via remoteConnect__.", false, 0, "PORT");
    // cmd.add(serverArg);

    TCLAP::SwitchArg enableClassnameCheckArg("c", "check-classnames", "Enables the config checking for eg. createVehicle.", false);
    cmd.add(enableClassnameCheckArg);

    TCLAP::SwitchArg interactiveArg("", "interactive", "Starts into the interactive mode. Interactive mode will run the VM in a separate thread, allowing you "
        "to control the behavior via basic commands.", false);
    cmd.add(interactiveArg);

    TCLAP::SwitchArg suppressWelcomeArg("", "suppress-welcome", "Suppresses the welcome message during execution.", false);
    cmd.add(suppressWelcomeArg);



    // TCLAP::MultiArg<std::string> loadArg("l", "load", "Adds provided path to the allowed locations list. " RELPATHHINT "\n"
    //     "An allowed location, is a location SQF-VM will be allowed to load files from."
    //     "If you try to load a file from a given directory that is not in the allowed list,"
    //     "the file loading WILL fail."
    //     "Only the root path of a given folder needs to be added, sub-folders are accessible automatically."
    //     "!BE AWARE! This is case-sensitive!", false, "PATH");
    // cmd.add(loadArg);

    TCLAP::MultiArg<std::string> virtualArg("v", "virtual", "Creates a mapping for a virtual and a physical path."
        "Mapping is separated by a '|', with the left side being the physical, and the right argument the virtual path. " RELPATHHINT, false, "PATH|VIRTUAL");
    cmd.add(virtualArg);

    TCLAP::SwitchArg verboseArg("", "verbose", "Enables additional output.", false);
    cmd.add(verboseArg);

    TCLAP::SwitchArg parseOnlyArg("", "parse-only", "Disables code execution and performs only parsing.", false);
    cmd.add(parseOnlyArg);

    // TCLAP::SwitchArg lintPrivateVarExistingArg("", "lint-private-var-usage", "Adds the 'private_var_usage' lint check to the SQF-VM SQF Parser. Note that this check requires assembly generation.", false);
    // cmd.add(lintPrivateVarExistingArg);

    TCLAP::SwitchArg noExecutePrintArg("", "no-execute-print", "Disables the `Executing...` and two horizontal lines hint printing.", false);
    cmd.add(noExecutePrintArg);

    TCLAP::SwitchArg noLoadExecDirArg("", "no-load-execdir", "Prevents automatically adding the workspace to the path of allowed locations.", false);
    cmd.add(noLoadExecDirArg);

    TCLAP::SwitchArg noSpawnPlayerArg("", "no-spawn-player", "Prevents automatic \"spawn\" of the player.", false);
    cmd.add(noSpawnPlayerArg);

    TCLAP::SwitchArg noWrokPrintArg("", "no-work-print", "Prevents the results printing of contexts that reached an empty state.", false);
    cmd.add(noWrokPrintArg);

    // TCLAP::SwitchArg noAssemblyCreationArg("", "no-assembly-creation", "Will force to use only the SQF parser. "
    //     "Execution of SQF-code will not work with this. "
    //     "Useful, if one only wants to perform syntax checks.", false);
    // cmd.add(noAssemblyCreationArg);

    cmd.getArgList().reverse();

    cmd.parse(argc, argv);

    // ALWAYS needs to be parsed first!
    if (!cliFileArg.getValue().empty())
    {
        if (isInLoadFileCliMode)
        {
            std::cout << "'" << cliFileArg.getName() << "' is not allowed inside of the actual file!." << std::endl;
            return -1;
        }
        isInLoadFileCliMode = true;
        auto f = cliFileArg.getValue();
        if (f.empty())
        {
            std::cout << "'" << f << "' is no valid file path after sanitize." << std::endl;
            return -1;
        }
        if (f.length() > 2 && f[0] == '.' && (f[1] == '/' || f[1] == '\\'))
        {
            f = std::filesystem::absolute((std::filesystem::path(executable_path) / f).lexically_normal()).string();
        }
        try
        {
            auto file = sqf::runtime::fileio::read_file_from_disk(f);
            if (!file.has_value())
            {
                std::cout << "Failed to load file '" << f << "'" << std::endl;
                return -1;
            }
            auto str = *file;
            std::vector<char*> args;
            args.push_back(argv[0]); // ToDo: Catch those moments when argv[0] is not set by the OS
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

            main(static_cast<int>(args.size()), args.data());

            if (args.size() > 1)
            {
                for (auto it = args.begin() + 1; it != args.end(); it++)
                {
                    delete[] * it;
                }
            }
            return 0;
        }
        catch (std::runtime_error& err)
        {
            std::cout << err.what() << std::endl;
            return -1;
        }
    }

    std::vector<std::string> sqf_files = inputSqfArg.getValue();
    std::vector<std::string> config_files = inputConfigArg.getValue();
    std::vector<std::string> pbo_files = inputPboArg.getValue();
    bool errflag = false;
    bool automated = automatedArg.getValue();
    // bool noAssemblyCreation = noAssemblyCreationArg.getValue();
    bool parseOnly = parseOnlyArg.getValue(); // || noAssemblyCreation;
    for (auto& f : inputArg.getValue())
    {
        auto ext = extension(f);
        if (ext == "sqf")
        {
            sqf_files.push_back(f);
        }
        else if (ext == "cpp" || ext == "hpp" || ext == "ext")
        {
            config_files.push_back(f);
        }
        else if (ext == "pbo")
        {
            pbo_files.push_back(f);
        }
        else
        {
            errflag = true;
            std::cerr << "The file extension '" << ext << "' is not understandible. Consider using '--input-sqf' for example, to explicitly add your file." << std::endl;
        }
    }

    std::reverse(sqf_files.begin(), sqf_files.end());
    std::reverse(config_files.begin(), config_files.end());
    std::reverse(pbo_files.begin(), pbo_files.end());

    bool noLoadExecDir = noLoadExecDirArg.getValue();
    bool verbose = verboseArg.getValue();



    StdOutLogger logger;
    sqf::runtime::runtime::runtime_conf conf;
    conf.enable_classname_check = enableClassnameCheckArg.getValue();
    conf.print_context_work_to_log_on_exit = !noWrokPrintArg.getValue();
    conf.max_runtime = std::chrono::milliseconds(maxRuntimeArg.getValue());
    

    sqf::runtime::runtime runtime(logger, conf);
    runtime.fileio(std::make_unique<sqf::fileio::default>());
    runtime.parser_config(std::make_unique<sqf::parser::config::default>(logger));
    runtime.parser_preprocessor(std::make_unique<sqf::parser::preprocessor::default>(logger));
    runtime.parser_sqf(std::make_unique<sqf::parser::sqf::default>(logger));
    sqf::operators::ops_config(runtime);
    sqf::operators::ops_diag(runtime);
    sqf::operators::ops_generic(runtime);
    sqf::operators::ops_group(runtime);
    sqf::operators::ops_logic(runtime);
    sqf::operators::ops_markers(runtime);
    sqf::operators::ops_math(runtime);
    sqf::operators::ops_namespace(runtime);
    sqf::operators::ops_object(runtime);
    sqf::operators::ops_sqfvm(runtime);
    sqf::operators::ops_string(runtime);

    if (!noSpawnPlayerArg.getValue())
    {
        auto player_object = sqf::types::object::create(runtime, {}, false);;
        runtime.storage<sqf::types::object::object_storage>().player(player_object);
    }

    //netserver* srv = nullptr;

    // if (lintPrivateVarExistingArg.getValue())
    // {
    //     sqf::linting::add_to(&runtime, sqf::linting::check::private_var_usage);
    // }

    
    // if (maxInstructionsArg.getValue() != 0)
    // {
    //     runtime.set_max_instructions(maxInstructionsArg.getValue());
    // }


    // Prepare Virtual-File-System
    if (!noLoadExecDir)
    {
        runtime.fileio().add_mapping(executable_path, "/");
    }
    // for (auto& f : loadArg.getValue())
    // {
    //     if (f.empty())
    //     {
    //         continue;
    //     }
    //     if (f.length() > 2 && f[0] == '.' && (f[1] == '/' || f[1] == '\\'))
    //     {
    //         f = std::filesystem::absolute((std::filesystem::path(executable_path) / f).lexically_normal()).string();
    //     }
    //     runtime.get_filesystem().add_allowed_physical(f);
    //     if (verbose)
    //     {
    //         std::cout << "Added '" << f << "' to allowed paths." << std::endl;
    //     }
    // }
    for (auto& f : virtualArg.getValue())
    {
        auto split_index = f.find('|');
        if (split_index == std::string::npos)
        {
            errflag = true;
            std::cerr << "Failed find splitter '|' for mapping '" << f << "'." << std::endl;
            continue;
        }
        auto phys = f.substr(0, split_index);
        auto virt = f.substr(split_index + 1);
        if (phys.empty() || virt.empty())
        {
            continue;
        }
        if (f.length() > 2 && f[0] == '.' && (f[1] == '/' || f[1] == '\\'))
        {
            phys = (std::filesystem::path(executable_path) / phys).lexically_normal().string();
        }
        runtime.fileio().add_mapping(phys, virt);
        if (verbose)
        {
            std::cout << "Mapped '" << virt << "' onto '" << phys << "'." << std::endl;
        }
    }
    

    // Prepare Dummy-Commands
    for (auto& f : commandDummyNular.getValue())
    {
        runtime.register_sqfop(sqf::runtime::sqfop::nular(f, "DUMMY", [](sqf::runtime::runtime& runtime) -> sqf::runtime::value {
            runtime.__logmsg(logmessage::runtime::ErrorMessage(runtime.context_active().current_frame().diag_info_from_position(), "DUMMY", "DUMMY")); return {};
        }));
    }
    for (auto& f : commandDummyUnary.getValue())
    {
        runtime.register_sqfop(sqf::runtime::sqfop::unary(f, sqf::types::t_any(), "DUMMY", [](sqf::runtime::runtime& runtime, sqf::runtime::value::cref r) -> sqf::runtime::value {
            runtime.__logmsg(logmessage::runtime::ErrorMessage(runtime.context_active().current_frame().diag_info_from_position(), "DUMMY", "DUMMY")); return {};
        }));
    }
    for (auto& f : commandDummyBinary.getValue())
    {
        auto split_index = f.find('|');
        if (split_index == std::string::npos)
        {
            errflag = true;
            std::cerr << "Failed find splitter '|' for precedence '" << f << "'." << std::endl;
            continue;
        }
        auto precedence = f.substr(0, split_index);
        auto name = f.substr(split_index + 1);
        runtime.register_sqfop(sqf::runtime::sqfop::binary(static_cast<short>(std::stoi(precedence)), name, sqf::types::t_any(), sqf::types::t_any(), "DUMMY", [](sqf::runtime::runtime& runtime, sqf::runtime::value::cref l, sqf::runtime::value::cref r) -> sqf::runtime::value {
            runtime.__logmsg(logmessage::runtime::ErrorMessage(runtime.context_active().current_frame().diag_info_from_position(), "DUMMY", "DUMMY")); return {};
        }));
    }

    // Prepare Defines
    for (auto& d : defineArg.getValue())
    {
        auto eqIndex = d.find('=');
        if (eqIndex == std::string::npos)
        {
            runtime.parser_preprocessor().push_back({ d });
        }
        else
        {
            runtime.parser_preprocessor().push_back({ d.substr(0, eqIndex), d.substr(eqIndex + 1) });
        }
    }

    if (errflag)
    {
        if (!automated)
        {
            std::string line;
            std::cout << std::endl << "Press [ENTER] to continue...";
            std::getline(std::cin, line);
        }
        return -1;
    }

    // // Execute all possible Pretty-Print requests
    // for (auto& f : prettyPrintArg.getValue())
    // {
    //     try
    //     {
    //         if (verbose)
    //         {
    //             std::cout << "Loading file '" << f << "' to pretty print..." << std::endl;
    //         }
    //         auto str = load_file(f);
    //         std::cout << runtime.pretty_print_sqf(str) << std::endl;
    //     }
    //     catch (const std::runtime_error& ex)
    //     {
    //         errflag = true;
    //         std::cerr << "Failed to load file " << ex.what() << std::endl;
    //     }
    // }

    // Preprocess the files
    for (auto& f : preprocessFileArg.getValue())
    {
        auto sanitized = std::filesystem::absolute((std::filesystem::path(executable_path) / f).lexically_normal()).string();
        try
        {
            if (sanitized.empty())
            {
                continue;
            }
            if (verbose)
            {
                std::cout << "Loading file '" << sanitized << "' for preprocessing ..." << std::endl;
            }
            auto file = sqf::runtime::fileio::read_file_from_disk(sanitized);
            if (!file.has_value())
            {
                std::cout << "Failed to load file '" << sanitized << "'" << std::endl;
                errflag = true;
                continue;
            }
            auto str = *file;
            if (verbose)
            {
                std::cout << "Preprocessing file '" << sanitized << std::endl;
            }
            auto ppedStr = runtime.parser_preprocessor().preprocess(runtime, str, { sanitized, {} });
            if (ppedStr.has_value())
            {
                std::cout << *ppedStr << std::endl;
            }
            else
            {
                errflag = true;
                std::cout << "Failed to preprocess file '" << sanitized << "'" << std::endl;
            }
        }
        catch (const std::runtime_error& ex)
        {
            errflag = true;
            std::cout << "Failed to load file " << ex.what() << std::endl;
        }
    }

    // Load all sqf-files provided via arg.
    for (auto& sqf_file : sqf_files)
    {
        auto sanitized = std::filesystem::absolute((std::filesystem::path(executable_path) / sqf_file).lexically_normal()).string();
        try
        {
            if (sanitized.empty())
            {
                continue;
            }
            if (verbose)
            {
                std::cout << "Loading file '" << sanitized << "' for sqf processing ..." << std::endl;
            }
            auto file = sqf::runtime::fileio::read_file_from_disk(sanitized);
            if (!file.has_value())
            {
                std::cout << "Failed to load file '" << sanitized << "'" << std::endl;
                errflag = true;
                continue;
            }
            auto str = *file;
            if (verbose)
            {
                std::cout << "Preprocessing file '" << sanitized << std::endl;
            }
            auto ppedStr = runtime.parser_preprocessor().preprocess(runtime, str, { sanitized, {} });
            if (ppedStr.has_value())
            {
                if (verbose)
                {
                    std::cout << "Parsing file '" << sanitized << std::endl;
                }
                if (parseOnlyArg.getValue())
                {
                    errflag = runtime.parser_sqf().check_syntax(runtime, *ppedStr, { sanitized, {} });
                }
                else
                {
                    auto set = runtime.parser_sqf().parse(runtime, *ppedStr, { sanitized, {} });
                    if (set.has_value())
                    {
                        auto context = runtime.context_create().lock();
                        sqf::runtime::frame f(runtime.default_value_scope(), *set);
                        context->push_frame(f);
                        context->name(sanitized);
                        if (verbose)
                        {
                            std::cout << "Created Context '" << sanitized << "'" << std::endl;
                        }
                    }
                    else
                    {
                        errflag = true;
                        std::cout << "Failed to parse file '" << sanitized << "'" << std::endl;
                    }
                }
            }
            else
            {
                errflag = true;
                std::cout << "Failed to preprocess file '" << sanitized << "'" << std::endl;
            }
        }
        catch (const std::runtime_error& ex)
        {
            errflag = true;
            std::cout << "Failed to load file '" << sanitized << "': " << ex.what() << std::endl;
        }
    }

    // Load & merge all config-files provided via arg.
    for (auto& f : config_files)
    {
        auto sanitized = std::filesystem::absolute((std::filesystem::path(executable_path) / f).lexically_normal()).string();
        try
        {
            if (sanitized.empty())
            {
                continue;
            }
            if (verbose)
            {
                std::cout << "Loading file '" << sanitized << "' for config processing ..." << std::endl;
            }
            auto file = sqf::runtime::fileio::read_file_from_disk(sanitized);
            if (!file.has_value())
            {
                std::cout << "Failed to load file '" << sanitized << "'" << std::endl;
                errflag = true;
                continue;
            }
            auto str = *file;
            if (verbose)
            {
                std::cout << "Preprocessing file '" << sanitized << std::endl;
            }
            auto ppedStr = runtime.parser_preprocessor().preprocess(runtime, str, { sanitized, {} });
            if (ppedStr.has_value())
            {
                if (verbose)
                {
                    std::cout << "Parsing file '" << sanitized << std::endl;
                }
                if (parseOnlyArg.getValue())
                {
                    errflag = runtime.parser_config().check_syntax(*ppedStr, { sanitized, {} });
                }
                else
                {
                    auto res = runtime.parser_config().parse(runtime.confighost(), *ppedStr, { sanitized, {} });
                    if (!res)
                    {
                        errflag = true;
                        std::cout << "Failed to parse file '" << sanitized << "'" << std::endl;
                    }
                }
            }
            else
            {
                errflag = true;
                std::cout << "Failed to preprocess file '" << sanitized << "'" << std::endl;
            }
        }
        catch (const std::runtime_error& ex)
        {
            errflag = true;
            std::cout << "Failed to load file '" << sanitized << "': " << ex.what() << std::endl;
        }
    }
    if (errflag || parseOnly)
    {
        if (verbose && errflag)
        {
            std::cout << "Exiting due to error." << std::endl;
        }
        if (!automated)
        {
            std::string line;
            std::cout << std::endl << "Press [ENTER] to continue...";
            std::getline(std::cin, line);
        }
        return errflag ? -1 : 0;
    }

    // Load all sqf-code provided via arg.
    for (auto raw = sqfArg.getValue().rbegin(); raw != sqfArg.getValue().rend(); raw++)
    {
        std::string sanitized = "__commandline";
        auto set = runtime.parser_sqf().parse(runtime, *raw, { sanitized, {} });
        if (set.has_value())
        {
            auto context = runtime.context_create().lock();
            sqf::runtime::frame f(runtime.default_value_scope(), *set);
            context->push_frame(f);
            context->name(sanitized);
            if (verbose)
            {
                std::cout << "Created Context '" << sanitized << "'" << std::endl;
            }
        }
        else
        {
            errflag = true;
            std::cout << "Failed to parse commandline input." << std::endl;
        }
    }

    // Load & merge all config-code provided via arg.
    for (auto& raw : configArg.getValue())
    {
        std::string sanitized = "__commandline";
        auto res = runtime.parser_config().parse(runtime.confighost(), raw, { sanitized, {} });
        if (!res)
        {
            errflag = true;
            std::cout << "Failed to parse commandline input." << std::endl;
        }
    }
//     if (serverArg.isSet())
//     {
//         networking_init();
//         auto port = serverArg.getValue();
//         SOCKET socket;
//         if (networking_create_server(&socket))
//         {
// #if _WIN32
//             std::cout << "Something moved wrong during creation of the client socket"
//                          "(0x" << std::hex << GetLastError() << ")" << std::endl;
// #else
//             std::cout << "Something moved wrong during creation of the client socket." << std::endl;
// #endif
//             return -1;
//         }
//         try
//         {
//             runtime.set_networking(std::make_shared<sqf::networking::server>(port));
//             std::cout << "Listening on port " << port << std::endl;
//         }
//         catch (std::runtime_error e)
//         {
//             std::cout << "Something moved wrong during creation of the client socket: " << e.what() << std::endl;
//             return -1;
//         }
//     }
    if (interactiveArg.getValue())
    { // Interactive Mode
        interactive_helper helper(runtime);
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
            if (!automated)
            {
                //Prompt user to type in code.
                int i = 1;
                if (!suppressWelcomeArg.getValue())
                {
                    printf("You can disable this message using `--suppress-welcome`.\n"
                        "Please enter your SQF code.\n"
                        "To get info about a command, use the `help__` operator.\n"
                        "For a list of all implemented commands, use the `cmds__` operator.\n"
                        "For a list of all SQF-VM internal commands, use the `vm__` operator.\n"
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

                std::string sanitized = (std::filesystem::path(executable_path) / "__commandlinefeed.sqf").string();
                auto ppedStr = runtime.parser_preprocessor().preprocess(runtime, input, { sanitized, {} });
                if (ppedStr.has_value())
                {
                    if (parseOnlyArg.getValue())
                    {
                        errflag = runtime.parser_sqf().check_syntax(runtime, *ppedStr, { sanitized, {} });
                    }
                    else
                    {
                        auto set = runtime.parser_sqf().parse(runtime, *ppedStr, { sanitized, {} });
                        if (set.has_value())
                        {
                            if (set->empty())
                            {
                                std::cout << "Compilation yielded no assembly." << std::endl;
                            }
                            else
                            {
                                sqf::runtime::frame f(runtime.default_value_scope(), *set);
                                runtime.context_active().push_frame(f);
                            }
                        }
                        else
                        {
                            errflag = true;
                            std::cout << "Failed to parse commandline feed." << std::endl;
                        }
                    }
                }
                else
                {
                    errflag = true;
                    std::cout << "Failed to preprocess file '" << sanitized << "'" << std::endl;
                }
            }


            sqf::runtime::runtime::result result;
            if (!noExecutePrintArg.getValue())
            {
                std::cout << "Executing..." << std::endl;
                std::cout << std::string(console_width(), '-') << std::endl;
            }
            result = runtime.execute(sqf::runtime::runtime::action::start);
            if (!noExecutePrintArg.getValue())
            {
                std::cout << std::string(console_width(), '-') << std::endl;
            }
            if (result != sqf::runtime::runtime::result::ok)
            {
                runtime.execute(sqf::runtime::runtime::action::abort);
            }
            switch (result)
            {
            case sqf::runtime::runtime::result::invalid:
                if (verbose)
                {
                    std::cout << "Invalid result. Please raise a bug at github concerning this." << std::endl;
                }
                break;
            case sqf::runtime::runtime::result::empty:
                if (verbose)
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
                if (verbose)
                {
                    std::cout << "Runtime Error occured." << std::endl;
                }
                break;
            default:
                break;
            }
        } while (!automated && !runtime.is_exit_requested());
    }

    // if (runtime.is_networking_set())
    // {
    //     runtime.release_networking();
    // }
    // 
    // networking_cleanup();
    auto exitcode = runtime.exit_code();
    if (exitcode.has_value())
    {
        return exitcode.value();
    }
    return 0;
}
