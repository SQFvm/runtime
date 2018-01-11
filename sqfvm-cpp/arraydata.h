#ifndef _ARRAYDATA
#define _ARRAYDATA 1

#if !defined(_STRING) & !defined(_STRING_)
#error booldata requires <string> header
#endif // !_STRING
#if !defined(_MEMORY_) & !defined(_MEMORY_)
#error booldata requires <memory> header
#endif // !_STRING
#if !defined(_DATA)
#error booldata requires "data.h" header
#endif // !_DATA

namespace sqf
{
	class arraydata : public data
	{
	private:
		std::vector<value_s> mvalue;
	public:
		arraydata();
		template<size_t SIZE> arraydata(std::array<value_s, SIZE>);
		virtual std::wstring to_string(void) const;
		template<size_t SIZE> operator std::array<value_s, SIZE>() const;
		value_s& operator[](int);
		value_s operator[](int) const;
	};
	typedef std::shared_ptr<arraydata> array_s;
	typedef std::weak_ptr<arraydata> array_w;
	typedef std::unique_ptr<arraydata> array_u;
}
#endif // !_ARRAYDATA