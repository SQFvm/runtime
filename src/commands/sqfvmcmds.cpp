#ifdef _WIN32
// Required due to some headers using WinSock2.h
// & some headers requiring windows.h
// If this was not here, a link conflict would emerge due to
// windows.h including winsock1
#include <WinSock2.h>
#endif


#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../virtualmachine.h"
#include "../configdata.h"
#include "../arraydata.h"
#include "../innerobj.h"
#include "../objectdata.h"
#include "../parsepreprocessor.h"
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

using namespace sqf;
namespace
{
	value assembly___code(virtualmachine* vm, value::cref right)
	{
		auto codedata = right.data<sqf::codedata>();
		std::vector<sqf::value> outarr;
		for (auto it = codedata->instructions_begin(); it != codedata->instructions_end(); it++)
		{
			outarr.push_back(sqf::value((*it)->to_string()));
		}
		return outarr;
	}
	value assembly___string(virtualmachine* vm, value::cref right)
	{
		auto str = right.as_string();
		auto cs = std::make_shared<callstack>(vm->active_vmstack()->stacks_top()->get_namespace());
		vm->parse_sqf(str, cs);
		sqf::codedata codedata(cs);
		std::vector<sqf::value> outarr;
		for (auto it = codedata.instructions_begin(); it != codedata.instructions_end(); it++)
		{
			outarr.push_back(sqf::value((*it)->to_string()));
		}
		return outarr;
	}
	value cmds___(virtualmachine* vm)
	{
		std::vector<sqf::value> outarr;
		auto str = "n";
		for (auto& pair : commandmap::get().all_n())
		{
			outarr.push_back(sqf::value(std::vector<sqf::value> { str,
				pair.first
			}));
		}
		str = "u";
		for (auto& pair : commandmap::get().all_u())
		{
			for (auto& it : *pair.second.get())
			{
				outarr.push_back(sqf::value(std::vector<sqf::value> { str,
					sqf::value(pair.first),
					sqf::value(sqf::type_str(it->rtype()))
				}));
			}
		}
        str = "b";
		for (auto& pair : commandmap::get().all_b())
		{
			for (auto& it : *pair.second.get())
			{
				outarr.push_back(sqf::value(std::vector<sqf::value> { str,
					sqf::value(sqf::type_str(it->ltype())),
					sqf::value(pair.first),
					sqf::value(sqf::type_str(it->rtype()))
				}));
			}
		}
		return outarr;
	}
	value cmdsimplemented___(virtualmachine* vm)
	{
		std::vector<sqf::value> outarr;
		auto str = "n";
		for (auto& pair : commandmap::get().all_n())
		{
			if (pair.second->desc().length() == 0)
				continue;
			outarr.push_back(sqf::value(std::vector<sqf::value> { str,
				sqf::value(pair.first)
			}));
		}
		str = "u";
		for (auto& pair : commandmap::get().all_u())
		{
			for (auto& it : *pair.second.get())
			{
				if (it->desc().length() == 0)
					continue;
				outarr.push_back(sqf::value(std::vector<sqf::value> { str,
					sqf::value(pair.first),
					sqf::value(sqf::type_str(it->rtype()))
				}));
			}
		}
		str = "b";
		for (auto& pair : commandmap::get().all_b())
		{
			for (auto& it : *pair.second.get())
			{
				if (it->desc().length() == 0)
					continue;
				outarr.push_back(sqf::value(std::vector<sqf::value> { str,
					sqf::value(sqf::type_str(it->ltype())),
					sqf::value(pair.first),
					sqf::value(sqf::type_str(it->rtype()))
				}));
			}
		}
		return outarr;
	}
	value vm___(virtualmachine* vm)
	{
		std::vector<sqf::value> outarr;
		auto str = "n";
		for (auto& pair : commandmap::get().all_n())
		{
			if (str_ew(pair.first.c_str(), "__") == 0)
				continue;
			outarr.push_back(sqf::value(std::vector<sqf::value> { str,
				sqf::value(pair.first)
			}));
		}
		str = "u";
		for (auto& pair : commandmap::get().all_u())
		{
			if (str_ew(pair.first.c_str(), "__") == 0)
				continue;
			for (auto& it : *pair.second.get())
			{
				outarr.push_back(sqf::value(std::vector<sqf::value> { str,
					sqf::value(pair.first),
					sqf::value(sqf::type_str(it->rtype()))
				}));
			}
		}
		str = "b";
		for (auto& pair : commandmap::get().all_b())
		{
			if (str_ew(pair.first.c_str(), "__") == 0)
				continue;
			for (auto& it : *pair.second.get())
			{
				outarr.push_back(sqf::value(std::vector<sqf::value> { str,
					sqf::value(sqf::type_str(it->ltype())),
					sqf::value(pair.first),
					sqf::value(sqf::type_str(it->rtype()))
				}));
			}
		}
		return outarr;
	}
	value tree___string(virtualmachine* vm, value::cref right)
	{
		auto str = right.as_string();
		std::stringstream sstream;
		vm->parse_sqf_tree(str, &sstream);
		return sstream.str();
	}
	value tree___code(virtualmachine* vm, value::cref right)
	{
		auto code = right.data<codedata>();
		auto str = code->tosqf();
		std::stringstream sstream;
		vm->parse_sqf_tree(str, &sstream);
		return sstream.str();
	}
	value help___string(virtualmachine* vm, value::cref right)
	{
		std::stringstream sstream;
		auto str = right.as_string();
		bool wasfound = false;
		for (auto& pair : commandmap::get().all_n())
		{
			if (str_cmpi(pair.first.c_str(), static_cast<int>(pair.first.length()), str.c_str(), static_cast<int>(str.length())) != 0)
				continue;
			auto cmd = pair.second;
			if (cmd->desc().empty())
				vm->out() << "NULAR '" << pair.first << "'\t<" << cmd->name() << ">" << std::endl;
			else
				vm->out() << "NULAR '" << pair.first << "'\t<" << cmd->name() << ">\t" << cmd->desc() << std::endl;
			wasfound = true;
		}
		for (auto& pair : commandmap::get().all_u())
		{
			if (str_cmpi(pair.first.c_str(), static_cast<int>(pair.first.length()), str.c_str(), static_cast<int>(str.length())) != 0)
				continue;
			auto cmds = pair.second;
			for (auto& cmd : *cmds)
			{
				if (!cmd->desc().empty())
					vm->out() << "UNARY '" << pair.first << "'\t<" << cmd->name() << " " << sqf::type_str(cmd->rtype()) << ">\t" << cmd->desc() << std::endl;
				else
					vm->out() << "UNARY '" << pair.first << "'\t<" << cmd->name() << " " << sqf::type_str(cmd->rtype()) << ">" << std::endl;
				wasfound = true;
			}
		}
		for (auto& pair : commandmap::get().all_b())
		{
			if (str_cmpi(pair.first.c_str(), static_cast<int>(pair.first.length()), str.c_str(), static_cast<int>(str.length())) != 0)
				continue;
			auto cmds = pair.second;
			for (auto& cmd : *cmds)
			{
				if (!cmd->desc().empty())
					vm->out() << "BINARY '" << pair.first << "'\t<" << sqf::type_str(cmd->ltype()) << " " << cmd->name() << " " << sqf::type_str(cmd->rtype()) << ">\t" << cmd->desc() << std::endl;
				else
					vm->out() << "BINARY '" << pair.first << "'\t<" << sqf::type_str(cmd->ltype()) << " " << cmd->name() << " " << sqf::type_str(cmd->rtype()) << ">" << std::endl;
			}
			wasfound = true;
		}
		if (!wasfound)
		{
			vm->out() << "Could not find any command with that name." << std::endl;
		}
		return {};
	}
	value configparse___string(virtualmachine* vm, value::cref right)
	{
		auto str = right.as_string();
		auto cd = std::make_shared<sqf::configdata>();
		vm->parse_config(str, cd);
		return value(cd);
	}
	value merge___config_config(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto target = left.data<configdata>();
		auto source = right.data<configdata>();
		source->mergeinto(target);
		return {};
	}
	value allObjects__(virtualmachine* vm)
	{
		auto arr = std::make_shared<arraydata>();
		for (auto& object : vm->get_objlist())
		{
			arr->push_back(value(std::make_shared<objectdata>(object)));
		}
		return value(arr);
	}
	value pretty___code(virtualmachine* vm, value::cref right)
	{
		auto code = right.data<codedata>();
		auto str = code->tosqf();
		vm->pretty_print_sqf(str);
		return {};
	}
	value prettysqf___string(virtualmachine* vm, value::cref right)
	{
		auto str = right.as_string();
		vm->pretty_print_sqf(str);
		return {};
	}
	value exit___(virtualmachine* vm)
	{
		vm->exit_flag(true);
		return {};
	}
	value exit___scalar(virtualmachine* vm, value::cref right)
	{
		vm->exit_flag(true, static_cast<int>(std::round(right.as_float())));
		return {};
	}
    value respawn___(virtualmachine* vm)
	{
		vm->player_obj(innerobj::create(vm, "CAManBase", false));
		return value(std::make_shared<objectdata>(vm->player_obj()));
	}
	value preprocess___string(virtualmachine* vm, value::cref right)
	{
		auto content = right.as_string();
		bool errflag = false;
		auto ppres = sqf::parse::preprocessor::parse(vm, content, errflag, "__preprocess__.sqf");
		if (errflag)
		{
			return {};
		}
		else
		{
			return ppres;
		}
	}
	value except___code_code(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto cs = std::make_shared<callstack_sqftry>(vm->active_vmstack()->stacks_top()->get_namespace(), right.data<codedata>());
		vm->active_vmstack()->push_back(cs);
		left.data<codedata>()->loadinto(vm->active_vmstack(), cs);
		return {};
	}
	value callstack___(virtualmachine* vm)
	{
		auto stackdump = vm->active_vmstack()->dump_callstack_diff({});
		auto sqfarr = std::make_shared<arraydata>();
		for (auto& it : stackdump)
		{
			std::vector<sqf::value> vec = {
					sqf::value(it.namespace_used->get_name()),
					sqf::value(it.scope_name),
					sqf::value(it.callstack_name),
					sqf::value(it.line),
					sqf::value(it.column),
					sqf::value(it.file),
					sqf::value(it.dbginf)
			};
			sqfarr->push_back(value(std::make_shared<arraydata>(vec)));
		}
		return value(sqfarr);
	}
	value allfiles___(virtualmachine* vm, value::cref right)
	{
#if !defined(FILESYSTEM_DISABLE_DISALLOW)
		if (vm->get_filesystem().disallow())
		{
			vm->wrn() << "FILE SYSTEM IS DISABLED" << std::endl;
			return sqf::value(std::make_shared<arraydata>());
		}
#endif
		auto arr = right.data<arraydata>();
		if (!arr->check_type(vm, sqf::type::STRING, 0, arr->size()))
		{
			return {};
		}
		auto files = std::vector<sqf::value>();
		//recursively search for pboprefix
		for (auto phys : vm->get_filesystem().m_physicalboundaries)
		{
			for (auto i = std::filesystem::recursive_directory_iterator(phys, std::filesystem::directory_options::follow_directory_symlink);
				i != std::filesystem::recursive_directory_iterator();
				++i)
			{
				bool skip = false;
				for (auto& ext : *arr) {
					if (i->is_directory() || i->path().extension().compare(ext.as_string()))
					{
						skip = true;
						break;
					}
				}
				if (!skip)
				{
					files.push_back(i->path().string());
				}
			}
		}
		return sqf::value(std::make_shared<arraydata>(files));
	}
	value pwd___(virtualmachine* vm)
	{
		auto path = std::filesystem::path(vm->active_vmstack()->stacks_top()->current_instruction()->file());
		return std::filesystem::absolute(path).string();
	}
	value currentDirectory___(virtualmachine* vm)
	{
		auto path = std::filesystem::path(vm->active_vmstack()->stacks_top()->current_instruction()->file());
		
		return std::filesystem::absolute(path.parent_path()).string();
	}
	value trim___(virtualmachine* vm, value::cref right)
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
	value remoteConnect___(virtualmachine* vm, value::cref right)
	{
		if (!vm->allow_networking())
		{
			vm->wrn() << "NETWORKING DISABLED!" << std::endl;
			return false;
		}
		networking_init();
		if (vm->is_networking_set())
		{
			vm->err() << "Already connected to something." << std::endl;
			return {};
		}
		auto s = right.as_string();
		auto index = s.find(':');
		if (index == std::string::npos)
		{
			vm->err() << "Invalid input format." << std::endl;
			return {};
		}
		auto address = s.substr(0, index);
		auto port = s.substr(index + 1);
		SOCKET socket;
		if (networking_create_client(address.c_str(), port.c_str(), &socket))
		{
			vm->wrn() << "Something moved wrong during creation of the client socket." << std::endl;
			return false;
		}
		vm->set_networking(std::make_shared<networking::client>(socket));
		return true;
	}
	value closeconnection___(virtualmachine* vm)
	{
		vm->release_networking();
		return {};
	}
	value provide___code_string(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto arr = right.data<arraydata>();
		if (!arr->check_type(vm, sqf::type::STRING, 1, 3))
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
				[](virtualmachine* vm, std::shared_ptr<codedata> code) -> value
				{
					code->loadinto(vm, vm->active_vmstack());
					vm->active_vmstack()->stacks_top()->set_variable("_this", value());
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
				[](virtualmachine* vm, std::shared_ptr<codedata> code, sqf::value::cref right) -> value
				{
					code->loadinto(vm, vm->active_vmstack());
					vm->active_vmstack()->stacks_top()->set_variable("_this", right);
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
				[](virtualmachine* vm, std::shared_ptr<codedata> code, sqf::value::cref left, sqf::value::cref right) -> value
				{
					code->loadinto(vm, vm->active_vmstack());
					vm->active_vmstack()->stacks_top()->set_variable("_this", value({ left , right }));
					return {};
				}));
			break;
		}
		return {};
	}
}
void sqf::commandmap::initsqfvmcmds()
{
	add(unary("tree__", sqf::type::STRING, "Returns a string containing the abstract syntax tree for the provided SQF expression.", tree___string));
	add(unary("tree__", sqf::type::CODE, "Returns a string containing the abstract syntax tree for the provided SQF expression.", tree___code));
	add(nular("cmds__", "Returns an array containing all commands available.", cmds___));
	add(nular("cmdsimplemented__", "Returns an array containing all commands that are actually implemented.", cmdsimplemented___));
	add(unary("help__", sqf::type::STRING, "Displays all available information for a single command.", help___string));
	add(unary("configparse__", sqf::type::STRING, "Parses provided string as config into a new config object.", configparse___string));
	add(binary(4, "merge__", sqf::type::CONFIG, sqf::type::CONFIG, "Merges contents from the right config into the left config. Duplicate entries will be overriden. Contents will not be copied but referenced.", merge___config_config));
	add(nular("allObjects__", "Returns an array containing all objects created.", allObjects__));
	add(unary("pretty__", sqf::type::CODE, "Takes provided SQF code and pretty-prints it to output.", pretty___code));
	add(unary("prettysqf__", sqf::type::CODE, "Takes provided SQF code and pretty-prints it to output.", pretty___code));
	add(unary("prettysqf__", sqf::type::STRING, "Takes provided SQF code and pretty-prints it to output.", prettysqf___string));
	add(nular("exit__", "Exits the VM execution immediately. Will not notify debug interface when used.", exit___));
	add(unary("exitcode__", sqf::type::SCALAR, "Exits the VM execution immediately. Will not notify debug interface when used. Allows to pass an exit code to the VM.", exit___scalar));
	add(nular("vm__", "Provides a list of all SQF-VM only commands.", vm___));
	add(nular("respawn__", "'Respawns' the player object.", respawn___));
	add(unary("preprocess__", sqf::type::STRING, "Runs the PreProcessor on provided string.", preprocess___string));
	add(unary("assembly__", sqf::type::CODE, "returns an array, containing the assembly instructions as string.", assembly___code));
	add(unary("assembly__", sqf::type::STRING, "returns an array, containing the assembly instructions as string.", assembly___string));
	add(binary(4, "except__", sqf::type::CODE, sqf::type::CODE, "Allows to define a block that catches VM exceptions. It is to note, that this will also catch exceptions in spawn! Exception will be put into the magic variable '_exception'. A callstack is available in '_callstack'.", except___code_code));
	add(nular("callstack__", "Returns an array containing the whole callstack.", callstack___));
	add(unary("allFiles__", sqf::type::ARRAY, "Returns all files available in currently loaded paths with the given file extensions.", allfiles___));
	add(nular("pwd__", "Current path determined by current instruction.", pwd___));
	add(nular("currentDirectory__", "Current directory determined by current instruction.", currentDirectory___));
	add(unary("trim__", sqf::type::STRING, "Trims provided strings start and end.", trim___));
	add(unary("remoteConnect__", sqf::type::STRING, "Connects this as a client to the provided endpoint. Endpoint is expected to have the format ADDRESS:PORT. Returns TRUE on success, false if it failed. Note that IP-Address is required, not DNS names (eg. use '127.0.0.1' instead of 'localhost').", remoteConnect___));
	add(nular("closeConnection__", "Closes the connection previously opened using remoteConnect__.", closeconnection___));
	add(binary(4, "provide__", sqf::type::CODE, sqf::type::ARRAY, "Allows to provide an implementation for a given operator. Will NOT override existing definitions. Array is expected to be of the following formats:"
		"nular: [\"name\"], unary: [\"name\", \"type\"], binary: [\"ltype\", \"name\", \"rtype\"]", provide___code_string));
}
