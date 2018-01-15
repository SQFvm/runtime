#ifndef _BOOLDATA
#define _BOOLDATA 1

#if !defined(_STRING) & !defined(_STRING_)
#error booldata requires <string> header
#endif // !_STRING
#if !defined(_MEMORY) & !defined(_MEMORY_)
#error booldata requires <memory> header
#endif // !_STRING
#if !defined(_DATA)
#error booldata requires "data.h" header
#endif // !_DATA

namespace sqf
{
	class booldata : public data
	{
	private:
		bool mflag;
	public:
		booldata();
		booldata(bool);
		virtual std::wstring to_string(void) const;
		operator bool() const;
	};
	typedef std::shared_ptr<booldata> bool_s;
	typedef std::weak_ptr<booldata> bool_w;
	typedef std::unique_ptr<booldata> bool_u;
}
#endif // !_BOOLDATA