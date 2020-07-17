#ifdef _WIN32
// Required due to some headers using WinSock2.h
// & some headers requiring windows.h
// If this was not here, a link conflict would emerge due to
// windows.h including winsock1
#include <WinSock2.h>
#endif

#include "ops_sqfvm.h"
#include "../runtime/runtime.h"
#include "../runtime/type.h"
#include "../runtime/sqfop.h"
#include "../types/d_array.h"
#include "../types/d_boolean.h"
#include "../types/d_code.h"
#include "../types/d_scalar.h"
#include "../types/d_string.h"


#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../virtualmachine.h"
#include "../configdata.h"
#include "../arraydata.h"
#include "../innerobj.h"
#include "../objectdata.h"
#include "../parsing/parsepreprocessor.h"
#include "../vmstack.h"
#include "../sqfnamespace.h"
#include "../callstack_sqftry.h"
#include "../instruction.h"
#include "../codedata.h"
#include "../networking/network_client.h"
#include "../networking.h"
#include "../codedata.h"
#include "filesystem.h"
#include <sstream>
#include <array>
#include <algorithm> 
#include <cctype>
#include <cmath>
#include <locale>

namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;

namespace
{
    value assembly___code(runtime& runtime, value::cref right)
    {
        auto code = right.data<d_code>();
        
        std::vector<value> outarr;
        for (auto it = code->value().begin(); it != code->value().end(); it++)
        {
            outarr.push_back(val((*it)->to_string()));
        }
        return val(outarr);
    }
    value assembly___string(runtime& runtime, value::cref right)
    {
        auto str = right.data<d_string>();
        auto set = runtime.parser_sqf().parse(runtime, *str);
        if (set.has_value())
        {
            std::vector<value> outarr;
            for (auto it = set->begin(); it != set->end(); it++)
            {
                outarr.push_back(val((*it)->to_string()));
            }
            return val(outarr);
        }
        else
        {
            return {};
        }
    }
    value cmds___(runtime& runtime)
    {
        std::vector<value> outarr;
        auto str = "n";
        for (auto& pair = runtime.sqfop_nular_begin(); pair != runtime.sqfop_nular_end(); pair++)
        {
            outarr.push_back(val(std::vector<value> {
                val(str),
                val(pair->first.name)
            }));
        }
        str = "u";
        for (auto& pair = runtime.sqfop_unary_begin(); pair != runtime.sqfop_unary_end(); pair++)
        {
            outarr.push_back(val(std::vector<value> {
                val(str),
                val(pair->first.name),
                val(pair->first.right_type.to_string())
            }));
        }
        str = "b";
        for (auto& pair = runtime.sqfop_binary_begin(); pair != runtime.sqfop_binary_end(); pair++)
        {
            outarr.push_back(val(std::vector<value> {
                val(str),
                val(pair->first.left_type.to_string()),
                val(pair->first.name),
                val(pair->first.right_type.to_string())
            }));
        }
        return val(outarr);
    }
    value cmdsimplemented___(runtime& runtime)
    {
        std::vector<value> outarr;
        auto str = "n";
        for (auto& pair = runtime.sqfop_nular_begin(); pair != runtime.sqfop_nular_end(); pair++)
        {
            if (pair->second.description().empty())
                continue;
            outarr.push_back(val(std::vector<value> {
                val(str),
                val(pair->first.name)
            }));
        }
        str = "u";
        for (auto& pair = runtime.sqfop_unary_begin(); pair != runtime.sqfop_unary_end(); pair++)
        {
            if (pair->second.description().empty())
                continue;
            outarr.push_back(val(std::vector<value> {
                val(str),
                val(pair->first.name),
                val(pair->first.right_type.to_string())
            }));
        }
        str = "b";
        for (auto& pair = runtime.sqfop_binary_begin(); pair != runtime.sqfop_binary_end(); pair++)
        {
            if (pair->second.description().empty())
                continue;
            outarr.push_back(val(std::vector<value> {
                val(str),
                val(pair->first.left_type.to_string()),
                val(pair->first.name),
                val(pair->first.right_type.to_string())
            }));
        }
        return val(outarr);
    }
    value vm___(runtime& runtime)
    {
        std::vector<value> outarr;
        auto str = "n";
        for (auto& pair = runtime.sqfop_nular_begin(); pair != runtime.sqfop_nular_end(); pair++)
        {
            if (pair->first.name.length() < 2 || pair->first.name[pair->first.name.length() - 1] != '_' || pair->first.name[pair->first.name.length() - 2] != '_')
                continue;
            outarr.push_back(val(std::vector<value> {
                val(str),
                    val(pair->first.name)
            }));
        }
        str = "u";
        for (auto& pair = runtime.sqfop_unary_begin(); pair != runtime.sqfop_unary_end(); pair++)
        {
            if (pair->first.name.length() < 2 || pair->first.name[pair->first.name.length() - 1] != '_' || pair->first.name[pair->first.name.length() - 2] != '_')
                continue;
            outarr.push_back(val(std::vector<value> {
                val(str),
                    val(pair->first.name),
                    val(pair->first.right_type.to_string())
            }));
        }
        str = "b";
        for (auto& pair = runtime.sqfop_binary_begin(); pair != runtime.sqfop_binary_end(); pair++)
        {
            if (pair->first.name.length() < 2 || pair->first.name[pair->first.name.length() - 1] != '_' || pair->first.name[pair->first.name.length() - 2] != '_')
                continue;
            outarr.push_back(val(std::vector<value> {
                val(str),
                    val(pair->first.left_type.to_string()),
                    val(pair->first.name),
                    val(pair->first.right_type.to_string())
            }));
        }
        return val(outarr);
    }
    // value tree___string(runtime& runtime, value::cref right)
    // {
    //     auto str = right.as_string();
    //     std::stringstream sstream;
    //     runtime.parse_sqf_tree(str, &sstream);
    //     return sstream.str();
    // }
    // value tree___code(runtime& runtime, value::cref right)
    // {
    //     auto code = right.data<codedata>();
    //     auto str = code->tosqf();
    //     std::stringstream sstream;
    //     runtime.parse_sqf_tree(str, &sstream);
    //     return sstream.str();
    // }
    value help___string(runtime& runtime, value::cref right)
    {
        std::stringstream sstream;
        std::string str = *right.data<d_string>();
        bool wasfound = false;
        for (auto& pair = runtime.sqfop_nular_begin(); pair != runtime.sqfop_nular_end(); pair++)
        {
            if (pair->first.name != str)
                continue;
            auto cmd = pair->second;
            if (cmd.description().empty())
            {
                sstream << "NULAR '" << pair->first.name << "'\t<" << pair->first.name << ">" << std::endl;
            }
            else
            {
                sstream << "NULAR '" << pair->first.name << "'\t<" << cmd.name() << ">\t" << cmd.description() << std::endl;
            }
            wasfound = true;
        }
        for (auto& pair = runtime.sqfop_unary_begin(); pair != runtime.sqfop_unary_end(); pair++)
        {
            if (pair->first.name != str)
                continue;
            auto cmd = pair->second;
            if (cmd.description().empty())
            {
                sstream << "UNARY '" << pair->first.name << "'\t<" << cmd.name() << " " << pair->first.right_type.to_string() << ">" << std::endl;
            }
            else
            {
                sstream << "UNARY '" << pair->first.name << "'\t<" << cmd.name() << " " << pair->first.right_type.to_string() << ">\t" << cmd.description() << std::endl;
            }
            wasfound = true;
        }
        for (auto& pair = runtime.sqfop_binary_begin(); pair != runtime.sqfop_binary_end(); pair++)
        {
            if (pair->first.name != str)
                continue;
            auto cmd = pair->second;
            if (cmd.description().empty())
            {
                sstream << "BINARY '" << pair->first.name << "'\t<" <<
                    pair->first.left_type.to_string() << " " << cmd.name() << " " << pair->first.right_type.to_string() << ">" <<
                    std::endl;
            }
            else
            {
                sstream << "BINARY '" << pair->first.name << "'\t<" <<
                    pair->first.left_type.to_string() << " " << cmd.name() << " " << pair->first.right_type.to_string() << ">\t" <<
                    cmd.description() << std::endl;
            }
            wasfound = true;
        }
        if (!wasfound)
        {
            sstream << "Could not find any command with that name." << std::endl;
        }
        runtime.__logmsg(err::InfoMessage((*runtime.active_context()->current_frame().current())->diag_info(), "HELP", sstream.str()));
        return {};
    }
    value configparse___string(runtime& runtime, value::cref right)
    {
        auto str = right.as_string();
        auto cd = std::make_shared<sqf::configdata>();
        runtime.parse_config(str, "__configparse___string", cd);
        return value(cd);
    }
    value merge___config_config(runtime& runtime, value::cref left, value::cref right)
    {
        auto target = left.data<configdata>();
        auto source = right.data<configdata>();
        source->mergeinto(target);
        return {};
    }
    value allObjects__(runtime& runtime)
    {
        auto arr = std::make_shared<arraydata>();
        for (auto& object : runtime.get_objlist())
        {
            arr->push_back(value(std::make_shared<objectdata>(object)));
        }
        return value(arr);
    }
    value pretty___code(runtime& runtime, value::cref right)
    {
        auto code = right.data<codedata>();
        auto str = code->tosqf();

        runtime.logmsg(err::InfoMessage(*runtime.current_instruction(), "PRETTY", runtime.pretty_print_sqf(str)));
        return {};
    }
    value prettysqf___string(runtime& runtime, value::cref right)
    {
        auto str = right.as_string();
        runtime.logmsg(err::InfoMessage(*runtime.current_instruction(), "PRETTY", runtime.pretty_print_sqf(str)));
        return {};
    }
    value exit___(runtime& runtime)
    {
        runtime.exit(0);
        return {};
    }
    value exit___scalar(runtime& runtime, value::cref right)
    {
        runtime.exit(static_cast<int>(std::round(*right.data<d_scalar>())));
        return {};
    }
    value respawn___(runtime& runtime)
    {
        runtime.player_obj(innerobj::create(vm, "CAManBase", false));
        return value(std::make_shared<objectdata>(runtime.player_obj()));
    }
    value preprocess___string(runtime& runtime, value::cref right)
    {
        auto content = right.as_string();
        bool errflag = false;
        auto ppres = runtime.preprocess(content, errflag, "__preprocess__");
        if (errflag)
        {
            return {};
        }
        else
        {
            return ppres;
        }
    }
    value except___code_code(runtime& runtime, value::cref left, value::cref right)
    {
        auto cs = std::make_shared<callstack_sqftry>(runtime.active_vmstack()->stacks_top()->get_namespace(), right.data<codedata>());
        runtime.active_vmstack()->push_back(cs);
        left.data<codedata>()->loadinto(runtime.active_vmstack(), cs);
        return {};
    }
    value callstack___(runtime& runtime)
    {
        auto stackdump = runtime.active_vmstack()->dump_callstack_diff({});
        auto sqfarr = std::make_shared<arraydata>();
        for (auto& it : stackdump)
        {
            std::vector<value> vec = {
                    value(it.namespace_used->get_name()),
                    value(it.scope_name),
                    value(it.callstack_name),
                    value(it.line),
                    value(it.column),
                    value(it.file),
                    value(it.dbginf)
            };
            sqfarr->push_back(value(std::make_shared<arraydata>(vec)));
        }
        return value(sqfarr);
    }
    value allfiles___array(runtime& runtime, value::cref right)
    {
#if !defined(FILESYSTEM_DISABLE_DISALLOW)
        if (runtime.get_filesystem().disallow())
        {
            runtime.logmsg(err::FileSystemDisabled(*runtime.current_instruction()));
            return value(std::make_shared<arraydata>());
        }
#endif
        auto arr = right.data<arraydata>();
        if (!arr->check_type(vm, t_string(), 0, arr->size()))
        {
            return {};
        }
        auto files = std::vector<value>();
        //recursively search for pboprefix
        for (auto phys : runtime.get_filesystem().m_physicalboundaries)
        {
            for (auto i = std::filesystem::recursive_directory_iterator(phys, std::filesystem::directory_options::follow_directory_symlink);
                i != std::filesystem::recursive_directory_iterator();
                ++i)
            {
                bool skip = true;
                for (auto& ext : *arr)
                {
                    if (i->is_directory())
                    {
                        break;
                    }
                    if (!(i->path().extension().compare(ext.as_string())))
                    {
                        skip = false;
                        break;
                    }
                }
                if (!skip)
                {
                    files.push_back(i->path().string());
                }
            }
        }
        return value(std::make_shared<arraydata>(files));
    }
    value pwd___(runtime& runtime)
    {
        auto path = std::filesystem::path(runtime.active_vmstack()->stacks_top()->current_instruction()->file());
        return std::filesystem::absolute(path).string();
    }
    value currentDirectory___(runtime& runtime)
    {
        auto path = std::filesystem::path(runtime.active_vmstack()->stacks_top()->current_instruction()->file());
        
        return std::filesystem::absolute(path.parent_path()).string();
    }
    value trim___(runtime& runtime, value::cref right)
    {
        auto str = right.as_string();
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
            return !std::isspace(ch);
            }));
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
            return !std::isspace(ch);
            }).base(), str.end());
        return str;
    }
    value remoteConnect___(runtime& runtime, value::cref right)
    {
        if (!runtime.allow_networking())
        {
            runtime.logmsg(err::NetworkingDisabled(*runtime.current_instruction()));
            return false;
        }
        networking_init();
        if (runtime.is_networking_set())
        {
            runtime.logmsg(err::AlreadyConnected(*runtime.current_instruction()));
            return {};
        }
        auto s = right.as_string();
        auto index = s.find(':');
        if (index == std::string::npos)
        {
            runtime.logmsg(err::NetworkingFormatMissmatch(*runtime.current_instruction(), s));
            return {};
        }
        auto address = s.substr(0, index);
        auto port = s.substr(index + 1);
        SOCKET socket;
        if (networking_create_client(address.c_str(), port.c_str(), &socket))
        {
            runtime.logmsg(err::FailedToEstablishConnection(*runtime.current_instruction()));
            return false;
        }
        runtime.set_networking(std::make_shared<networking::client>(socket));
        return true;
    }
    value closeconnection___(runtime& runtime)
    {
        runtime.release_networking();
        return {};
    }
    value vmctrl___string(runtime& runtime, value::cref right)
    {
        auto str = right.as_string();

        if (str == "abort")
        {
            runtime.execute(sqf::virtualmachine::execaction::abort);
        }
        else if (str == "assembly_step")
        {
            runtime.execute(sqf::virtualmachine::execaction::assembly_step);
        }
        else if (str == "leave_scope")
        {
            runtime.execute(sqf::virtualmachine::execaction::leave_scope);
        }
        else if (str == "reset_run_atomic")
        {
            runtime.execute(sqf::virtualmachine::execaction::reset_run_atomic);
        }
        else if (str == "start")
        {
            runtime.execute(sqf::virtualmachine::execaction::start);
        }
        else if (str == "stop")
        {
            runtime.execute(sqf::virtualmachine::execaction::stop);
        }
        else
        {
            // ToDo: Create custom log message for enum errors
            runtime.logmsg(err::ErrorMessage(*runtime.current_instruction(), "vmctrl", "exec unknown"));
        }
        return {};
    }
    value provide___code_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto arr = right.data<arraydata>();
        if (!arr->check_type(vm, t_string(), 1, 3))
        {
            return {};
        }
        sqf::type ltype;
        sqf::type rtype;
        std::string name;
        int size = arr->size();
        switch (size)
        {
            case 1:
                name = arr->at(0).as_string();
            break;
            case 3:
            {
                name = arr->at(1).as_string();
                auto l = arr->at(0).as_string();
                ltype = parsetype(l);
                auto r = arr->at(arr->size() - 1).as_string();
                rtype = parsetype(r);
            }
            break;
            case 2:
            {
                name = arr->at(0).as_string();
                auto r = arr->at(arr->size() - 1).as_string();
                rtype = parsetype(r);
            } break;
            default:
                return {};
        }
        switch (size)
        {
        case 1:
            sqf::commandmap::get().remove(name);
            sqf::commandmap::get().add(sqf::nulardata<std::shared_ptr<codedata>>(
                left.data<codedata>(),
                name,
                "",
                [](runtime& runtime, std::shared_ptr<codedata> code) -> value
                {
                    code->loadinto(vm, runtime.active_vmstack());
                    runtime.active_vmstack()->stacks_top()->set_variable("_this", value());
                    return {};
                }));
            break;
        case 2:
            sqf::commandmap::get().remove(name, rtype);
            sqf::commandmap::get().add(sqf::unarydata<std::shared_ptr<codedata>>(
                left.data<codedata>(),
                name,
                rtype,
                "",
                [](runtime& runtime, std::shared_ptr<codedata> code, value::cref right) -> value
                {
                    code->loadinto(vm, runtime.active_vmstack());
                    runtime.active_vmstack()->stacks_top()->set_variable("_this", right);
                    return {};
                }));
            break;
        case 3:
            sqf::commandmap::get().remove(ltype, name, rtype);
            sqf::commandmap::get().add(sqf::binarydata<std::shared_ptr<codedata>>(
                left.data<codedata>(),
                (short)4,
                name,
                ltype,
                rtype,
                "",
                [](runtime& runtime, std::shared_ptr<codedata> code, value::cref left, value::cref right) -> value
                {
                    code->loadinto(vm, runtime.active_vmstack());
                    runtime.active_vmstack()->stacks_top()->set_variable("_this", value({ left , right }));
                    return {};
                }));
            break;
        }
        return {};
    }
}
void sqf::operators::ops_sqfvm(sqf::runtime::runtime& runtime)
{
    using namespace sqf::runtime::sqfop;
    //add(unary("tree__", t_string(), "Returns a string containing the abstract syntax tree for the provided SQF expression.", tree___string));
    //add(unary("tree__", t_code(), "Returns a string containing the abstract syntax tree for the provided SQF expression.", tree___code));
    runtime.register_sqfop(nular("cmds__", "Returns an array containing all commands available.", cmds___));
    runtime.register_sqfop(nular("cmdsimplemented__", "Returns an array containing all commands that are actually implemented.", cmdsimplemented___));
    runtime.register_sqfop(unary("help__", t_string(), "Displays all available information for a single command.", help___string));
    runtime.register_sqfop(unary("configparse__", t_string(), "Parses provided string as config into a new config object.", configparse___string));
    runtime.register_sqfop(binary(4, "merge__", t_config(), t_config(), "Merges contents from the right config into the left config. Duplicate entries will be overriden. Contents will not be copied but referenced.", merge___config_config));
    runtime.register_sqfop(nular("allObjects__", "Returns an array containing all objects created.", allObjects__));
    runtime.register_sqfop(unary("pretty__", t_code(), "Takes provided SQF code and pretty-prints it to output.", pretty___code));
    runtime.register_sqfop(unary("prettysqf__", t_code(), "Takes provided SQF code and pretty-prints it to output.", pretty___code));
    runtime.register_sqfop(unary("prettysqf__", t_string(), "Takes provided SQF code and pretty-prints it to output.", prettysqf___string));
    runtime.register_sqfop(nular("exit__", "Exits the VM execution immediately. Will not notify debug interface when used.", exit___));
    runtime.register_sqfop(unary("vmctrl__", t_string(), "Executes the provided SQF-VM exection action.", vmctrl___string));
    runtime.register_sqfop(unary("exitcode__", t_scalar(), "Exits the VM execution immediately. Will not notify debug interface when used. Allows to pass an exit code to the VM.", exit___scalar));
    runtime.register_sqfop(nular("vm__", "Provides a list of all SQF-VM only commands.", vm___));
    runtime.register_sqfop(nular("respawn__", "'Respawns' the player object.", respawn___));
    runtime.register_sqfop(unary("preprocess__", t_string(), "Runs the PreProcessor on provided string.", preprocess___string));
    runtime.register_sqfop(unary("assembly__", t_code(), "returns an array, containing the assembly instructions as string.", assembly___code));
    runtime.register_sqfop(unary("assembly__", t_string(), "returns an array, containing the assembly instructions as string.", assembly___string));
    runtime.register_sqfop(binary(4, "except__", t_code(), t_code(), "Allows to define a block that catches VM exceptions. It is to note, that this will also catch exceptions in spawn! Exception will be put into the magic variable '_exception'. A callstack is available in '_callstack'.", except___code_code));
    runtime.register_sqfop(nular("callstack__", "Returns an array containing the whole callstack.", callstack___));
    runtime.register_sqfop(unary("allFiles__", t_array(), "Returns all files available in currently loaded paths with the given file extensions.", allfiles___array));
    runtime.register_sqfop(nular("pwd__", "Current path determined by current instruction.", pwd___));
    runtime.register_sqfop(nular("currentDirectory__", "Current directory determined by current instruction.", currentDirectory___));
    runtime.register_sqfop(unary("trim__", t_string(), "Trims provided strings start and end.", trim___));
    runtime.register_sqfop(unary("remoteConnect__", t_string(), "Connects this as a client to the provided endpoint. Endpoint is expected to have the format ADDRESS:PORT. Returns TRUE on success, false if it failed. Note that IP-Address is required, not DNS names (eg. use '127.0.0.1' instead of 'localhost').", remoteConnect___));
    runtime.register_sqfop(nular("closeConnection__", "Closes the connection previously opened using remoteConnect__.", closeconnection___));
    runtime.register_sqfop(binary(4, "provide__", t_code(), t_array(), "Allows to provide an implementation for a given operator. Will NOT override existing definitions. Array is expected to be of the following formats:"
        "nular: [\"name\"], unary: [\"name\", \"type\"], binary: [\"ltype\", \"name\", \"rtype\"]", provide___code_string));
}