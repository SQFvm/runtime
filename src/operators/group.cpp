#include "group.h"

std::shared_ptr<sqf::types::group> sqf::types::group::create(::sqf::runtime::runtime& runtime)
{
    auto& storage = runtime.storage<group_storage>();
    auto sp_obj = std::shared_ptr<group>(new sqf::types::group());
    auto net_id = storage.push_back(sp_obj);
    sp_obj->m_netid = net_id;
    return sp_obj;
}

void sqf::types::group::destroy(::sqf::runtime::runtime& runtime)
{
    auto& storage = runtime.storage<group_storage>();
    storage.erase(shared_from_this());
}