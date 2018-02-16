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
		virtual std::wstring tosqf(void) const;
		inline bool isnull(void) const { return mobj.expired(); }
		inline std::weak_ptr<innerobj> obj(void) { if (mobj.expired()) return std::weak_ptr<innerobj>(); auto sptr = mobj.lock(); return sptr; }
		virtual bool equals(std::shared_ptr<data> d) const { return this == d.get(); }
	};
}