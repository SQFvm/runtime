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
		virtual std::string tosqf(void) const;

		inline std::array<double, 3> pos(void) const { return { mposx, mposy, mposz }; }
		inline double posx(void) const { return mposx; }
		inline double posy(void) const { return mposy; }
		inline double posz(void) const { return mposz; }
		inline std::array<double, 3> vel(void) const { return { mvelx, mvely, mvelz }; }
		inline double velx(void) const { return mvelx; }
		inline double vely(void) const { return mvely; }
		inline double velz(void) const { return mvelz; }
		inline std::string classname(void) const { return mclassname; }
		inline size_t netid(void) const { return mnetid; }
		inline bool is_vehicle(void) const { return misvehicle; }
		inline std::shared_ptr<groupdata> group(void) const { return mgroup; }

		inline double distance3dsqr(std::shared_ptr<innerobj> obj) const { return distance3dsqr(obj->pos()); }
		inline double distance3dsqr(const innerobj* obj) const { return distance3dsqr(obj->pos()); }
		double distance3dsqr(std::array<double, 3> otherpos) const;
		inline double distance3d(std::shared_ptr<innerobj> obj) const { return distance3d(obj->pos()); }
		inline double distance3d(const innerobj* obj) const { return distance3d(obj->pos()); }
		double distance3d(std::array<double, 3> otherpos) const;

		inline double distance2dsqr(std::shared_ptr<innerobj> obj) const { return distance2dsqr(std::array<double, 2> { obj->mposx, obj->mposy }); }
		inline double distance2dsqr(const innerobj* obj) const { return distance2dsqr(std::array<double, 2> { obj->mposx, obj->mposy }); }
		double distance2dsqr(std::array<double, 2> otherpos) const;
		inline double distance2d(std::shared_ptr<innerobj> obj) const { return distance2d(std::array<double, 2> { obj->mposx, obj->mposy }); }
		inline double distance2d(const innerobj* obj) const { return distance2d(std::array<double, 2> { obj->mposx, obj->mposy }); }
		double distance2d(std::array<double, 2> otherpos) const;

		inline void pos(std::array<double, 3> arr) { mposx = arr[0]; mposy = arr[1]; mposz = arr[2]; }
		inline void posx(double d) { mposx = d; }
		inline void posy(double d) { mposy = d; }
		inline void posz(double d) { mposz = d; }
		inline void vel(std::array<double, 3> arr) { mvelx = arr[0]; mvely = arr[1]; mvelz = arr[2]; }
		inline void velx(double d) { mvelx = d; }
		inline void vely(double d) { mvely = d; }
		inline void velz(double d) { mvelz = d; }
		inline void group(std::shared_ptr<groupdata> g) { mgroup = g; }

		void destroy(sqf::virtualmachine*);

		static std::shared_ptr<sqf::innerobj> create(sqf::virtualmachine* vm, std::string classname, bool isvehicle);
	};
}