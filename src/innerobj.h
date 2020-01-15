#pragma once
#include "varscope.h"
#include <string>
#include <memory>
#include <array>
#include <vector>
#include <algorithm>

namespace sqf
{
	class virtualmachine;
	class groupdata;
	class value;
	class objectdata;
	class innerobj : public std::enable_shared_from_this<innerobj>, public varscope
	{
	private:
		size_t m_netid;
		double m_position_x;
		double m_position_y;
		double m_position_z;
		double m_velocity_x;
		double m_velocity_y;
		double m_velocity_z;
		double m_damage;
		std::string m_classname;
		std::shared_ptr<groupdata> m_group;
		bool m_is_vehicle;

		bool m_has_driver;
		bool m_has_gunner;
		bool m_has_commander;
		size_t mtransportSoldier;

		std::shared_ptr<objectdata> m_parent_object;
		std::shared_ptr<objectdata> m_driver;
		std::shared_ptr<objectdata> m_gunner;
		std::shared_ptr<objectdata> m_commander;
		std::vector<std::shared_ptr<sqf::objectdata>> m_soldiers;

		innerobj(std::string classname, bool isvehicle);
	public:
		virtual std::string tosqf() const;

		std::array<double, 3> pos() const { return { m_position_x, m_position_y, m_position_z }; }
		double posx() const { return m_position_x; }
		double posy() const { return m_position_y; }
		double posz() const { return m_position_z; }
		std::array<double, 3> vel() const { return { m_velocity_x, m_velocity_y, m_velocity_z }; }
		double velx() const { return m_velocity_x; }
		double vely() const { return m_velocity_y; }
		double velz() const { return m_velocity_z; }
		std::string classname() const { return m_classname; }
		size_t netid() const { return m_netid; }
		bool is_vehicle() const { return m_is_vehicle; }
		std::shared_ptr<groupdata> group() const { return m_group; }

		double distance3dsqr(std::shared_ptr<innerobj> obj) const { return distance3dsqr(obj->pos()); }
		double distance3dsqr(const innerobj* obj) const { return distance3dsqr(obj->pos()); }
		double distance3dsqr(std::array<double, 3> otherpos) const;
		double distance3d(std::shared_ptr<innerobj> obj) const { return distance3d(obj->pos()); }
		double distance3d(const innerobj* obj) const { return distance3d(obj->pos()); }
		double distance3d(std::array<double, 3> otherpos) const;

		double distance2dsqr(std::shared_ptr<innerobj> obj) const { return distance2dsqr(std::array<double, 2> { obj->m_position_x, obj->m_position_y }); }
		double distance2dsqr(const innerobj* obj) const { return distance2dsqr(std::array<double, 2> { obj->m_position_x, obj->m_position_y }); }
		double distance2dsqr(std::array<double, 2> otherpos) const;
		double distance2d(std::shared_ptr<innerobj> obj) const { return distance2d(std::array<double, 2> { obj->m_position_x, obj->m_position_y }); }
		double distance2d(const innerobj* obj) const { return distance2d(std::array<double, 2> { obj->m_position_x, obj->m_position_y }); }
		double distance2d(std::array<double, 2> otherpos) const;

		double damage() const { return m_damage; }
		void damage(double val) { m_damage = val < 0 ? 0 : val > 1 ? 1 : val; }
		void damage_by(double val) { auto newdmg = m_damage += val; damage(newdmg); }
		bool alive() { return m_damage < 1; }

		void pos(std::array<double, 3> arr) { m_position_x = arr[0]; m_position_y = arr[1]; m_position_z = arr[2]; }
		void posx(double d) { m_position_x = d; }
		void posy(double d) { m_position_y = d; }
		void posz(double d) { m_position_z = d; }
		void vel(std::array<double, 3> arr) { m_velocity_x = arr[0]; m_velocity_y = arr[1]; m_velocity_z = arr[2]; }
		void velx(double d) { m_velocity_x = d; }
		void vely(double d) { m_velocity_y = d; }
		void velz(double d) { m_velocity_z = d; }
		void group(std::shared_ptr<groupdata> g) { m_group = g; }

		std::shared_ptr<sqf::objectdata> parent_object() const { return m_parent_object; }

		std::shared_ptr<sqf::objectdata> driver() const { return m_driver; }
		std::shared_ptr<sqf::objectdata> gunner() const { return m_gunner; }
		std::shared_ptr<sqf::objectdata> commander() const { return m_commander; }
		std::vector<std::shared_ptr<sqf::objectdata>>::iterator soldiers_begin() { return m_soldiers.begin(); }
		std::vector<std::shared_ptr<sqf::objectdata>>::iterator soldiers_end() { return m_soldiers.end(); }
		const std::vector<std::shared_ptr<sqf::objectdata>>& soldiers() const { return m_soldiers; }

		void driver(std::shared_ptr<sqf::objectdata> val);
		void gunner(std::shared_ptr<sqf::objectdata> val);
		void commander(std::shared_ptr<sqf::objectdata> val);
		void clear_driver();
		void clear_gunner();
		void clear_commander();
		// Attempts to add provided value to the soldiers list.
		// Will return true if it succeeded, false if the value provided
		// was null, not an object, not a unit, already inside the vehicle or no slots
		// are left to occupy.
		bool soldiers_push_back(sqf::value val);
		// Attempts to add provided objectdata to the soldiers list.
		// Will return true if it succeeded, false if the value provided
		// was null, not a unit, already inside the vehicle or no slots are left to occupy.
		bool soldiers_push_back(std::shared_ptr<sqf::objectdata> val);
		void soldiers_pop_back();
		void soldiers_erase(std::shared_ptr<sqf::objectdata> obj);

		bool iskindof(std::string_view cfgname) const;

		void destroy(sqf::virtualmachine*);

		// Attempts to read the vehicle config entry for this
		// and sets some local variables according to the config.
		// Returns TRUE on success. FALSE will be returned, if
		// the config class for the vehicle could not be received.
		bool update_values_from_configbin();

		static std::shared_ptr<sqf::innerobj> create(sqf::virtualmachine* vm, std::string classname, bool isvehicle);
	};
}