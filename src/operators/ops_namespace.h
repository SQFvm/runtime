#pragma once
#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"
#include "../runtime/value_scope.h"

#include <string>
#include <memory>

namespace sqf
{
    namespace runtime
    {
        inline static const std::string missionNamespace = "missionNamespace";
        inline static const std::string profileNamespace = "profileNamespace";
        inline static const std::string uiNamespace = "uiNamespace";
        inline static const std::string parsingNamespace = "parsingNamespace";
        inline static const std::string localNamespace = "localNamespace";
        
        struct t_with : public sqf::runtime::type::extend<t_with> { t_with() : extend() {} static const std::string name() { return "WITH"; } };
        struct t_namespace : public sqf::runtime::type::extend<t_namespace> { t_namespace() : extend() {} static const std::string name() { return "NAMESPACE"; } };
        class runtime;
        class context;
        class instruction_set;
    }
    namespace types
    {
        class d_namespace : public sqf::runtime::data
        {
        public:
            using data_type = sqf::runtime::t_namespace;
        private:
            std::weak_ptr<sqf::runtime::value_scope> m_scope;
        protected:
            bool do_equals(std::shared_ptr<sqf::runtime::data> other, bool ignoreCase) const override
            {
                auto otherNs = std::static_pointer_cast<d_namespace>(other);
                if (!m_scope.expired() && !otherNs->m_scope.expired())
                {
                    return m_scope.lock() == otherNs->m_scope.lock();
                }
                else if (m_scope.expired() && otherNs->m_scope.expired())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        public:
            d_namespace() = default;
            d_namespace(std::shared_ptr<sqf::runtime::value_scope> scope) : m_scope(scope) {}

            virtual std::string to_string_sqf() const override
            {
                if (m_scope.expired())
                {
                    return "nil";
                }
                else
                {
                    return std::string(m_scope.lock()->scope_name());
                }
            }

            virtual std::string to_string() const override { return to_string_sqf(); }
            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override { return 0; }

            std::shared_ptr<sqf::runtime::value_scope> value() { return m_scope.lock(); }
            operator std::shared_ptr<sqf::runtime::value_scope>() { return m_scope.lock(); }
        };
        class d_with : public sqf::types::d_namespace
        {
        public:
            using data_type = sqf::runtime::t_with;
        public:
            d_with() = default;
            d_with(std::shared_ptr<sqf::runtime::value_scope> scope) : d_namespace(scope) {}

            sqf::runtime::type type() const override { return data_type(); }
        };
    }

    namespace operators
    {
        void ops_namespace(::sqf::runtime::runtime& runtime);
    }
}