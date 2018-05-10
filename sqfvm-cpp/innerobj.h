#pragma once
#include <string>
#include <memory>
#include <array>

namespace sqf
{
	class virtualmachine;
	class innerobj
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

		bool misvehicle;
		innerobj(std::string classname, bool isvehicle) : mclassname(classname), misvehicle(isvehicle) {}
	public:
		virtual std::string tosqf(void) const;

		inline std::array<double, 3> pos(void) { return { mposx, mposy, mposz }; }
		inline double posx(void) { return mposx; }
		inline double posy(void) { return mposy; }
		inline double posz(void) { return mposz; }
		inline std::array<double, 3> vel(void) { return { mvelx, mvely, mvelz }; }
		inline double velx(void) { return mvelx; }
		inline double vely(void) { return mvely; }
		inline double velz(void) { return mvelz; }
		inline std::string classname(void) { return mclassname; }
		inline size_t netid(void) { return mnetid; }

		inline void pos(std::array<double, 3> arr) { mposx = arr[0]; mposy = arr[1]; mposz = arr[2]; }
		inline void posx(double d) { mposx = d; }
		inline void posy(double d) { mposy = d; }
		inline void posz(double d) { mposz = d; }
		inline void vel(std::array<double, 3> arr) { mvelx = arr[0]; mvely = arr[1]; mvelz = arr[2]; }
		inline void velx(double d) { mvelx = d; }
		inline void vely(double d) { mvely = d; }
		inline void velz(double d) { mvelz = d; }

		static std::shared_ptr<sqf::innerobj> create(sqf::virtualmachine* vm, std::string classname, bool isvehicle);
	};
}