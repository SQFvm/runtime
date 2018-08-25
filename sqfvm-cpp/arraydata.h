#pragma once
#include <string>
#include <memory>
#include <vector>
#include <array>

#include "data.h"


namespace sqf
{
	class value;
	class virtualmachine;
	enum type;
	class arraydata : public data
	{
	private:
		std::vector<std::shared_ptr<value>> mvalue;
	protected:
		inline std::vector<std::shared_ptr<value>>& innervector(void) { return mvalue; }
	public:
		arraydata() : mvalue(std::vector<std::shared_ptr<value>>()) {}
		arraydata(size_t size) : mvalue(std::vector<std::shared_ptr<value>>(size)) {}
		arraydata(std::vector<std::shared_ptr<value>> v) : mvalue(std::vector<std::shared_ptr<value>>(v)) {}
		virtual std::string tosqf(void) const;
		std::shared_ptr<value>& operator[](int index) { return mvalue.at(index); }
		std::shared_ptr<value> operator[](int index) const { return index < 0 || index >= (int)mvalue.size() ? std::make_shared<value>() : mvalue[index]; }
		std::shared_ptr<value>& at(int index) { return mvalue.at(index); }
		const std::shared_ptr<value> at(int index) const { return mvalue.at(index); }
		inline size_t size(void) const { return mvalue.size(); }
		operator std::vector<std::shared_ptr<value>>(void) const { return mvalue; }
		virtual bool equals(std::shared_ptr<data> d) const;

		inline void push_back(std::shared_ptr<value> val) { mvalue.push_back(val); }
		inline std::shared_ptr<value> pop_back(void) { auto back = mvalue.back(); mvalue.pop_back(); return back; }
		void resize(int newsize);
		void reverse();
		void extend(std::vector<std::shared_ptr<value>> other);
		void delete_at(int position);
		std::array<double, 3> as_vector(void) const;
		operator std::array<double, 3>(void) const { return as_vector(); }

		bool check_type(virtualmachine*, type, size_t) const;
	};
}