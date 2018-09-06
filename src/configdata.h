#pragma once
#include <string>
#include <memory>

#include "arraydata.h"
#include "string_op.h"
#include "type.h"

namespace sqf
{
	class configdata : public arraydata
	{
	private:
		std::string mname;
		std::string mparentname;
		std::weak_ptr<configdata> mlogicparent;
		std::shared_ptr<value> mvalue;
		bool misnull;

		std::shared_ptr<sqf::value> parent_unsafe(void);
		std::shared_ptr<sqf::value> navigate_unsafe(std::string nextnode);
		std::shared_ptr<sqf::value> navigate_full_unsafe(std::string nextnode);
	public:
		configdata(std::string name) : mname(name), misnull(true) {}
		configdata() : mname("bin\\config.bin"), misnull(false) {}
		configdata(std::weak_ptr<configdata> logicparent, std::string name) : mname(name), mlogicparent(logicparent), misnull(false) {}
		configdata(std::weak_ptr<configdata> logicparent, std::string name, std::string parentname) : mname(name), mparentname(parentname), mlogicparent(logicparent), misnull(false) {}

		inline std::shared_ptr<sqf::value> cfgvalue(void) const { return mvalue; }
		inline void cfgvalue(std::shared_ptr<sqf::value> val) { mvalue = val; }
		inline std::string name(void) const { return mname; }

		virtual std::string tosqf(void) const;
		virtual bool equals(std::shared_ptr<data> d) const { return d.get() == this; }
		size_t logicalparentcount(void) const { if (auto sptr = mlogicparent.lock()) { return sptr->logicalparentcount() + 1; } return 0; }
		std::shared_ptr<sqf::value> parent(void) { auto val = parent_unsafe(); return val.get() ? val : configNull(); }
		inline std::shared_ptr<sqf::value> navigate(std::string nextnode) { auto val = navigate_full_unsafe(nextnode); return val.get() ? val : configNull(); }

		inline bool is_null(void) { return misnull; }
		inline bool haslogicparent(void) { return !mlogicparent.expired(); }
		std::shared_ptr<sqf::value> logicparent(void);

		void mergeinto(std::shared_ptr<configdata>);

		static std::shared_ptr<sqf::value> configFile(void);
		static std::shared_ptr<sqf::value> configNull(void);
	};
}
