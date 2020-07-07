#pragma once
#include <string>
#include <string_view>
#include <memory>
#include <vector>

#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"


namespace sqf
{
	namespace runtime
	{
		struct t_string : public type::extend<t_string> { t_string() : extend() {} static const std::string name() { return "STRING"; } };
	}
	namespace types
	{
		class d_string : public sqf::runtime::data
		{
		public:
			static constexpr sqf::runtime::type cexp_type() { return sqf::runtime::t_boolean(); }
		private:
			std::string m_value;
		protected:
			bool do_equals(std::shared_ptr<data> other, bool invariant) const override
			{
				auto otherstr = std::static_pointer_cast<d_string>(other)->m_value;
				if (invariant)
				{
					if (m_value.length() != otherstr.length()) { return false; }
					for (size_t i = 0; i < m_value.size(); i++)
					{
						if (std::tolower(m_value[i]) != std::tolower(otherstr[i])) { return false; }
					}
					return true;
				}
				else
				{
					return m_value.compare(m_value) == 0;
				}
			}
		public:
			d_string() = default;
			d_string(std::string value) : m_value(value) {}
			d_string(std::string_view value) : m_value(value) {}

			std::string to_string_sqf() const override
			{
				size_t count = /* count quotes */ std::count(m_value.begin(), m_value.end(), '"') + 2 /* outter quotes */;
				size_t size = m_value.length() + count;
				std::vector<char> arr;
				arr.reserve(size);

				arr.emplace_back('"'); // starting quote

				for (char c : m_value)
				{
					arr.emplace_back(c);
					if (c == '"')
					{
						arr.emplace_back(c);
					}
				}
				arr.emplace_back('"'); // ending quote
				return std::string(arr.data(), arr.size());
			}
			std::string to_string() const override
			{
				return m_value;
			}
			static d_string from_sqf(std::string_view sview)
			{
				size_t count = 0;
				char start = s[0];
				if (sview.length() == 0) { return {}; }
				if (start != '"' && start != '\'') { return {}; }
				if (s.length() == 2) { return {}; }

				std::vector<char> arr;
				arr.reserve(s.size());
				for (size_t i = 1; i < s.length() - 1; i++)
				{
					char c = s[i];
					if (c == start && s[i + 1] == start) //quote type
					{
						i++;
					}
					arr.emplace_back(c);
				}
				return { std::string(arr.data(), arr.size()) };
			}
			sqf::runtime::type type() const override { return cexp_type(); }
			std::string value() const { return m_value; }
			void value(std::string string) { m_value = string; }

			operator std::string() { return value.m_value; }
			operator std::string_view() { return value.m_value; }
		};
		operator std::string(sqf::runtime::value::cref value)
		{
			auto data = value.data_try<d_string>();
			if (!data) { return {}; }
			return data->value();
		}
		operator std::string_view(sqf::runtime::value::cref value)
		{
			auto data = value.data_try<d_string>();
			if (!data) { return {}; }
			return data->value();
		}
	}
}