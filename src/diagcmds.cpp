#include "commandmap.h"
#include "value.h"
#include "cmd.h"
#include "virtualmachine.h"

using namespace sqf;
namespace
{
	std::shared_ptr<value> diag_log_any(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_string();
		vm->out() << "[DIAG]\t" << r << std::endl;
		return std::make_shared<value>();
	}
	std::shared_ptr<value> diag_tickTime_(virtualmachine* vm)
	{
		long r = std::chrono::duration_cast<std::chrono::milliseconds>(virtualmachine::system_time().time_since_epoch()).count();
		return std::make_shared<value>(r);
	}
}
void sqf::commandmap::initdiagcmdss()
{
	add(unary("diag_log", sqf::type::ANY, "Dumps the argument's value to the report file. Each call creates a new line in the file.", diag_log_any));
	add(nular("diag_tickTime", "In SQF-VM: returns current systemtime in ms. In ArmA: Real time spent from the start of the game. Expressed in fractions of second. Resolution of 1 tick is 1 ms.", diag_tickTime_));
}
