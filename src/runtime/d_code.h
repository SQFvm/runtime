#pragma once
#include "data.h"
#include "type.h"
#include "value.h"
#include "instruction_blob.hpp"

#include <string>
#include <memory>
#include <algorithm>
#include <sstream>


namespace sqf
{
    namespace runtime
    {
        class instruction_handle;
        class instruction_blob;
        struct t_code : public type::extend<t_code> { t_code() : extend() {} static const std::string name() { return "CODE"; } };
    }
    namespace types
    {
        class d_code : public sqf::runtime::data
        {
        public:
            using data_type = sqf::runtime::t_code;
        private:
            sqf::runtime::instruction_blob m_value;

        protected:
            bool do_equals(std::shared_ptr<data> other, bool invariant) const override
            {
                auto casted = std::static_pointer_cast<d_code>(other);
                return std::equal(m_value.begin(), m_value.end(), casted->m_value.begin(), casted->m_value.end());
            }
        public:
            d_code() = default;
            d_code(sqf::runtime::instruction_blob set) : m_value(set) {}

            std::string to_string_sqf() const override
            {
                std::vector<std::string> strs;
                auto it = m_value.rbegin();
                for (; it != m_value.rend(); it++)
                {
                    auto opt = (*it)->reconstruct(it, m_value.rend(), 0, false);
                    if (!opt.has_value())
                    {
                        return {};
                    }
                    if (opt.value().empty())
                    {
                        continue;
                    }
                    strs.push_back(*opt);
                }
                std::reverse(strs.begin(), strs.end());

                std::stringstream sstream;
                sstream << "{ ";
                if (!strs.empty())
                {
                    sstream << strs.front();
                    for (auto it2 = strs.begin() + 1; it2 != strs.end(); it2++)
                    {
                        sstream << "; " << *it2;
                    }
                }
                sstream << " }";

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

            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override { return std::hash<std::string>()(to_string_sqf()); }

            const sqf::runtime::instruction_blob& value() const { return m_value; }
            void value(sqf::runtime::instruction_blob value) { m_value = value; }

            operator const sqf::runtime::instruction_blob&() const { return m_value; }
            operator sqf::runtime::instruction_blob&() { return m_value; }

            void write(sqf::runtime::instruction_blob& b) const override;
            static std::shared_ptr<d_code> read(sqf::runtime::instruction_handle& h);
        };

        template<>
        inline std::shared_ptr<sqf::runtime::data> to_data<sqf::runtime::instruction_blob>(sqf::runtime::instruction_blob set)
        {
            return std::make_shared<d_code>(set);
        }
    }
}