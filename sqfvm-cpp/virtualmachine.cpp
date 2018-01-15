#include "full.h"
#include <iostream>

sqf::virtualmachine::virtualmachine(unsigned long long maxinst)
{
	mout = &std::wcout;
	mwrn = &std::wcerr;
	merr = &std::wcerr;
	minstcount = 0;
	mmaxinst = maxinst;
}

void sqf::virtualmachine::execute(void)
{
	instruction_s inst;
	while ((inst = mstack->popinst()).get())
	{
		minstcount++;
		if (mmaxinst != 0 && mmaxinst == minstcount)
		{
			err() << L"MAX INST COUNT REACHED (" << mmaxinst << L")" << std::endl;
			break;
		}
		inst->execute(this, mstack);
	}
}
