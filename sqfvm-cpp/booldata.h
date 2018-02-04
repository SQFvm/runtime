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
		booldata() : mflag(false) {}
		booldata(bool flag) : mflag(flag) {}
		virtual std::wstring tosqf(void) const { return std::wstring(mflag ? L"true" : L"false"); }
		operator bool() const { return mflag; }
		virtual bool equals(std::shared_ptr<data> d) const { return mflag == std::dynamic_pointer_cast<booldata>(d)->mflag; }
	};
	typedef std::shared_ptr<booldata> bool_s;
	typedef std::weak_ptr<booldata> bool_w;
	typedef std::unique_ptr<booldata> bool_u;
}
#endif // !_BOOLDATA