#pragma once
#include "../runtime/vec.h"
#include "../runtime/confighost.h"
#include "../runtime/value_scope.h"
#include "../runtime/runtime.h"
#include "d_side.h"
#include "d_object.h"


#include <string>
#include <string_view>
#include <memory>
#include <array>
#include <vector>
#include <algorithm>

namespace sqf
{
    namespace runtime
    {
        class value;
    }
    namespace types
    {
        class d_group;
        class object;
        class group : public std::enable_shared_from_this<::sqf::types::group>, public ::sqf::runtime::value_scope
        {
        public:
            class group_storage : public sqf::runtime::runtime::datastorage
            {
            private:
                std::vector<std::shared_ptr<group>> m_inner;
                size_t m_id;
            public:
                group_storage() : m_inner(), m_id(0) {}
                virtual ~group_storage() override {}
                size_t push_back(std::shared_ptr<group> obj) { m_inner.push_back(obj); return ++m_id; }
                void erase(std::shared_ptr<group> obj)
                {
                    auto it = std::find(m_inner.begin(), m_inner.end(), obj);
                    if (it != m_inner.end())
                    {
                        *it = m_inner.back();
                        m_inner.pop_back();
                    }
                }
            };
            using iterator = std::vector<std::shared_ptr<sqf::types::d_object>>::iterator;
            using const_iterator = std::vector<std::shared_ptr<sqf::types::d_object>>::const_iterator;
        private:
            ::sqf::types::d_side::side m_side;
            std::string m_group_id;
            size_t m_netid;
            std::shared_ptr<sqf::types::d_object> m_leader;
            std::vector<std::shared_ptr<sqf::types::d_object>> m_units;
            group() = default;
            group(const group& obj) = delete;
        public:
            size_t netid() const { return m_netid; }

            std::shared_ptr<sqf::types::d_object> leader() const { return m_leader; }
            void leader(std::shared_ptr<sqf::types::d_object> leader) { m_leader = leader; }

            std::string_view group_id() const { return m_group_id; }
            void group_id(std::string str) { m_group_id = str; }

            ::sqf::types::d_side::side side() const { return m_side; }
            void side(::sqf::types::d_side::side side) { m_side = side; }

            void push_back(std::shared_ptr<d_object> obj) { m_units.push_back(obj); }
            void erase(std::shared_ptr<d_object> obj) { auto it = std::find(m_units.begin(), m_units.end(), obj); if (it != m_units.end()) { *it = m_units.back(); m_units.pop_back(); } }
            void erase(std::shared_ptr<object> obj)
            {
                auto it = std::find_if(m_units.begin(), m_units.end(),
                    [obj](std::shared_ptr<d_object> it) { return it->value().get() == obj.get(); });
                if (it != m_units.end())
                {
                    *it = m_units.back();
                    m_units.pop_back();
                }
            }
            void erase(const object* obj)
            {
                auto it = std::find_if(m_units.begin(), m_units.end(),
                    [obj](std::shared_ptr<d_object> it) { return it->value().get() == obj; });
                if (it != m_units.end())
                {
                    *it = m_units.back();
                    m_units.pop_back();
                }
            }

            iterator begin() { return m_units.begin(); }
            iterator end() { return m_units.end(); }
            const_iterator begin() const { return m_units.begin(); }
            const_iterator end() const { return m_units.end(); }
            bool empty() const { return m_units.empty(); }
            size_t size() const { return m_units.size(); }

            /// <summary>
            /// Creates a new group instance.
            /// </summary>
            /// <param name="runtime"></param>
            /// <param name="classname"></param>
            /// <param name="isvehicle"></param>
            /// <returns></returns>
            static std::shared_ptr<group> create(::sqf::runtime::runtime& runtime);

            /// <summary>
            /// Destroys a given group, invalidating all living d_group instances (making them null).
            /// </summary>
            /// <param name="runtime"></param>
            void destroy(::sqf::runtime::runtime& runtime);
        };
    }
}
