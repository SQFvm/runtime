#include "objectdata.h"
#include "innerobj.h"

std::string sqf::objectdata::tosqf() const { if (mobj.expired()) return std::string("nul"); auto sptr = mobj.lock(); return sptr->tosqf(); }
