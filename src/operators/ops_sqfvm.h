#pragma once
#include "../runtime/type.h"
#include "../runtime/data.h"
#include <memory>


namespace sqf::runtime
{
    class runtime;
    struct t_runtime : public type::extend<t_runtime> { t_runtime() : extend() { } static const std::string name() { return "RUNTIME"; } };
}
namespace sqf::types
{
    class d_runtime : public sqf::runtime::data
    {
    public:
        using data_type = sqf::runtime::t_runtime;
    private:
        std::shared_ptr<sqf::runtime::runtime> m_value;
    protected:
        bool do_equals(std::shared_ptr<data> other, bool invariant) const override
        {
            return m_value == std::static_pointer_cast<d_runtime>(other)->m_value;
        }
    public:
        d_runtime() = default;
        d_runtime(std::shared_ptr<sqf::runtime::runtime> value) : m_value(value) { }

        std::string to_string_sqf() const override { return std::string("RUNTIME"); }
        std::string to_string() const override { return std::string("RUNTIME"); }

        sqf::runtime::type type() const override { return data_type(); }
        virtual std::size_t hash() const override { return 0; }
        std::shared_ptr<sqf::runtime::runtime> value() const { return m_value; }
        void value(std::shared_ptr<sqf::runtime::runtime> val) { m_value = val; }
    };
}
namespace sqf::operators
{
    void ops_sqfvm(::sqf::runtime::runtime& runtime);
}