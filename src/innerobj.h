#pragma once
#include <string>
#include <memory>
#include <array>

namespace sqf
{
	class virtualmachine;
	class groupdata;
	class innerobj : public std::enable_shared_from_this<innerobj>
	{
	private:
		size_t mnetid;
		double mposx;
		double mposy;
		double mposz;
		double mvelx;
		double mvely;
		double mvelz;
		std::string mclassname;
		std::shared_ptr<groupdata> mgroup;

		bool misvehicle;
		innerobj(std::string classname, bool isvehicle) : mclassname(classname), misvehicle(isvehicle) {}
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

		void pos(std::array<double, 3> arr) { mposx = arr[0]; mposy = arr[1]; mposz = arr[2]; }
		void posx(double d) { mposx = d; }
		void posy(double d) { mposy = d; }
		void posz(double d) { mposz = d; }
		void vel(std::array<double, 3> arr) { mvelx = arr[0]; mvely = arr[1]; mvelz = arr[2]; }
		void velx(double d) { mvelx = d; }
		void vely(double d) { mvely = d; }
		void velz(double d) { mvelz = d; }
		void group(std::shared_ptr<groupdata> g) { mgroup = g; }

		bool iskindof(std::string cfgname);

		void destroy(sqf::virtualmachine*);

		static std::shared_ptr<sqf::innerobj> create(sqf::virtualmachine* vm, std::string classname, bool isvehicle);
	};
}