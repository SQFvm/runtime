#include "commandmap.h"
#include "value.h"
#include "cmd.h"
#include "virtualmachine.h"
#include "objectdata.h"
#include "innerobj.h"
#include "varscope.h"
#include <algorithm>

using namespace sqf;
namespace
{
	std::shared_ptr<value> objnull_(virtualmachine* vm)
	{
		return std::make_shared<value>(std::make_shared<objectdata>(), sqf::type::OBJECT);
	}
	std::shared_ptr<value> typeof_object(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto obj = right->data<objectdata>();
		if (obj->isnull())
		{
			return std::make_shared<value>(L"");
		}
		else
		{
			return std::make_shared<value>(obj->obj()->classname());
		}
	}
}
void sqf::commandmap::initobjectcmds(void)
{
	//GetVariable & SetVariable & AllVariables are in namespacecmds as simple alias.
	add(nular(L"objNull", L"A non-existent Object. To compare non-existent objects use isNull or isEqualTo.", objnull_));
	add(unary(L"typeOf", sqf::type::OBJECT, L"Returns the config class name of given object.", typeof_object));
}
