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
			static sqf::runtime::type cexp_type() { return sqf::runtime::t_scalar(); }
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
			d_scalar(int8_t  value)  : m_value((float)value) {}
			d_scalar(int16_t value)  : m_value((float)value) {}
			d_scalar(int32_t value)  : m_value((float)value) {}
			d_scalar(int64_t value)  : m_value((float)value) {}
			d_scalar(uint8_t  value) : m_value((float)value) {}
			d_scalar(uint16_t value) : m_value((float)value) {}
			d_scalar(uint32_t value) : m_value((float)value) {}
			d_scalar(uint64_t value) : m_value((float)value) {}
			d_scalar(float value)    : m_value((float)value) {}
			d_scalar(double value)   : m_value((float)value) {}

			std::string to_string_sqf() const override;
			std::string to_string() const override;

			sqf::runtime::type type() const override { return cexp_type(); }
			float value() const { return m_value; }
			void value(float f) { m_value = f; }
			operator float() { return m_value; }
			static void set_decimals(int val) { s_decimals = val; }
		};
		::sqf::runtime::value val(int8_t  value)  { return std::make_shared<sqf::types::d_scalar>(value); }
		::sqf::runtime::value val(int16_t value)  { return std::make_shared<sqf::types::d_scalar>(value); }
		::sqf::runtime::value val(int32_t value)  { return std::make_shared<sqf::types::d_scalar>(value); }
		::sqf::runtime::value val(int64_t value)  { return std::make_shared<sqf::types::d_scalar>(value); }
		::sqf::runtime::value val(uint8_t  value) { return std::make_shared<sqf::types::d_scalar>(value); }
		::sqf::runtime::value val(uint16_t value) { return std::make_shared<sqf::types::d_scalar>(value); }
		::sqf::runtime::value val(uint32_t value) { return std::make_shared<sqf::types::d_scalar>(value); }
		::sqf::runtime::value val(uint64_t value) { return std::make_shared<sqf::types::d_scalar>(value); }
		::sqf::runtime::value val(float value)    { return std::make_shared<sqf::types::d_scalar>(value); }
		::sqf::runtime::value val(double value)   { return std::make_shared<sqf::types::d_scalar>(value); }
	}
}