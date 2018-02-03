#ifndef _DATA
#define _DATA 1

#if !defined(_STRING) & !defined(_STRING_)
#error data requires <string> header
#endif // !_STRING
#if !defined(_MEMORY) & !defined(_MEMORY_)
#error data requires <memory> header
#endif // !_STRING

namespace sqf
{
	class data
	{
	public:
		virtual std::wstring to_string(void) const = 0;
		virtual bool equals(std::shared_ptr<data>) const = 0;
	};
	typedef std::shared_ptr<data> data_s;
	typedef std::weak_ptr<data> data_w;
	typedef std::unique_ptr<data> data_u;
}
#endif // !_DATA