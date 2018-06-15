#pragma once
#include <string>

class netserver;
namespace sqf {
	class virtualmachine;

	class debugger
	{
		netserver* _server;
		void breakmode(virtualmachine* vm);
	public:
		debugger(netserver* server) : _server(server) {}
		void check(virtualmachine* vm);
		void error(virtualmachine* vm, int line, int col, std::string file, std::string msg);
		bool stop(virtualmachine* vm);
	};
}