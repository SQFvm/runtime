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

using namespace sqf;
namespace
{
	value copytoclipboard_string(virtualmachine* vm, value::cref right)
	{
#if defined(_WIN32) & !defined(DISABLE_CLIPBOARD)
		auto data = (std::string)right;
		if (!OpenClipboard(NULL))
		{
			vm->wrn() << "Failed to access clipboard." << std::endl;
			return {};
		}
		EmptyClipboard();
		HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, data.length() + 1);
		if (hClipboardData == NULL)
		{
			vm->wrn() << "Failed to allocate clipboard." << std::endl;
			return {};
		}
		char* pchData = (char*)GlobalLock(hClipboardData);
		strcpy(pchData, data.c_str());
		GlobalUnlock(hClipboardData);
		SetClipboardData(CF_TEXT, hClipboardData);
		CloseClipboard();

#else
		vm->wrn() << "Clipboard-Access is not available." << std::endl;
#endif
		return {};

	}
}
void sqf::commandmap::initosspecificcmds()
{
	add(unary("copyToClipboard", type::STRING, "Copy text string to the clipboard.", copytoclipboard_string)); /* STRING */
}
