#pragma once
#include <string>
#include <memory>
#include <vector>

#include "data.h"
#include "vmstack.h"
namespace sqf
{
	class instruction;
	class callstack;
	class virtualmachine;
	class codedata : public data
	{
	private:
		std::vector<std::shared_ptr<sqf::instruction>> minsts;
	public:
		codedata(std::shared_ptr<sqf::callstack>);
		codedata(std::vector<std::shared_ptr<sqf::instruction>> v) : minsts(v) {}
		virtual std::string tosqf() const;
		void add(std::shared_ptr<sqf::instruction> inst) { minsts.push_back(inst); }
		void loadinto(sqf::virtualmachine*, std::shared_ptr<sqf::vmstack>);
		void loadinto(std::shared_ptr<sqf::vmstack>, std::shared_ptr<sqf::callstack>);
		virtual bool equals(std::shared_ptr<data> d) const { return minsts == std::dynamic_pointer_cast<codedata>(d)->minsts; }
	};
}