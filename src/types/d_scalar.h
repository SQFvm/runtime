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
		struct t_scalar : public type::extend<t_scalar> { t_scalar() : extend() {} static const std::string name() { return "SCALAR"; } };
	}
	namespace types
	{
		class d_scalar : public sqf::runtime::data
		{
		public:
			static constexpr sqf::runtime::type cexp_type() { return sqf::runtime::t_scalar(); }
		private:
			float m_value;
			inline static int s_decimals = 0;
		protected:
			bool do_equals(std::shared_ptr<data> other, bool invariant) const override
			{
				return m_value == std::static_pointer_cast<d_scalar>(other)->m_value;
			}
		public:
			d_scalar() = default;
			d_scalar(float value) : m_value(value) {}

			std::string to_string_sqf() const override;
			std::string to_string() const override;

			sqf::runtime::type type() const override { return cexp_type(); }
			float value() const { return m_value; }
			void value(float f) { m_value = f; }
			operator float() { return value.m_value; }
			static void set_decimals(int val) { s_decimals = val; }
		};
		operator float(sqf::runtime::value::cref value)
		{
			auto data = value.data_try<d_scalar>();
			if (!data) { return {}; }
			return data->value();
		}
	}
}