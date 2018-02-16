#include "sidedata.h"

static bool friendlymap[10][10] = {
	{ false, false, false, false, false, false, false, false, true , true  }, //empty
	{ false, false, false, false, false, false, false, false, true , true  }, //unknown
	{ true , false, true , true , false, true , false, false, true , true  }, //civ
	{ true , false, true , true , false, true , false, false, true , true  }, //west
	{ false, false, true , false, true , false, false, false, true , true  }, //east
	{ false, false, true , true , false, true , false, false, true , true  }, //guer
	{ false, false, true , true , true , true , false, false, true , true  }, //logic
	{ false, false, false, false, false, false, false, false, true , true  }, //friendly
	{ true , true , true , true , true , true , true , true , true , true  }, //ambient
};

bool sqf::sidedata::is_friendly_to(eside self, eside other)
{
	return friendlymap[self][other];
}

void sqf::sidedata::set_friendly_to(eside self, eside other, bool flag)
{
	friendlymap[self][other] = flag;
}
