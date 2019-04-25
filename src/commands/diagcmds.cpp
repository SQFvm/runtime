#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../debugger.h"
#include "../virtualmachine.h"

using namespace sqf;
namespace
{
	value diag_log_any(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_string();
		vm->out() << "[DIAG]\t" << r << std::endl;
		return {};
	}
	value diag_tickTime_(virtualmachine* vm)
	{
		auto curtime = sqf::virtualmachine::system_time().time_since_epoch();
		auto starttime = vm->get_created_timestamp().time_since_epoch();
		// Time is since beginning of game so long is fine.
		long r = static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(curtime - starttime).count());
		return (float)r * 0.001;
	}
	value assert_bool(virtualmachine* vm, value::cref right)
	{
		if (!right.as_bool())
		{
			vm->err() << "Assert failed." << std::endl;
			if (vm->dbg())
			{
				vm->halt();
			}
		}
		return right;
	}
	value halt_(virtualmachine* vm)
	{
		if (vm->dbg())
		{
			vm->halt();
		}
		else
		{
			vm->wrn() << "No debugger connected." << std::endl;
		}
		return {};
	}
}
void sqf::commandmap::initdiagcmdss()
{
	add(unary("diag_log", sqf::type::ANY, "Dumps the argument's value to the report file. Each call creates a new line in the file.", diag_log_any));
	add(nular("diag_tickTime", "In SQF-VM: returns current systemtime in ms. In ArmA: Real time spent from the start of the game. Expressed in fractions of second. Resolution of 1 tick is 1 ms.", diag_tickTime_));
	add(unary("assert", type::BOOL, "Tests a condition and if the condition is false, displays error on screen.", assert_bool));
	add(nular("halt", "Halts the execution if a debugger is attached. If not, warning is logged and execution continues.", halt_));

}
