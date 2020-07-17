#pragma once
#include <string>
#include <string_view>
#include <memory>
#include <vector>

#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"
#include "../runtime/confighost.h"


namespace sqf
{
	namespace runtime
	{
		struct t_config : public type::extend<t_config> { t_config() : extend() {} static const std::string name() { return "CONFIG"; } };
	}
	namespace types
	{
		class d_config : public sqf::runtime::data
		{
		public:
			static sqf::runtime::type cexp_type() { return sqf::runtime::t_config(); }
		private:
			sqf::runtime::confighost::config m_value;
		protected:
			bool do_equals(std::shared_ptr<data> other, bool invariant) const override
			{
				auto other_config = std::static_pointer_cast<d_config>(other)->m_value;
			}
		public:
			d_config() = default;
			d_config(sqf::runtime::confighost::config conf) : m_value(conf) {}

			std::string to_string_sqf() const override
			{
				return std::string(m_value.name());
			}
			std::string to_string() const override
			{
				return std::string(m_value.name());
			}
			sqf::runtime::type type() const override { return cexp_type(); }
			sqf::runtime::confighost::config value() const { return m_value; }
			void value(sqf::runtime::confighost::config conf) { m_value = conf; }

			operator sqf::runtime::confighost::config() { return m_value; }
		};

		std::shared_ptr<sqf::runtime::data>& operator<<(std::shared_ptr<sqf::runtime::data>& input, sqf::runtime::confighost::config conf)
		{
			input = std::make_shared<d_config>(conf);
			return input;
		}
	}
}