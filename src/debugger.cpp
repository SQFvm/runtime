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
		simplemessage(std::string msg) : _msg(msg) {}
		std::string serialize() {
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
		errormsg(std::string msg) : _msg(msg) {}
		std::string serialize() {
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
		std::string serialize() {
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
		callstackmsg(std::shared_ptr<sqf::vmstack> stack) : _stack(stack) {}
		std::string serialize() {
			nlohmann::json data;
			int lvl = 0;
			for (auto it = _stack->stacks_begin(); it != _stack->stacks_end(); it++)
			{
				nlohmann::json jcs;
				auto cs = *it;
				jcs["lvl"] = lvl--;
				jcs["scopename"] = cs->getscopename();
				jcs["namespace"] = cs->getnamespace()->get_name();
				nlohmann::json options;
				if (cs->inststacksize() <= 0)
				{
					options["line"] = 0;
					options["column"] = 0;
					options["file"] = "";
					options["available"] = false;
				}
				else
				{
					options["line"] = cs->peekinst()->line();
					options["column"] = cs->peekinst()->col();
					options["file"] = cs->peekinst()->file();
					options["available"] = true;
				}
				jcs["options"] = options;
				auto variables = nlohmann::json::array();
				for (auto pair : cs->varmap())
				{
					variables.push_back(nlohmann::json{ { "name", pair.first },{ "value", pair.second->as_string() } });
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
		variablemsg(sqf::virtualmachine * vm, std::shared_ptr<sqf::vmstack> stack, nlohmann::json data) : _stack(stack), _data(data), _vm(vm) {}
		std::string serialize() {
			auto data = nlohmann::json::array();
			for (auto it = _data.begin(); it != _data.end(); it++)
			{
				std::string name = it->at("name");
				auto scope = it->at("scope");
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
						for (s = _stack->stacks_begin(); s != _stack->stacks_end() && numscope != 0; s++, numscope++);
						if (s == _stack->stacks_end())
						{
							data.push_back(nlohmann::json{ { "name", name },{ "value", "nil" } });
						}
						else
						{
							data.push_back(nlohmann::json{ { "name", name },{ "value", (*s)->getvar(name)->as_string() } });
						}
					}
				}
				else if (scope.get<std::string>().compare("missionNamespace"))
				{
					auto ns = _vm->missionnamespace();
					data.push_back(nlohmann::json{ { "name", name },{ "value", ns->getvar(name)->as_string() } });
				}
				else if (scope.get<std::string>().compare("uiNamespace"))
				{
					auto ns = _vm->uinamespace();
					data.push_back(nlohmann::json{ { "name", name },{ "value", ns->getvar(name)->as_string() } });
				}
				else if (scope.get<std::string>().compare("profileNamespace"))
				{
					auto ns = _vm->profilenamespace();
					data.push_back(nlohmann::json{ { "name", name },{ "value", ns->getvar(name)->as_string() } });
				}
				else if (scope.get<std::string>().compare("parsingNamespace"))
				{
					auto ns = _vm->parsingnamespace();
					data.push_back(nlohmann::json{ { "name", name },{ "value", ns->getvar(name)->as_string() } });
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
		positionmsg(size_t line, size_t col, std::string file) : _line(line), _col(col), _file(file) {}
		std::string serialize() {
			nlohmann::json json = { { "mode", "position" }, { "data", { { "line", _line }, { "col", _col }, { "file", _file } } } };
			return json.dump();
		}
	};
}

void sqf::debugger::position(size_t line, size_t col, std::string file)
{
	_server->push_message(positionmsg(line, col, file));
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
		if (bp.line() == line && bp.file().compare(file) == 0)
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
			if (!mode.compare("get-callstack"))
			{
				_server->push_message(callstackmsg(vm->stack()));
			}
			else if (!mode.compare("control"))
			{
				std::string status = json["data"]["status"];
				if (!status.compare("run")) { _control = RUN; }
				else if (!status.compare("stop")) { _control = STOP; }
				else if (!status.compare("pause")) { if (_control != PAUSE) { breakmode(vm); } }
				else if (!status.compare("resume")) { _control = RESUME; }
				else if (!status.compare("quit")) { _control = QUIT; }
				else { _server->push_message(errormsg("Received invalid status. Accepted: [run, stop, pause, resume, quit]")); }
			}
			else if (!mode.compare("get-status"))
			{
				auto data = json["data"];
				_server->push_message(statusupdate(_status));
			}
			else if (!mode.compare("get-variables"))
			{
				auto data = json["data"];
				_server->push_message(variablemsg(vm, vm->stack(), data));
			}
			else if (!mode.compare("parse-sqf"))
			{
				auto data = json["data"];
				std::string sqf = data["sqf"];
				std::string file = data["file"];
				vm->parse_sqf(sqf, file);
			}
			else if (!mode.compare("load-sqf"))
			{
				auto data = json["data"];
				std::string path = data["path"];
				std::string name = data["name"];
				auto sqf = load_file(path);
				vm->parse_sqf(sqf, name);
			}
			else if (!mode.compare("set-breakpoint"))
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
					_breakpoints.push_back(breakpoint(data["line"], data["file"]));
				}
			}
			else
			{
				std::stringstream sstream;
				sstream << "Provided mode '" << mode << "' is not known to the debugger.";
				_server->push_message(errormsg(sstream.str()));
			}
		}
		catch (nlohmann::json::parse_error err)
		{
			_server->push_message(errormsg(err.what()));
		}
		catch (std::exception err)
		{
			_server->push_message(errormsg(err.what()));
		}
	}
}

void sqf::debugger::error(virtualmachine * vm, int line, int col, std::string file, std::string msg)
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
