#pragma once
#include "tokenizer.hpp"

#include <cstdint>
#include <string_view>
#include <optional>
#include <memory>
#include <vector>
#include <new>
namespace sqf::parser::sqf::bison
{
    class parser;
}
namespace sqf::parser::sqf::ast
{
    enum class nodetype
    {
        ////////////////////
        // Contain string //
        ////////////////////

        IDENT,
        NUMBER,
        HEXNUMBER,
        STRING,

        /////////////////////
        // Contain nothing //
        /////////////////////

        BOOLEAN_TRUE,
        BOOLEAN_FALSE,


        //////////////////////
        // Contain children //
        //////////////////////

        ASSIGNMENT,
        ASSIGNMENT_LOCAL,
        CODE,
        ARRAY,
        EXPN,
        EXPU,
        EXPB0,
        EXPB1,
        EXPB2,
        EXPB3,
        EXPB4,
        EXPB5,
        EXPB6,
        EXPB7,
        EXPB8,
        EXPB9
    };
    struct content
    {
        size_t line;
        size_t column;
        size_t offset;
        std::string_view path;
        std::string_view content;
    };
    struct content_container
    {
        size_t line;
        size_t column;
        size_t offset;
        uint16_t path_id;
        size_t content_length;
        char* content_data;
    };
    class host;
    class node;
    class node
    {
        nodetype m_type;
        size_t m_data;
        size_t m_previous_node;
        size_t m_next_node;
        friend class host;
    public:
        template<size_t index> const node* at(const host& h) const;
        const node* previous_node(const host& h) const;
        const node* next_node(const host& h) const;
        content get(const host& h) const;
        nodetype type() const { return m_type; }
        size_t position(const host& h) const;
    };
    class host
    {
        uint8_t* m_data;
        uint8_t* m_end;
        size_t m_size;
        size_t m_capacity;
        std::vector<std::string> m_paths;
        friend class node;
        friend class bison::parser;

    public:
        host() : m_data(nullptr), m_end(nullptr), m_size(0), m_capacity(0) { }

        // Looks up local paths wether it already exists. If it does, it returns a id for it.
        // If it does not, adds it to the local paths and returns the id of the added local path.
        //
        // ERROR - If there are more paths registered then available IDs, UINT16_MAX is returned as error.
        [[nodiscard]] uint16_t pathid(const std::string& str)
        {
            auto it = std::find(m_paths.begin(), m_paths.end(), str);
            if (m_paths.end() != it) [[likely]]
            {
                return it - m_paths.begin();
            }
            else if (m_paths.size() < UINT16_MAX)
            {
                m_paths.push_back(str);
                return m_paths.size() - 1;
            }
            else [[unlikely]]
            {
                return UINT16_MAX;
            }
        }
        [[nodiscard]] bool resize(size_t size)
        {
            if (size > m_capacity) [[likely]]
            {
                auto tmp = new (std::nothrow) uint8_t[size];
                if (!tmp) [[unlikely]]
                {
                    return false;
                }
                if (m_data) [[likely]]
                {
                    std::copy(m_data, m_end, tmp);
                }
                m_data = tmp;
                m_capacity = size;
                m_end = m_data + m_size;
                return true;
            }
        }
        [[nodiscard]] node* get_node(size_t offset)
        {
            if (offset < m_size) [[likely]]
            {
                return reinterpret_cast<node*>(m_data + offset);
            }
            else
            {
                return nullptr;
            }
        }
        [[nodiscard]] node* create_node(nodetype type)
        {
            if (!ensure((m_capacity + sizeof(node)) * 2)) [[unlikely]]
            {
                return nullptr;
            }
            auto tmp = m_end;
            m_end += sizeof(node);
            auto n = reinterpret_cast<node*>(tmp);
            n->m_type = type;
            n->m_previous_node = 0;
            n->m_next_node = 0;
            return n;
        }
        [[nodiscard]] node* create_node(nodetype type, tokenizer::token t)
        {
            // Create the node we return later
            auto node = create_node(type);
            if (!node) [[unlikely]]
            {
                return nullptr;
            }

            // Set its data index to our size
            node->m_data = m_size;

            // Ensure we have the size we need available
            ensure(sizeof(content_container) - sizeof(char*) + (sizeof(char) * t.contents.length()));

            // Get offset to data
            auto data = reinterpret_cast<content_container*>(m_end);

            // Reserve the data
            reserve(sizeof(size_t) + (sizeof(char) * t.contents.length()));

            // Set the data contents
            data->column = t.column;
            data->line = t.line;
            data->offset = t.offset;
            data->path_id = pathid(*t.path);
            data->content_length = t.contents.length();
            std::copy(t.contents.begin(), t.contents.end(), data->content_data);

            // Return node
            return node;
        }

        // Ensures that at least the provided size is available
        // as capacity remaining.
        [[nodiscard]] bool ensure(size_t size)
        {
            return resize((m_capacity + size) * 2);
        }

        // Increases the hosts size by the provided size and ensures prior
        // that the capacity is matching.
        [[nodiscard]] bool reserve(size_t size)
        {
            if (ensure(size)) [[likely]]
            {
                m_size += size;
                m_end += size;
                return true;
            }
            else
            {
                return false;
            }
        }
    };

    template<size_t index> const node* node::at(const host& h) const
    {
        // Get offset to data
        auto data = (h.m_data + m_data);

        // Navigate to the desired child
        for (size_t i = 0; i < index; i++)
        {
            // Add childs data size
            data += reinterpret_cast<node*>(data)->m_size;
            // Add size of node
            data += sizeof(node);
        }
        // return child at index
        return reinterpret_cast<node*>(data);
    }
    content node::get(const host& h) const
    {

        // Get offset to data
        auto data = reinterpret_cast<content_container*>(h.m_data + m_data);

        // return data at index
        content c;
        c.column = data->column;
        c.content = std::string_view(data->content_data, data->content_length);
        c.line = data->line;
        c.offset = data->offset;
        c.path = h.m_paths[data->path_id];
        return c;
    }
    const node* node::previous_node(const host& h) const
    {
        if (m_previous_node)
        {
            // Get offset to data
            auto data = (h.m_data + m_previous_node);

            // return data as node
            return reinterpret_cast<node*>(data);
        }
        else
        {
            return nullptr;
        }
    }
    const node* node::next_node(const host& h) const
    {
        if (m_next_node)
        {
            // Get offset to data
            auto data = (h.m_data + m_next_node);

            // return data as node
            return reinterpret_cast<node*>(data);
        }
        else
        {
            return nullptr;
        }
    }
    size_t node::position(const host& h) const
    {
        return reinterpret_cast<const uint8_t*>(this) - h.m_data;
    }
}