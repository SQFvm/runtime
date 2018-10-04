#pragma once
#include <string>
#include <vector>

class netserver;
namespace sqf {
	class virtualmachine;

	class debugger
	{
	public:
		class breakpoint
		{
			int _line;
			std::string _file;
		public:
			breakpoint(int line, std::string file) : _line(line), _file(file) {}
			int line() { return _line; }
			std::string file() { return _file; }
		};
		enum srvstatus {
			NA,
			HALT,
			RUNNING,
			DONE
		};
		enum srvcontrol {
			QUIT,
			RESUME,
			PAUSE,
			STOP,
			RUN
		};
	private:
		netserver* _server;
		std::vector<breakpoint> _breakpoints;
		srvstatus _status;
		srvcontrol _control;

	public:
		debugger(netserver* server) : _server(server), _status(NA), _control(RUN) {}
		void breakmode(virtualmachine* vm);
		void check(virtualmachine* vm);
		bool hitbreakpoint(size_t line, std::string file);
		void error(virtualmachine* vm, int line, int col, std::string file, std::string msg);
		bool stop(virtualmachine* vm);
		void position(size_t line, size_t col, std::string file);
		void message(std::string);
		srvstatus status() { return _status; }
		srvcontrol controlstatus() { return _control; }
		void status(srvstatus status) { _status = status; }
	};
}