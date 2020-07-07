#pragma once
#include <string>
#include <memory>
#include <utility>
#include "data.h"

namespace sqf
{
	class innerobj;
	class value;
	class objectdata : public data
	{
	private:
		std::weak_ptr<innerobj> mobj;
	public:
        objectdata() = default;
		objectdata(std::weak_ptr<innerobj> obj) : mobj(std::move(obj)) {}
		std::string tosqf() const override;
		bool is_null() const { return mobj.expired(); }
		std::shared_ptr<innerobj> obj() const { if (mobj.expired()) return std::shared_ptr<innerobj>(); auto sptr = mobj.lock(); return sptr; }
		bool equals(std::shared_ptr<data> d) const override { return obj().get() == std::dynamic_pointer_cast<objectdata>(d)->obj().get(); }
        sqf::type dtype() const override { return sqf::type::OBJECT; }
		static std::shared_ptr<objectdata> objnull_data();
		static std::shared_ptr<sqf::value> objnull();
	};
}