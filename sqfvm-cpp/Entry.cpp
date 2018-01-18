#include "full.h"

int main(int argc, char** argv)
{
	sqf::commandmap::get().init();
	auto vm = sqf::virtualmachine();

	vm.parse_assembly(L"callnullar test; callnullar test;");
	vm.execute();

	sqf::commandmap::get().uninit();
	while (1);
}