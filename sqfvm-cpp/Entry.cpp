#include "full.h"

int main(int argc, char** argv)
{
	sqf::commandmap::get().init();
	auto vm = sqf::virtualmachine();

	

	sqf::commandmap::get().uninit();
}