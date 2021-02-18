#pragma once
#include <string>
#include <memory>

#include "data.h"
#include "type.h"
#include "value.h"


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
            using data_type = sqf::runtime::t_scalar;
        private:
            float m_value;
            inline static int s_decimals = -1;
        protected:
            bool do_equals(std::shared_ptr<data> other, bool invariant) const override
            {
                return m_value == std::static_pointer_cast<d_scalar>(other)->m_value;
            }
        public:
            d_scalar() = default;
            d_scalar(int8_t  value)       : m_value((float)value) {}
            d_scalar(int16_t value)       : m_value((float)value) {}
            d_scalar(int32_t value)       : m_value((float)value) {}
            d_scalar(int64_t value)       : m_value((float)value) {}
            d_scalar(uint8_t  value)      : m_value((float)value) {}
            d_scalar(uint16_t value)      : m_value((float)value) {}
            d_scalar(uint32_t value)      : m_value((float)value) {}
            d_scalar(uint64_t value)      : m_value((float)value) {}
#ifdef __APPLE__
            d_scalar(long value) : m_value((float)value) {}
            d_scalar(unsigned long value) : m_value((float)value) {}
#endif //  __APPLE__
            d_scalar(float value)         : m_value((float)value) {}
            d_scalar(double value)        : m_value((float)value) {}

            std::string to_string_sqf() const override;
            std::string to_string() const override;

            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override { return std::hash<float>()(m_value); }
            float value() const { return m_value; }
            void value(float f) { m_value = f; }
            operator float() { return m_value; }
            static void set_decimals(int val) { s_decimals = val; }
        };

        template<> inline std::shared_ptr<sqf::runtime::data> to_data<int8_t>(int8_t  value)              { return std::make_shared<d_scalar>(value); }
        template<> inline std::shared_ptr<sqf::runtime::data> to_data<int16_t>(int16_t value)             { return std::make_shared<d_scalar>(value); }
        template<> inline std::shared_ptr<sqf::runtime::data> to_data<int32_t>(int32_t value)             { return std::make_shared<d_scalar>(value); }
        template<> inline std::shared_ptr<sqf::runtime::data> to_data<int64_t>(int64_t value)             { return std::make_shared<d_scalar>(value); }
        template<> inline std::shared_ptr<sqf::runtime::data> to_data<uint8_t>(uint8_t  value)            { return std::make_shared<d_scalar>(value); }
        template<> inline std::shared_ptr<sqf::runtime::data> to_data<uint16_t>(uint16_t value)           { return std::make_shared<d_scalar>(value); }
        template<> inline std::shared_ptr<sqf::runtime::data> to_data<uint32_t>(uint32_t value)           { return std::make_shared<d_scalar>(value); }
        template<> inline std::shared_ptr<sqf::runtime::data> to_data<uint64_t>(uint64_t value)           { return std::make_shared<d_scalar>(value); }
#ifdef __APPLE__
        template<> inline std::shared_ptr<sqf::runtime::data> to_data<long>(long value)                   { return std::make_shared<d_scalar>(value); }
        template<> inline std::shared_ptr<sqf::runtime::data> to_data<unsigned long>(unsigned long value) { return std::make_shared<d_scalar>(value); }
#endif //  __APPLE__
        template<> inline std::shared_ptr<sqf::runtime::data> to_data<float>(float value)                 { return std::make_shared<d_scalar>(value); }
        template<> inline std::shared_ptr<sqf::runtime::data> to_data<double>(double value)               { return std::make_shared<d_scalar>(value); }
    }

}