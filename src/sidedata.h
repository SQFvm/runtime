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
		std::string tosqf() const override {
			switch (mside)
			{
			case sqf::sidedata::Empty:
				return "EMPTY";
			case sqf::sidedata::Civilian:
				return "CIV";
			case sqf::sidedata::West:
				return "WEST";
			case sqf::sidedata::East:
				return "EAST";
			case sqf::sidedata::Guerilla:
				return "GUER";
			case sqf::sidedata::Logic:
				return "LOGIC";
			case sqf::sidedata::Enemy:
				return "ENEMY";
			case sqf::sidedata::Friendly:
				return "FRIENDLY";
			case sqf::sidedata::AmbientLife:
				return "AMBIENT LIFE";
			default:
				return "UNKNOWN";
			}
		}
		sidedata(eside side) : mside(side) {}
		operator eside() const { return mside; }
		eside side() const { return mside; }
		bool equals(std::shared_ptr<data> d) const override { return mside == std::dynamic_pointer_cast<sidedata>(d)->mside; }
        sqf::type dtype() const override { return sqf::type::SIDE; }

		bool is_friendly_to(eside other) const { return is_friendly_to(this->mside, other); }
		void set_friendly_to(eside other, bool flag) const { set_friendly_to(this->mside, other, flag); }
		static bool is_friendly_to(eside self, eside other);
		static void set_friendly_to(eside self, eside other, bool flag);
	};
}