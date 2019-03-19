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
		codedata(std::vector<std::shared_ptr<sqf::instruction>> v) : minsts(std::move(v)) {}
		std::string tosqf() const override;
		void add(std::shared_ptr<sqf::instruction> inst) { minsts.push_back(inst); }
		void loadinto(sqf::virtualmachine*, std::shared_ptr<sqf::vmstack>);
		void loadinto(std::shared_ptr<sqf::vmstack>, std::shared_ptr<sqf::callstack>);
		bool equals(std::shared_ptr<data> d) const override { return minsts == std::dynamic_pointer_cast<codedata>(d)->minsts; }

		std::vector<std::shared_ptr<sqf::instruction>>::iterator instructions_begin() { return minsts.begin(); }
		std::vector<std::shared_ptr<sqf::instruction>>::iterator instructions_end() { return minsts.end(); }
		std::vector<std::shared_ptr<sqf::instruction>>::reverse_iterator instructions_rbegin() { return minsts.rbegin(); }
		std::vector<std::shared_ptr<sqf::instruction>>::reverse_iterator instructions_rend() { return minsts.rend(); }
	};
}