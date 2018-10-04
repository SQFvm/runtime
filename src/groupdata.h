#pragma once
#include <string>
#include <memory>
#include <vector>
#include "data.h"
#include "varscope.h"
#include "sidedata.h"

namespace sqf
{
	class virtualmachine;
	class innerobj;
	class groupdata : public data, public varscope, public std::enable_shared_from_this<groupdata>
	{
	private:
		std::vector<std::shared_ptr<innerobj>> munits;
		std::shared_ptr<sidedata> mside;
		std::string mgroupid;
		bool misnull;
		groupdata(std::shared_ptr<sidedata> side) : mside(side), misnull(false) { }
		groupdata() { }
	public:

		operator std::shared_ptr<sidedata>() const { return mside; }
		operator sidedata::eside() const { return mside->side(); }
		std::shared_ptr<sidedata> side() const { return mside; }
		virtual bool equals(std::shared_ptr<data> d) const { return this == d.get(); }
		inline std::string groupid() { return mgroupid; }
		inline void groupid(std::string id) { mgroupid = id; }
		virtual std::string tosqf() const { return mgroupid; }

		const std::vector<std::shared_ptr<innerobj>>& get_units() { return munits; }
		inline bool is_empty() const { return munits.size() == 0; }
		inline bool is_null() const { return misnull; }

		static std::shared_ptr<sqf::groupdata> create();
		static std::shared_ptr<sqf::groupdata> create(sqf::virtualmachine* vm, std::shared_ptr<sqf::sidedata> side);

		void add_unit(std::shared_ptr<innerobj> unit);
		void drop_unit(std::shared_ptr<innerobj> unit);
	};
}