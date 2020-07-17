#pragma once
#include <string>
#include <memory>

#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"


namespace sqf
{
	namespace runtime
	{
		struct t_boolean : public type::extend<t_boolean> { t_boolean() : extend() {} static const std::string name() { return "BOOL"; } };
	}
	namespace types
	{
		class d_boolean : public sqf::runtime::data
		{
		public:
			static sqf::runtime::type cexp_type() { return sqf::runtime::t_boolean(); }
		private:
			bool m_value;
		protected:
			bool do_equals(std::shared_ptr<data> other, bool invariant) const override
			{
				return m_value == std::static_pointer_cast<d_boolean>(other)->m_value;
			}
		public:
			d_boolean() = default;
			d_boolean(bool flag) : m_value(flag) {}

			std::string to_string_sqf() const override
			{
				return m_value ? "true" : "false";
			}
			std::string to_string() const override
			{
				return m_value ? "true" : "false";
			}

			sqf::runtime::type type() const override { return cexp_type(); }

			bool value() const { return m_value; }
			void value(bool flag) { m_value = flag; }

			operator bool()
			{
				return m_value;
			}
		};
		std::shared_ptr<sqf::runtime::data>& operator<<(std::shared_ptr<sqf::runtime::data>& input, bool flag)
		{
			input = std::make_shared<d_boolean>(flag);
			return input;
		}
	}
}