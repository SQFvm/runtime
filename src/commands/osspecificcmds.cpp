#ifndef NO_COMMANDS
#ifdef _WIN32
// Required due to some headers using WinSock2.h
// & some headers requiring windows.h
// If this was not here, a link conflict would emerge due to
// windows.h including winsock1
#include <Windows.h>
#endif


#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../virtualmachine.h"

namespace err = logmessage::runtime;
using namespace sqf;
namespace
{
	value copytoclipboard_string(virtualmachine* vm, value::cref right)
	{
#if defined(_WIN32) & !defined(DISABLE_CLIPBOARD)
		auto data = (std::string)right;
		if (!OpenClipboard(NULL))
		{
			vm->logmsg(err::FailedToCopyToClipboard(*vm->current_instruction()));
			return {};
		}
		EmptyClipboard();
		HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, data.length() + 1);
		if (hClipboardData == NULL)
		{
			vm->logmsg(err::FailedToCopyToClipboard(*vm->current_instruction()));
			return {};
		}
		char* pchData = (char*)GlobalLock(hClipboardData);
		strcpy(pchData, data.c_str());
		GlobalUnlock(hClipboardData);
		SetClipboardData(CF_TEXT, hClipboardData);
		CloseClipboard();

#else
		vm->logmsg(err::ClipboardDisabled(*vm->current_instruction()));
#endif
		return {};

	}
}
void sqf::commandmap::initosspecificcmds()
{
	add(unary("copyToClipboard", type::STRING, "Copy text string to the clipboard.", copytoclipboard_string)); /* STRING */
}

#endif