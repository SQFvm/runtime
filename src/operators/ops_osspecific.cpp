#include "ops_osspecific.h"
#include "../runtime/runtime.h"
#include "../runtime/value.h"
#include "../runtime/d_string.h"

#ifdef _WIN32
// Required due to some headers using WinSock2.h
// & some headers requiring windows.h
// If this was not here, a link conflict would emerge due to
// windows.h including winsock1
#include <Windows.h>
#endif

namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;
namespace
{
    sqf::runtime::value copytoclipboard_string(sqf::runtime::runtime& runtime, sqf::runtime::value::cref right)
    {
#if defined(_WIN32) & !defined(DISABLE_CLIPBOARD)
        auto data = right.data<d_string, std::string>();
        if (!OpenClipboard(NULL))
        {
            //vm->wrn() << "Failed to access clipboard." << std::endl;
            runtime.__logmsg(err::FailedToCopyToClipboard(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        EmptyClipboard();
        HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, data.length() + 1);
        if (hClipboardData == NULL)
        {
            //vm->wrn() << "Failed to allocate clipboard." << std::endl;
            runtime.__logmsg(err::FailedToCopyToClipboard(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        char* pchData = (char*)GlobalLock(hClipboardData);
        strcpy(pchData, data.c_str());
        GlobalUnlock(hClipboardData);
        SetClipboardData(CF_TEXT, hClipboardData);
        CloseClipboard();

#else
        runtime.__logmsg(err::ClipboardDisabled(runtime.context_active().current_frame().diag_info_from_position()));
#endif
        return {};

    }
}
void sqf::operators::ops_osspecific(sqf::runtime::runtime& runtime)
{
    using namespace sqf::runtime::sqfop;
    runtime.register_sqfop(unary("copyToClipboard", t_string(), "Copy text string to the clipboard.", copytoclipboard_string)); /* STRING */
}