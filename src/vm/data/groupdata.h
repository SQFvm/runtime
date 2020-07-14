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
		std::vector<std::shared_ptr<innerobj>> m_units;
		std::shared_ptr<sidedata> m_side;
		std::string m_groupid;
		bool m_isnull;
		std::shared_ptr<innerobj> m_leader;
		groupdata(std::shared_ptr<sidedata> side) : m_side(std::move(side)), m_isnull(false) { }
        groupdata() = default;
	public:

		operator std::shared_ptr<sidedata>() const { return m_side; }
		operator sidedata::eside() const { return m_side->side(); }

		std::shared_ptr<sidedata> side() const { return m_side; }
		bool equals(std::shared_ptr<data> d) const override { return this == d.get(); }
        sqf::type dtype() const override { return sqf::type::GROUP; }
		std::string groupid() const { return m_groupid; }
		void groupid(std::string id) { m_groupid = id; }
		std::string tosqf() const override { return m_groupid; }

		const std::vector<std::shared_ptr<innerobj>>& get_units() const { return m_units; }
		bool is_empty() const { return m_units.size() == 0; }
		bool is_null() const { return m_isnull; }

		static std::shared_ptr<sqf::groupdata> create();
		static std::shared_ptr<sqf::groupdata> create(sqf::virtualmachine* vm, std::shared_ptr<sqf::sidedata> side);

		void add_unit(std::shared_ptr<innerobj> unit);
		void drop_unit(std::shared_ptr<innerobj> unit);

		std::shared_ptr<innerobj> leader() { return m_leader; }
		void leader(std::shared_ptr<innerobj> leader) { m_leader = leader; }
	};
}