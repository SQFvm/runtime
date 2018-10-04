#pragma once
#include <string>
#include <memory>
#include "data.h"

namespace sqf
{
	class innerobj;
	class objectdata : public data
	{
	private:
		std::weak_ptr<innerobj> mobj;
	public:
		objectdata() {}
		objectdata(std::weak_ptr<innerobj> obj) : mobj(obj) {}
		virtual std::string tosqf() const;
		inline bool is_null() const { return mobj.expired(); }
		inline std::shared_ptr<innerobj> obj() const { if (mobj.expired()) return std::shared_ptr<innerobj>(); auto sptr = mobj.lock(); return sptr; }
		virtual bool equals(std::shared_ptr<data> d) const { return obj().get() == std::dynamic_pointer_cast<objectdata>(d)->obj().get(); }
	};
}