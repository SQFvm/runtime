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
		long mnetid;
		double mposx;
		double mposy;
		double mposz;
		double mvelx;
		double mvely;
		double mvelz;
		std::wstring mclassname;
	public:
		innerobj(sqf::virtualmachine* vm, std::wstring classname);
		virtual std::wstring tosqf(void) const = 0;

		inline std::array<double, 3> pos(void) { return { mposx, mposy, mposz }; }
		inline double posx(void) { return mposx; }
		inline double posy(void) { return mposy; }
		inline double posz(void) { return mposz; }
		inline std::array<double, 3> vel(void) { return { mvelx, mvely, mvelz }; }
		inline double velx(void) { return mvelx; }
		inline double vely(void) { return mvely; }
		inline double velz(void) { return mvelz; }
	};
}