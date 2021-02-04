#pragma once
#include "value.h"

#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <unordered_map>

namespace sqf::runtime
{
    class config;
    class confighost;
    class confignav;

    class config
    {
        friend class confighost;
        friend class confignav;
    public:
        static const size_t invalid_id = ~((size_t)0);
        struct container
        {
        private:
            std::vector<size_t> m_children_vec;
            std::unordered_map<std::string, size_t> m_children;
        public:
            using iterator = std::unordered_map<std::string, size_t>::iterator;
            sqf::runtime::value value;
            size_t id;
            size_t id_parent_logical;
            size_t id_parent_inherited;
            std::string name;

            container(size_t id, std::string name) noexcept : m_children_vec(), m_children(), id(id), id_parent_logical(invalid_id), id_parent_inherited(invalid_id), name(name) {}
            container() noexcept : m_children_vec(), m_children(), id(invalid_id), id_parent_logical(invalid_id), id_parent_inherited(invalid_id), name({}) {}
            container(const container& copy) = delete;
            container(container&& move) noexcept :
                m_children_vec(std::move(move.m_children_vec)),
                m_children(std::move(move.m_children)),
                value(std::move(move.value)),
                id(move.id),
                id_parent_logical(move.id_parent_logical),
                id_parent_inherited(move.id_parent_inherited),
                name(std::move(move.name)) {}

            size_t size() const { return m_children_vec.size(); }
            size_t operator[] (std::string index) { return m_children[index]; }
            size_t operator[] (std::string index) const { return m_children.at(index); }
            size_t operator[] (size_t index) { return m_children_vec[index]; }
            size_t operator[] (size_t index) const { return m_children_vec[index]; }
            iterator begin() noexcept { return m_children.begin(); }
            iterator end() noexcept { return m_children.end(); }
            iterator find(std::string index) { return m_children.find(index); }
            void push_back(std::string key, size_t target_id)
            {
                auto res = m_children.find(key);
                if (res == m_children.end())
                {
                    m_children_vec.push_back(target_id);
                }
                else if (res->second == target_id)
                {
                    return;
                }
                else
                {
                    for (auto& it : m_children_vec)
                    {
                        if (it == res->second)
                        {
                            it = target_id;
                        }
                    }
                }
                m_children[key] = target_id;
            }
        };

    private:
        size_t m_parent_inherited_id;
        size_t m_parent_logical_id;
        size_t m_container_id;
        std::string_view m_name;

        config(size_t logical, size_t container_id, size_t inherited, std::string_view name) :
            m_parent_inherited_id(inherited),
            m_parent_logical_id(logical),
            m_container_id(container_id),
            m_name(name) {}
        config(const container& cont) : config(cont.id_parent_logical, cont.id, cont.id_parent_inherited, cont.name) {}
    public:

        config() : config(invalid_id, invalid_id, invalid_id, {}) {}

        bool operator==(const config& other) { return m_container_id == other.m_container_id; }
        bool operator!=(const config& other) { return !(*this == other); }

        confignav navigate(confighost& host) const;
        std::string_view name() const { return m_name; }
        bool is_null() const { return m_container_id == invalid_id; }
        size_t container_id() const { return m_container_id; }
    };
    class confighost
    {
        friend class confignav;
        friend class config;
    public:
        using config_iterator = std::vector<config>::iterator;
    private:
        std::vector<config::container> m_containers;

    public:
        confignav root();
        confighost(const confighost& copy) = delete;
        confighost()
        {
            m_containers.emplace_back(0, "config/bin");
        }
    };
    class confignav
    {
        friend class confighost;
        friend class config;
    public:
        template<bool recursive>
        class iterator_base
        {
        private:
            confighost& m_confighost;
            size_t m_index;
            size_t m_id;
        public:
            // iterator traits
            using difference_type = size_t;
            using value_type = config;
            using pointer = value_type*;
            using reference = value_type&;
            using iterator_category = std::forward_iterator_tag;

            iterator_base(const iterator_base<recursive>& copy) :
                m_confighost(copy.m_confighost),
                m_index(copy.m_index),
                m_id(copy.m_id) {}
            iterator_base(confighost& confighost, size_t config_index) :
                m_confighost(confighost),
                m_index(0),
                m_id(config_index) {}

            iterator_base<recursive>& operator++()
            {
                while (m_id != config::invalid_id)
                {
                    auto& container = m_confighost.m_containers[m_id];
                    if (container.size() != m_index)
                    {
                        m_index++;
                        return *this;
                    }
                    else
                    {
                        m_index = 0;
                        if constexpr (recursive)
                        {
                            m_id = container.id_parent_inherited;
                        }
                        else
                        {
                            m_id = config::invalid_id;
                        }
                    }
                }
                return *this;
            }
            iterator_base<recursive> operator++(int) { iterator retval = *this; ++(*this); return retval; }
            bool operator==(iterator_base<recursive> other) const { return m_index == other.m_id; }
            bool operator!=(iterator_base<recursive> other) const { return !(*this == other); }
            value_type operator*()
            {
                auto& container = m_confighost.m_containers[m_id];
                auto actual_index = container[m_index];
                return m_confighost.m_containers[actual_index];
            }
        };
        using iterator = iterator_base<false>;
        using iterator_recursive = iterator_base<true>;
    private:
        confighost& m_confighost;
        size_t m_index;
        confignav(confighost& host, size_t current) : m_confighost(host), m_index(current) {}
    public:
        confignav(const confignav& copy) : m_confighost(copy.m_confighost), m_index(copy.m_index) {}

        
        void operator=(const confignav& nav) // Required thanks to compiler magix
        {
            m_index = nav.m_index;
        }

        bool empty() const { return m_index == config::invalid_id; }
        const config::container* operator->() const
        {
            if (!empty())
            {
                return &*(m_confighost.m_containers.begin() + m_index);
            }
            return {};
        }
        config operator*() const
        {
            if (!empty())
            {
                return { m_confighost.m_containers.at(m_index) };
            }
            return {};
        }
        operator config() const { return **this; }
        confignav operator/(std::string target) const { return lookup_in_inherited(target); }
        confignav operator/(size_t index) const { return at(index); }
        confignav at(size_t index) const
        {
            if (!empty())
            {
                auto& container = m_confighost.m_containers.at(index);
                if (index < container.size())
                {
                    return { m_confighost, container[index] };
                }
            }
            return { m_confighost, config::invalid_id };
        }
        confignav lookup_in_inherited(std::string target) const
        {
            size_t index = m_index;
            while (index != config::invalid_id)
            {
                auto& container = m_confighost.m_containers.at(index);

                auto res = container.find(target);
                if (res != container.end())
                {
                    return { m_confighost, res->second };
                }
                else
                {
                    index = container.id_parent_inherited;
                }
            }
            return { m_confighost, config::invalid_id };
        }
        confignav lookup_in_logical(std::string target) const
        {
            size_t index = m_index;
            while (index != config::invalid_id)
            {
                auto& container = m_confighost.m_containers.at(index);

                auto res = container.find(target);
                if (res != container.end())
                {
                    return { m_confighost, res->second };
                }
                else
                {
                    index = container.id_parent_logical;
                }
            }
            return { m_confighost, config::invalid_id };
        }
        confignav append_or_replace(std::string_view target, std::string_view inherited = {}) const
        {
            std::string s_target = { target.data(), target.length() };
            std::string s_inherited = { inherited.data(), inherited.length() };
            return append_or_replace(s_target, s_inherited);
        }
        confignav append_or_replace(std::string target, std::string inherited = {}) const
        {
            if (!empty())
            {
                // get config container from master-register
                auto& container = m_confighost.m_containers.at(m_index);


                // Find the targeted config ...
                auto find_res = container.find(target);
                if (find_res == container.end())
                { // ... not found
                    // Create new container
                    auto& created = m_confighost.m_containers.emplace_back(m_confighost.m_containers.size(), target); // container might be invalidated here due to m_containers resizing.

                    // Set logical parent to current container
                    created.id_parent_logical = m_index;

                    // check if inherited is empty
                    if (!inherited.empty())
                    { // it is not
                        // attempt to find inherited parent
                        auto nav = lookup_in_logical(inherited);

                        // and set it as parent
                        created.id_parent_inherited = nav.m_index; // index is invalid if parent was not found
                    }

                    // Add created container as child to parent container
                    m_confighost.m_containers.at(m_index).push_back(target, created.id);

                    // Return created container as confignav
                    return { m_confighost, created.id };
                }
                else
                { // ... found
                    // Receive existing container
                    auto& replaced = m_confighost.m_containers[find_res->second];

                    // Check if inherited is empty
                    if (!inherited.empty())
                    { // it is not
                        // Lookup inherited node and replace it
                        auto nav = lookup_in_logical(inherited);
                        replaced.id_parent_inherited = nav.m_index;
                    }

                    // Return found container as confignav
                    return { m_confighost, replaced.id };
                }
            }
            return { m_confighost, config::invalid_id };
        }
        void delete_inherited_or_replace(std::string_view target) const
        {
            std::string s_target = { target.data(), target.length() };
            delete_inherited_or_replace(s_target);
        }
        void delete_inherited_or_replace(std::string target) const
        {
            if (!empty())
            {
                auto& container = m_confighost.m_containers.at(m_index);
                container.push_back(target, config::invalid_id);
            }
        }
        bool has_inherited_with_name(std::string target) const
        {
            size_t index = m_index;
            while (index != config::invalid_id)
            {
                auto& container = m_confighost.m_containers.at(index);

                if (container.name == target)
                {
                    return true;
                }
                else
                {
                    index = container.id_parent_inherited;
                }
            }
            return false;
        }
        bool has_logical_with_name(std::string target) const
        {
            size_t index = m_index;
            while (index != config::invalid_id)
            {
                auto& container = m_confighost.m_containers.at(index);

                if (container.name == target)
                {
                    return true;
                }
                else
                {
                    index = container.id_parent_logical;
                }
            }
            return false;
        }

        confignav parent_inherited() const
        {
            if (!empty())
            {
                size_t index = m_index;
                auto& container = m_confighost.m_containers.at(index);
                return { m_confighost, container.id_parent_inherited };
            }
            return { m_confighost, config::invalid_id };
        }
        confignav parent_logical() const
        {
            if (!empty())
            {
                size_t index = m_index;
                auto& container = m_confighost.m_containers.at(index);
                return { m_confighost, container.id_parent_logical };
            }
            return { m_confighost, config::invalid_id };
        }
        void value(::sqf::runtime::value val) const
        {
            if (!empty())
            {
                size_t index = m_index;
                auto& container = m_confighost.m_containers.at(index);
                container.value = val;
            }
        }

        iterator begin() const { return { m_confighost, m_index }; }
        iterator end() const { return { m_confighost, config::invalid_id }; }

        iterator_recursive recursive_begin() const { return { m_confighost, m_index }; }
        iterator_recursive recursive_end() const { return { m_confighost, config::invalid_id }; }
    };
    inline confignav config::navigate(confighost& host) const { return { host, m_container_id }; }
    inline confignav confighost::root() { return { *this, 0 }; }
}
