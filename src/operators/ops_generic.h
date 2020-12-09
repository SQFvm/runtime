#pragma once
#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"
#include "../runtime/d_boolean.h"
#include "../runtime/d_code.h"
#include "../runtime/context.h"
#include "../runtime/instruction_set.h"

#include <string>
#include <memory>

namespace sqf
{
    namespace runtime
    {
        struct t_if : public sqf::runtime::type::extend<t_if> { t_if() : extend() {} static const std::string name() { return "IF"; } };
        struct t_script : public sqf::runtime::type::extend<t_script> { t_script() : extend() {} static const std::string name() { return "SCRIPT"; } };
        struct t_switch : public sqf::runtime::type::extend<t_switch> { t_switch() : extend() {} static const std::string name() { return "SWITCH"; } };
        struct t_while : public sqf::runtime::type::extend<t_while> { t_while() : extend() {} static const std::string name() { return "WHILE"; } };
        struct t_for : public sqf::runtime::type::extend<t_for> { t_for() : extend() {} static const std::string name() { return "FOR"; } };;
        struct t_exception : public sqf::runtime::type::extend<t_exception> { t_exception() : extend() {} static const std::string name() { return "EXCEPTION"; } };
        class runtime;
    }
    namespace types
    {
        class d_if : public sqf::types::d_boolean
        {
        public:
            using data_type = sqf::runtime::t_if;
        public:
            d_if() = default;
            d_if(bool flag) : d_boolean(flag) {}

            sqf::runtime::type type() const override { return data_type(); }
        };
        class d_exception : public sqf::types::d_code
        {
        public:
            using data_type = sqf::runtime::t_exception;
        public:
            d_exception() = default;
            d_exception(sqf::runtime::instruction_set set) : d_code(set) {}

            sqf::runtime::type type() const override { return data_type(); }
        };
        class d_script : public sqf::runtime::data
        {
        public:
            using data_type = sqf::runtime::t_script;
        private:
            std::weak_ptr<sqf::runtime::context> m_context;
        protected:
            bool do_equals(std::shared_ptr<sqf::runtime::data> other, bool invariant) const override
            {
                return this == other.get();
            }
        public:
            d_script() = default;
            d_script(std::weak_ptr<sqf::runtime::context> weak) : m_context(weak) {}

            std::string to_string_sqf() const override { return to_string(); }
            std::string to_string() const override
            {
                if (m_context.expired())
                {
                    return "SCRIPT";
                }
                auto lock = m_context.lock();
                if (lock->name().empty())
                {
                    return "SCRIPT";
                }
                else
                {
                    return lock->name();
                }
            }

            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override { return 0; }

            std::shared_ptr<sqf::runtime::context> value() const { return m_context.lock(); }
            void value(std::weak_ptr<sqf::runtime::context> weak) { m_context = weak; }

            bool done() const { return m_context.expired(); }
        };
        class d_switch : public sqf::runtime::data
        {
        public:
            using data_type = sqf::runtime::t_switch;
            inline static const std::string magic = "___switch";
        private:
            sqf::runtime::instruction_set m_target_code;
            sqf::runtime::value m_value;
            bool m_match_now;
            bool m_has_match;
        protected:
            bool do_equals(std::shared_ptr<sqf::runtime::data> other, bool invariant) const override
            {
                return this == other.get();
            }
        public:
            d_switch() = default;
            d_switch(sqf::runtime::value val) : m_value(val), m_match_now(false), m_has_match(false) {}

            std::string to_string_sqf() const override
            {
                return m_value.to_string_sqf();
            }
            std::string to_string() const override
            {
                return m_value.to_string();
            }

            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override { return std::hash<sqf::runtime::value>()(m_value); }

            sqf::runtime::instruction_set target_code() const { return m_target_code; }
            void target_code(sqf::runtime::instruction_set set) { m_target_code = set; }

            sqf::runtime::value::cref value() const { return m_value; }
            void value(sqf::runtime::value set) { m_value = set; }

            bool has_match() const { return m_has_match; }
            void has_match(bool set) { m_has_match = set; }

            bool match_now() const { return m_match_now; }
            void match_now(bool set) { m_match_now = set; }
        };
        class d_while : public sqf::types::d_code
        {
        public:
            using data_type = sqf::runtime::t_while;
        public:
            d_while() = default;
            d_while(sqf::runtime::instruction_set set) : d_code(set) {}

            sqf::runtime::type type() const override { return data_type(); }
        };
        class d_for : public sqf::runtime::data
        {
        public:
            using data_type = sqf::runtime::t_for;
        private:
            std::string m_variable;
            float m_from;
            float m_to;
            float m_step;
        protected:
            bool do_equals(std::shared_ptr<sqf::runtime::data> other, bool invariant) const override
            {
                auto of = std::static_pointer_cast<d_for>(other);
                return m_variable == of->m_variable && m_from == of->m_from && m_to == of->m_to && m_step == of->m_step;
            }
        public:
            d_for() : m_variable({}), m_from(0), m_to(0), m_step(1.0F) {}

            std::string to_string_sqf() const override
            {
                return "ForType <invisible>";
            }
            std::string to_string() const override
            {
                return "ForType <invisible>";
            }

            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override
            {
                size_t hash = 0x9e3779b9;
                hash ^= std::hash<std::string>()(m_variable) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                hash ^= std::hash<float>()(m_from) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                hash ^= std::hash<float>()(m_to) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                hash ^= std::hash<float>()(m_step) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                return hash;
            }

            std::string variable() const { return m_variable; }
            void variable(std::string value) { m_variable = value; }
            float from() const { return m_from; }
            void from(float value) { m_from = value; }
            float to() const { return m_to; }
            void to(float value) { m_to = value; }
            float step() const { return m_step; }
            void step(float value) { m_step = value; }
        };
    }

    namespace operators
    {
        void ops_generic(::sqf::runtime::runtime& runtime);
    }
}