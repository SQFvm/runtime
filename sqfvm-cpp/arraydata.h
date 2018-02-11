#ifndef _ARRAYDATA
#define _ARRAYDATA 1

#if !defined(_STRING) & !defined(_STRING_)
#error arraydata requires <string> header
#endif // !_STRING
#if !defined(_MEMORY) & !defined(_MEMORY_)
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
	protected:
		inline std::vector<value_s>& innervector(void) { return mvalue; }
	public:
		arraydata() : mvalue(std::vector<value_s>()) {}
		arraydata(size_t size) : mvalue(std::vector<value_s>(size)) {}
		arraydata(std::vector<value_s> v) : mvalue(std::vector<value_s>(v)) {}
		virtual std::wstring tosqf(void) const;
		value_s& operator[](int index) { return mvalue.at(index); }
		value_s operator[](int index) const { return index < 0 || index >= (int)mvalue.size() ? std::make_shared<value>() : mvalue[index]; }
		inline size_t size(void) { return mvalue.size(); }
		operator std::vector<value_s>(void) const { return mvalue; }
		virtual bool equals(std::shared_ptr<data> d) const { return mvalue == std::dynamic_pointer_cast<arraydata>(d)->mvalue; }

		inline void push_back(value_s val) { mvalue.push_back(val); }
		inline value_s pop_back(void) { auto back = mvalue.back(); mvalue.pop_back(); return back; }
	};
	typedef std::shared_ptr<arraydata> array_s;
	typedef std::weak_ptr<arraydata> array_w;
	typedef std::unique_ptr<arraydata> array_u;
}
#endif // !_ARRAYDATA