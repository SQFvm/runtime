#include "objectdata.h"
#include "innerobj.h"

std::wstring sqf::objectdata::tosqf(void) const { if (mobj.expired()) return std::wstring(L"null"); auto sptr = mobj.lock(); return sptr->tosqf(); }
