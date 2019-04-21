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
		size_t mnetid;
		double mposx;
		double mposy;
		double mposz;
		double mvelx;
		double mvely;
		double mvelz;
		double mdamage;
		std::string mclassname;
		std::shared_ptr<groupdata> mgroup;
		bool misvehicle;

		bool mhasDriver;
		bool mhasGunner;
		bool mhasCommander;
		size_t mtransportSoldier;

		std::shared_ptr<objectdata> mparent_object;
		std::shared_ptr<objectdata> mdriver;
		std::shared_ptr<objectdata> mgunner;
		std::shared_ptr<objectdata> mcommander;
		std::vector<std::shared_ptr<sqf::objectdata>> msoldiers;

		innerobj(std::string classname, bool isvehicle);
	public:
		virtual std::string tosqf() const;

		std::array<double, 3> pos() const { return { mposx, mposy, mposz }; }
		double posx() const { return mposx; }
		double posy() const { return mposy; }
		double posz() const { return mposz; }
		std::array<double, 3> vel() const { return { mvelx, mvely, mvelz }; }
		double velx() const { return mvelx; }
		double vely() const { return mvely; }
		double velz() const { return mvelz; }
		std::string classname() const { return mclassname; }
		size_t netid() const { return mnetid; }
		bool is_vehicle() const { return misvehicle; }
		std::shared_ptr<groupdata> group() const { return mgroup; }

		double distance3dsqr(std::shared_ptr<innerobj> obj) const { return distance3dsqr(obj->pos()); }
		double distance3dsqr(const innerobj* obj) const { return distance3dsqr(obj->pos()); }
		double distance3dsqr(std::array<double, 3> otherpos) const;
		double distance3d(std::shared_ptr<innerobj> obj) const { return distance3d(obj->pos()); }
		double distance3d(const innerobj* obj) const { return distance3d(obj->pos()); }
		double distance3d(std::array<double, 3> otherpos) const;

		double distance2dsqr(std::shared_ptr<innerobj> obj) const { return distance2dsqr(std::array<double, 2> { obj->mposx, obj->mposy }); }
		double distance2dsqr(const innerobj* obj) const { return distance2dsqr(std::array<double, 2> { obj->mposx, obj->mposy }); }
		double distance2dsqr(std::array<double, 2> otherpos) const;
		double distance2d(std::shared_ptr<innerobj> obj) const { return distance2d(std::array<double, 2> { obj->mposx, obj->mposy }); }
		double distance2d(const innerobj* obj) const { return distance2d(std::array<double, 2> { obj->mposx, obj->mposy }); }
		double distance2d(std::array<double, 2> otherpos) const;

		double damage() const { return mdamage; }
		void damage(double val) { mdamage = val < 0 ? 0 : val > 1 ? 1 : val; }
		void damage_by(double val) { auto newdmg = mdamage += val; damage(newdmg); }
		bool alive() { return mdamage < 1; }

		void pos(std::array<double, 3> arr) { mposx = arr[0]; mposy = arr[1]; mposz = arr[2]; }
		void posx(double d) { mposx = d; }
		void posy(double d) { mposy = d; }
		void posz(double d) { mposz = d; }
		void vel(std::array<double, 3> arr) { mvelx = arr[0]; mvely = arr[1]; mvelz = arr[2]; }
		void velx(double d) { mvelx = d; }
		void vely(double d) { mvely = d; }
		void velz(double d) { mvelz = d; }
		void group(std::shared_ptr<groupdata> g) { mgroup = g; }

		std::shared_ptr<sqf::objectdata> parent_object() const { return mparent_object; }

		std::shared_ptr<sqf::objectdata> driver() const { return mdriver; }
		std::shared_ptr<sqf::objectdata> gunner() const { return mgunner; }
		std::shared_ptr<sqf::objectdata> commander() const { return mcommander; }
		std::vector<std::shared_ptr<sqf::objectdata>>::iterator soldiers_begin() { return msoldiers.begin(); }
		std::vector<std::shared_ptr<sqf::objectdata>>::iterator soldiers_end() { return msoldiers.end(); }
		const std::vector<std::shared_ptr<sqf::objectdata>>& soldiers() const { return msoldiers; }

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