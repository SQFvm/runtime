#pragma once
#include <string>
#include <memory>
#include "data.h"
#include "varscope.h"
#include "sidedata.h"

namespace sqf
{
	class virtualmachine;
	class groupdata : public data, public varscope
	{
	private:
		std::shared_ptr<sidedata> mside;
		std::string mgroupid;
		groupdata(std::shared_ptr<sidedata> side) : mside(side) { }
		groupdata() { }
	public:

		operator std::shared_ptr<sidedata>() const { return mside; }
		operator sidedata::eside() const { return mside->side(); }
		std::shared_ptr<sidedata> side() const { return mside; }
		virtual bool equals(std::shared_ptr<data> d) const { return this == d.get(); }
		inline std::string groupid(void) { return mgroupid; }
		inline void groupid(std::string id) { mgroupid = id; }
		virtual std::string tosqf(void) const { return mgroupid; }

		static inline std::shared_ptr<sqf::groupdata> create() { return std::shared_ptr<groupdata>(); }
		static std::shared_ptr<sqf::groupdata> create(sqf::virtualmachine* vm, std::shared_ptr<sqf::sidedata> side);
	};
}