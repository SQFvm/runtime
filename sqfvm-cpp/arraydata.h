#ifndef _ARRAYDATA
#define _ARRAYDATA 1

#if !defined(_STRING) & !defined(_STRING_)
#error arraydata requires <string> header
#endif // !_STRING
#if !defined(_MEMORY_) & !defined(_MEMORY_)
#error arraydata requires <memory> header
#endif // !_MEMORY_
#if !defined(_VECTOR) & !defined(_VECTOR_)
#error arraydata requires <vector> header
#endif // !_VECTOR
#if !defined(_DATA)
#error arraydata requires "data.h" header
#endif // !_DATA

namespace sqf
{
	class arraydata : public data
	{
	private:
		std::vector<value_s> mvalue;
	public:
		arraydata();
		arraydata(std::vector<value_s>);
		virtual std::wstring to_string(void) const;
		value_s& operator[](int);
		value_s operator[](int) const;
		operator std::vector<value_s>(void) const;
	};
	typedef std::shared_ptr<arraydata> array_s;
	typedef std::weak_ptr<arraydata> array_w;
	typedef std::unique_ptr<arraydata> array_u;
}
#endif // !_ARRAYDATA