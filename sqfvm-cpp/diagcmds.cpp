#include "full.h"
#include <ctime>
#include <sys/timeb.h>

using namespace sqf;
namespace
{
	int64_t system_time_ms(void)
	{
#ifdef _WIN32
		struct _timeb timebuffer;
		_ftime(&timebuffer);
		return (int64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
#else
		struct timeb timebuffer;
		ftime(&timebuffer);
		return (int64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
#endif
	}
	value_s diag_log_any(const virtualmachine* vm, value_s right)
	{
		auto r = right->as_string();
		vm->out() << r << std::endl;
		return std::make_shared<value>();
	}
	value_s diag_tickTime_(const virtualmachine* vm)
	{
		auto r = (long)system_time_ms();
		return std::make_shared<value>(r);
	}
}
void sqf::commandmap::initdiagcmdss(void)
{
	add(unary(L"diag_log", sqf::type::ANY, L"Dumps the argument's value to the report file. Each call creates a new line in the file.", diag_log_any));
	add(nular(L"diag_tickTime", L"In SQF-VM: returns current systemtime in ms. In ArmA: Real time spent from the start of the game. Expressed in fractions of second. Resolution of 1 tick is 1 ms.", diag_tickTime_));
}
