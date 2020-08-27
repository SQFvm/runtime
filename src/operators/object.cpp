#include "object.h"
#include "d_object.h"
#include "d_group.h"
#include "../runtime/d_boolean.h"
#include "../runtime/d_array.h"
#include "../runtime/d_scalar.h"

#pragma region ::sqf::types::object::soldiers

bool sqf::types::object::soldiers_::push_back(sqf::runtime::value val)
{
    if (val.is<sqf::runtime::t_object>())
    {
        return false;
    }
    return push_back(val.data<d_object>());
}

bool sqf::types::object::soldiers_::push_back(std::shared_ptr<d_object> val)
{
    if (val->is_null() || val->value()->is_vehicle() || m_inner.size() >= m_owner->m_configuration.transport_soldier)
    {
        return false;
    }
    if (val->value()->m_parent_object && !val->value()->m_parent_object->is_null())
    {
        val->value()->m_parent_object->value()->m_soldiers.erase(val);
    }
    val->value()->m_parent_object = std::make_shared<d_object>(m_owner->shared_from_this());
    m_inner.push_back(val);
    return true;
}

void sqf::types::object::soldiers_::pop_back()
{
    auto back = m_inner.back();
    if (!back->is_null())
    {
        back->value()->m_parent_object->value({});
    }
    m_inner.pop_back();
}

void sqf::types::object::soldiers_::erase(std::shared_ptr<d_object> obj)
{
    auto res = std::find(m_inner.begin(), m_inner.end(), obj);
    if (res != m_inner.end())
    {
        if (!(*res)->is_null())
        {
            (*res)->value()->m_parent_object->value({});
        }
        m_inner.erase(res);
    }
}

#pragma endregion

#pragma region ::sqf::types::object

sqf::types::object::object(sqf::runtime::config config, bool is_vehicle) :
    m_netid(~(size_t)0),
    m_config(config),
    m_is_vehicle(is_vehicle),
    m_varname(""),
    m_damage(0),
    m_group(),
    m_position({ 0,0,0 }),
    m_velocity({ 0,0,0 }),
    m_parent_object(),
    m_driver(),
    m_gunner(),
    m_commander(),
    m_soldiers(this),
    m_configuration()
{
}

void sqf::types::object::driver(std::shared_ptr<d_object> val)
{
    if (m_configuration.has_driver && (val->is_null() || !val->value()->is_vehicle()))
    {
        if (val->value()->m_parent_object && !val->value()->m_parent_object->is_null())
        {
            val->value()->m_parent_object->value()->m_soldiers.erase(val);
        }
        m_driver = val;
        if (!val->is_null())
        {
            m_driver->value()->m_parent_object = std::make_shared<d_object>(this->shared_from_this());
        }
    }
}

void sqf::types::object::gunner(std::shared_ptr<d_object> val)
{
    if (m_configuration.has_gunner && (val->is_null() || !val->value()->is_vehicle()))
    {
        if (val->value()->m_parent_object && !val->value()->m_parent_object->is_null())
        {
            val->value()->m_parent_object->value()->m_soldiers.erase(val);
        }
        m_gunner = val;
        if (!val->is_null())
        {
            m_gunner->value()->m_parent_object = std::make_shared<d_object>(this->shared_from_this());
        }
    }
}

void sqf::types::object::commander(std::shared_ptr<d_object> val)
{
    if (m_configuration.has_commander && (val->is_null() || !val->value()->is_vehicle()))
    {
        if (val->value()->m_parent_object && !val->value()->m_parent_object->is_null())
        {
            val->value()->m_parent_object->value()->m_soldiers.erase(val);
        }
        m_commander = val;
        if (!val->is_null())
        {
            m_commander->value()->m_parent_object = std::make_shared<d_object>(this->shared_from_this());
        }
    }
}

std::shared_ptr<sqf::types::object> sqf::types::object::create(::sqf::runtime::runtime& runtime, sqf::runtime::config config, bool is_vehicle)
{
    auto& storage = runtime.storage<object_storage>();
    auto sp_obj = std::shared_ptr<object>(new object(config, is_vehicle));
    auto net_id = storage.push_back(sp_obj);
    sp_obj->m_netid = net_id;
    return sp_obj;
}

void sqf::types::object::destroy(::sqf::runtime::runtime& runtime)
{
    auto& storage = runtime.storage<object_storage>();
    storage.erase(shared_from_this());
    if (m_group)
    {
        m_group->value()->erase(this);
    }
}

template<class TData, typename TType>
inline TType extract(sqf::runtime::confignav conf, TType def)
{
    if (!conf.empty())
    {
        return def;
    }
    auto val = conf->value;
    auto res = val.data_try<TData>();
    if (res)
    {
        return *res;
    }
    else
    {
        return def;
    }
}
bool sqf::types::object::update_values_from_confighost(sqf::runtime::confighost host)
{
    auto nav = m_config.navigate(host);
    m_configuration.has_driver = extract<d_boolean, bool>(nav / "hasDriver", false);

    m_configuration.has_gunner = extract<d_boolean, bool>(nav / "hasGunner", false);

    m_configuration.has_commander = extract<d_boolean, bool>(nav / "hasCommander", false);

    m_configuration.transport_soldier = (size_t)extract<d_scalar, float>(nav / "transportSoldier", false);
    m_soldiers.m_inner.resize(m_configuration.transport_soldier);

    return true;
}


float sqf::types::object::distance3dsqr(std::array<float, 3> otherpos) const
{
    return sqf::types::distance3dsqr(std::array<float, 3> { m_position.x, m_position.y, m_position.z  }, otherpos);
}
float sqf::types::object::distance3d(std::array<float, 3> otherpos) const
{
    return sqf::types::distance3d(std::array<float, 3> { m_position.x, m_position.y, m_position.z  }, otherpos);
}
float sqf::types::object::distance2dsqr(std::array<float, 2> otherpos) const
{
    return sqf::types::distance2dsqr(std::array<float, 2> { m_position.x, m_position.y  }, otherpos);
}
float sqf::types::object::distance2d(std::array<float, 2> otherpos) const
{
    return sqf::types::distance2d(std::array<float, 2> { m_position.x, m_position.y }, otherpos);
}

#pragma endregion
