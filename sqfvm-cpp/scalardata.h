#ifndef _SCALARDATA
#define _SCALARDATA 1

#if !defined(_STRING) & !defined(_STRING_)
#error scalardata requires <string> header
#endif // !_STRING
#if !defined(_MEMORY) & !defined(_MEMORY_)
#error scalardata requires <memory> header
#endif // !_STRING
#if !defined(_DATA)
#error scalardata requires "data.h" header
#endif // !_DATA

namespace sqf
{
	class scalardata : public data
	{
	private:
		double mvalue;
	public:
		scalardata(void);
		scalardata(char);
		scalardata(short);
		scalardata(int);
		scalardata(long);
		scalardata(float);
		scalardata(double);
		virtual std::wstring to_string(void) const;
		operator char(void) const;
		operator short(void) const;
		operator int(void) const;
		operator long(void) const;
		operator float(void) const;
		operator double(void) const;
	};
	typedef std::shared_ptr<scalardata> scalar_s;
	typedef std::weak_ptr<scalardata> scalar_w;
	typedef std::unique_ptr<scalardata> scalar_u;
}
#endif // !_SCALARDATA