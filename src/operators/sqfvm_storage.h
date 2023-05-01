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

class sqfvm_storage : public sqf::runtime::runtime::datastorage
{
private:
    bool m_is_server;
public:
    sqfvm_storage() : m_is_server(true) {}
    bool is_server() { return m_is_server; }
    void is_server(bool value) { m_is_server = value; }
};