#include "commandmap.h"
#include "value.h"
#include "cmd.h"
#include "virtualmachine.h"
#include "objectdata.h"
#include <algorithm>

using namespace sqf;
namespace
{
	std::shared_ptr<value> objnull_(virtualmachine* vm)
	{
		return std::make_shared<value>(std::make_shared<objectdata>());
	}
}
void sqf::commandmap::initobjectcmds(void)
{
	add(nular(L"objNull", L"A non-existent Object. To compare non-existent objects use isNull or isEqualTo.", objnull_));
}
