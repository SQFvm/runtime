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
		groupdata(std::shared_ptr<sidedata> side) : mside(std::move(side)), misnull(false) { }
        groupdata() = default;
	public:

		operator std::shared_ptr<sidedata>() const { return mside; }
		operator sidedata::eside() const { return mside->side(); }
		std::shared_ptr<sidedata> side() const { return mside; }
		bool equals(std::shared_ptr<data> d) const override { return this == d.get(); }
        sqf::type dtype() const override { return sqf::type::GROUP; }
		std::string groupid() const { return mgroupid; }
		void groupid(std::string id) { mgroupid = id; }
		std::string tosqf() const override { return mgroupid; }

		const std::vector<std::shared_ptr<innerobj>>& get_units() const { return munits; }
		bool is_empty() const { return munits.size() == 0; }
		bool is_null() const { return misnull; }

		static std::shared_ptr<sqf::groupdata> create();
		static std::shared_ptr<sqf::groupdata> create(sqf::virtualmachine* vm, std::shared_ptr<sqf::sidedata> side);

		void add_unit(std::shared_ptr<innerobj> unit);
		void drop_unit(std::shared_ptr<innerobj> unit);
	};
}