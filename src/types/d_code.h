#pragma once
#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"
#include "../runtime/instruction_set.h"

#include <string>
#include <memory>
#include <algorithm>
#include <sstream>


namespace sqf
{
	namespace runtime
	{
		struct t_code : public type::extend<t_code> { t_code() : extend() {} static const std::string name() { return "CODE"; } };
	}
	namespace types
	{
		class d_code : public sqf::runtime::data
		{
		public:
			static sqf::runtime::type cexp_type() { return sqf::runtime::t_code(); }
		private:
			sqf::runtime::instruction_set m_value;

		protected:
			bool do_equals(std::shared_ptr<data> other, bool invariant) const override
			{
				auto casted = std::static_pointer_cast<d_code>(other);
				return std::equal(m_value.begin(), m_value.end(), casted->m_value.begin(), casted->m_value.end(),
					[](::sqf::runtime::instruction::sptr left, ::sqf::runtime::instruction::sptr right) { return left == right; });
			}
		public:
			d_code() = default;
			d_code(sqf::runtime::instruction_set set) : m_value(set) {}

			std::string to_string_sqf() const override
			{
				std::stringstream sstream;
				auto it = m_value.begin();
				sstream << "{ ";
				for (; it != m_value.end(); it++)
				{
					auto opt = (*it)->reconstruct(it, m_value.end(), 0, false);
					if (!opt.has_value())
					{
						return {};
					}
					sstream << *opt;
				}
				sstream << "}";

				return sstream.str();
			}
			std::string to_string() const override
			{
				std::stringstream sstream;
				auto it = m_value.begin();
				sstream << "[ ";
				for (; it != m_value.end(); it++)
				{
					if (it != m_value.begin())
					{
						sstream << ", ";
					}
					sstream << (*it)->to_string();
				}
				sstream << "]";

				return sstream.str();
			}

			sqf::runtime::type type() const override { return cexp_type(); }

			const sqf::runtime::instruction_set& value() const { return m_value; }
			void value(sqf::runtime::instruction_set flag) { m_value = flag; }

			operator const sqf::runtime::instruction_set&() { return m_value; }
		};
		::sqf::runtime::value val(sqf::runtime::instruction_set set) { return std::make_shared<sqf::types::d_code>(set); }
	}
}