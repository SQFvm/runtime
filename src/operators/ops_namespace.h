#pragma once
#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"

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
            static sqf::runtime::type cexp_type() { return sqf::runtime::t_namespace(); }
        private:
            std::weak_ptr<sqf::runtime::value_scope> m_scope;
        public:
            d_namespace() = default;
            d_namespace(std::shared_ptr<sqf::runtime::value_scope> scope) : m_scope(scope) {}

            sqf::runtime::type type() const override { return cexp_type(); }

            std::shared_ptr<sqf::runtime::value_scope> value() { return m_scope.lock(); }
            operator std::shared_ptr<sqf::runtime::value_scope>() { return m_scope.lock(); }
        };
        class d_with : public sqf::types::d_namespace
        {
        public:
            static sqf::runtime::type cexp_type() { return sqf::runtime::t_with(); }
        public:
            d_with() = default;
            d_with(std::shared_ptr<sqf::runtime::value_scope> scope) : d_namespace(scope) {}

            sqf::runtime::type type() const override { return cexp_type(); }
        };
    }

    namespace operators
    {
        void ops_namespace(::sqf::runtime::runtime& runtime);
    }
}