#include "debugger.h"
#include "netserver.h"
#include "virtualmachine.h"
#include <nlohmann/json.hpp>
#include "vmstack.h"
#include "callstack.h"
#include "sqfnamespace.h"
#include "value.h"
#include "instruction.h"
#include "fileio.h"
#include "parsepreprocessor.h"
#include <sstream>

namespace {
	class srvmessage {
	public:
		virtual std::string serialize() = 0;
		operator std::string() { return serialize(); }
	};
	class simplemessage : public srvmessage {
		std::string _msg;
	public:
		simplemessage(std::string msg) : _msg(std::move(msg)) {}
		std::string serialize() override {
			nlohmann::json json = {
				{ "mode", "message" },
				{ "data", _msg }
			};
			return json.dump();
		}
	};
	class errormsg : public srvmessage {
		std::string _msg;
	public:
		errormsg(std::string msg) : _msg(std::move(msg)) {}
		std::string serialize() override {
			nlohmann::json json = {
				{ "mode", "error" },
				{ "data", _msg }
			};
			return json.dump();
		}
	};
	class statusupdate : public srvmessage {
		sqf::debugger::srvstatus _status;
	public:
		statusupdate(sqf::debugger::srvstatus status) : _status(status) {}
		std::string serialize() override {
			nlohmann::json json;
			json["mode"] = "status";
			switch (_status)
			{
			case sqf::debugger::HALT:
				json["data"] = "HALT";
				break;
			case sqf::debugger::RUNNING:
				json["data"] = "RUNNING";
				break;
			case sqf::debugger::DONE:
				json["data"] = "DONE";
				break;
			default:
				json["data"] = "NA";
				break;
			}
			return json.dump();
		}
	};
	class callstackmsg : public srvmessage {
		std::shared_ptr<sqf::vmstack> _stack;
	public:
		callstackmsg(std::shared_ptr<sqf::vmstack> stack) : _stack(std::move(stack)) {}
		std::string serialize() override {
			nlohmann::json data;
			int lvl = 0;
			for (auto it = _stack->stacks_begin(); it != _stack->stacks_end(); ++it)
			{
				nlohmann::json jcs;
				auto cs = *it;
				jcs["lvl"] = lvl--;
				jcs["scopename"] = cs->get_scopename();
				jcs["namespace"] = cs->get_namespace()->get_name();
				nlohmann::json options;
				if (cs->size_instructions() <= 0)
				{
					options["line"] = 0;
					options["column"] = 0;
					options["file"] = "";
					options["available"] = false;
				}
				else
				{
					options["line"] = cs->current_instruction()->line();
					options["column"] = cs->current_instruction()->col();
					options["file"] = cs->current_instruction()->file();
					options["available"] = true;
				}
				jcs["options"] = options;
				auto variables = nlohmann::json::array();
				for (auto& pair : cs->get_variable_map())
				{
					variables.push_back(nlohmann::json{ { "name", pair.first },{ "value", pair.second.as_string() } });
				}
				jcs["variables"] = variables;
				data.push_back(jcs);
			}

			return nlohmann::json{
				{ "mode", "callstack" },
			{ "data", data }
			}.dump();
		}
	};
	class variablemsg : public srvmessage {
		std::shared_ptr<sqf::vmstack> _stack;
		sqf::virtualmachine * _vm;
		nlohmann::json _data;
	public:
		variablemsg(sqf::virtualmachine * vm, std::shared_ptr<sqf::vmstack> stack, nlohmann::json data) : _stack(std::move(stack)), _vm(vm), _data(std::move(data)) {}
		std::string serialize() override {
			auto data = nlohmann::json::array();
			for (auto& it : _data) 
			{
				std::string name = it.at("name");
				auto scope = it.at("scope");
				if (scope.is_number())
				{
					int numscope = scope;
					if (numscope > 0)
					{
						throw std::runtime_error("numscope has to be <= 0");
					}
					else
					{
						std::vector<std::shared_ptr<sqf::callstack>>::reverse_iterator s;
						for (s = _stack->stacks_begin(); s != _stack->stacks_end() && numscope != 0; ++s, numscope++);
						if (s == _stack->stacks_end())
						{
							data.push_back(nlohmann::json{ { "name", name },{ "value", "nil" } });
						}
						else
						{
							data.push_back(nlohmann::json{ { "name", name },{ "value", (*s)->get_variable(name).as_string() } });
						}
					}
				}
				else if (scope.get<std::string>() == "missionNamespace")
				{
					auto ns = _vm->missionnamespace();
					data.push_back(nlohmann::json{ { "name", name },{ "value", ns->get_variable(name).as_string() } });
				}
				else if (scope.get<std::string>() =="uiNamespace")
				{
					auto ns = _vm->uinamespace();
					data.push_back(nlohmann::json{ { "name", name },{ "value", ns->get_variable(name).as_string() } });
				}
				else if (scope.get<std::string>() =="profileNamespace")
				{
					auto ns = _vm->profilenamespace();
					data.push_back(nlohmann::json{ { "name", name },{ "value", ns->get_variable(name).as_string() } });
				}
				else if (scope.get<std::string>() =="parsingNamespace")
				{
					auto ns = _vm->parsingnamespace();
					data.push_back(nlohmann::json{ { "name", name },{ "value", ns->get_variable(name).as_string() } });
				}
			}
			return nlohmann::json{
				{ "mode", "variables" },
			{ "data", data }
			}.dump();
		}
	};
	class positionmsg : public srvmessage {
		size_t _line;
		size_t _col;
		std::string _file;
	public:
		positionmsg(size_t line, size_t col, std::string file) : _line(line), _col(col), _file(std::move(file)) {}
		std::string serialize() override {
			nlohmann::json json = { { "mode", "position" }, { "data", { { "line", _line }, { "col", _col }, { "file", _file } } } };
			return json.dump();
		}
	};
}

void sqf::debugger::position(size_t line, size_t col, std::string file)
{
	_server->push_message(positionmsg(line, col, std::move(file)));
}
void sqf::debugger::message(std::string s)
{
	_server->push_message(simplemessage(s));
}
void sqf::debugger::breakmode(virtualmachine * vm)
{
	_control = srvcontrol::PAUSE;
	if (_status == RUNNING)
	{
		_status = HALT;
		_server->push_message(statusupdate(_status));
	}
	while (_status != srvstatus::RUNNING && _server->is_accept())
	{
		check(vm);
		if (_control != PAUSE)
		{
			_status = srvstatus::RUNNING;
			_server->push_message(statusupdate(srvstatus::RUNNING));
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

bool sqf::debugger::hitbreakpoint(size_t line, std::string file)
{
	for (auto& bp : _breakpoints)
	{
		if (bp.line() == line && bp.file() == file)
		{
			return true;
		}
	}
	return false;
}

void sqf::debugger::check(virtualmachine * vm)
{
	while (_server->has_message())
	{
		try
		{
			auto msg = _server->next_message();
			auto json = nlohmann::json::parse(msg);
			std::string mode = json["mode"];
			if (mode == "get-callstack")
			{
				_server->push_message(callstackmsg(vm->active_vmstack()));
			}
			else if (mode == "control")
			{
				std::string status = json["data"]["status"];
				if (status == "run") { _control = RUN; }
				else if (status == "stop") { _control = STOP; }
				else if (status == "pause") { if (_control != PAUSE) { breakmode(vm); } }
				else if (status == "resum") { _control = RESUME; }
				else if (status == "quit") { _control = QUIT; }
				else { _server->push_message(errormsg("Received invalid status. Accepted: [run, stop, pause, resume, quit]")); }
			}
			else if (mode == "get-status")
			{
				auto data = json["data"];
				_server->push_message(statusupdate(_status));
			}
			else if (mode == "get-variables")
			{
				auto data = json["data"];
				_server->push_message(variablemsg(vm, vm->active_vmstack(), data));
			}
			else if (mode == "preprocess")
			{
				auto data = json["data"];
				auto res = data.find("path");
				if (res != data.end())
				{
					std::string path = *res;
					path = vm->get_filesystem().sanitize(path);
					auto phys = vm->get_filesystem().try_get_physical_path(path);
					if (phys.has_value())
					{
						auto filecontents = load_file(phys.value());
						bool errflag = false;
						auto ppres = sqf::parse::preprocessor::parse(vm, filecontents, errflag, path);
						if (errflag)
						{
							auto err = vm->err().str();
							vm->err_clear();
							_server->push_message(errormsg(err));
						}
						else
						{
							nlohmann::json json = {
								{ "mode", "message" },
							{ "data", ppres }
							};
							_server->push_message(json.dump());
						}
					}
					else
					{
						_server->push_message(errormsg("Path not found."));
					}
				}
				else
				{
					std::string content = data["content"];
					auto filecontents = load_file(content);
					bool errflag = false;
					auto ppres = sqf::parse::preprocessor::parse(vm, filecontents, errflag, "__debugger.sqf");
					if (errflag)
					{
						auto err = vm->err().str();
						vm->err_clear();
						_server->push_message(errormsg(err));
					}
					else
					{
						nlohmann::json json = {
							{ "mode", "message" },
						{ "data", ppres }
						};
						_server->push_message(json.dump());
					}
				}
			}
			else if (mode == "parse-sqf")
			{
				auto data = json["data"];
				std::string sqf = data["sqf"];
				std::string file = data["file"];
				vm->parse_sqf(sqf, file);
			}
			else if (mode == "load-sqf")
			{
				auto data = json["data"];
				std::string path = data["path"];
				std::string name = data["name"];
				auto sqf = load_file(path);
				vm->parse_sqf(sqf, name);
			}
			else if (mode == "set-breakpoint")
			{
				auto data = json["data"];
				if (!data.at("line").is_number())
				{
					_server->push_message(errormsg("'data.line' has to be of type number"));
				}
				else if (!data.at("file").is_string())
				{
					_server->push_message(errormsg("'data.file' has to be of type string"));
				}
				else
				{
					_breakpoints.emplace_back(data["line"], data["file"]);
				}
			}
			else
			{
				std::stringstream sstream;
				sstream << "Provided mode '" << mode << "' is not known to the debugger.";
				_server->push_message(errormsg(sstream.str()));
			}
		}
		catch (const nlohmann::json::parse_error& err)
		{
			_server->push_message(errormsg(err.what()));
		}
		catch (const std::exception& err)
		{
			_server->push_message(errormsg(err.what()));
		}
	}
}

void sqf::debugger::error(virtualmachine * vm, size_t line, size_t col, std::string file, std::string msg)
{
	_status = HALT;
	_server->push_message(errormsg(msg));
	breakmode(vm);
}

bool sqf::debugger::stop(virtualmachine * vm)
{
	if (!_server->is_accept() || _control == srvcontrol::QUIT)
		return false;
	_status = DONE;
	_server->push_message(statusupdate(_status));
	breakmode(vm);
	return true;
}
