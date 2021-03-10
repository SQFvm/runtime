#include "sqc_parser.h"

#include "tokenizer.h"
#include "parser.tab.hh"

#include "../opcodes/common.h"
#include "../runtime/d_array.h"
#include "../runtime/d_string.h"
#include "../runtime/d_scalar.h"
#include "../runtime/d_boolean.h"
#include "../runtime/d_code.h"
#include <algorithm>
#include <charconv>
#include <sstream>
#include <functional>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace sqf::sqc::util
{
    static std::string strip_formatted(std::string_view sview)
    {
        if (sview.length() == 0) { return {}; }
        if (sview.length() == 2) { return {}; }

        std::vector<char> arr;
        arr.reserve(sview.size());
        for (size_t i = 1; i < sview.length() - 1; i++)
        {
            char c = sview[i];
            if (::sqf::parser::util::is_match_repeated<2, '"'>(sview.data() + i) ||
                ::sqf::parser::util::is_match_repeated<2, '{'>(sview.data() + i) ||
                ::sqf::parser::util::is_match_repeated<2, '}'>(sview.data() + i))
            { // Double "", {{, }} have to be escaped
                i++;
            }
            arr.emplace_back(c);
        }
        return std::string(arr.data(), arr.size());
    }
    class setbuilder {
    public:
        enum position
        {
            before,
            current,
            after
        };
    private:
        struct region_impl
        {
            friend class setbuilder;
            setbuilder& owner;
            std::vector<::sqf::runtime::instruction::sptr> pre;
            std::vector<::sqf::runtime::instruction::sptr> actual;
            std::vector<::sqf::runtime::instruction::sptr> post;
            region_impl(setbuilder& owning) : owner(owning) {}
            void push_back(::sqf::runtime::instruction::sptr ptr, position pos)
            {
                switch (pos)
                {
                case before:
                    pre.push_back(ptr);
                    break;
                case current:
                    actual.push_back(ptr);
                    break;
                case after:
                    post.push_back(ptr);
                    break;
                }
            }
            ~region_impl()
            {
                owner.inner.insert(owner.inner.end(), pre.begin(), pre.end());
                owner.inner.insert(owner.inner.end(), actual.begin(), actual.end());
                owner.inner.insert(owner.inner.end(), post.begin(), post.end());
            }
        };
    public:
        class region
        {
            friend class setbuilder;
            setbuilder& m_ref;
            region(const region& copy) = delete;
        public:
            region(setbuilder& sb) : m_ref(sb)
            {
                m_ref.m_regions.emplace_back(m_ref);
            }
            ~region()
            {
                m_ref.m_regions.pop_back();
            }
        };
        class parent_lock
        {
            friend class setbuilder;
            setbuilder& m_ref;
            parent_lock(const parent_lock& copy) = delete;
        public:
            parent_lock(setbuilder& sb, ::sqf::sqc::bison::astkind kind) : m_ref(sb)
            {
                m_ref.m_parents.emplace_back(kind);
            }
            ~parent_lock()
            {
                m_ref.m_parents.pop_back();
            }
        };
    private:
        std::vector<::sqf::runtime::instruction::sptr> inner;
        std::vector<region_impl> m_regions;
        std::vector<::sqf::sqc::bison::astkind> m_parents;
        std::string_view m_contents;
        setbuilder(std::string_view contents, std::vector<::sqf::sqc::bison::astkind> m_parents) : m_parents(m_parents), m_contents(contents) {}
    public:
        setbuilder(std::string_view contents) : m_contents(contents) {}

        std::vector<::sqf::runtime::instruction::sptr>& instructions()
        {
            return inner;
        }

        setbuilder create_from() const
        {
            return { m_contents, m_parents };
        }
        void push_back(const ::sqf::sqc::tokenizer::token& t, ::sqf::runtime::instruction::sptr ptr, position pos = current)
        {
            ptr->diag_info({ t.line, t.column, t.offset, { t.path, {} }, ::sqf::runtime::parser::sqf::create_code_segment(m_contents, t.offset, t.contents.length()) });
            if (m_regions.empty())
            {
                inner.push_back(ptr);
            }
            else
            {
                m_regions.back().push_back(ptr, pos);
            }
        }
        void push_back(const ::sqf::sqc::tokenizer::token& t, size_t custom_length, ::sqf::runtime::instruction::sptr ptr, position pos = current)
        {
            ptr->diag_info({ t.line, t.column, t.offset, { t.path, {} }, ::sqf::runtime::parser::sqf::create_code_segment(m_contents, t.offset, custom_length) });
            if (m_regions.empty())
            {
                inner.push_back(ptr);
            }
            else
            {
                m_regions.back().push_back(ptr, pos);
            }
        }
        operator ::sqf::runtime::instruction_set() const
        {
            return { inner };
        }
        parent_lock lock_parent(::sqf::sqc::bison::astkind kind)
        {
            return parent_lock(*this, kind);
        }
        ::sqf::sqc::bison::astkind parent() const
        {
            if (m_parents.empty()) return {};
            return m_parents.back();
        }
        bool has_parent(::sqf::sqc::bison::astkind kind, size_t max_hirarchy = ~0) const
        {
            max_hirarchy = m_parents.size() < max_hirarchy ? m_parents.size() : max_hirarchy;
            for (size_t i = m_parents.size() - 1; max_hirarchy-- != 0; --i)
            {
                if (m_parents[i] == kind) return true;
            }
            return false;
        }
    };
}

void sqf::sqc::parser::to_assembly(::sqf::runtime::runtime& runtime, util::setbuilder& set, std::vector<emplace>& locals, const ::sqf::sqc::bison::astnode& node)
{
    bool is_top_level = set.has_parent(::sqf::sqc::bison::astkind::STATEMENTS, 2)
        || set.has_parent(::sqf::sqc::bison::astkind::FUNCTION, 2)
        || set.has_parent(::sqf::sqc::bison::astkind::FUNCTION_DECLARATION, 2)
        || set.has_parent(::sqf::sqc::bison::astkind::FINAL_FUNCTION_DECLARATION, 2);


    util::setbuilder::position icpp_pos;
    float icpp_value;
    auto parent_lock = set.lock_parent(node.kind);
    switch (node.kind)
    {
    case ::sqf::sqc::bison::astkind::RETURN: {
        if (is_top_level)
        {
            if (!node.children.empty())
            {
                to_assembly(runtime, set, locals, node.children[0]);
            }
        }
        else
        {
            if (node.children.empty())
            {
                set.push_back(node.token, std::make_shared<opcodes::push>(key_scopename_function));
                set.push_back(node.token, std::make_shared<opcodes::call_unary>("breakout"s));
            }
            else
            {
                util::setbuilder::region __region(set);
                to_assembly(runtime, set, locals, node.children[0]);
                set.push_back(node.token, std::make_shared<opcodes::push>(key_scopename_function));
                set.push_back(node.token, std::make_shared<opcodes::call_binary>("breakout"s, (short)4));
            }
        }
    } break;
    case ::sqf::sqc::bison::astkind::THROW: {
        util::setbuilder::region __region(set);
        to_assembly(runtime, set, locals, node.children[0]);
        set.push_back(node.token, std::make_shared<opcodes::call_unary>("throw"s));
    } break;
    case ::sqf::sqc::bison::astkind::ASSIGNMENT: {
        util::setbuilder::region __region(set);
        // Push Value
        to_assembly(runtime, set, locals, node.children[1]);

        // Assign Value
        std::string var(node.children[0].token.contents);
        auto tmp = var;
        std::transform(var.begin(), var.end(), var.begin(), [](char c) { return (char)std::tolower(c); });
        auto fres = std::find_if(locals.begin(), locals.end(), [&var](auto& it) { return it.ident == var; });
        if (fres != locals.end())
        {
            set.push_back(node.children[0].token, std::make_shared<opcodes::assign_to>(fres->replace));
        }
        else
        {
            set.push_back(node.children[0].token, std::make_shared<opcodes::assign_to>(tmp));
        }
    } break;
    case ::sqf::sqc::bison::astkind::ASSIGNMENT_PLUS:
    case ::sqf::sqc::bison::astkind::ASSIGNMENT_MINUS:
    case ::sqf::sqc::bison::astkind::ASSIGNMENT_STAR:
    case ::sqf::sqc::bison::astkind::ASSIGNMENT_SLASH: {
        util::setbuilder::region __region(set);
        std::string var(node.children[0].token.contents);
        auto tmp = var;
        std::transform(var.begin(), var.end(), var.begin(), [](char c) { return (char)std::tolower(c); });

        // Push Left-Value (self)
        auto fres = std::find_if(locals.begin(), locals.end(), [&var](auto& it) { return it.ident == var; });
        if (fres != locals.end())
        {
            set.push_back(node.children[0].token, std::make_shared<opcodes::get_variable>(fres->replace));
        }
        else
        {
            set.push_back(node.children[0].token, std::make_shared<opcodes::get_variable>(tmp));
        }

        // Push Right-Value
        to_assembly(runtime, set, locals, node.children[1]);

        switch (node.kind)
        {
        case ::sqf::sqc::bison::astkind::ASSIGNMENT_PLUS:
            // Emit "+"
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("+"s, (short)6)); 
            break;
        case ::sqf::sqc::bison::astkind::ASSIGNMENT_MINUS:
            // Emit "-"
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
            break;
        case ::sqf::sqc::bison::astkind::ASSIGNMENT_STAR:
            // Emit "*"
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("*"s, (short)7));
            break;
        case ::sqf::sqc::bison::astkind::ASSIGNMENT_SLASH:
            // Emit "/"
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("/"s, (short)7));
            break;
        default:
            break;
        }

        // Assign Value
        fres = std::find_if(locals.begin(), locals.end(), [&var](auto& it) { return it.ident == var; });
        if (fres != locals.end())
        {
            set.push_back(node.children[0].token, std::make_shared<opcodes::assign_to>(fres->replace));
        }
        else
        {
            set.push_back(node.children[0].token, std::make_shared<opcodes::assign_to>(tmp));
        }
    } break;
    case ::sqf::sqc::bison::astkind::OP_ARRAY_SET: {
        util::setbuilder::region __region(set);
        // Handle OP_ARRAY_GET
        {
            // Push actual array onto value stack
            to_assembly(runtime, set, locals, node.children[0].children[0]);

            // Push Index-Expression to stack
            to_assembly(runtime, set, locals, node.children[0].children[1]);
        }
        // Push Value-Expression to stack
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "makeArray" instruction to craft the right-handed argument
        set.push_back(node.token, std::make_shared<opcodes::make_array>(2));

        // Emit "set" to perform the array assignment
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("set"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_ARRAY_SET_PLUS:
    case ::sqf::sqc::bison::astkind::OP_ARRAY_SET_MINUS:
    case ::sqf::sqc::bison::astkind::OP_ARRAY_SET_STAR:
    case ::sqf::sqc::bison::astkind::OP_ARRAY_SET_SLASH: {
        util::setbuilder::region __region(set);
        // Push actual array onto value stack (LEFT from set)
        to_assembly(runtime, set, locals, node.children[0].children[0]);

        { // RIGHT from set
            // Push Index-Expression to stack
            to_assembly(runtime, set, locals, node.children[0].children[1]);

            // Push actual value onto value stack (LEFT from op)
            to_assembly(runtime, set, locals, node.children[0]);

            // Push Value-Expression to stack (RIGHT from op)
            to_assembly(runtime, set, locals, node.children[1]);
            switch (node.kind)
            {
            case ::sqf::sqc::bison::astkind::OP_ARRAY_SET_PLUS:
                // Emit "+"
                set.push_back(node.token, std::make_shared<opcodes::call_binary>("+"s, (short)6));
                break;
            case ::sqf::sqc::bison::astkind::OP_ARRAY_SET_MINUS:
                // Emit "-"
                set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
                break;
            case ::sqf::sqc::bison::astkind::OP_ARRAY_SET_STAR:
                // Emit "*"
                set.push_back(node.token, std::make_shared<opcodes::call_binary>("*"s, (short)7));
                break;
            case ::sqf::sqc::bison::astkind::OP_ARRAY_SET_SLASH:
                // Emit "/"
                set.push_back(node.token, std::make_shared<opcodes::call_binary>("/"s, (short)7));
                break;
            default:
                break;
            }

            // Emit "makeArray" instruction to craft the right-handed argument
            set.push_back(node.token, std::make_shared<opcodes::make_array>(2));
        }
        // Emit "set" to perform the array assignment
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("set"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_ARRAY_GET: {
        // Push actual array onto value stack
        to_assembly(runtime, set, locals, node.children[0]);

        // Push Index-Expression to stack
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "select" to perform the array index access
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("select"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_ACCESS_SET: {
        util::setbuilder::region __region(set);
        // Handle OP_ACCESS_GET
        {
            // Push actual array onto value stack
            to_assembly(runtime, set, locals, node.children[0].children[0]);

            // Push ident as string to stack
            std::string access(node.children[0].children[1].token.contents.begin(), node.children[0].children[1].token.contents.end());
            set.push_back(node.children[0].children[1].token, std::make_shared<opcodes::push>(access));
        }
        // Push Value-Expression to stack
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "makeArray" instruction to craft the right-handed argument
        set.push_back(node.token, std::make_shared<opcodes::make_array>(2));

        // Emit "set" to perform the array assignment
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("set"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_ACCESS_SET_PLUS:
    case ::sqf::sqc::bison::astkind::OP_ACCESS_SET_MINUS:
    case ::sqf::sqc::bison::astkind::OP_ACCESS_SET_STAR:
    case ::sqf::sqc::bison::astkind::OP_ACCESS_SET_SLASH: {
        util::setbuilder::region __region(set);
        // Push actual array onto value stack (LEFT from set)
        to_assembly(runtime, set, locals, node.children[0].children[0]);

        { // RIGHT from set
            // Push ident as string to stack
            std::string access(node.children[0].children[1].token.contents.begin(), node.children[0].children[1].token.contents.end());
            set.push_back(node.children[0].children[1].token, std::make_shared<opcodes::push>(access));

            // Push actual value onto value stack (LEFT from op)
            to_assembly(runtime, set, locals, node.children[0]);

            // Push Value-Expression to stack (RIGHT from op)
            to_assembly(runtime, set, locals, node.children[1]);
            switch (node.kind)
            {
            case ::sqf::sqc::bison::astkind::OP_ACCESS_SET_PLUS:
                // Emit "+"
                set.push_back(node.token, std::make_shared<opcodes::call_binary>("+"s, (short)6));
                break;
            case ::sqf::sqc::bison::astkind::OP_ACCESS_SET_MINUS:
                // Emit "-"
                set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
                break;
            case ::sqf::sqc::bison::astkind::OP_ACCESS_SET_STAR:
                // Emit "*"
                set.push_back(node.token, std::make_shared<opcodes::call_binary>("*"s, (short)7));
                break;
            case ::sqf::sqc::bison::astkind::OP_ACCESS_SET_SLASH:
                // Emit "/"
                set.push_back(node.token, std::make_shared<opcodes::call_binary>("/"s, (short)7));
                break;
            default:
                break;
            }

            // Emit "makeArray" instruction to craft the right-handed argument
            set.push_back(node.token, std::make_shared<opcodes::make_array>(2));
        }
        // Emit "set" to perform the array assignment
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("set"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_ACCESS_GET: {
        // Push actual array onto value stack
        to_assembly(runtime, set, locals, node.children[0]);

        // Push ident as string to stack
        std::string access(node.children[1].token.contents.begin(), node.children[1].token.contents.end());
        set.push_back(node.children[1].token, std::make_shared<opcodes::push>(access));

        // Emit "select" to perform the array index access
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("get"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_RANGE_INDEX: {
        // Push actual array/string onto value stack
        to_assembly(runtime, set, locals, node.children[0]);

        // Push Index-Expression to stack
        to_assembly(runtime, set, locals, node.children[1]);

        // Make array out of it
        set.push_back(node.token, std::make_shared<opcodes::make_array>(1));

        // Emit "select" to perform the array index access
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("select"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_RANGE_INDEX_LENGTH: {
        // Push actual array/string onto value stack
        to_assembly(runtime, set, locals, node.children[0]);

        // Push Index-Expression to stack
        to_assembly(runtime, set, locals, node.children[1]);

        // Push Length-Expression to stack
        {
            to_assembly(runtime, set, locals, node.children[2]);
            to_assembly(runtime, set, locals, node.children[1]);
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
        }

        // Make array out of them
        set.push_back(node.token, std::make_shared<opcodes::make_array>(2));

        // Emit "select" to perform the array index access
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("select"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_RANGE_LENGTH: {
        // Push actual array/string onto value stack
        to_assembly(runtime, set, locals, node.children[0]);

        // Push Index-Expression to stack
        set.push_back(node.children[1].token, std::make_shared<opcodes::push>(std::make_shared<::sqf::types::d_scalar>(0)));

        // Push Length-Expression to stack
        to_assembly(runtime, set, locals, node.children[1]);

        // Make array out of them
        set.push_back(node.token, std::make_shared<opcodes::make_array>(2));

        // Emit "select" to perform the array index access
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("select"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_RANGE_INDEX_RLENGTH: {
        // Push actual array/string onto value stack
        to_assembly(runtime, set, locals, node.children[0]);

        // Push Index-Expression to stack
        to_assembly(runtime, set, locals, node.children[1]);

        // Push Length-Expression to stack
        {
            to_assembly(runtime, set, locals, node.children[0]);
            set.push_back(node.token, std::make_shared<opcodes::call_unary>("count"s));
            to_assembly(runtime, set, locals, node.children[2]);
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
            to_assembly(runtime, set, locals, node.children[1]);
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
        }

        // Make array out of them
        set.push_back(node.token, std::make_shared<opcodes::make_array>(2));

        // Emit "select" to perform the array index access
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("select"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_RANGE_RINDEX_LENGTH: {
        // Push actual array/string onto value stack
        to_assembly(runtime, set, locals, node.children[0]);

        // Push Index-Expression to stack
        {
            to_assembly(runtime, set, locals, node.children[0]);
            set.push_back(node.token, std::make_shared<opcodes::call_unary>("count"s));
            to_assembly(runtime, set, locals, node.children[1]);
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
        }

        // Push Length-Expression to stack
        {
            to_assembly(runtime, set, locals, node.children[2]);
            to_assembly(runtime, set, locals, node.children[0]);
            set.push_back(node.token, std::make_shared<opcodes::call_unary>("count"s));
            to_assembly(runtime, set, locals, node.children[1]);
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
        }

        // Make array out of them
        set.push_back(node.token, std::make_shared<opcodes::make_array>(2));

        // Emit "select" to perform the array index access
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("select"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_RANGE_RINDEX: {
        // Push actual array/string onto value stack
        to_assembly(runtime, set, locals, node.children[0]);

        // Push Reversed-Index-Expression to stack
        {
            to_assembly(runtime, set, locals, node.children[0]);
            set.push_back(node.token, std::make_shared<opcodes::call_unary>("count"s));
            to_assembly(runtime, set, locals, node.children[1]);
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
        }

        // Make array out of it
        set.push_back(node.token, std::make_shared<opcodes::make_array>(1));

        // Emit "select" to perform the array index access
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("select"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_RANGE_RINDEX_RLENGTH: {
        // Push actual array/string onto value stack
        to_assembly(runtime, set, locals, node.children[0]);

        // Push Index-Expression to stack
        {
            to_assembly(runtime, set, locals, node.children[0]);
            set.push_back(node.token, std::make_shared<opcodes::call_unary>("count"s));
            to_assembly(runtime, set, locals, node.children[1]);
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
        }

        // Push Length-Expression to stack
        {
            to_assembly(runtime, set, locals, node.children[0]);
            set.push_back(node.token, std::make_shared<opcodes::call_unary>("count"s));
            to_assembly(runtime, set, locals, node.children[2]);
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
            to_assembly(runtime, set, locals, node.children[0]);
            set.push_back(node.token, std::make_shared<opcodes::call_unary>("count"s));
            to_assembly(runtime, set, locals, node.children[1]);
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
        }

        // Make array out of them
        set.push_back(node.token, std::make_shared<opcodes::make_array>(2));

        // Emit "select" to perform the array index access
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("select"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_RANGE_RLENGTH: {
        // Push actual array/string onto value stack
        to_assembly(runtime, set, locals, node.children[0]);

        // Push Index-Expression to stack
        set.push_back(node.children[1].token, std::make_shared<opcodes::push>(std::make_shared<::sqf::types::d_scalar>(0)));

        // Push Length-Expression to stack
        {
            to_assembly(runtime, set, locals, node.children[0]);
            set.push_back(node.token, std::make_shared<opcodes::call_unary>("count"s));
            to_assembly(runtime, set, locals, node.children[1]);
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
        }

        // Make array out of them
        set.push_back(node.token, std::make_shared<opcodes::make_array>(2));

        // Emit "select" to perform the array index access
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("select"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::DECLARATION: {
        // Push assigned value
        to_assembly(runtime, set, locals, node.children[1]);

        // Assign variable
        std::string var(node.children[0].token.contents);
        auto lvar = "_"s + var;
        set.push_back(node.children[0].token, std::make_shared<opcodes::assign_to_local>(lvar));

        // Add variable to locals
        std::transform(var.begin(), var.end(), var.begin(), [](char c) { return (char)std::tolower(c); });
        locals.push_back({ var, lvar });
        if (runtime.sqfop_exists_nular(var))
        {
            log(logmessage::runtime::ErrorMessage({}, "SQC", "Hiding nular operator."));
        }
        else if (runtime.sqfop_exists_unary(var))
        {
            log(logmessage::runtime::ErrorMessage({}, "SQC", "Hiding unary operator."));
        }
        else if (runtime.sqfop_exists_binary(var))
        {
            log(logmessage::runtime::ErrorMessage({}, "SQC", "Hiding binary operator."));
        }
    } break;
    case ::sqf::sqc::bison::astkind::FORWARD_DECLARATION: {
        // Push assigned value
        to_assembly(runtime, set, locals, node.children[1]);

        // Declare variable
        std::string var(node.children[0].token.contents);
        auto lvar = "_"s + var;
        set.push_back(node.children[0].token, std::make_shared<opcodes::push>(lvar));
        set.push_back(node.token, std::make_shared<opcodes::call_unary>("private"s));

        // Add variable to locals
        std::transform(var.begin(), var.end(), var.begin(), [](char c) { return (char)std::tolower(c); });
        locals.push_back({ var ,lvar });
    } break;
    case ::sqf::sqc::bison::astkind::FUNCTION_DECLARATION: {
        auto local_set = set.create_from();
        std::vector<emplace> new_locals;
        local_set.push_back(node.token, std::make_shared<opcodes::push>(key_scopename_function));
        local_set.push_back(node.token, std::make_shared<opcodes::call_unary>("scopename"));

        to_assembly(runtime, local_set, new_locals, node.children[1]);
        if (!node.children[2].children.empty())
        {
            auto& codeset = node.children[2];
            auto lastChild = codeset.children.begin() + 1;
            for (auto it = codeset.children.begin(); it != codeset.children.end(); ++it)
            {
                util::setbuilder::region __region(local_set);
                if (it == lastChild && it->kind == ::sqf::sqc::bison::astkind::RETURN)
                {
                    if (!it->children.empty())
                    {
                        to_assembly(runtime, local_set, new_locals, it->children[0]);
                    }
                }
                else
                {
                    to_assembly(runtime, local_set, new_locals, *it);
                }
            }
        }
        set.push_back(node.children[0].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set }));
        set.push_back(node.token, std::make_shared<opcodes::assign_to>(std::string(node.children[0].token.contents)));
    } break;
    case ::sqf::sqc::bison::astkind::FINAL_FUNCTION_DECLARATION: {
        auto local_set = set.create_from();
        std::vector<emplace> new_locals;
        local_set.push_back(node.token, std::make_shared<opcodes::push>(key_scopename_function));
        local_set.push_back(node.token, std::make_shared<opcodes::call_unary>("scopename"));

        to_assembly(runtime, local_set, new_locals, node.children[1]);
        if (!node.children[2].children.empty())
        {
            auto& codeset = node.children[2];
            auto lastChild = codeset.children.begin() + 1;
            for (auto it = codeset.children.begin(); it != codeset.children.end(); ++it)
            {
                util::setbuilder::region __region(local_set);
                if (it == lastChild && it->kind == ::sqf::sqc::bison::astkind::RETURN)
                {
                    if (!it->children.empty())
                    {
                        to_assembly(runtime, local_set, new_locals, it->children[0]);
                    }
                }
                else
                {
                    to_assembly(runtime, local_set, new_locals, *it);
                }
            }
        }

        // Push instructions as string
        auto code = std::make_shared<::sqf::types::d_code>(runtime::instruction_set{ local_set });
        auto sqf = code->to_string_sqf();
        if (sqf.length() >= 2)
        {
            sqf = sqf.substr(1, sqf.length() - 2);
        }
        set.push_back(node.children[0].token, std::make_shared<opcodes::push>(sqf));

        // Emit "compileFinal"
        set.push_back(node.token, std::make_shared<opcodes::call_unary>("compilefinal"));

        // Assign to variable
        set.push_back(node.token, std::make_shared<opcodes::assign_to>(std::string(node.children[0].token.contents)));
    } break;
    case ::sqf::sqc::bison::astkind::FUNCTION: {
        auto local_set = set.create_from();
        std::vector<emplace> new_locals = locals;
        local_set.push_back(node.token, std::make_shared<opcodes::push>(key_scopename_function));
        local_set.push_back(node.token, std::make_shared<opcodes::call_unary>("scopename"));

        to_assembly(runtime, local_set, new_locals, node.children[0]);
        if (!node.children[1].children.empty())
        {
            auto& codeset = node.children[1];
            auto lastChild = codeset.children.begin() + 1;
            for (auto it = codeset.children.begin(); it != codeset.children.end(); ++it)
            {
                util::setbuilder::region __region(local_set);
                if (it == lastChild && it->kind == ::sqf::sqc::bison::astkind::RETURN)
                {
                    if (!it->children.empty())
                    {
                        to_assembly(runtime, local_set, new_locals, it->children[0]);
                    }
                }
                else
                {
                    to_assembly(runtime, local_set, new_locals, *it);
                }
            }
        }
        set.push_back(node.token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set }));
    } break;
    case ::sqf::sqc::bison::astkind::ARGLIST: {
        size_t param_count = 0;
        for (auto child : node.children)
        {
            // Push the variable
            auto var = std::string(child.token.contents);
            std::transform(var.begin(), var.end(), var.begin(), [](char c) { return (char)std::tolower(c); });
            if (child.kind == ::sqf::sqc::bison::astkind::ARGITEM_EMPLACE)
            {
                auto lvar = types::d_string::from_sqf(child.children[0].token.contents);
                locals.push_back({ var, lvar });
            }
            else
            {
                auto lvar = "_"s + var;
                locals.push_back({ var, lvar });
                set.push_back(child.token, std::make_shared<opcodes::push>(lvar));
                param_count++;

                // Handle different argitem cases
                switch (child.kind)
                {
                case ::sqf::sqc::bison::astkind::ARGITEM: { /* do nothing as we are already done */ } break;
                case ::sqf::sqc::bison::astkind::ARGITEM_DEFAULT: {
                    // push default value
                    to_assembly(runtime, set, locals, child.children[0]);

                    // Make array 
                    set.push_back(node.token, std::make_shared<opcodes::make_array>(2));
                } break;
                case ::sqf::sqc::bison::astkind::ARGITEM_TYPE: {
                    // push default value
                    set.push_back(child.token, std::make_shared<opcodes::call_nular>("nil"));

                    // Check most common data-types & push default type of it
                    auto dataType = std::string(child.children[0].token.contents);
                    std::transform(dataType.begin(), dataType.end(), dataType.begin(), [](char c) { return (char)std::tolower(c); });
                    if (dataType == "array") { set.push_back(child.token, std::make_shared<opcodes::make_array>(0)); }
                    else if (dataType == "bool") { set.push_back(child.token, std::make_shared<opcodes::push>(false)); }
                    else if (dataType == "code") { set.push_back(child.token, std::make_shared<opcodes::push>(std::make_shared<::sqf::types::d_code>())); }
                    else if (dataType == "config") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("confignull")); }
                    else if (dataType == "control") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("controlnull")); }
                    else if (dataType == "display") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("displaynull")); }
                    else if (dataType == "group") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("grpnull")); }
                    else if (dataType == "location") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("locationnull")); }
                    else if (dataType == "object") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("objnull")); }
                    else if (dataType == "scalar") { set.push_back(child.token, std::make_shared<opcodes::push>(0)); }
                    else if (dataType == "script") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("scriptnull")); }
                    else if (dataType == "side") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("civilian")); }
                    else if (dataType == "string") { set.push_back(child.token, std::make_shared<opcodes::push>("")); }
                    else if (dataType == "text") { set.push_back(child.token, std::make_shared<opcodes::push>("")); set.push_back(child.token, std::make_shared<opcodes::call_unary>("text")); }
                    else if (dataType == "team_member") { log(logmessage::runtime::ErrorMessage({}, "SQC", "Cannot interpret type `TEAM_MEMBER`")); }
                    else if (dataType == "namespace") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("missionNamespace")); }
                    else if (dataType == "diary_record" || dataType == "diary" || dataType == "diaryrecord") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("diaryrecordnull")); }
                    else if (dataType == "task") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("tasknull")); }
                    else
                    {
                        std::transform(dataType.begin(), dataType.end(), dataType.begin(), [](char c) { return (char)std::toupper(c); });
                        log(logmessage::runtime::ErrorMessage({}, "SQC", "Cannot interpret type `" + dataType + "`"));
                    }
                    // make type array
                    set.push_back(node.token, std::make_shared<opcodes::make_array>(1));

                    // Make array 
                    set.push_back(node.token, std::make_shared<opcodes::make_array>(3));
                } break;
                case ::sqf::sqc::bison::astkind::ARGITEM_TYPE_DEFAULT: {
                    // push default value
                    to_assembly(runtime, set, locals, child.children[1]);

                    // Check most common data-types & push default type of it
                    auto dataType = std::string(child.children[0].token.contents);
                    std::transform(dataType.begin(), dataType.end(), dataType.begin(), [](char c) { return (char)std::tolower(c); });
                    if (dataType == "array") { set.push_back(child.token, std::make_shared<opcodes::make_array>(0)); }
                    else if (dataType == "bool") { set.push_back(child.token, std::make_shared<opcodes::push>(false)); }
                    else if (dataType == "code") { set.push_back(child.token, std::make_shared<opcodes::push>(std::make_shared<::sqf::types::d_code>())); }
                    else if (dataType == "config") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("confignull")); }
                    else if (dataType == "control") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("controlnull")); }
                    else if (dataType == "display") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("displaynull")); }
                    else if (dataType == "group") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("grpnull")); }
                    else if (dataType == "location") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("locationnull")); }
                    else if (dataType == "object") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("objnull")); }
                    else if (dataType == "scalar") { set.push_back(child.token, std::make_shared<opcodes::push>(0)); }
                    else if (dataType == "script") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("scriptnull")); }
                    else if (dataType == "side") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("civilian")); }
                    else if (dataType == "string") { set.push_back(child.token, std::make_shared<opcodes::push>("")); }
                    else if (dataType == "text") { set.push_back(child.token, std::make_shared<opcodes::push>("")); set.push_back(child.token, std::make_shared<opcodes::call_unary>("text")); }
                    else if (dataType == "team_member") { log(logmessage::runtime::ErrorMessage({}, "SQC", "Cannot interpret type `TEAM_MEMBER`")); }
                    else if (dataType == "namespace") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("missionNamespace")); }
                    else if (dataType == "diary_record" || dataType == "diary" || dataType == "diaryrecord") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("diaryrecordnull")); }
                    else if (dataType == "task") { set.push_back(child.token, std::make_shared<opcodes::call_nular>("tasknull")); }
                    else
                    {
                        std::transform(dataType.begin(), dataType.end(), dataType.begin(), [](char c) { return (char)std::toupper(c); });
                        log(logmessage::runtime::ErrorMessage({}, "SQC", "Cannot interpret type `" + dataType + "`"));
                    }
                    // make type array
                    set.push_back(node.token, std::make_shared<opcodes::make_array>(1));

                    // Make array 
                    set.push_back(node.token, std::make_shared<opcodes::make_array>(3));
                } break;
                default: break;
                }
            }
        }
        if (set.has_parent(::sqf::sqc::bison::astkind::OP_ACCESS_SET, 3) || set.has_parent(::sqf::sqc::bison::astkind::OBJECT_ITEM, 3))
        {
            ++param_count;
            set.push_back(node.token, std::make_shared<opcodes::push>(std::string(key_self)));
        }
        if (param_count > 0)
        {
            set.push_back(node.token, std::make_shared<opcodes::make_array>(param_count));
            set.push_back(node.token, std::make_shared<opcodes::call_unary>("params"s));
        }
    } break;
    case ::sqf::sqc::bison::astkind::CODEBLOCK: {
        // Create copy of locals to ensure
        // locals stay local to this scope.
        auto locals_copy = locals;
        for (auto child : node.children)
        {
            util::setbuilder::region __region(set);
            to_assembly(runtime, set, locals_copy, child);
        }
    } break;
    case ::sqf::sqc::bison::astkind::IF: {
        util::setbuilder::region __region(set);
        // Emit condition
        to_assembly(runtime, set, locals, node.children[0]);
        set.push_back(node.children[0].token, std::make_shared<opcodes::call_unary>("if"s));

        auto local_set = set.create_from();
        to_assembly(runtime, local_set, locals, node.children[1]);

        // Handle the case that the last instruction was `return` before pushing
        bool should_exit_with = is_top_level;
        if (should_exit_with && local_set.instructions().size() >= 2)
        {
            auto& instructions = local_set.instructions();
            auto& back = instructions.back();
            auto casted_binary = std::dynamic_pointer_cast<const ::sqf::opcodes::call_binary>(back);
            if (casted_binary.get())
            {
                if (should_exit_with = casted_binary->operator_name() == "breakout"s)
                {
                    instructions.erase(instructions.end() - 2, instructions.end() - 2);
                }
            }
            else
            {
                auto casted_unary = std::dynamic_pointer_cast<const ::sqf::opcodes::call_unary>(back);
                if (casted_unary.get())
                {
                    if (should_exit_with = casted_unary->operator_name() == "breakout"s)
                    {
                        instructions.erase(instructions.end() - 2, instructions.end());
                    }
                }
            }
        }
        set.push_back(node.children[1].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set }));

        if (should_exit_with)
        {
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("exitwith"s, (short)4));
        }
        else
        {
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("then"s, (short)4));
        }
    } break;
    case ::sqf::sqc::bison::astkind::OP_TERNARY: /* fallthrough */
    case ::sqf::sqc::bison::astkind::IFELSE: {
        util::setbuilder::region __region(set);
        // Emit condition
        to_assembly(runtime, set, locals, node.children[0]);
        set.push_back(node.children[0].token, std::make_shared<opcodes::call_unary>("if"s));

        // Emit on-true
        auto local_set1 = set.create_from();
        to_assembly(runtime, local_set1, locals, node.children[1]);
        set.push_back(node.children[1].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));

        // Emit on-false
        auto local_set2 = set.create_from();
        to_assembly(runtime, local_set2, locals, node.children[2]);
        set.push_back(node.children[2].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set2 }));
        set.push_back(node.children[2].token, std::make_shared<opcodes::call_binary>("else"s, (short)5));

        set.push_back(node.token, std::make_shared<opcodes::call_binary>("then"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::FOR: {
        util::setbuilder::region __region(set);
        // Emit "for"
        std::string var(node.children[0].token.contents);
        std::string lvar = "_"s + var;
        std::transform(var.begin(), var.end(), var.begin(), [](char c) { return (char)std::tolower(c); });
        set.push_back(node.children[0].token, std::make_shared<opcodes::push>(lvar));
        set.push_back(node.children[0].token, std::make_shared<opcodes::call_unary>("for"s));

        // Emit "from"
        to_assembly(runtime, set, locals, node.children[1]);
        set.push_back(node.children[1].token, std::make_shared<opcodes::call_binary>("from"s, (short)4));

        // Emit "to"
        to_assembly(runtime, set, locals, node.children[2]);
        set.push_back(node.children[2].token, std::make_shared<opcodes::call_binary>("to"s, (short)4));

        // Emit Codeblock
        {
            // Create additional instruction_set vector
            auto local_set1 = set.create_from();

            // Create copy of locals where var exists
            auto locals_copy = locals;
            locals_copy.push_back({ var ,lvar });

            // Fill actual instruction_set
            to_assembly(runtime, local_set1, locals_copy, node.children[3]);
            set.push_back(node.children[3].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));
        }

        // Emit "do"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("do"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::FORSTEP: {
        util::setbuilder::region __region(set);
        // Emit "for"
        std::string var(node.children[0].token.contents);
        std::string lvar = "_"s + var;
        std::transform(var.begin(), var.end(), var.begin(), [](char c) { return (char)std::tolower(c); });
        set.push_back(node.children[0].token, std::make_shared<opcodes::push>(lvar));
        set.push_back(node.children[0].token, std::make_shared<opcodes::call_unary>("for"s));

        // Emit "from"
        to_assembly(runtime, set, locals, node.children[1]);
        set.push_back(node.children[1].token, std::make_shared<opcodes::call_binary>("from"s, (short)4));

        // Emit "to"
        to_assembly(runtime, set, locals, node.children[2]);
        set.push_back(node.children[2].token, std::make_shared<opcodes::call_binary>("to"s, (short)4));

        // Emit "step"
        to_assembly(runtime, set, locals, node.children[3]);
        set.push_back(node.children[3].token, std::make_shared<opcodes::call_binary>("step"s, (short)4));

        // Emit Codeblock
        {
            // Create additional instruction_set vector
            auto local_set1 = set.create_from();

            // Create copy of locals where var exists
            auto locals_copy = locals;
            locals_copy.push_back({ var ,lvar });

            // Fill actual instruction_set
            to_assembly(runtime, local_set1, locals_copy, node.children[4]);
            set.push_back(node.children[4].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));
        }

        // Emit "do"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("do"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::FOREACH: {
        util::setbuilder::region __region(set);
        // Emit Codeblock
        {
            // Create additional instruction_set vector
            auto local_set1 = set.create_from();

            // Create copy of locals
            auto locals_copy = locals;

            // Check if locals _x already exists and, if it does, emit "private _x_red_LINE_COL"
            auto res_x = std::find_if(locals_copy.begin(), locals_copy.end(),
                [](emplace &emp) -> bool { return emp.replace == "_x"; });
            if (res_x != locals_copy.end())
            {
                res_x->replace = "_x_redirect_" +
                    std::to_string(node.children[0].token.line) +
                    "_" +
                    std::to_string(node.children[0].token.column);

                set.push_back(node.children[2].token, std::make_shared<opcodes::get_variable>("_x"sv));
                set.push_back(node.children[2].token, std::make_shared<opcodes::assign_to_local>(res_x->replace));
            }

            // Assign variable for forEach _x
            std::string var(node.children[0].token.contents);
            std::transform(var.begin(), var.end(), var.begin(), [](char c) -> char { return (char)std::tolower(c); });
            locals_copy.push_back({ var, "_x" });

            // Fill actual instruction_set
            to_assembly(runtime, local_set1, locals_copy, node.children[2]);
            set.push_back(node.children[2].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));
        }
        // Emit value
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "forEach"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("foreach"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::WHILE: {
        util::setbuilder::region __region(set);
        // Emit "while"
        {
            // Create additional instruction_set vector for condition expression
            auto local_set1 = set.create_from();

            // Fill actual instruction_set for code
            to_assembly(runtime, local_set1, locals, node.children[0]);
            set.push_back(node.children[0].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));
        }
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("while"s, (short)4));

        // Emit codeblock
        {
            // Create additional instruction_set vector for condition expression
            auto local_set1 = set.create_from();

            // Fill actual instruction_set for code
            to_assembly(runtime, local_set1, locals, node.children[1]);
            set.push_back(node.children[1].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));
        }

        // Emit "do"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("do"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::DOWHILE: {
        util::setbuilder::region __region(set);
        // Emit call before while for the "do once"
        {
            // Create additional instruction_set vector for condition expression
            auto local_set1 = set.create_from();

            // Fill actual instruction_set for code
            to_assembly(runtime, local_set1, locals, node.children[0]);
            set.push_back(node.children[0].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));

            // Use unary call to call created scope
            set.push_back(node.children[0].token, std::make_shared<opcodes::call_unary>("call"s));
        }

        // Emit "while"
        {
            // Create additional instruction_set vector for condition expression
            auto local_set1 = set.create_from();

            // Fill actual instruction_set for code
            to_assembly(runtime, local_set1, locals, node.children[1]);
            set.push_back(node.children[1].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));
        }
        set.push_back(node.children[1].token, std::make_shared<opcodes::call_binary>("while"s, (short)4));

        // Emit codeblock
        {
            // Create additional instruction_set vector for condition expression
            auto local_set1 = set.create_from();

            // Fill actual instruction_set for code
            to_assembly(runtime, local_set1, locals, node.children[0]);
            set.push_back(node.children[0].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));
        }

        // Emit "do"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("do"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::TRYCATCH: {
        // Emit "try"
        {
            // Create additional instruction_set vector for condition expression
            auto local_set1 = set.create_from();

            // Fill actual instruction_set for code
            to_assembly(runtime, local_set1, locals, node.children[0]);
            set.push_back(node.children[0].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));

            // Use unary call to call created scope
            set.push_back(node.children[0].token, std::make_shared<opcodes::call_unary>("try"s));
        }

        // Emit "catch"
        {
            // Create additional instruction_set vector
            auto local_set1 = set.create_from();

            // Assign variable for _exception
            set.push_back(node.children[0].token, std::make_shared<opcodes::get_variable>("_exception"s));
            std::string var(node.children[0].token.contents);
            std::string lvar = "_"s + var;
            std::transform(var.begin(), var.end(), var.begin(), [](char c) { return (char)std::tolower(c); });
            set.push_back(node.children[0].token, std::make_shared<opcodes::assign_to_local>(lvar));

            // Fill actual instruction_set for code
            to_assembly(runtime, local_set1, locals, node.children[1]);
            set.push_back(node.children[1].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));
        }
        set.push_back(node.children[1].token, std::make_shared<opcodes::call_binary>("catch"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::SWITCH: {
        util::setbuilder::region __region(set);
        // Emit switch
        to_assembly(runtime, set, locals, node.children[0]);
        set.push_back(node.children[0].token, std::make_shared<opcodes::call_unary>("switch"s));

        // Emit "do switch"
        {
            // Create additional instruction_set vector for condition expression
            auto local_set1 = set.create_from();

            // Fill actual instruction_set with cases
            for (auto it = node.children.begin() + 1; it != node.children.end(); it++)
            {
                to_assembly(runtime, local_set1, locals, *it);
            }
            set.push_back(node.token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));

            // Emit "do"
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("do"s, (short)4));
        }
    } break;
    case ::sqf::sqc::bison::astkind::CASE: {
        util::setbuilder::region __region(set);
        // Emit "case"
        to_assembly(runtime, set, locals, node.children[0]);
        set.push_back(node.children[0].token, std::make_shared<opcodes::call_unary>("case"s));

        // If has codeblock, emit ":"
        if (node.children.size() == 2) {
            // Create additional instruction_set vector for condition expression
            auto local_set1 = set.create_from();

            // Fill actual instruction_set for code
            to_assembly(runtime, local_set1, locals, node.children[1]);
            set.push_back(node.children[1].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));

            // Emit "colon"
            set.push_back(node.token, std::make_shared<opcodes::call_binary>(":"s, (short)4));
        }
    } break;
    case ::sqf::sqc::bison::astkind::CASE_DEFAULT: {
        // Create additional instruction_set vector for condition expression
        auto local_set1 = set.create_from();

        // Fill actual instruction_set for code
        to_assembly(runtime, local_set1, locals, node.children[0]);
        set.push_back(node.children[0].token, std::make_shared<opcodes::push>(runtime::instruction_set{ local_set1 }));

        // Emit "default"
        set.push_back(node.token, std::make_shared<opcodes::call_unary>("default"s));
    } break;
    case ::sqf::sqc::bison::astkind::OP_OR: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "or"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("||"s, (short)1));
    } break;
    case ::sqf::sqc::bison::astkind::OP_AND: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "and"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("&&"s, (short)2));
    } break;
    case ::sqf::sqc::bison::astkind::OP_EQUALEXACT: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "==="
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("isequalto"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_EQUAL: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "=="
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("=="s, (short)3));
    } break;
    case ::sqf::sqc::bison::astkind::OP_NOTEQUALEXACT: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "!=="
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("isnotequalto"s, (short)4));
    } break;
    case ::sqf::sqc::bison::astkind::OP_NOTEQUAL: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "!="
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("!="s, (short)3));
    } break;
    case ::sqf::sqc::bison::astkind::OP_LESSTHAN: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "<"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("<"s, (short)3));
    } break;
    case ::sqf::sqc::bison::astkind::OP_GREATERTHAN: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit ">"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>(">"s, (short)3));
    } break;
    case ::sqf::sqc::bison::astkind::OP_LESSTHANEQUAL: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "<="
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("<="s, (short)3));
    } break;
    case ::sqf::sqc::bison::astkind::OP_GREATERTHANEQUAL: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit ">="
        set.push_back(node.token, std::make_shared<opcodes::call_binary>(">="s, (short)3));
    } break;
    case ::sqf::sqc::bison::astkind::OP_PLUS: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "+"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("+"s, (short)6));
    } break;
    case ::sqf::sqc::bison::astkind::OP_MINUS: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "-"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("-"s, (short)6));
    } break;
    case ::sqf::sqc::bison::astkind::OP_MULTIPLY: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "*"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("*"s, (short)7));
    } break;
    case ::sqf::sqc::bison::astkind::OP_DIVIDE: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "/"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("/"s, (short)7));
    } break;
    case ::sqf::sqc::bison::astkind::OP_CONFIGNAV: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "/"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>(">>"s, (short)3));
    } break;
    case ::sqf::sqc::bison::astkind::OP_REMAINDER: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);
        // Emit Right-Argument
        to_assembly(runtime, set, locals, node.children[1]);

        // Emit "%"
        set.push_back(node.token, std::make_shared<opcodes::call_binary>("%"s, (short)7));
    } break;
    case ::sqf::sqc::bison::astkind::OP_NOT: {
        // Emit Left-Argument
        to_assembly(runtime, set, locals, node.children[0]);

        // Emit "!"
        set.push_back(node.token, std::make_shared<opcodes::call_unary>("!"s));
    } break;
    case ::sqf::sqc::bison::astkind::OBJECT: {
        for (auto child : node.children)
        {
            to_assembly(runtime, set, locals, child);
        }
        set.push_back(node.token, std::make_shared<opcodes::make_array>(node.children.size()));
        set.push_back(node.token, std::make_shared<opcodes::call_unary>("createhashmapfromarray"s));
    } break;
    case ::sqf::sqc::bison::astkind::OBJECT_ITEM: {
        set.push_back(node.token, std::make_shared<opcodes::push>(std::string(node.token.contents.begin(), node.token.contents.end())));
        to_assembly(runtime, set, locals, node.children[0]);
        set.push_back(node.token, std::make_shared<opcodes::make_array>(2));
    } break;
    case ::sqf::sqc::bison::astkind::OP_CALL: {
        auto opname = std::string(node.children[1].token.contents);
        std::transform(opname.begin(), opname.end(), opname.begin(), [](char c) { return (char)std::tolower(c); });
        if (runtime.sqfop_exists_binary(opname))
        {
            // Emit Left-Argument
            to_assembly(runtime, set, locals, node.children[0]);

            auto matchingOps = runtime.sqfop_binary_by_name(opname);

            if (node.children[2].children.size() > 1)
            {
                // Emit Right-Argument
                to_assembly(runtime, set, locals, node.children[2]);
                set.push_back(node.children[2].token, std::make_shared<opcodes::make_array>(std::max(node.children[2].children.size(), (size_t)1)));
            }
            else if (node.children.size() == 3)
            {
                // Emit Right-Argument
                to_assembly(runtime, set, locals, node.children[2]);
            }
            else
            {

                log(logmessage::runtime::ErrorMessage({}, "SQC", "Empty right-arguments for binary operator: " + opname));
                set.push_back(node.token, std::make_shared<opcodes::call_nular>("nil"s));
            }

            // Emit binary operator
            set.push_back(node.token, std::make_shared<opcodes::call_binary>(std::string(matchingOps.front().get().name()), matchingOps.front().get().precedence()));
        }
        else
        { // Hashmap access
            // Emit arguments
            {
                if (node.children.size() == 3)
                {
                    // Emit Right-Arguments
                    to_assembly(runtime, set, locals, node.children[2]);

                    // Emit Hashmap last (we might want to reuse the method and have no this then)
                    to_assembly(runtime, set, locals, node.children[0]);

                    // Emit array for hashmap and arguments
                    set.push_back(node.children[2].token, std::make_shared<opcodes::make_array>(node.children[2].children.size() + 1));
                }
                else
                {
                    // Emit Hashmap
                    to_assembly(runtime, set, locals, node.children[0]);

                    // Emit array for hashmap
                    set.push_back(node.children[0].token, std::make_shared<opcodes::make_array>(1));
                }
            }

            // Emit `Hashmap`
            {
                // Emit Hashmap
                to_assembly(runtime, set, locals, node.children[0]);

                // Push ident as string to stack
                std::string access(node.children[1].token.contents.begin(), node.children[1].token.contents.end());
                set.push_back(node.children[1].token, std::make_shared<opcodes::push>(access));

                // Emit `get`
                set.push_back(node.token, std::make_shared<opcodes::call_binary>("get", 4));
            }

            // Emit binary operator
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("call"s, (short)4));
        }
    } break;
    case ::sqf::sqc::bison::astkind::OP_UNARY: {
        auto opname = std::string(node.children[0].token.contents);
        std::transform(opname.begin(), opname.end(), opname.begin(), [](char c) { return std::tolower(c); });
        if (runtime.sqfop_exists_unary(opname))
        {
            auto matchingOps = runtime.sqfop_unary_by_name(opname);

            if (node.children.size() == 1)
            {
                log(logmessage::assembly::ExpectedPush({}));
                set.push_back(node.token, std::make_shared<opcodes::call_nular>("nil"s));
            }
            else
            {
                if (node.children[1].children.size() > 1)
                { // No arg provided
                    // Emit Right-Argument
                    to_assembly(runtime, set, locals, node.children[1]);
                    set.push_back(node.children[1].token, std::make_shared<opcodes::make_array>(std::max(node.children[1].children.size(), (size_t)1)));
                }
                else
                {
                    // Emit Right-Argument
                    to_assembly(runtime, set, locals, node.children[1]);
                }
            }

            // Emit binary operator
            set.push_back(node.token, std::make_shared<opcodes::call_unary>(std::string(matchingOps.front().get().name())));
        }
        else
        {
            // Emit Right-Argument
            if (node.children.size() == 1)
            { // No arg provided
                set.push_back(node.token, std::make_shared<opcodes::push>(::sqf::runtime::value({ 0 })));
            }
            else
            {
                to_assembly(runtime, set, locals, node.children[1]);
                set.push_back(node.token, std::make_shared<opcodes::make_array>(std::max(node.children[1].children.size(), (size_t)1)));
            }

            // Emit function
            std::string var(node.children[0].token.contents);
            auto fres = std::find_if(locals.begin(), locals.end(), [&var](auto& it) { return it.ident == var; });
            if (fres != locals.end())
            {
                set.push_back(node.children[0].token, std::make_shared<opcodes::get_variable>(fres->replace));
            }
            else
            {
                set.push_back(node.children[0].token, std::make_shared<opcodes::get_variable>(var));
            }

            // Emit binary operator
            set.push_back(node.token, std::make_shared<opcodes::call_binary>("call"s, (short)4));
        }
    } break;
    case ::sqf::sqc::bison::astkind::VAL_STRING: {
        set.push_back(node.token, std::make_shared<opcodes::push>(types::d_string::from_sqf(node.token.contents)));
    } break;
    case ::sqf::sqc::bison::astkind::VAL_ARRAY: {
        for (auto child : node.children)
        {
            to_assembly(runtime, set, locals, child);
        }
        set.push_back(node.token, std::make_shared<opcodes::make_array>(node.children.size()));
    } break;
    case ::sqf::sqc::bison::astkind::VAL_NUMBER: {
        double d;
        try
        {
            set.push_back(node.token, std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>((double)std::stod(std::string(node.token.contents))))));
        }
        catch (std::out_of_range&)
        {
            log(logmessage::assembly::NumberOutOfRange({}));
            set.push_back(node.token, std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>(std::nanf("")))));
        }
        // We cannot use "modern" variant due to lack of GCC support in GitHub Actions as of 29.08.2020
        // auto result = std::from_chars(node.token.contents.data(), node.token.contents.data() + node.token.contents.size(), d);
        // if (result.ec == std::errc())
        // {
        //     log(logmessage::runtime::ReturningScalarZero({}));
        //     set.push_back(std::make_shared<::sqf::opcodes::push>(std::make_shared<::sqf::types::d_scalar>(0)));
        // }
        // else
        // {
        //     set.push_back(std::make_shared<::sqf::opcodes::push>(std::make_shared<::sqf::types::d_scalar>(d)));
        // }
    } break;
    case ::sqf::sqc::bison::astkind::VAL_TRUE: {
        set.push_back(node.token, std::make_shared<::sqf::opcodes::push>(true));
    } break;
    case ::sqf::sqc::bison::astkind::VAL_FALSE: {
        set.push_back(node.token, std::make_shared<::sqf::opcodes::push>(false));
    } break;
    case ::sqf::sqc::bison::astkind::VAL_NIL: {
        set.push_back(node.token, std::make_shared<::sqf::opcodes::push>(runtime::value{}));
    } break;
    case ::sqf::sqc::bison::astkind::VAL_THIS: {
        set.push_back(node.token, std::make_shared<opcodes::get_variable>(std::string(key_self)));
    } break;
    case ::sqf::sqc::bison::astkind::GET_VARIABLE: {
        std::string var(node.token.contents);
        std::transform(var.begin(), var.end(), var.begin(), [](char c) { return (char)std::tolower(c); });
        auto fres = std::find_if(locals.begin(), locals.end(), [&var](auto& it) { return it.ident == var; });
        if (fres != locals.end())
        {
            set.push_back(node.token, std::make_shared<opcodes::get_variable>(fres->replace));
        }
        else if (runtime.sqfop_exists(::sqf::runtime::sqfop_nular::key{ var }))
        {
            set.push_back(node.token, std::make_shared<opcodes::call_nular>(var));
        }
        else
        {
            set.push_back(node.token, std::make_shared<opcodes::get_variable>(var));
        }
    } break;
    case ::sqf::sqc::bison::astkind::SVAL_FORMAT_STRING: {
        if (node.children.size() == 1)
        {
            // Empty format string ($"..."), just shift by one and pass to push
            set.push_back(node.children[0].token, std::make_shared<opcodes::push>(types::d_string::from_sqf(node.children[0].token.contents.substr(1))));
        }
        else
        {
            // Prepare sstream, will contain our actual format
            std::stringstream sstream;

            // Read the format-string parts into sstream
            size_t index = 0; // Format arg index. Will also automagically contain the actual size of the format array
            for (const auto& child : node.children)
            {
                switch (child.token.type)
                {
                case tokenizer::etoken::t_formatted_string_start:
                    sstream << util::strip_formatted(child.token.contents.substr(1)) << "%" << ++index;
                    break;

                case tokenizer::etoken::t_formatted_string_continue:
                    sstream << util::strip_formatted(child.token.contents) << "%" << ++index;
                    break;

                case tokenizer::etoken::t_formatted_string_final:
                    sstream << util::strip_formatted(child.token.contents);
                    break;
                default:
                    break;
                }
            }

            // Push the format string to stack
            set.push_back(node.children[0].token, std::make_shared<opcodes::push>(sstream.str()));

            // Push the values on the stack too
            for (const auto& child : node.children)
            {
                switch (child.token.type)
                {
                case tokenizer::etoken::t_formatted_string_start:
                case tokenizer::etoken::t_formatted_string_continue:
                case tokenizer::etoken::t_formatted_string_final:
                    break;
                default:
                    to_assembly(runtime, set, locals, child);
                }
            }
            // Emit "makearray"
            set.push_back(
                node.children[0].token,
                (node.children.back().token.offset + node.children.back().token.contents.length()) - node.children.front().token.offset,
                std::make_shared<opcodes::make_array>(index + 1));

            // Emit "callunary" `format`
            set.push_back(
                node.children[0].token,
                (node.children.back().token.offset + node.children.back().token.contents.length()) - node.children.front().token.offset,
                std::make_shared<opcodes::call_unary>("format"));
        }
    } break;

    case ::sqf::sqc::bison::astkind::INC_PRE:
        icpp_pos = util::setbuilder::before;
        icpp_value = 1;
        goto INC_DEC_PRE_POST;
    case ::sqf::sqc::bison::astkind::INC_POST:
        icpp_pos = util::setbuilder::after;
        icpp_value = 1;
        goto INC_DEC_PRE_POST;
    case ::sqf::sqc::bison::astkind::DEC_PRE:
        icpp_pos = util::setbuilder::before;
        icpp_value = -1;
        goto INC_DEC_PRE_POST;
    case ::sqf::sqc::bison::astkind::DEC_POST:
        icpp_pos = util::setbuilder::after;
        icpp_value = -1;
        goto INC_DEC_PRE_POST;
    INC_DEC_PRE_POST:
    {
        // Emit the Pre-Fix
        {
            // Prepare variable name
            std::string var(node.children[0].token.contents);
            auto tmp = var;
            std::transform(var.begin(), var.end(), var.begin(), [](char c) { return (char)std::tolower(c); });

            // Push Left-Value (self)
            auto fres = std::find_if(locals.begin(), locals.end(), [&var](auto& it) { return it.ident == var; });
            if (fres != locals.end())
            {
                set.push_back(node.children[0].token, std::make_shared<opcodes::get_variable>(fres->replace), icpp_pos);
            }
            else
            {
                set.push_back(node.children[0].token, std::make_shared<opcodes::get_variable>(tmp), icpp_pos);
            }

            // Push Right-Value
            set.push_back(node.token, std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>(icpp_value))), icpp_pos);

            // Emit "+"/"-"
            set.push_back(node.token, std::make_shared<opcodes::call_binary>(icpp_value > 0 ? "+"s : "-"s, (short)6), icpp_pos);

            // Assign Value
            fres = std::find_if(locals.begin(), locals.end(), [&var](auto& it) { return it.ident == var; });
            if (fres != locals.end())
            {
                set.push_back(node.children[0].token, std::make_shared<opcodes::assign_to>(fres->replace), icpp_pos);
            }
            else
            {
                set.push_back(node.children[0].token, std::make_shared<opcodes::assign_to>(tmp), icpp_pos);
            }
        }
        if (!set.has_parent(::sqf::sqc::bison::astkind::STATEMENTS, 2))
        {
            // Emit the Get-Variable
            to_assembly(runtime, set, locals, node.children[0]);
        }
    } break;
    case ::sqf::sqc::bison::astkind::STATEMENTS: {
        for (const auto& child : node.children)
        {
            util::setbuilder::region __region(set);
            to_assembly(runtime, set, locals, child);
        }
    } break;
    default: {
        for (const auto& child : node.children)
        {
            to_assembly(runtime, set, locals, child);
        }
    } break;
    }
}

bool sqf::sqc::parser::check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file)
{
    tokenizer t(contents.begin(), contents.end(), file.physical);
    ::sqf::sqc::bison::astnode res;
    ::sqf::sqc::bison::parser p(t, res, *this, file.physical);
    bool success = p.parse() == 0;
    return success;
}
std::optional<::sqf::runtime::instruction_set> sqf::sqc::parser::parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file)
{
    tokenizer t(contents.begin(), contents.end(), file.physical);
    util::setbuilder set(contents);
    ::sqf::sqc::bison::astnode res;
    ::sqf::sqc::bison::parser p(t, res, *this, file.physical);
    // p.set_debug_level(1);
    bool success = p.parse() == 0;
    if (!success)
    {
        return {};
    }
    std::vector<emplace> locals;
    set.push_back({}, std::make_shared<opcodes::push>(key_scopename_function));
    set.push_back({}, std::make_shared<opcodes::call_unary>("scopename"));
    set.push_back({}, std::make_shared<opcodes::end_statement>());
    to_assembly(runtime, set, locals, res);
    return set;
}


enum class source_type
{
    NA,
    ROOT,
    ASSIGN,
    OPERATOR,
    CODE,
    PARAMS,
    PARAMS_ENTRY,
    CALL,
    CLAUSE,
    EXITWITH,
    FORMAT,
    GET_TYPE,
    SELECT
};
static void to_sqc_actual(std::stringstream& out_sstream, source_type type, ::sqf::runtime::instruction_set::iterator begin, ::sqf::runtime::instruction_set::iterator end, uint8_t tab)
{
    struct my_stack;
    using func = std::function<void(my_stack& stack, std::stringstream& sstream, source_type type, uint8_t tab)>;
    struct my_stack : public std::vector<func>
    {
        func pop()
        {
            if (!empty())
            {
                auto val = back();
                pop_back();
                return val;
            }
            else
            {
                return [](my_stack& stack, std::stringstream& sstream, source_type type, uint8_t tab) { };
            }
        }
    };
    my_stack stack;
    using namespace std::string_literals;
    using namespace std::string_view_literals;
    bool starts_with_params = false;

    // Iterate over instructions like this would be the VM
    for (auto it = begin; it != end; it++)
    {
        std::shared_ptr<sqf::opcodes::assign_to> assign_to;
        std::shared_ptr<sqf::opcodes::assign_to_local> assign_to_local;
        std::shared_ptr<sqf::opcodes::call_binary> call_binary;
        std::shared_ptr<sqf::opcodes::call_nular> call_nular;
        std::shared_ptr<sqf::opcodes::call_unary> call_unary;
        std::shared_ptr<sqf::opcodes::end_statement> end_statement;
        std::shared_ptr<sqf::opcodes::get_variable> get_variable;
        std::shared_ptr<sqf::opcodes::make_array> make_array;
        std::shared_ptr<sqf::opcodes::push> push;
        if ((assign_to = std::dynamic_pointer_cast<sqf::opcodes::assign_to>(*it)))
        {
            stack.push_back([assign_to](my_stack& stack, std::stringstream& sstream, source_type type, uint8_t tab) {
                if (stack.size() >= 1)
                {
                    sstream << assign_to->variable_name() << " = "sv;
                    stack.pop()(stack, sstream, source_type::ASSIGN, tab);
                }
            });
        }
        else if ((assign_to_local = std::dynamic_pointer_cast<sqf::opcodes::assign_to_local>(*it)))
        {
            stack.push_back([assign_to_local](my_stack& stack, std::stringstream& sstream, source_type type, uint8_t tab) {
                if (stack.size() >= 1)
                {
                    sstream << "private "sv << assign_to_local->variable_name() << " = "sv;
                    stack.pop()(stack, sstream, source_type::ASSIGN, tab);
                }
            });
        }
        else if ((call_binary = std::dynamic_pointer_cast<sqf::opcodes::call_binary>(*it)))
        {
            stack.push_back([call_binary](my_stack& stack, std::stringstream& sstream, source_type type, uint8_t tab) {
                if (stack.size() >= 2)
                {
                    if (type == source_type::SELECT)
                    {
                        sstream << "(";
                    }
                    auto n = call_binary->operator_name();

                    if (n == "call")
                    {
                        std::stringstream tmpsstream;
                        auto rgen = stack.pop();
                        rgen(stack, tmpsstream, source_type::CALL, tab);
                        auto rstring = tmpsstream.str();
                        tmpsstream.str("");

                        auto lgen = stack.pop();
                        lgen(stack, tmpsstream, source_type::CALL, tab);
                        auto lstring = tmpsstream.str();
                        sstream << rstring << "(" << lstring << ")";
                    }
                    else if (n == "select")
                    {
                        std::stringstream tmpsstream;
                        auto rgen = stack.pop();
                        rgen(stack, tmpsstream, source_type::SELECT, tab);
                        auto rstring = tmpsstream.str();
                        tmpsstream.str("");

                        auto lgen = stack.pop();
                        lgen(stack, tmpsstream, source_type::SELECT, tab);
                        auto lstring = tmpsstream.str();
                        sstream << lstring << "[" << rstring << "]";
                    }
                    else if (n == "then")
                    {
                        std::stringstream tmpsstream;
                        auto rgen = stack.pop();
                        rgen(stack, tmpsstream, source_type::CLAUSE, tab);
                        auto rstring = tmpsstream.str();
                        tmpsstream.str("");

                        auto lgen = stack.pop();
                        lgen(stack, tmpsstream, source_type::OPERATOR, tab);
                        auto lstring = tmpsstream.str();
                        sstream << lstring << rstring;
                    }
                    else if (n == "exitwith")
                    {
                        std::stringstream tmpsstream;
                        auto rgen = stack.pop();
                        rgen(stack, tmpsstream, source_type::EXITWITH, tab);
                        auto rstring = tmpsstream.str();
                        tmpsstream.str("");

                        auto lgen = stack.pop();
                        lgen(stack, tmpsstream, source_type::OPERATOR, tab);
                        auto lstring = tmpsstream.str();
                        sstream << lstring << rstring;
                    }
                    else if (n == "else")
                    {
                        std::stringstream tmpsstream;
                        auto rgen = stack.pop();
                        rgen(stack, tmpsstream, source_type::CLAUSE, tab);
                        auto rstring = tmpsstream.str();
                        tmpsstream.str("");

                        auto lgen = stack.pop();
                        lgen(stack, tmpsstream, source_type::CLAUSE, tab);
                        auto lstring = tmpsstream.str();
                        sstream << lstring << "\n"sv << std::string(tab * 4, ' ') << "else"sv << rstring;
                    }
                    else if (n == "mod")
                    {
                        std::stringstream tmpsstream;
                        auto rgen = stack.pop();
                        rgen(stack, tmpsstream, type, tab);
                        auto rstring = tmpsstream.str();
                        tmpsstream.str("");

                        auto lgen = stack.pop();
                        lgen(stack, tmpsstream, type, tab);
                        auto lstring = tmpsstream.str();
                        sstream << lstring << " % "sv << rstring;
                    }
                    else if (n == "isEqualTo")
                    {
                        std::stringstream tmpsstream;
                        auto rgen = stack.pop();
                        rgen(stack, tmpsstream, type, tab);
                        auto rstring = tmpsstream.str();
                        tmpsstream.str("");

                        auto lgen = stack.pop();
                        lgen(stack, tmpsstream, type, tab);
                        auto lstring = tmpsstream.str();
                        sstream << lstring << " === "sv << rstring;
                    }
                    else if (n == "isNotEqualTo")
                    {
                        std::stringstream tmpsstream;
                        auto rgen = stack.pop();
                        rgen(stack, tmpsstream, type, tab);
                        auto rstring = tmpsstream.str();
                        tmpsstream.str("");

                        auto lgen = stack.pop();
                        lgen(stack, tmpsstream, type, tab);
                        auto lstring = tmpsstream.str();
                        sstream << lstring << " !== "sv << rstring;
                    }
                    else if (n == "+" || n == "-" || n == "*" || n == "/"
                        || n == "%" || n == "==" || n == ">=" || n == ">>" || n == "<="
                        || n == "!=" || n == ">" || n == "<")
                    {
                        std::stringstream tmpsstream;
                        auto rgen = stack.pop();
                        rgen(stack, tmpsstream, type, tab);
                        auto rstring = tmpsstream.str();
                        tmpsstream.str("");

                        auto lgen = stack.pop();
                        lgen(stack, tmpsstream, type, tab);
                        auto lstring = tmpsstream.str();
                        sstream << lstring << " "sv << n << " "sv << rstring;
                    }
                    else
                    {
                        std::stringstream tmpsstream;
                        auto rgen = stack.pop();
                        rgen(stack, tmpsstream, source_type::CALL, tab);
                        auto rstring = tmpsstream.str();
                        tmpsstream.str("");

                        auto lgen = stack.pop();
                        lgen(stack, tmpsstream, source_type::CALL, tab);
                        auto lstring = tmpsstream.str();
                        sstream << lstring << "."sv << n << "(" << rstring << ")";
                    }
                    if (type == source_type::SELECT)
                    {
                        sstream << ")";
                    }
                }
            });
        }
        else if ((call_nular = std::dynamic_pointer_cast<sqf::opcodes::call_nular>(*it)))
        {
            stack.push_back([call_nular](my_stack& stack, std::stringstream& sstream, source_type type, uint8_t tab) {
                    sstream << call_nular->operator_name();
            });
        }
        else if ((call_unary = std::dynamic_pointer_cast<sqf::opcodes::call_unary>(*it)))
        {
            if (call_unary->operator_name() == "params")
            {
                stack.push_back([&starts_with_params, call_unary](my_stack& stack, std::stringstream& sstream, source_type type, uint8_t tab) {
                    if (stack.size() >= 1)
                    {
                        sstream << "("sv;
                        stack.pop()(stack, sstream, source_type::PARAMS, tab);
                        sstream << ")"sv;
                        if (stack.size() == 0)
                        {
                            starts_with_params = true;
                        }
                    }
                });
            }
            else
            {
                stack.push_back([call_unary](my_stack& stack, std::stringstream& sstream, source_type type, uint8_t tab) {
                    if (stack.size() >= 1)
                    {
                        auto n = call_unary->operator_name();
                        if (n == "format")
                        {
                            stack.pop()(stack, sstream, source_type::FORMAT, tab);
                        }
                        else if (n == "!" || n == "+" || n == "-")
                        {
                            sstream << call_unary->operator_name();
                            stack.pop()(stack, sstream, source_type::OPERATOR, tab);
                        }
                        else
                        {
                            sstream << call_unary->operator_name() << "("sv;
                            stack.pop()(stack, sstream, source_type::CALL, tab);
                            sstream << ")"sv;
                        }
                    }
                });
            }
        }
        else if ((end_statement = std::dynamic_pointer_cast<sqf::opcodes::end_statement>(*it)))
        {
        }
        else if ((get_variable = std::dynamic_pointer_cast<sqf::opcodes::get_variable>(*it)))
        {
            stack.push_back([get_variable](my_stack& stack, std::stringstream& sstream, source_type type, uint8_t tab) {
                    sstream << get_variable->variable_name();
            });
        }
        else if ((make_array = std::dynamic_pointer_cast<sqf::opcodes::make_array>(*it)))
        {
            stack.push_back([make_array](my_stack& stack, std::stringstream& sstream, source_type type, uint8_t tab) {
                switch (type)
                {
                    case source_type::SELECT:
                    {
                        std::vector<std::string> strings;
                        std::stringstream tmpsstream;
                        for (size_t i = 0; i < make_array->array_size() && !stack.empty(); i++)
                        {
                            stack.pop()(stack, tmpsstream, source_type::NA, tab);
                            strings.push_back(tmpsstream.str());
                            tmpsstream.str("");
                        }
                        if (strings.empty()) { break; }
                        std::reverse(strings.begin(), strings.end());

                        if (strings.size() >= 1) { sstream << strings[0]; }
                        sstream << "..";
                        if (strings.size() >= 2) { sstream << strings[1]; }
                    }
                    break;
                    case source_type::FORMAT:
                    {
                        std::vector<std::string> strings;
                        std::stringstream tmpsstream;
                        for (size_t i = 0; i < make_array->array_size() && !stack.empty(); i++)
                        {
                            stack.pop()(stack, tmpsstream, source_type::NA, tab);
                            strings.push_back(tmpsstream.str());
                            tmpsstream.str("");
                        }
                        if (strings.empty()) { break; }
                        std::reverse(strings.begin(), strings.end());
                        auto str = strings[0];
                        bool was_percent = false;
                        size_t index = 0;

                        sstream << "$";
                        for (auto c : str)
                        {
                            if (was_percent)
                            {
                                switch (c)
                                {
                                    case '0': index *= 10; index += 0; break;
                                    case '1': index *= 10; index += 1; break;
                                    case '2': index *= 10; index += 2; break;
                                    case '3': index *= 10; index += 3; break;
                                    case '4': index *= 10; index += 4; break;
                                    case '5': index *= 10; index += 5; break;
                                    case '6': index *= 10; index += 6; break;
                                    case '7': index *= 10; index += 7; break;
                                    case '8': index *= 10; index += 8; break;
                                    case '9': index *= 10; index += 9; break;
                                    default:
                                    if (index != 0 && index < strings.size())
                                    {
                                        sstream << "{" << strings[index] << "}";
                                    }
                                    if (c == '%')
                                    {
                                        index = 0;
                                    }
                                    else
                                    {
                                        sstream << c;
                                        was_percent = false;
                                    }
                                    break;
                                }
                            }
                            else if (c == '%')
                            {
                                was_percent = true;
                                index = 0;
                            }
                            else if (c == '{' || c == '}')
                            {
                                sstream << c << c;
                            }
                            else
                            {
                                sstream << c;
                            }
                        }
                    } break;
                    case source_type::CALL:
                    {
                        std::vector<std::string> strings;
                        std::stringstream tmpsstream;
                        for (size_t i = 0; i < make_array->array_size() && !stack.empty(); i++)
                        {
                            stack.pop()(stack, tmpsstream, source_type::NA, tab);
                            strings.push_back(tmpsstream.str());
                            tmpsstream.str("");
                        }
                        if (strings.size() <= 1)
                        {
                            sstream << "[";
                        }
                        for (auto rit = strings.rbegin(); rit != strings.rend(); rit++)
                        {
                            if (rit != strings.rbegin()) { sstream << ", "; }
                            sstream << *rit;
                        }
                        if (strings.size() <= 1)
                        {
                            sstream << "]";
                        }
                    } break;
                    case source_type::PARAMS:
                    {
                        std::vector<std::string> strings;
                        std::stringstream tmpsstream;
                        for (size_t i = 0; i < make_array->array_size() && !stack.empty(); i++)
                        {
                            stack.pop()(stack, tmpsstream, source_type::PARAMS_ENTRY, tab);
                            strings.push_back(tmpsstream.str());
                            tmpsstream.str("");
                        }
                        for (auto rit = strings.rbegin(); rit != strings.rend(); rit++)
                        {
                            if (rit != strings.rbegin()) { sstream << ", "; }
                            sstream << *rit;
                        }
                    } break;
                    case source_type::PARAMS_ENTRY:
                    {
                        std::vector<func> gens;
                        for (size_t i = 0; i < make_array->array_size() && !stack.empty(); i++)
                        {
                            gens.push_back(stack.pop());
                        }
                        std::reverse(gens.begin(), gens.end());
                        if (gens.size() >= 3)
                        {
                            gens[2](stack, sstream, source_type::GET_TYPE, tab);
                            sstream << " ";
                        }
                        if (gens.size() >= 1)
                        {
                            std::stringstream tmpsstream;
                            gens[0](stack, tmpsstream, source_type::NA, tab);
                            auto str = tmpsstream.str();
                            sstream << sqf::runtime::util::trim(str, "\""sv);
                        }
                        if (gens.size() >= 2)
                        {
                            sstream << " = "sv;
                            gens[1](stack, sstream, source_type::NA, tab);
                        }
                        for (size_t i = 3; i < gens.size(); i++)
                        {
                            std::stringstream tmpsstream;
                            gens[i](stack, tmpsstream, source_type::NA, tab);
                        }
                    } break;
                    default:
                    {
                        std::vector<std::string> strings;
                        std::stringstream tmpsstream;
                        for (size_t i = 0; i < make_array->array_size() && !stack.empty(); i++)
                        {
                            stack.pop()(stack, tmpsstream, source_type::NA, tab);
                            strings.push_back(tmpsstream.str());
                            tmpsstream.str("");
                        }
                        sstream << "[";
                        // Read in vector reversed
                        for (auto rit = strings.rbegin(); rit != strings.rend(); rit++)
                        {
                            if (rit != strings.rbegin()) { sstream << ", "; }
                            sstream << *rit;
                        }
                        sstream << "]";
                    } break;
                }
            });
        }
        else if ((push = std::dynamic_pointer_cast<sqf::opcodes::push>(*it)))
        {
            stack.push_back([push](my_stack& stack, std::stringstream& sstream, source_type type, uint8_t tab) {
                if (type == source_type::GET_TYPE)
                {
                    auto sview = push->value().type().to_string();
                    std::string str(sview.begin(), sview.end());
                    std::transform(str.begin(), str.end(), str.begin(), [](char c) -> char { return (char)std::tolower(c); });
                    sstream << str;
                }
                else if (push->value().type() == sqf::runtime::t_code())
                {
                    if (type == source_type::CLAUSE)
                    {
                        auto& lset = push->value().data<::sqf::types::d_code>()->value();
                        sstream << "\n"sv << std::string(tab * 4, ' ') << "{"sv;
                        to_sqc_actual(sstream, source_type::NA, lset.begin(), lset.end(), tab + 1);
                        sstream << "\n"sv << std::string(tab * 4, ' ') << "}"sv;
                    }
                    else if (type == source_type::EXITWITH)
                    {
                        auto& lset = push->value().data<::sqf::types::d_code>()->value();
                        if (lset.empty())
                        {
                            sstream << " { return; }"sv;
                        }
                        else
                        {
                            sstream << "\n"sv << std::string(tab * 4, ' ') << "{"sv;
                            to_sqc_actual(sstream, source_type::NA, lset.begin(), lset.end(), tab + 1);
                            sstream << "\n"sv << std::string((tab + 1) * 4, ' ') << "return;"sv; // just always insert empty return here
                            sstream << "\n"sv << std::string(tab * 4, ' ') << "}"sv;
                        }
                    }
                    else
                    {
                        auto& lset = push->value().data<::sqf::types::d_code>()->value();
                        to_sqc_actual(sstream, source_type::CODE, lset.begin(), lset.end(), tab + 1);
                    }
                }
                else
                {
                    sstream << push->value().data<::sqf::runtime::data>()->to_string_sqf();
                }
            });
        }
    }



    // Write out every generator method on stack into vector
    std::vector<std::string> strings;
    std::stringstream sstream;
    while (!stack.empty())
    {
        stack.pop()(stack, sstream, type, tab);
        strings.push_back(sstream.str());
        sstream.str("");
    }

    if (strings.size() == 0)
    {
        if (type == source_type::CODE)
        {
            out_sstream << "function() {}";
        }
    }
    else
    {
        if (type == source_type::ROOT && starts_with_params)
        {
            out_sstream << "params";
        }
        bool skip_final_newline = false;
        // Read in vector reversed
        for (auto rit = strings.rbegin(); rit != strings.rend(); rit++)
        {
            if (type == source_type::CODE && rit == strings.rbegin())
            {
                out_sstream << "function"sv;
                if (!starts_with_params)
                {
                    if (rit->length() < 32 && strings.size() == 1)
                    {
                        skip_final_newline = true;
                        out_sstream << "() { "sv << *rit << ";"sv;
                    }
                    else
                    {
                        out_sstream << "() {\n"sv << std::string(tab * 4, ' ') << *rit << ";"sv;
                    }
                }
                else
                {
                    out_sstream << std::string(tab * 4, ' ') << *rit << ") {"sv;
                }
            }
            else
            {
                out_sstream << "\n"sv << std::string(tab * 4, ' ') << *rit << ";"sv;
            }
        }
        if (type == source_type::CODE)
        {
            if (skip_final_newline)
            {
                out_sstream << " "sv;
            }
            else
            {
                out_sstream << "\n"sv << std::string((tab - 1) * 4, ' ');
            }
            out_sstream << "}"sv;
        }
    }
}
std::string sqf::sqc::parser::to_sqc(const::sqf::runtime::instruction_set& set)
{
    auto begin = set.begin();
    auto end = set.end();
    std::stringstream sstream;
    to_sqc_actual(sstream, source_type::ROOT, begin, end, 0);
    auto str = sstream.str();
    return str;
}
