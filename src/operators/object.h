#pragma once
#include "../runtime/vec.h"
#include "../runtime/confighost.h"
#include "../runtime/value_scope.h"
#include "../runtime/runtime.h"


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
        class d_object;
        class object : public std::enable_shared_from_this<::sqf::types::object>, public ::sqf::runtime::value_scope
        {
        public:
            class object_storage : public sqf::runtime::runtime::datastorage
            {
            private:
                std::vector<std::shared_ptr<object>> m_inner;
                size_t m_id;
                std::shared_ptr<object> m_player;
            public:
                object_storage() : m_inner(), m_id(0) {}
                virtual ~object_storage() override {}
                size_t push_back(std::shared_ptr<object> obj) { m_inner.push_back(obj); return ++m_id; }
                void erase(std::shared_ptr<object> obj) { auto it = std::find(m_inner.begin(), m_inner.end(), obj); if (it != m_inner.end()) { *it = m_inner.back(); m_inner.pop_back(); } }
                std::vector<std::shared_ptr<object>>::iterator begin() { return m_inner.begin(); }
                std::vector<std::shared_ptr<object>>::iterator end() { return m_inner.end(); }
                std::shared_ptr<object> player() { return m_player; }
                void player(std::shared_ptr<object> obj) { m_player = obj; }
            };
            struct configuration_
            {
                size_t transport_soldier;
                bool has_driver;
                bool has_gunner;
                bool has_commander;
            };
            class soldiers_
            {
            private:
                friend class object;
                std::vector<std::shared_ptr<d_object>> m_inner;
                object* m_owner;
                soldiers_(object* owner) : m_owner(owner) {}
            public:
                std::vector<std::shared_ptr<d_object>>::iterator begin() { return m_inner.begin(); }
                std::vector<std::shared_ptr<d_object>>::const_iterator begin() const { return m_inner.begin(); }

                std::vector<std::shared_ptr<d_object>>::iterator end() { return m_inner.end(); }
                std::vector<std::shared_ptr<d_object>>::const_iterator end() const { return m_inner.end(); }

                const std::vector<std::shared_ptr<d_object>>& vector() const { return m_inner; }

                /// <summary>
                /// Attempts to add provided value to the soldiers list.
                /// Will return true if it succeeded, false if the value provided
                /// was null, not an object, not a unit, already inside the vehicle or no slots
                /// are left to occupy.
                /// </summary>
                /// <param name="val"></param>
                /// <returns></returns>
                bool push_back(sqf::runtime::value val);

                /// <summary>
                /// Attempts to add provided objectdata to the soldiers list.
                /// Will return true if it succeeded, false if the value provided
                /// was null, not a unit, already inside the vehicle or no slots are left to occupy.
                /// </summary>
                /// <param name="val"></param>
                /// <returns></returns>
                bool push_back(std::shared_ptr<d_object> val);

                void pop_back();

                void erase(std::shared_ptr<d_object> obj);
            };
        private:
            size_t m_netid;
            sqf::runtime::config m_config;
            bool m_is_vehicle;

            std::string m_varname;
            float m_damage;
            std::shared_ptr<d_group> m_group;

            ::sqf::runtime::vec3 m_position;
            ::sqf::runtime::vec3 m_velocity;

            std::shared_ptr<d_object> m_parent_object;
            std::shared_ptr<d_object> m_driver;
            std::shared_ptr<d_object> m_gunner;
            std::shared_ptr<d_object> m_commander;
            soldiers_ m_soldiers;
            configuration_ m_configuration;

            object(sqf::runtime::config config, bool is_vehicle);
            object(const object& obj) = delete;
        public:
            sqf::runtime::config config() const { return m_config; }
            size_t netid() const { return m_netid; }
            bool is_vehicle() const { return m_is_vehicle; }
            bool alive() const { return m_damage < 1; }

            ::sqf::runtime::vec3 position() const { return m_position; }
            void position(::sqf::runtime::vec3 vec) { m_position = vec; }

            ::sqf::runtime::vec3 velocity() const { return m_velocity; }
            void velocity(::sqf::runtime::vec3 vec) { m_velocity = vec; }

            std::string_view varname() const { return m_varname; }
            void varname(std::string str) { m_varname = str; }

            std::shared_ptr<d_group> group() const { return m_group; }
            void group(std::shared_ptr<d_group> g) { m_group = g; }

            float damage() const { return m_damage; }
            void damage(float val) { m_damage = val < 0 ? 0 : val > 1 ? 1 : val; }
            void damage_by(float val) { auto newdmg = m_damage += val; damage(newdmg); }

            /// <summary>
            /// d_object this object is part of (eg. Unit X sits in Vehicle Y, Y will be parent of X)
            /// </summary>
            /// <returns></returns>
            std::shared_ptr<d_object> parent_object() const { return m_parent_object; }

            std::shared_ptr<d_object> driver() const { return m_driver; }
            void driver(std::shared_ptr<d_object> val);

            std::shared_ptr<d_object> gunner() const { return m_gunner; }
            void gunner(std::shared_ptr<d_object> val);

            std::shared_ptr<d_object> commander() const { return m_commander; }
            void commander(std::shared_ptr<d_object> val);

            soldiers_ soldiers() { return m_soldiers; }
            const configuration_ configuration() const { return m_configuration; }


            /// <summary>
            /// Creates a new object instance.
            /// </summary>
            /// <param name="runtime"></param>
            /// <param name="classname"></param>
            /// <param name="isvehicle"></param>
            /// <returns></returns>
            static std::shared_ptr<object> create(::sqf::runtime::runtime& runtime, sqf::runtime::config config, bool is_vehicle);

            /// <summary>
            /// Destroys a given object, invalidating all living d_object instances (making them null).
            /// </summary>
            /// <param name="runtime"></param>
            void destroy(::sqf::runtime::runtime& runtime);

            /// <summary>
            /// Attempts to read the vehicle config entry for this
            /// and sets some local variables according to the config.
            /// </summary>
            /// <returns>
            /// Returns TRUE on success. FALSE will be returned, if
            /// the config class for the vehicle could not be received.
            /// </returns>
            bool update_values_from_confighost(sqf::runtime::confighost host);



            float distance3dsqr(std::shared_ptr<object> obj) const { return distance3dsqr(obj->position()); }
            float distance3dsqr(const object* obj) const { return distance3dsqr(obj->position()); }
            float distance3dsqr(::sqf::runtime::vec3 obj) const { return distance3dsqr(std::array<float, 3>{ obj.x, obj.y, obj.z }); }
            float distance3dsqr(std::array<float, 3> obj) const;
            float distance3d(std::shared_ptr<object> obj) const { return distance3d(obj->position()); }
            float distance3d(const object* obj) const { return distance3d(obj->position()); }
            float distance3d(::sqf::runtime::vec3 obj) const { return distance3d(std::array<float, 3>{ obj.x, obj.y, obj.z }); }
            float distance3d(std::array<float, 3> obj) const;

            float distance2dsqr(std::shared_ptr<object> obj) const { return distance2dsqr(obj->position()); }
            float distance2dsqr(const object* obj) const { return distance2dsqr(obj->position()); }
            float distance2dsqr(::sqf::runtime::vec3 obj) const { return distance2dsqr(std::array<float, 2>{ obj.x, obj.y }); }
            float distance2dsqr(std::array<float, 2> obj) const;
            float distance2d(std::shared_ptr<object> obj) const { return distance2d(obj->position()); }
            float distance2d(const object* obj) const { return distance2d(obj->position()); }
            float distance2d(::sqf::runtime::vec3 obj) const { return distance2d(std::array<float, 2>{ obj.x, obj.y }); }
            float distance2d(std::array<float, 2> obj) const;
        };
    }
}
