#ifdef _WIN32
// Required due to some headers using WinSock2.h
// & some headers requiring windows.h
// If this was not here, a link conflict would emerge due to
// windows.h including winsock1
#include <WinSock2.h>
#endif

#include "ops_sqfvm.h"
#include "../runtime/runtime.h"
#include "../runtime/type.h"
#include "../runtime/frame.h"
#include "../runtime/sqfop.h"
#include "../runtime/d_array.h"
#include "../runtime/d_boolean.h"
#include "../runtime/d_code.h"
#include "../runtime/d_scalar.h"
#include "../runtime/d_string.h"
#include "../runtime/diagnostics/stacktrace.h"
#include "../runtime/diagnostics/d_stacktrace.h"
#include "d_config.h"
#include "d_object.h"
#include "ops_namespace.h"
#include "../opcodes/common.h"


#include <sstream>
#include <array>
#include <algorithm> 
#include <cctype>
#include <cmath>
#include <locale>

#if defined(DF__SQF_FILEIO__ALL_FILES)
#include <iostream>
#endif

namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;
using namespace std::string_literals;

namespace
{
    value fromAssembly___array(runtime& runtime, value::cref right)
    {
        auto arr = right.data<d_array, std::vector<value>>();

        std::vector<sqf::runtime::instruction::sptr> code;
        for (auto it = arr.begin(); it != arr.end(); it++)
        {
            if (!it->is<t_string>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(
                    runtime.context_active().current_frame().diag_info_from_position(),
                    it - arr.begin(), t_string(), it->type()));
                return {};
            }

            auto full = it->data<d_string, std::string>();
            std::string name = full;
            std::string arg;
            // Find either Space or end-of-string
            std::string::size_type nameEndIndex = full.find(' ');
            if (nameEndIndex + 1 > full.length())
            {
                runtime.__logmsg(err::InvalidAssemblyInstruction(
                    runtime.context_active().current_frame().diag_info_from_position(),
                    full));
                return {};
            }
            
            if (nameEndIndex != std::string::npos)
            {
                name = full.substr(0, nameEndIndex);
                arg = full.substr(nameEndIndex + 1);
            }

            std::transform(name.begin(), name.end(), name.begin(), [](char c) { return (char)std::tolower(c); });
            if (name == "assignto")
            {
                auto str = sqf::types::d_string::from_sqf(arg);
                code.push_back(std::make_shared<sqf::opcodes::assign_to>(str));
            }
            else if (name == "assigntolocal")
            {
                auto str = sqf::types::d_string::from_sqf(arg);
                code.push_back(std::make_shared<sqf::opcodes::assign_to_local>(str));
            }
            else if (name == "callbinary")
            {
                auto str = arg;
                if (!runtime.sqfop_exists_binary(str))
                {
                    runtime.__logmsg(err::InvalidAssemblyInstruction(
                        runtime.context_active().current_frame().diag_info_from_position(),
                        full));
                    return {};
                }
                auto binary_ops = runtime.sqfop_binary_by_name(str);
                auto prec = binary_ops.begin()->get().precedence();
                code.push_back(std::make_shared<sqf::opcodes::call_binary>(str, prec));
            }
            else if (name == "callnular")
            {
                auto str = arg;
                code.push_back(std::make_shared<sqf::opcodes::call_nular>(str));
            }
            else if (name == "callunary")
            {
                auto str = arg;
                code.push_back(std::make_shared<sqf::opcodes::call_unary>(str));
            }
            else if (name == "endstatement")
            {
                code.push_back(std::make_shared<sqf::opcodes::end_statement>());
            }
            else if (name == "getvariable")
            {
                auto str = sqf::types::d_string::from_sqf(arg);
                code.push_back(std::make_shared<sqf::opcodes::get_variable>(str));
            }
            else if (name == "makearray")
            {
                auto len = std::stof(arg);
                code.push_back(std::make_shared<sqf::opcodes::make_array>((size_t)len));
            }
            else if (name == "push")
            {
                auto result = runtime.parser_sqf().parse(runtime, arg, {});
                if (!result.has_value())
                {
                    runtime.__logmsg(err::InvalidAssemblyInstruction(
                        runtime.context_active().current_frame().diag_info_from_position(),
                        full));
                    return {};
                }
                if (!result->empty())
                {
                    auto instruction = *result->begin();
                    auto tmp = std::dynamic_pointer_cast<sqf::opcodes::push>(instruction);
                    if (tmp)
                    {
                        code.push_back(std::make_shared<sqf::opcodes::push>(tmp->value()));
                    }
                }
            }
            else
            {
                runtime.__logmsg(err::InvalidAssemblyInstruction(
                    runtime.context_active().current_frame().diag_info_from_position(),
                    full));
                return {};
            }
        }
        return std::make_shared<d_code>(code);
    }
    value assembly___code(runtime& runtime, value::cref right)
    {
        auto code = right.data<d_code>();

        std::vector<value> outarr;
        for (auto it = code->value().begin(); it != code->value().end(); it++)
        {
            outarr.push_back((*it)->to_string());
        }
        return outarr;
    }
    value assembly___string(runtime& runtime, value::cref right)
    {
        auto str = right.data<d_string>();
        auto set = runtime.parser_sqf().parse(runtime, *str, runtime.context_active().current_frame().diag_info_from_position().path);
        if (set.has_value())
        {
            std::vector<value> outarr;
            for (auto it = set->begin(); it != set->end(); it++)
            {
                outarr.push_back((*it)->to_string());
            }
            return outarr;
        }
        else
        {
            return {};
        }
    }
    value cmds___(runtime& runtime)
    {
        std::vector<value> outarr;
        auto str = "n";
        for (auto pair = runtime.sqfop_nular_begin(); pair != runtime.sqfop_nular_end(); pair++)
        {
            outarr.push_back(std::vector<value> {
                str,
                    pair->first.name
            });
        }
        str = "u";
        for (auto pair = runtime.sqfop_unary_begin(); pair != runtime.sqfop_unary_end(); pair++)
        {
            outarr.push_back(std::vector<value> {
                str,
                    pair->first.name,
                    pair->first.right_type.to_string()
            });
        }
        str = "b";
        for (auto pair = runtime.sqfop_binary_begin(); pair != runtime.sqfop_binary_end(); pair++)
        {
            outarr.push_back(std::vector<value> {
                str,
                    pair->first.left_type.to_string(),
                    pair->first.name,
                    pair->first.right_type.to_string()
            });
        }
        return outarr;
    }
    value cmdsimplemented___(runtime& runtime)
    {
        std::vector<value> outarr;
        auto str = "n";
        for (auto pair = runtime.sqfop_nular_begin(); pair != runtime.sqfop_nular_end(); pair++)
        {
            if (pair->second.description().empty())
                continue;
            outarr.push_back(std::vector<value> {
                str,
                    pair->first.name
            });
        }
        str = "u";
        for (auto pair = runtime.sqfop_unary_begin(); pair != runtime.sqfop_unary_end(); pair++)
        {
            if (pair->second.description().empty())
                continue;
            outarr.push_back(std::vector<value> {
                str,
                    pair->first.name,
                    pair->first.right_type.to_string()
            });
        }
        str = "b";
        for (auto pair = runtime.sqfop_binary_begin(); pair != runtime.sqfop_binary_end(); pair++)
        {
            if (pair->second.description().empty())
                continue;
            outarr.push_back(std::vector<value> {
                str,
                    pair->first.left_type.to_string(),
                    pair->first.name,
                    pair->first.right_type.to_string()
            });
        }
        return outarr;
    }
    value cmdsvm___(runtime& runtime)
    {
        std::vector<value> outarr;
        auto str = "n";
        for (auto pair = runtime.sqfop_nular_begin(); pair != runtime.sqfop_nular_end(); pair++)
        {
            if (pair->first.name.length() < 2 || pair->first.name[pair->first.name.length() - 1] != '_' || pair->first.name[pair->first.name.length() - 2] != '_')
                continue;
            outarr.push_back(std::vector<value> {
                str,
                    pair->first.name
            });
        }
        str = "u";
        for (auto pair = runtime.sqfop_unary_begin(); pair != runtime.sqfop_unary_end(); pair++)
        {
            if (pair->first.name.length() < 2 || pair->first.name[pair->first.name.length() - 1] != '_' || pair->first.name[pair->first.name.length() - 2] != '_')
                continue;
            outarr.push_back(std::vector<value> {
                str,
                    pair->first.name,
                    pair->first.right_type.to_string()
            });
        }
        str = "b";
        for (auto pair = runtime.sqfop_binary_begin(); pair != runtime.sqfop_binary_end(); pair++)
        {
            if (pair->first.name.length() < 2 || pair->first.name[pair->first.name.length() - 1] != '_' || pair->first.name[pair->first.name.length() - 2] != '_')
                continue;
            outarr.push_back(std::vector<value> {
                str,
                    pair->first.left_type.to_string(),
                    pair->first.name,
                    pair->first.right_type.to_string()
            });
        }
        return outarr;
    }
    value help___string(runtime& runtime, value::cref right)
    {
        std::stringstream sstream;
        std::string str = *right.data<d_string>();
        bool wasfound = false;
        for (auto pair = runtime.sqfop_nular_begin(); pair != runtime.sqfop_nular_end(); pair++)
        {
            if (pair->first.name != str)
                continue;
            auto cmd = pair->second;
            if (cmd.description().empty())
            {
                sstream << "NULAR '" << pair->first.name << "'\t<" << pair->first.name << ">" << std::endl;
            }
            else
            {
                sstream << "NULAR '" << pair->first.name << "'\t<" << cmd.name() << ">\t" << cmd.description() << std::endl;
            }
            wasfound = true;
        }
        for (auto pair = runtime.sqfop_unary_begin(); pair != runtime.sqfop_unary_end(); pair++)
        {
            if (pair->first.name != str)
                continue;
            auto cmd = pair->second;
            if (cmd.description().empty())
            {
                sstream << "UNARY '" << pair->first.name << "'\t<" << cmd.name() << " " << pair->first.right_type.to_string() << ">" << std::endl;
            }
            else
            {
                sstream << "UNARY '" << pair->first.name << "'\t<" << cmd.name() << " " << pair->first.right_type.to_string() << ">\t" << cmd.description() << std::endl;
            }
            wasfound = true;
        }
        for (auto pair = runtime.sqfop_binary_begin(); pair != runtime.sqfop_binary_end(); pair++)
        {
            if (pair->first.name != str)
                continue;
            auto cmd = pair->second;
            if (cmd.description().empty())
            {
                sstream << "BINARY '" << pair->first.name << "'\t<" <<
                    pair->first.left_type.to_string() << " " << cmd.name() << " " << pair->first.right_type.to_string() << ">" <<
                    std::endl;
            }
            else
            {
                sstream << "BINARY '" << pair->first.name << "'\t<" <<
                    pair->first.left_type.to_string() << " " << cmd.name() << " " << pair->first.right_type.to_string() << ">\t" <<
                    cmd.description() << std::endl;
            }
            wasfound = true;
        }
        if (!wasfound)
        {
            sstream << "Could not find any command with that name." << std::endl;
        }
        runtime.__logmsg(err::InfoMessage(runtime.context_active().current_frame().diag_info_from_position(), "HELP", sstream.str()));
        return {};
    }
    value configparse___string(runtime& runtime, value::cref right)
    {
        auto str = right.data<d_string, std::string>();
        runtime.parser_config().parse(runtime.confighost(), str, runtime.context_active().current_frame().diag_info_from_position());
        return {};
    }
    value allObjects__(runtime& runtime)
    {
        auto arr = std::make_shared<d_array>();
        for (auto& object : runtime.storage<object, object::object_storage>())
        {
            arr->push_back(std::make_shared<d_object>(object));
        }
        return value(arr);
    }
    value exit___(runtime& runtime)
    {
        runtime.exit(0);
        return {};
    }
    value exit___scalar(runtime& runtime, value::cref right)
    {
        runtime.exit(static_cast<int>(std::round(*right.data<d_scalar>())));
        return {};
    }
    value respawn___(runtime& runtime)
    {
        auto current = runtime.storage<object, object::object_storage>().player();
        if (current)
        {
            current->destroy(runtime);
        }
        auto root = runtime.confighost().root();
        auto cfgVehicles = root / "CfgVehicles";
        sqf::runtime::config entry;
        if (!cfgVehicles.empty())
        {
            auto caManBase = cfgVehicles / "CAManBase";
            if (!caManBase.empty())
            {
                entry = *caManBase;
            }
        }
        current = object::create(runtime, entry, false);
        runtime.storage<object, object::object_storage>().player(current);
        return std::make_shared<d_object>(current);
    }
    value preprocess___string(runtime& runtime, value::cref right)
    {
        auto content = right.data<d_string>();
        auto ppres = runtime.parser_preprocessor().preprocess(runtime, *content, { {}, "__preprocess__"s });
        if (ppres.has_value())
        {
            return *ppres;
        }
        else
        {
            return ""s;
        }
    }
    value except___code_code(runtime& runtime, value::cref left, value::cref right)
    {
        class behavior_except : public frame::behavior
        {
        private:
            instruction_set m_set;
            bool m_exchanged;
        public:
            behavior_except(instruction_set set) : m_set(set), m_exchanged(false) {}
            virtual sqf::runtime::instruction_set get_instruction_set(sqf::runtime::frame& frame) override { return m_set; };
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                if (!m_exchanged)
                {
                    m_exchanged = true;
                    auto val = runtime.context_active().pop_value();
                    runtime.context_active().clear_values();
                    frame.clear_value_scope();
                    frame["_exception"] = val.has_value() ? *val : value{};
                    return result::exchange;
                }
                else
                {
                    return result::fail;
                }
            };
        };
        frame f(
            runtime.default_value_scope(),
            left.data<d_code, sqf::runtime::instruction_set>(),
            {},
            std::make_shared<behavior_except>(right.data<d_code, sqf::runtime::instruction_set>()));
        runtime.context_active().push_frame(f);
        return {};
    }
    value callstack___(runtime& runtime)
    {
        auto context = runtime.context_active();
        std::vector<sqf::runtime::frame> stacktrace_frames(context.frames_rbegin(), context.frames_rend());
        sqf::runtime::diagnostics::stacktrace stacktrace(stacktrace_frames);
        return stacktrace;
    }
    value allfiles___array(runtime& runtime, value::cref right)
    {
        auto arr = right.data<d_array>();
        if (!arr->check_type(runtime, t_string(), 0, arr->size()))
        {
            return {};
        }
        auto files = std::vector<value>();
        auto& fileio = runtime.fileio();
        auto paths = fileio.get_directories();
        if (paths.empty())
        {
            return files;
        }
        // recursively search for pboprefix
        for (auto& path : paths)
        {
            const auto options = std::filesystem::directory_options::skip_permission_denied | std::filesystem::directory_options::follow_directory_symlink;
            auto begin = std::filesystem::recursive_directory_iterator(path, options);
            auto end = std::filesystem::recursive_directory_iterator();
            for (auto i = begin; i != end; ++i)
            {
                bool skip = true;
                for (auto& ext : *arr)
                {
                    if (i->is_directory())
                    {
                        break;
                    }
                    if (!(i->path().extension().compare(ext.data<d_string, std::string>())))
                    {
                        skip = false;
                        break;
                    }
                }
                auto str = i->path().string();
                std::replace(str.begin(), str.end(), '\\', '/');
                if (!skip)
                {
                    files.push_back(str);
                }

#ifdef DF__SQF_FILEIO__ALL_FILES
                std::cout << "\x1B[94m[FILEIO-ALLFILES]\033[0m" << "    " << (skip ? "SKIPPED" : "PICKED ") << "    " << str << std::endl;
#endif // DF__SQF_FILEIO__ALL_FILES


            }
        }
        return files;
    }
    value pwd___(runtime& runtime)
    {
        auto path = std::filesystem::path(runtime.context_active().current_frame().diag_info_from_position().path.physical);
        auto str = std::filesystem::absolute(path).string();
        std::replace(str.begin(), str.end(), '\\', '/');
        return str;
    }
    value currentdirectory___(runtime& runtime)
    {
        auto pathinfo = runtime.context_active().current_frame().diag_info_from_position().path;
        auto path = std::filesystem::path(pathinfo.physical);
        auto str = std::filesystem::absolute(path.parent_path()).string();
        std::replace(str.begin(), str.end(), '\\', '/');
        return str;
    }
    value trim___(runtime& runtime, value::cref right)
    {
        auto str = right.data<d_string, std::string>();
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
            return !std::isspace(ch);
            }));
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
            return !std::isspace(ch);
            }).base(), str.end());
        return str;
    }
    //    value remoteConnect___(runtime& runtime, value::cref right)
    //    {
    //        if (!runtime.allow_networking())
    //        {
    //            runtime.__logmsg(err::NetworkingDisabled(runtime.context_active().current_frame().diag_info_from_position()));
    //            return false;
    //        }
    //        networking_init();
    //        if (runtime.is_networking_set())
    //        {
    //            runtime.__logmsg(err::AlreadyConnected(runtime.context_active().current_frame().diag_info_from_position()));
    //            return {};
    //        }
    //        auto s = right.as_string();
    //        auto index = s.find(':');
    //        if (index == std::string::npos)
    //        {
    //            runtime.__logmsg(err::NetworkingFormatMissmatch(runtime.context_active().current_frame().diag_info_from_position(), s));
    //            return {};
    //        }
    //        auto address = s.substr(0, index);
    //        auto port = s.substr(index + 1);
    //        SOCKET socket;
    //        if (networking_create_client(address.c_str(), port.c_str(), &socket))
    //        {
    //            runtime.__logmsg(err::FailedToEstablishConnection(runtime.context_active().current_frame().diag_info_from_position()));
    //            return false;
    //        }
    //        runtime.set_networking(std::make_shared<networking::client>(socket));
    //        return true;
    //    }
    //    value closeconnection___(runtime& runtime)
    //    {
    //        runtime.release_networking();
    //        return {};
    //    }
    value vmctrl___string(runtime& runtime, value::cref right)
    {
        auto str = right.data<d_string, std::string>();

        if (str == "abort")
        {
            runtime.execute(sqf::runtime::runtime::action::abort);
        }
        else if (str == "assembly_step")
        {
            runtime.execute(sqf::runtime::runtime::action::assembly_step);
        }
        else if (str == "leave_scope")
        {
            runtime.execute(sqf::runtime::runtime::action::leave_scope);
        }
        else if (str == "reset_run_atomic")
        {
            runtime.execute(sqf::runtime::runtime::action::reset_run_atomic);
        }
        else if (str == "start")
        {
            runtime.execute(sqf::runtime::runtime::action::start);
        }
        else if (str == "stop")
        {
            runtime.execute(sqf::runtime::runtime::action::stop);
        }
        else
        {
            // ToDo: Create custom log message for enum errors
            runtime.__logmsg(err::ErrorMessage(runtime.context_active().current_frame().diag_info_from_position(), "vmctrl", "exec unknown"));
        }
        return {};
    }
    value customnamespace___string(runtime& runtime, value::cref right)
    {
        return { std::make_shared<d_namespace>(runtime.get_value_scope(right.data<d_string, std::string>())) };
    }
    value nobubble___any_code(runtime& runtime, value::cref left, value::cref right)
    {
        frame f = { runtime.default_value_scope(), right.data<d_code, instruction_set>() };
        f["_this"] = left;
        f.bubble_variable(false);
        runtime.context_active().push_frame(f);
        return {};
    }
    value nobubble___code(runtime& runtime, value::cref right)
    {
        frame f = { runtime.default_value_scope(), right.data<d_code, instruction_set>() };
        f["_this"] = {};
        f.bubble_variable(false);
        runtime.context_active().push_frame(f);
        return {};
    }
    value measureperformance___CODE(runtime& runtime, value::cref right)
    {
        class behavior_measureperformance : public frame::behavior
        {
        private:
            std::chrono::high_resolution_clock::time_point m_start;
            size_t m_amount;
            size_t m_max;
            std::chrono::nanoseconds m_compensation;
        public:
            behavior_measureperformance(std::chrono::nanoseconds comp, size_t max) : m_start(std::chrono::high_resolution_clock::now()), m_amount(0), m_max(max), m_compensation(comp) {}
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                if (++m_amount < m_max)
                {
                    return result::seek_start;
                }
                else
                {
                    auto end = std::chrono::high_resolution_clock::now();
                    auto delta = end - m_start;
                    runtime.context_active().push_value(((std::chrono::duration_cast<std::chrono::nanoseconds>(delta) - m_compensation) / (std::chrono::nanoseconds::rep)m_amount).count());
                    return result::ok;
                }
            };
        };
        class behavior_measureoverhead : public frame::behavior
        {
        private:
            std::chrono::high_resolution_clock::time_point m_start;
            size_t m_amount;
            size_t m_max;
            instruction_set m_set;
            std::chrono::nanoseconds m_overhead;
        public:
            behavior_measureoverhead(sqf::runtime::instruction_set set, size_t max) : m_start(std::chrono::high_resolution_clock::now()), m_amount(0), m_max(max), m_set(set), m_overhead(0) {}
            virtual std::shared_ptr<behavior> get_behavior() override { return std::make_shared<behavior_measureperformance>(m_overhead, m_max); };
            virtual sqf::runtime::instruction_set get_instruction_set(sqf::runtime::frame& frame) override { return m_set; };
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                if (++m_amount < m_max)
                {
                    return result::seek_start;
                }
                else
                {
                    auto end = std::chrono::high_resolution_clock::now();
                    auto delta = end - m_start;
                    m_overhead = std::chrono::duration_cast<std::chrono::nanoseconds>(delta) / (std::chrono::nanoseconds::rep)m_amount;
                    return result::replace_self_seek_start;
                }
            };
        };
        frame f = { runtime.default_value_scope(), {}, std::make_shared<behavior_measureoverhead>(right.data<d_code, sqf::runtime::instruction_set>(), 10000) };
        f["_this"] = {};
        f.bubble_variable(false);
        runtime.context_active().push_frame(f);
        return {};
    }
    //value provide___code_string(runtime& runtime, value::cref left, value::cref right)
    //{
    //    auto arr = right.data<d_array>();
    //    if (!arr->check_type(runtime, t_string(), 1, 3))
    //    {
    //        return {};
    //    }
    //    sqf::runtime::type ltype;
    //    sqf::runtime::type rtype;
    //    std::string name;
    //    int frames_size = arr->frames_size();
    //    switch (frames_size)
    //    {
    //        case 1:
    //            name = arr->at(0).data<d_string, std::string>();
    //        break;
    //        case 3:
    //        {
    //            name = arr->at(1).data<d_string, std::string>();
    //            auto l = arr->at(0).data<d_string, std::string>();
    //            ltype = sqf::runtime::type::typemap().at(l);
    //            auto r = arr->at(arr->frames_size() - 1).data<d_string, std::string>();
    //            rtype = sqf::runtime::type::typemap().at(r);
    //        }
    //        break;
    //        case 2:
    //        {
    //            name = arr->at(0).data<d_string, std::string>();
    //            auto r = arr->at(arr->frames_size() - 1).data<d_string, std::string>();
    //            rtype = sqf::runtime::type::typemap().at(r);
    //        } break;
    //        default:
    //            return {};
    //    }
    //    switch (frames_size)
    //    {
    //    case 1:
    //        sqf::commandmap::get().remove(name);
    //        sqf::commandmap::get().add(sqf::nulardata<std::shared_ptr<codedata>>(
    //            left.data<codedata>(),
    //            name,
    //            "",
    //            [](runtime& runtime, std::shared_ptr<codedata> code) -> value
    //            {
    //                code->loadinto(vm, runtime.active_vmstack());
    //                runtime.active_vmstack()->stacks_top()->set_variable("_this", value());
    //                return {};
    //            }));
    //        break;
    //    case 2:
    //        sqf::commandmap::get().remove(name, rtype);
    //        sqf::commandmap::get().add(sqf::unarydata<std::shared_ptr<codedata>>(
    //            left.data<codedata>(),
    //            name,
    //            rtype,
    //            "",
    //            [](runtime& runtime, std::shared_ptr<codedata> code, value::cref right) -> value
    //            {
    //                code->loadinto(vm, runtime.active_vmstack());
    //                runtime.active_vmstack()->stacks_top()->set_variable("_this", right);
    //                return {};
    //            }));
    //        break;
    //    case 3:
    //        sqf::commandmap::get().remove(ltype, name, rtype);
    //        sqf::commandmap::get().add(sqf::binarydata<std::shared_ptr<codedata>>(
    //            left.data<codedata>(),
    //            (short)4,
    //            name,
    //            ltype,
    //            rtype,
    //            "",
    //            [](runtime& runtime, std::shared_ptr<codedata> code, value::cref left, value::cref right) -> value
    //            {
    //                code->loadinto(vm, runtime.active_vmstack());
    //                runtime.active_vmstack()->stacks_top()->set_variable("_this", value({ left , right }));
    //                return {};
    //            }));
    //        break;
    //    }
    //    return {};
    //}
}
void sqf::operators::ops_sqfvm(sqf::runtime::runtime& runtime)
{
    using namespace sqf::runtime::sqfop;
    //add(unary("tree__", t_string(), "Returns a string containing the abstract syntax tree for the provided SQF expression.", tree___string));
    //add(unary("tree__", t_code(), "Returns a string containing the abstract syntax tree for the provided SQF expression.", tree___code));
    runtime.register_sqfop(nular("cmds__", "Returns an array containing all commands available.", cmds___));
    runtime.register_sqfop(nular("cmdsimplemented__", "Returns an array containing all commands that are actually implemented.", cmdsimplemented___));
    runtime.register_sqfop(unary("help__", t_string(), "Displays all available information for a single command.", help___string));
    runtime.register_sqfop(unary("configparse__", t_string(), "Parses provided string as config into a new config object.", configparse___string));
    // runtime.register_sqfop(binary(4, "merge__", t_config(), t_config(), "Merges contents from the right config into the left config. Duplicate entries will be overriden. Contents will not be copied but referenced.", merge___config_config));
    runtime.register_sqfop(nular("allObjects__", "Returns an array containing all objects created.", allObjects__));
    // runtime.register_sqfop(unary("pretty__", t_code(), "Takes provided SQF code and pretty-prints it to output.", pretty___code));
    // runtime.register_sqfop(unary("prettysqf__", t_code(), "Takes provided SQF code and pretty-prints it to output.", pretty___code));
    // runtime.register_sqfop(unary("prettysqf__", t_string(), "Takes provided SQF code and pretty-prints it to output.", prettysqf___string));
    runtime.register_sqfop(nular("exit__", "Exits the VM execution immediately. Will not notify debug interface when used.", exit___));
    runtime.register_sqfop(unary("vmctrl__", t_string(), "Executes the provided SQF-VM exection action.", vmctrl___string));
    runtime.register_sqfop(unary("exitcode__", t_scalar(), "Exits the VM execution immediately. Will not notify debug interface when used. Allows to pass an exit code to the VM.", exit___scalar));
    runtime.register_sqfop(nular("cmdsvm__", "Provides a list of all SQF-VM only commands.", cmdsvm___));
    runtime.register_sqfop(nular("respawn__", "'Respawns' the player object.", respawn___));
    runtime.register_sqfop(unary("preprocess__", t_string(), "Runs the PreProcessor on provided string.", preprocess___string));
    runtime.register_sqfop(unary("fromAssembly__", t_array(), "Parses the provided array of assembly instruction strings into actual code.", fromAssembly___array));
    runtime.register_sqfop(unary("assembly__", t_code(), "returns an array, containing the assembly instructions as string.", assembly___code));
    runtime.register_sqfop(unary("assembly__", t_string(), "returns an array, containing the assembly instructions as string.", assembly___string));
    runtime.register_sqfop(binary(4, "except__", t_code(), t_code(), "Allows to define a block that catches VM exceptions. It is to note, that this will also catch exceptions in spawn! Exception will be put into the magic variable '_exception'. A callstack is available in '_callstack'.", except___code_code));
    runtime.register_sqfop(nular("callstack__", "Returns an array containing the whole callstack.", callstack___));
    runtime.register_sqfop(unary("allFiles__", t_array(),
        "Returns all files available in that path with the given file extensions." " "
        "RIGHT: File extension filters that are looked for. If empty, all files are returned." " "
        , allfiles___array));
    runtime.register_sqfop(nular("pwd__", "Current path determined by current instruction.", pwd___));
    runtime.register_sqfop(nular("currentDirectory__", "Current directory determined by current instruction.", currentdirectory___));
    runtime.register_sqfop(unary("trim__", t_string(), "Trims provided strings start and end.", trim___));
    //    runtime.register_sqfop(unary("remoteConnect__", t_string(), "Connects this as a client to the provided endpoint. Endpoint is expected to have the format ADDRESS:PORT. Returns TRUE on success, false if it failed. Note that IP-Address is required, not DNS names (eg. use '127.0.0.1' instead of 'localhost').", remoteConnect___));
    //    runtime.register_sqfop(nular("closeConnection__", "Closes the connection previously opened using remoteConnect__.", closeconnection___));
        // runtime.register_sqfop(binary(4, "provide__", t_code(), t_array(), "Allows to provide an implementation for a given operator. Will NOT override existing definitions. Array is expected to be of the following formats: nular: [\"name\"], unary: [\"name\", \"type\"], binary: [\"ltype\", \"name\", \"rtype\"]", provide___code_string));
    runtime.register_sqfop(unary("measurePerformance__", t_code(), "Measures the time required to execute the provided piece of code by executing it 10000 times and pushes the average time required onto the stack in nanoseconds. Will try to compensate for the measure overhead.", measureperformance___CODE));
    runtime.register_sqfop(unary("noBubble__", t_code(), "Acts like call but disables bubbling of variables for the lower scope. (lower scope will have no access to upper scope variables)", nobubble___code));
    runtime.register_sqfop(binary(4, "noBubble__", t_any(), t_code(), "Acts like call but disables bubbling of variables for the lower scope. (lower scope will have no access to upper scope variables)", nobubble___any_code));
    runtime.register_sqfop(unary("customNamespace__", t_string(), "operator to get a custom namespace that lives as long as the VM lives.", customnamespace___string));
}