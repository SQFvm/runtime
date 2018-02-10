#ifndef _FORDATA
#define _FORDATA 1

#if !defined(_STRING) & !defined(_STRING_)
#error fordata requires <string> header
#endif // !_STRING
#if !defined(_MEMORY) & !defined(_MEMORY_)
#error fordata requires <memory> header
#endif // !_STRING
#if !defined(_DATA)
#error fordata requires "data.h" header
#endif // !_DATA

namespace sqf
{
	class fordata : public data
	{
	private:
		std::wstring mvar;
		double mfrom;
		double mto;
		double mstep;
	public:
		fordata(std::wstring var) : mvar(var), mfrom(0), mto(0), mstep(1) {}
		virtual std::wstring tosqf(void) const { return L"ForType <invisible>"; }
		std::wstring variable(void) const { return mvar; }
		double from(void) const { return mfrom; }
		double to(void) const { return mto; }
		double step(void) const { return mstep; }
		void from(double d) { mfrom = d; }
		void to(double d) { mto = d; }
		void step(double d) { mstep = d; }
		virtual bool equals(std::shared_ptr<data> d) const { return false; }
	};
}
#endif // !_FORDATA