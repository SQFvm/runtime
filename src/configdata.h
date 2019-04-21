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
		std::string m_name;
		std::string m_inherited_parent_name;
		std::weak_ptr<configdata> m_logical_parent;
		value m_value;
		bool m_null;

		sqf::value inherited_parent_unsafe() const;
		sqf::value navigate_unsafe(std::string_view nextnode) const;
		sqf::value navigate_full_unsafe(std::string_view nextnode) const;
	public:
		configdata(std::string name) : m_name(std::move(name)), m_null(true) {}
		configdata() : m_name("bin\\config.bin"), m_null(false) {}
		configdata(std::weak_ptr<configdata> logicparent, std::string name) : m_name(std::move(name)), m_logical_parent(std::move(logicparent)), m_null(false) {}
		configdata(std::weak_ptr<configdata> logicparent, std::string name, std::string parentname) : m_name(std::move(name)), m_inherited_parent_name(std::move(parentname)), m_logical_parent(std::move(logicparent)), m_null(false) {}

		std::string name() const { return m_name; }
		std::string inherited_parent_name() { return m_inherited_parent_name; }
		sqf::value inherited_parent() const { auto val = inherited_parent_unsafe(); return val; }

		sqf::value cfgvalue() const { return m_value; }
		void set_cfgvalue(sqf::value val) { m_value = std::move(val); }
		bool is_null() const { return m_null; }
		bool has_logical_parent() const { return !m_logical_parent.expired(); }
		sqf::value logical_parent() const;

        sqf::value navigate(std::string_view nextnode) const { auto val = navigate_full_unsafe(nextnode); return val; }
		bool is_kind_of(std::string_view s) const;
		size_t count_logical_parents() const { if (auto sptr = m_logical_parent.lock()) { return sptr->count_logical_parents() + 1; } return 0; }


		bool cfgvalue(std::string_view key, bool def) const;
		float cfgvalue(std::string_view key, int def) const { return cfgvalue(key, static_cast<float>(def)); }
		float cfgvalue(std::string_view key, float def) const;
		std::string cfgvalue(std::string_view key, std::string def) const;

		std::string tosqf() const override;
		void mergeinto(std::shared_ptr<configdata>);
		bool equals(std::shared_ptr<data> d) const override { return d.get() == this; }
        sqf::type dtype() const override { return sqf::type::CONFIG; }

		static sqf::value configFile();
		static sqf::value configNull();
	};
}
