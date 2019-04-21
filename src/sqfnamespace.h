#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "data.h"
#include "varscope.h"

namespace sqf
{
	class sqfnamespace : public data, public varscope
	{
	private:
		std::string mname;
	public:
		sqfnamespace(std::string name) { mname = std::move(name); }
		std::string tosqf() const override { return mname; }
		std::string get_name() const { return mname; }
		bool equals(std::shared_ptr<data> d) const override { return this == d.get(); }
        sqf::type dtype() const override { return sqf::type::NAMESPACE; }
	};
    class sqfwith : public sqfnamespace {
    public:
        sqfwith(std::shared_ptr<sqfnamespace> o) : sqfnamespace(*o) {}
        sqf::type dtype() const override { return sqf::type::WITH; }
    };

}