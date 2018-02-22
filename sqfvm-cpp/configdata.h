#pragma once
#include <string>
#include <memory>

#include "arraydata.h"
#include "wstring_op.h"
#include "type.h"

namespace sqf
{
	class configdata : public arraydata
	{
	private:
		std::wstring mname;
		std::wstring mparentname;
		std::weak_ptr<configdata> mlogicparent;
		std::shared_ptr<value> mvalue;
		bool misnull;

		std::shared_ptr<sqf::value> parent_unsafe(void);
		std::shared_ptr<sqf::value> navigate_unsafe(std::wstring nextnode);
		std::shared_ptr<sqf::value> navigate_full_unsafe(std::wstring nextnode);
	public:
		configdata(std::wstring name) : mname(name), misnull(true) {}
		configdata() : mname(L"bin\\config.bin"), misnull(false) {}
		configdata(std::weak_ptr<configdata> logicparent, std::wstring name) : mname(name), mlogicparent(logicparent), misnull(false) {}
		configdata(std::weak_ptr<configdata> logicparent, std::wstring name, std::wstring parentname) : mname(name), mparentname(parentname), mlogicparent(logicparent), misnull(false) {}

		inline std::shared_ptr<sqf::value> value(void) const { return mvalue; }
		inline void value(std::shared_ptr<sqf::value> val) { mvalue = val; }
		inline std::wstring name(void) const { return mname; }

		virtual std::wstring tosqf(void) const;
		virtual bool equals(std::shared_ptr<data> d) const { return d.get() == this; }
		size_t logicalparentcount(void) const { if (auto sptr = mlogicparent.lock()) { return sptr->logicalparentcount() + 1; } return 0; }
		std::shared_ptr<sqf::value> parent(void) { auto val = parent_unsafe(); return val.get() ? val : configNull(); }
		inline std::shared_ptr<sqf::value> navigate(std::wstring nextnode) { auto val = navigate_full_unsafe(nextnode); return val.get() ? val : configNull(); }

		inline bool haslogicparent(void) { return !mlogicparent.expired(); }
		std::shared_ptr<sqf::value> logicparent(void);

		void mergeinto(std::shared_ptr<configdata>);

		static std::shared_ptr<sqf::value> configFile(void);
		static std::shared_ptr<sqf::value> configNull(void);
	};
}
