#pragma once
#include "../runtime/runtime.h"
#include "dlops.h"

#include <memory>
namespace sqf::operators
{
    class dlops_storage : public ::sqf::runtime::runtime::datastorage
    {
    private:
        std::vector<std::shared_ptr<dlops>> m_dlops;
    public:
        virtual ~dlops_storage() override {}
        void push_back(std::shared_ptr<dlops> dl) { m_dlops.push_back(dl); }
        std::vector<std::shared_ptr<dlops>>::iterator begin() { return m_dlops.begin(); }
        std::vector<std::shared_ptr<dlops>>::iterator end() { return m_dlops.end(); }
    };
}