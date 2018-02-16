#pragma once
#include <string>
#include <memory>
#include "data.h"

namespace sqf
{
	class sidedata : public data
	{
	public:
		enum eside
		{
			Empty = 0,
			Unknown,
			Civilian,
			West,
			East,
			Guerilla,
			Logic,
			Enemy,
			Friendly,
			AmbientLife
		};
	private:
		eside mside;
	public:
		virtual std::wstring tosqf(void) const
		{
			switch (mside)
			{
			case sqf::sidedata::Empty:
				return L"EMPTY";
			case sqf::sidedata::Civilian:
				return L"CIV";
			case sqf::sidedata::West:
				return L"WEST";
			case sqf::sidedata::East:
				return L"EAST";
			case sqf::sidedata::Guerilla:
				return L"GUER";
			case sqf::sidedata::Logic:
				return L"LOGIC";
			case sqf::sidedata::Enemy:
				return L"ENEMY";
			case sqf::sidedata::Friendly:
				return L"FRIENDLY";
			case sqf::sidedata::AmbientLife:
				return L"AMBIENT LIFE";
			default:
				return L"UNKNOWN";
			}
		}
		sidedata(eside side) : mside(side) {}
		operator eside() const { return mside; }
		eside side() const { return mside; }
		virtual bool equals(std::shared_ptr<data> d) const { return mside == std::dynamic_pointer_cast<sidedata>(d)->mside; }

		inline bool is_friendly_to(eside other) { is_friendly_to(this->mside, other); }
		inline void set_friendly_to(eside other, bool flag) { set_friendly_to(this->mside, other, flag); }
		static bool is_friendly_to(eside self, eside other);
		static void set_friendly_to(eside self, eside other, bool flag);
	};
}