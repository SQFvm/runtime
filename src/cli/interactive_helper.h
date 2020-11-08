#pragma once
#include "../runtime/runtime.h"

#include <string>
#include <vector>
#include <array>

namespace sqf::runtime
{
    class context;
}
class interactive_helper
{
    typedef void (*interactive_callback)(interactive_helper&, std::string_view);
    struct command
    {
        std::vector<std::string> names;
        interactive_callback callback;
        std::string description;
    };
    std::vector<command> m_commands;


    sqf::runtime::runtime& m_runtime;
    sqf::runtime::runtime::action m_runtime_apply_action;
    bool m_thread_die;
    bool m_exit;
    std::weak_ptr<sqf::runtime::context> m_context_selected;

    static const size_t buffer_size = 16384;
    char* m_buffer;

    void virtualmachine_thread();
public:
    template<size_t size>
    void register_command(std::array<std::string, size> names, std::string description, interactive_callback callback)
    {
        m_commands.push_back({ std::vector(names.begin(), names.end()), callback, description });
    }
    interactive_helper(sqf::runtime::runtime& vm) :
        m_commands(),
        m_runtime(vm),
        m_runtime_apply_action(sqf::runtime::runtime::action::invalid),
        m_thread_die(false),
        m_exit(false),
        m_context_selected(),
        m_buffer(new char[buffer_size])
    {
    }
    ~interactive_helper()
    {
        delete[] m_buffer;
    }

    void init();
    void run();

    std::vector<command>::const_iterator commands_begin() const { return m_commands.begin(); }
    std::vector<command>::const_iterator commands_end() const { return m_commands.end(); }
    sqf::runtime::runtime& runtime() const { return m_runtime; }

    void context_selected(std::shared_ptr<sqf::runtime::context> sptr) { m_context_selected = sptr; }
    std::shared_ptr<sqf::runtime::context> context_selected() const { return m_context_selected.lock(); }
    void print_welcome();

    bool execute_next(sqf::runtime::runtime::action action)
    {
        if (m_runtime_apply_action == sqf::runtime::runtime::action::invalid)
        {
            m_runtime_apply_action = action;
            return true;
        }
        else
        {
            return false;
        }
    }
};
