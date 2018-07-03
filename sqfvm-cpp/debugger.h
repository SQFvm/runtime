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
			inline int line(void) { return _line; }
			inline std::string file(void) { return _file; }
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
		void breakmode(virtualmachine* vm);
		srvstatus _status;
		srvcontrol _control;

	public:
		debugger(netserver* server) : _server(server), _status(NA), _control(RUN) {}
		void check(virtualmachine* vm);
		void error(virtualmachine* vm, int line, int col, std::string file, std::string msg);
		bool stop(virtualmachine* vm);
		inline srvstatus status(void) { return _status; }
		inline srvcontrol controlstatus(void) { return _control; }
		inline void status(srvstatus status) { _status = status; }
	};
}