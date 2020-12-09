#pragma once
#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"

#include <string>
#include <memory>
#include <array>

namespace sqf
{
    namespace runtime
    {
        struct t_side : public type::extend<t_side> { t_side() : extend() {} static const std::string name() { return "SIDE"; } };
    }
    namespace types
    {
        class d_side : public sqf::runtime::data
        {
        public:
            enum side
            {
                Empty = 0,
                Unknown,
                Civilian,
                West,
                East,
                Guerilla,
                Logic,
                Enemy,
                Friendly,
                AmbientLife,
                __MAX_SIDE
            };
            static std::string to_string(side s)
            {
                switch (s)
                {
                case Empty:
                    return "EMPTY";
                case Civilian:
                    return "CIV";
                case West:
                    return "WEST";
                case East:
                    return "EAST";
                case Guerilla:
                    return "GUER";
                case Logic:
                    return "LOGIC";
                case Enemy:
                    return "ENEMY";
                case Friendly:
                    return "FRIENDLY";
                case AmbientLife:
                    return "AMBIENT LIFE";
                default:
                    return "UNKNOWN";
                }
            }
            using data_type = sqf::runtime::t_side;
        private:
            side m_value;
        protected:
            bool do_equals(std::shared_ptr<sqf::runtime::data> other, bool invariant) const override
            {
                return m_value == std::static_pointer_cast<d_side>(other)->m_value;
            }
        public:
            d_side() = default;
            d_side(side flag) : m_value(flag) {}

            std::string to_string_sqf() const override { return to_string(m_value); }
            std::string to_string() const override { return to_string_sqf(); }

            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override { return std::hash<size_t>()(static_cast<size_t>(m_value)); }

            side value() const { return m_value; }
            void value(side flag) { m_value = flag; }

            operator bool()
            {
                return m_value;
            }

        private:
            static bool& friendly_map(side is, side with)
            {
                static bool arr[__MAX_SIDE][__MAX_SIDE] = {
                   { false, false, false, false, false, false, false, false, true , true  }, // empty
                   { false, false, false, false, false, false, false, false, true , true  }, // unknown
                   { true , false, true , true , false, true , false, false, true , true  }, // civ
                   { true , false, true , true , false, true , false, false, true , true  }, // west
                   { false, false, true , false, true , false, false, false, true , true  }, // east
                   { false, false, true , true , false, true , false, false, true , true  }, // guer
                   { false, false, true , true , true , true , false, false, true , true  }, // logic
                   { false, false, false, false, false, false, false, false, true , true  }, // friendly
                   { true , true , true , true , true , true , true , true , true , true  }, // ambient
                };
                return arr[is][with];
            }
        public:
            bool is_friendly_to(side other) const { return is_friendly_to(m_value, other); }
            void set_friendly_to(side other, bool flag) const { set_friendly_to(m_value, other, flag); }
            static bool is_friendly_to(side self, side other) { return friendly_map(self, other); }
            static void set_friendly_to(side self, side other, bool flag) { friendly_map(self, other) = flag; }
        };
        template<>
        inline std::shared_ptr<sqf::runtime::data> to_data<d_side::side>(d_side::side side)
        {
            return std::make_shared<d_side>(side);
        }
    }
}