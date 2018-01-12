#ifndef _CONVERT
#define _CONVERT 1

#if !defined(_DATA)
#error convert requires "data.h" header
#endif // !_DATA

namespace sqf
{
	data_s convert(data_s val, type type);
}
#endif // !_CONVERT