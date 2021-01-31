#include "ops_generic.h"
#include "../runtime/value.h"
#include "../runtime/logging.h"
#include "../runtime/runtime.h"
#include "../runtime/version.h"
#include "../runtime/sqfop.h"
#include "../runtime/util.h"
#include "../runtime/instruction_set.h"
#include "../runtime/d_string.h"
#include "../runtime/d_scalar.h"
#include "../runtime/d_boolean.h"
#include "../runtime/d_array.h"
#include "../runtime/diagnostics/d_stacktrace.h"
#include "../runtime/d_code.h"
#include "d_text.h"
#include "../runtime/git_sha1.h"

#include "dlops_storage.h"


#include <cmath>



namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;
using namespace std::string_literals;


#define CALLEXTBUFFSIZE 10240
#define CALLEXTVERSIONBUFFSIZE 32
#define RVARGSLIMIT 2048

#if defined(__linux__) || defined(__APPLE__) || defined(__unix__)
#include <dlfcn.h>
typedef void(*RVExtensionVersion)(char*, int);
typedef void(*RVExtension)(char*, int, const char*);
typedef int(*RVExtensionArgs)(char*, int, const char*, const char**, int);
typedef int(*RVExtensionRegisterCallback_Proc)(char const* name, char const* function, char const* data);
typedef int(*RVExtensionRegisterCallback)(int(*callbackProc)(char const* name, char const* function, char const* data));
#elif defined(_WIN32)
typedef void(__stdcall* RVExtensionVersion)(char*, int);
typedef void(__stdcall* RVExtension)(char*, int, const char*);
typedef int(__stdcall* RVExtensionArgs)(char*, int, const char*, const char**, int);
typedef int(*RVExtensionRegisterCallback_Proc)(char const* name, char const* function, char const* data);
typedef int(__stdcall* RVExtensionRegisterCallback)(RVExtensionRegisterCallback_Proc);
#else
#error UNSUPPORTED PLATFORM
#endif

namespace
{
    value productversion_(runtime& runtime)
    {
        auto vec = std::vector<value>{
            value("SQF-runtime " SQFVM_RUNTIME_VERSION_FULL), //product name
            value("sqf-runtime"),
            value(CONCAT(SQFVM_RUNTIME_VERSION_MAJOR, SQFVM_RUNTIME_VERSION_MINOR)),
            value(SQFVM_RUNTIME_VERSION_REVISION),
            value("COMMUNITY-" + std::string{g_GIT_SHA1}),
            value(false),
#if _WIN32
            value("Windows"),
#elif __linux__
            value("Linux"),
#else
            value("NA"),
#endif
            value(ENVIRONMENTSTR)
        };
        return vec;
    }
    value call_code(runtime& runtime, value::cref right)
    {
        frame f = { runtime.default_value_scope(), right.data<d_code, instruction_set>() };
        auto _this = runtime.context_active().get_variable("_this");
        f["_this"] = _this.has_value() ? *_this : value{};
        runtime.context_active().push_frame(f);
        return {};
    }
    value call_any_code(runtime& runtime, value::cref left, value::cref right)
    {
        frame f = { runtime.default_value_scope(), right.data<d_code, instruction_set>() };
        f["_this"] = left;
        runtime.context_active().push_frame(f);
        return {};
    }
    value count_array(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_array>();
        return r->size();
    }
    value count_code_array(runtime& runtime, value::cref left, value::cref right)
    {
        class behavior_count_exit : public frame::behavior
        {
        private:
            std::shared_ptr<d_array> m_array;
            size_t m_index;
            size_t m_size;
            int m_count;
        public:
            behavior_count_exit(std::shared_ptr<d_array> arr) : m_array(arr), m_index(0), m_size(arr->size()), m_count(0) {}
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                auto res = runtime.context_active().pop_value();
                if (res.has_value())
                {
                    if (res->is<t_boolean>())
                    {
                        m_count += res->data<d_boolean, bool>() ? 1 : 0;
                    }
                    else if (res->empty())
                    {
                        runtime.__logmsg(logmessage::runtime::TypeMissmatchWeak(frame.diag_info_from_position(), t_boolean(), res->type()));
                    }
                    else
                    {
                        runtime.__logmsg(logmessage::runtime::TypeMissmatch(frame.diag_info_from_position(), t_boolean(), res->type()));
                    }
                }
                else
                {
                    runtime.__logmsg(logmessage::runtime::CallstackFoundNoValue(frame.diag_info_from_position(), "count"s));
                }
                if (m_size != m_array->size())
                {
                    runtime.__logmsg(logmessage::runtime::ArraySizeChanged(frame.diag_info_from_position(), m_size, m_array->size()));
                    m_size = m_array->size();
                }
                if (++m_index == m_size)
                {
                    runtime.context_active().push_value(m_count);
                    return result::ok;
                }
                else
                {
                    runtime.context_active().clear_values();
                    frame.clear_value_scope();
                    frame["_x"] = m_array->at(m_index);
                    return result::seek_start;
                }
            };
        };

        auto r = right.data<d_array>();
        if (r->size() == 0)
        {
            return 0;
        }
        else
        {
            frame f(runtime.default_value_scope(), left.data<d_code, instruction_set>(), std::make_shared<behavior_count_exit>(r));
            f["_x"] = r->at(0);
            runtime.context_active().push_frame(f);
        }
        return {};
    }
    value compile_string(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_string, std::string>();
        auto& parser = runtime.parser_sqf();
        auto res = parser.parse(runtime, r, runtime.context_active().current_frame().diag_info_from_position().path);
        if (!res.has_value())
        {
            runtime.__runtime_error() = true;
            return value{};
        }
        else
        {
            return *res;
        }
    }
    value typename_any(runtime& runtime, value::cref right)
    {
        return right.type().to_string();
    }
    value str_any(runtime& runtime, value::cref right)
    {
        return right.data()->to_string_sqf();
    }
    value nil_(runtime& runtime)
    {
        return {};
    }
    value comment_string(runtime& runtime, value::cref right)
    {
        return {};
    }
    value if_bool(runtime& runtime, value::cref right)
    {
        return std::make_shared<d_if>(right.data_try<d_boolean, bool>(false));
    }
    value then_if_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto ifcond = left.data<d_if, bool>();
        auto arr = right.data<d_array>()->value();
        if (arr.size() != 2)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, arr.size()));
            return {};
        }
        auto el0 = arr[0];
        auto el1 = arr[1];
        if (ifcond)
        {
            if (!el1.is<t_code>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), 1, t_code(), el1.type()));
            }
            if (el0.is<t_code>())
            {
                runtime.context_active().push_frame({ runtime.default_value_scope(), el0.data<d_code, instruction_set>() });
                return {};
            }
            else
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 0, t_code(), el0.type()));
                return {};
            }
        }
        else
        {
            if (!el0.is<t_code>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), 0, t_code(), el0.type()));
            }
            if (el1.is<t_code>())
            {
                runtime.context_active().push_frame({ runtime.default_value_scope(), el1.data<d_code, instruction_set>() });
                return {};
            }
            else
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 1, t_code(), el1.type()));
                return {};
            }
        }
    }
    value then_if_code(runtime& runtime, value::cref left, value::cref right)
    {
        auto ifcond = left.data<d_boolean, bool>();
        if (ifcond)
        {
            runtime.context_active().push_frame({ runtime.default_value_scope(), right.data<d_code, instruction_set>() });
            return {};
        }
        else
        {
            return {};
        }
    }
    value exitwith_if_code(runtime& runtime, value::cref left, value::cref right)
    {
        if (left.data<d_boolean, bool>())
        {
            runtime.context_active().current_frame().die();
            runtime.context_active().push_frame({ runtime.default_value_scope(), right.data<d_code, instruction_set>() });
            return {};
        }
        else
        {
            return {};
        }
    }
    value else_code_code(runtime& runtime, value::cref left, value::cref right)
    {
        auto vec = std::vector<value>(2);
        vec[0] = value(left);
        vec[1] = value(right);
        return vec;
    }
    value waituntil_code(runtime& runtime, value::cref right)
    {
        class behavior_waituntil_exit : public frame::behavior
        {
        private:
            size_t m_count;
        public:
            behavior_waituntil_exit() : m_count(0) {}
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                m_count++;
                auto res = runtime.context_active().pop_value();
                if (res.has_value())
                {
                    auto value = res->data_try<d_boolean, bool>();
                    if (value.has_value())
                    {
                        return result::ok;
                    }
                    else
                    {
                        runtime.__logmsg(logmessage::runtime::TypeMissmatch(frame.diag_info_from_position(), t_boolean(), res->type()));
                    }
                }
                else if (m_count > 30000 && runtime.context_active().can_suspend())
                {
                    runtime.__logmsg(logmessage::runtime::WaitUntilMaxLoopReached(frame.diag_info_from_position()));
                    return result::ok;
                }
                else
                {
                    runtime.__logmsg(logmessage::runtime::CallstackFoundNoValue(frame.diag_info_from_position(), "waitUntil"s));
                }
                // "Simulate" a frame wait
                runtime.context_active().suspend(std::chrono::milliseconds(10));
                runtime.context_active().clear_values();
                frame.clear_value_scope();
                return result::seek_start;
            };
        };

        frame f(runtime.default_value_scope(), right.data<d_code, instruction_set>(), std::make_shared<behavior_waituntil_exit>());
        runtime.context_active().push_frame(f);
        return {};
    }
    value while_code(runtime& runtime, value::cref right)
    {
        return std::make_shared<d_while>(right.data_try<d_code, instruction_set>({}));
    }
    value do_while_code(runtime& runtime, value::cref left, value::cref right)
    {
        class behavior_while_exit : public frame::behavior
        {
        private:
            enum class mode
            {
                Condition,
                Code
            };
            mode m_mode;
            instruction_set m_condition;
            instruction_set m_code;
        public:
            behavior_while_exit(instruction_set condition, instruction_set code) : m_mode(mode::Condition), m_condition(condition), m_code(code) {}
            virtual sqf::runtime::instruction_set get_instruction_set(sqf::runtime::frame& frame) override
            {
                switch (m_mode)
                {
                case behavior_while_exit::mode::Condition:
                    m_mode = mode::Code;
                    return m_code;
                case behavior_while_exit::mode::Code:
                    m_mode = mode::Condition;
                    return m_condition;
                default: return {};
                }
            }
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                switch (m_mode)
                {
                case behavior_while_exit::mode::Condition:
                {
                    auto res = runtime.context_active().pop_value();
                    if (res.has_value())
                    {
                        if (res->is<t_boolean>())
                        {
                            if (res->data<d_boolean, bool>())
                            {
                                runtime.context_active().clear_values();
                                frame.clear_value_scope();
                                return m_code.empty() ? result::seek_start : result::exchange;
                            }
                        }
                        else if (res->empty())
                        {
                            runtime.__logmsg(logmessage::runtime::TypeMissmatchWeak(frame.diag_info_from_position(), t_boolean(), res->type()));
                        }
                        else
                        {
                            runtime.__logmsg(logmessage::runtime::TypeMissmatch(frame.diag_info_from_position(), t_boolean(), res->type()));
                        }
                    }
                    else
                    {
                        runtime.__logmsg(logmessage::runtime::CallstackFoundNoValue(frame.diag_info_from_position(), "while"s));
                    }
                } break;
                case behavior_while_exit::mode::Code:
                    runtime.context_active().clear_values();
                    frame.clear_value_scope();
                    return result::exchange;
                }
                return result::ok;
            };
        };
        auto condition = left.data<d_code, instruction_set>();
        auto code = right.data<d_code, instruction_set>();

        if (condition.empty())
        {
            runtime.__logmsg(logmessage::runtime::ConditionEmpty(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }

        frame f(runtime.default_value_scope(), condition, std::make_shared<behavior_while_exit>(condition, code));
        runtime.context_active().push_frame(f);
        return {};
    }
    value for_string(runtime& runtime, value::cref right)
    {
        auto str = right.data<d_string, std::string>();
        auto fordata = std::make_shared<d_for>();
        fordata->variable(str);
        return fordata;
    }
    value from_for_scalar(runtime& runtime, value::cref left, value::cref right)
    {
        auto fordata = left.data<d_for>();
        auto index = right.data<d_scalar, float>();
        fordata->from(index);
        return left;
    }
    value to_for_scalar(runtime& runtime, value::cref left, value::cref right)
    {
        auto fordata = left.data<d_for>();
        auto index = right.data<d_scalar, float>();
        fordata->to(index);
        return left;
    }
    value step_for_scalar(runtime& runtime, value::cref left, value::cref right)
    {
        auto fordata = left.data<d_for>();
        auto index = right.data<d_scalar, float>();
        fordata->step(index);
        return left;
    }
    value do_for_code(runtime& runtime, value::cref left, value::cref right)
    {
        class behavior_for_exit : public frame::behavior
        {
        private:
            d_for m_for;
            float m_value;
        public:
            behavior_for_exit(std::shared_ptr<d_for> fordata) : m_for(*fordata), m_value(m_for.from()) {}
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                auto res = frame[m_for.variable()];
                auto value = res.data_try<d_scalar, float>();
                if (value.has_value())
                {
                    // Check if exit condition is met
                    auto updated = *value + m_for.step();
                    if (m_for.step() >= 0 ?
                        updated > m_for.to() :
                        updated < m_for.to())
                    {
                        return result::ok;
                    }
                    res = updated;
                    runtime.context_active().clear_values();
                    frame.clear_value_scope();
                    frame[m_for.variable()] = res;
                    return result::seek_start;
                }
                else
                {
                    runtime.__logmsg(logmessage::runtime::ForStepVariableTypeMissmatch(frame.diag_info_from_position(), m_for.variable(), t_scalar(), res.type()));
                    return result::ok;
                }
            };
        };

        auto fordata = left.data<d_for>();
        {
            /*
                for "_i" from 0 to  0 do {}; // will do once, with _i = 0
                for "_i" from 0 to -1 do {}; // will not do
                for "_i" from 0 to  1 step 0 do {}; // will loop forever
            */
            auto from = fordata->from();
            auto to = fordata->to();
            auto step = fordata->step();


            auto step_is_rero = std::abs(step) <= std::numeric_limits<float>::epsilon(); 
            if (!step_is_rero && (step > 0 ? from > to : to > from))
            {
                return {};
            }
        }
        frame f(runtime.default_value_scope(), right.data<d_code, instruction_set>(), std::make_shared<behavior_for_exit>(fordata));
        f[fordata->variable()] = fordata->from();
        runtime.context_active().push_frame(f);
        return {};
    }
    value foreach_code_array(runtime& runtime, value::cref left, value::cref right)
    {
        class behavior_foreach_exit : public frame::behavior
        {
        private:
            std::shared_ptr<d_array> m_array;
            size_t m_index;
            size_t m_size;
        public:
            behavior_foreach_exit(std::shared_ptr<d_array> arr) : m_array(arr), m_index(0), m_size(arr->size()) {}
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                if (m_size != m_array->size())
                {
                    runtime.__logmsg(logmessage::runtime::ArraySizeChanged(frame.diag_info_from_position(), m_size, m_array->size()));
                    m_size = m_array->size();
                }
                if (++m_index == m_size)
                {
                    return result::ok;
                }
                else
                {
                    runtime.context_active().clear_values();
                    frame.clear_value_scope();
                    frame["_forEachIndex"] = m_index;
                    frame["_x"] = m_array->at(m_index);
                    return result::seek_start;
                }
            };
        };
        auto arr = right.data<d_array>();
        if (arr->size() > 0)
        {
            frame f(runtime.default_value_scope(), left.data<d_code, instruction_set>(), std::make_shared<behavior_foreach_exit>(arr));
            f["_forEachIndex"] = 0;
            f["_x"] = arr->at(0);
            runtime.context_active().push_frame(f);
        }
        return {};
    }
    value select_array_scalar(runtime& runtime, value::cref left, value::cref right)
    {
        auto arr = left.data<d_array>()->value();
        auto index = static_cast<int>(std::round(right.data<d_scalar, float>()));

        if (static_cast<int>(arr.size()) < index || index < 0)
        {
            runtime.__logmsg(err::IndexOutOfRange(runtime.context_active().current_frame().diag_info_from_position(), arr.size(), index));
            return {};
        }
        if ((int)arr.size() == index)
        {
            runtime.__logmsg(err::IndexEqualsRange(runtime.context_active().current_frame().diag_info_from_position(), arr.size(), index));
            return {};
        }
        return arr[index];
    }
    value select_array_bool(runtime& runtime, value::cref left, value::cref right)
    {
        auto arr = left.data<d_array>()->value();
        auto flag = right.data<d_boolean, bool>();
        if (arr.size() != 2)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), 2, arr.size()));
        }
        if ((!flag && arr.size() < 2) || arr.size() < 1)
        {
            runtime.__logmsg(err::ReturningNil(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        else if (flag && arr.size() < 2)
        {
            runtime.__logmsg(err::ReturningNil(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }

        return flag ? arr[1] : arr[0];
    }
    value select_array_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto vec = left.data<d_array>()->value();
        auto arr = right.data<d_array>()->value();
        if (arr.size() < 1)
        {
            runtime.__logmsg(err::ExpectedMinimumArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 1, arr.size()));
            return {};
        }
        else if (arr.size() != 2)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), 2, arr.size()));
        }
        if (!arr[0].is<t_scalar>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 1, t_scalar(), arr[0].type()));
            return {};
        }
        int start = static_cast<int>(std::round(arr[0].data<d_scalar, float>()));
        if (start < 0)
        {
            runtime.__logmsg(err::NegativeIndexWeak(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningEmptyArray(runtime.context_active().current_frame().diag_info_from_position()));
            return value(std::make_shared<d_array>());
        }
        if (start > static_cast<int>(vec.size()))
        {
            runtime.__logmsg(err::IndexOutOfRangeWeak(runtime.context_active().current_frame().diag_info_from_position(), vec.size(), start));
            runtime.__logmsg(err::ReturningEmptyArray(runtime.context_active().current_frame().diag_info_from_position()));
            return value(std::make_shared<d_array>());
        }
        if (arr.size() >= 2)
        {
            if (!arr[1].is<t_scalar>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 1, t_scalar(), arr[1].type()));
                return {};
            }
            int length = static_cast<int>(std::round(arr[1].data<d_scalar, float>()));
            if (length < 0)
            {
                runtime.__logmsg(err::NegativeIndexWeak(runtime.context_active().current_frame().diag_info_from_position()));
                runtime.__logmsg(err::ReturningEmptyArray(runtime.context_active().current_frame().diag_info_from_position()));
                return value(std::make_shared<d_array>());
            }

            return value(std::vector<value>(vec.begin() + start, start + length > static_cast<int>(vec.size()) ? vec.end() : vec.begin() + start + length));
        }
        else
        {
            return value(std::make_shared<d_array>());
        }

    }
    value select_array_code(runtime& runtime, value::cref left, value::cref right)
    {
        class behavior_select_exit : public frame::behavior
        {
        private:
            std::shared_ptr<d_array> m_array;
            std::vector<value> m_out;
            size_t m_index;
            size_t m_size;
        public:
            behavior_select_exit(std::shared_ptr<d_array> arr) : m_array(arr), m_index(0), m_size(arr->size()) {}
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                auto res = runtime.context_active().pop_value();
                if (res.has_value())
                {
                    if (res->is<t_boolean>())
                    {
                        if (res->data<d_boolean, bool>())
                        {
                            m_out.push_back(m_array->at(m_index));
                        }
                    }
                    else if (res->empty())
                    {
                        runtime.__logmsg(logmessage::runtime::TypeMissmatchWeak(frame.diag_info_from_position(), t_boolean(), res->type()));
                    }
                    else
                    {
                        runtime.__logmsg(logmessage::runtime::TypeMissmatch(frame.diag_info_from_position(), t_boolean(), res->type()));
                    }
                }
                else
                {
                    runtime.__logmsg(logmessage::runtime::CallstackFoundNoValue(frame.diag_info_from_position(), "select"s));
                }

                if (m_size != m_array->size())
                {
                    runtime.__logmsg(logmessage::runtime::ArraySizeChanged(frame.diag_info_from_position(), m_size, m_array->size()));
                    m_size = m_array->size();
                }
                if (++m_index == m_size)
                {
                    runtime.context_active().push_value(m_out);
                    return result::ok;
                }
                else
                {
                    runtime.context_active().clear_values();
                    frame.clear_value_scope();
                    frame["_x"] = m_array->at(m_index);
                    return result::seek_start;
                }
            };
        };
        auto arr = left.data<d_array>();
        if (arr->size() > 0)
        {
            frame f(runtime.default_value_scope(), right.data<d_code, instruction_set>(), std::make_shared<behavior_select_exit>(arr));
            f["_x"] = arr->at(0);
            runtime.context_active().push_frame(f);
            return {};
        }
        return std::vector<value>();
    }
    value sort_array_boolean(runtime& runtime, value::cref left, value::cref right)
    {
        auto arr = left.data<d_array>();
        auto sort_flag = right.data<d_boolean, bool>();

        // No sort required (we don't strictly enforce constraints in this case)
        if (arr->size() <= 1) return {};

        // Check element types
        //#TODO d_array::front
        auto type = (*arr)[0].type();
        if (type != t_string() && type != t_scalar() && type != t_array())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(
                runtime.context_active().current_frame().diag_info_from_position(), 0, std::array<sqf::runtime::type, 3>{ t_string(), t_scalar(), t_array() }, type));
            return {};
        }
        if (!arr->check_type(runtime, type, arr->size(), arr->size()))
            return {};

        // Confirm array element sub arrays are consistent in structure
        if (type == t_array())
        {
            auto subarray = (*arr)[0].data<d_array>()->value();
            std::vector<sqf::runtime::type> types;
            std::transform(subarray.begin(), subarray.end(), std::back_inserter(types), [](const auto& elem) { return elem.type(); });
            for (const auto& elem : *arr)
            {
                if (!d_array{ elem.data<d_array>()->value() }.check_type(runtime, types))
                    return {};
            }
        }

        std::sort(arr->begin(), arr->end(), [sort_flag](sqf::runtime::value::cref a, sqf::runtime::value::cref b) -> bool {

            if (a.is<t_array>())
            {
                auto a_arr = a.data<d_array>()->value();
                auto b_arr = b.data<d_array>()->value();

                for (size_t idx = 0; idx < a_arr.size(); ++idx)
                {
                    const auto& a_elem = a_arr[idx];
                    const auto& b_elem = b_arr[idx];

                    if (a.is<t_string>())
                    {
                        if (a_elem.data<d_string, std::string>() < b_elem.data<d_string, std::string>()) return sort_flag;
                        if (a_elem.data<d_string, std::string>() > b_elem.data<d_string, std::string>()) return !sort_flag;
                    }
                    else if (a.is<t_scalar>())
                    {
                        if (a_elem.data<d_scalar, float>() < b_elem.data<d_scalar, float>()) return sort_flag;
                        if (a_elem.data<d_scalar, float>() > b_elem.data<d_scalar, float>()) return !sort_flag;
                    }
                }
                return !sort_flag;
            }
            else if (a.is<t_string>())
            {
                if (a.data<d_string, std::string>() < b.data<d_string, std::string>()) return sort_flag;
                if (a.data<d_string, std::string>() > b.data<d_string, std::string>()) return !sort_flag;
                return false;
            }
            else if (a.is<t_scalar>())
            {
                if (a.data<d_scalar, float>() < b.data<d_scalar, float>()) return sort_flag;
                if (a.data<d_scalar, float>() > b.data<d_scalar, float>()) return !sort_flag;
                return false;
            }
            return !sort_flag;
            });

        return {};
    }
    value resize_array_scalar(runtime& runtime, value::cref left, value::cref right)
    {
        auto i = right.data<d_scalar, size_t>();
        if (i < 0)
        {
            runtime.__logmsg(err::NegativeSize(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        left.data<d_array>()->resize(i);
        return {};
    }
    value deleterange_array_array(runtime& runtime, value::cref left, value::cref right)
    {
        if (!right.data<d_array>()->check_type(runtime, t_scalar(), 2))
        {
            return {};
        }
        auto from = (int)std::roundf((*right.data<d_array>())[0].data<d_scalar, float>());
        auto to = (int)std::roundf((*right.data<d_array>())[1].data<d_scalar, float>());

        auto arr = left.data<d_array>();
        if (from > to)
        {
            runtime.__logmsg(err::StartIndexExceedsToIndexWeak(runtime.context_active().current_frame().diag_info_from_position(), from, to));
            to = from;
        }
        if (from < 0)
        {
            runtime.__logmsg(err::NegativeIndexWeak(runtime.context_active().current_frame().diag_info_from_position()));
            runtime.__logmsg(err::ReturningNil(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        if (to >= (int)arr->size())
        {
            runtime.__logmsg(err::IndexOutOfRangeWeak(runtime.context_active().current_frame().diag_info_from_position(), arr->size(), to));
            to = (int)(arr->size() - 1);
        }
        arr->erase(arr->begin() + from, arr->begin() + to + 1);
        return {};
    }
    value pushback_array_any(runtime& runtime, value::cref left, value::cref right)
    {
        auto arr = left.data<d_array>();
        auto newindex = arr->size();
        if (!arr->push_back(value(right)))
        {
            runtime.__logmsg(err::ArrayRecursion(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        return newindex;
    }
    value pushbackunique_array_any(runtime& runtime, value::cref left, value::cref right)
    {
        auto arr = left.data<d_array>();
        int newindex = static_cast<int>(arr->size());
        auto found = std::find(arr->begin(), arr->end(), right);
        if (found == arr->end())
        {
            if (!arr->push_back(value(right)))
            {
                runtime.__logmsg(err::ArrayRecursion(runtime.context_active().current_frame().diag_info_from_position()));
                return {};
            }
        }
        else
        {
            newindex = -1;
        }
        return newindex;
    }
    value findif_array_code(runtime& runtime, value::cref left, value::cref right)
    {
        class behavior_findif_exit : public frame::behavior
        {
        private:
            std::shared_ptr<d_array> m_array;
            size_t m_index;
            size_t m_size;
        public:
            behavior_findif_exit(std::shared_ptr<d_array> arr) : m_array(arr), m_index(0), m_size(arr->size()) {}
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                auto res = runtime.context_active().pop_value();
                if (res.has_value())
                {
                    auto value = res->data_try<d_boolean, bool>();
                    if (value.has_value())
                    {
                        if (*value)
                        {
                            runtime.context_active().push_value(m_index);
                            return result::ok;
                        }
                    }
                    else
                    {
                        runtime.__logmsg(logmessage::runtime::TypeMissmatch(frame.diag_info_from_position(), t_boolean(), res->type()));
                    }
                }
                else
                {
                    runtime.__logmsg(logmessage::runtime::CallstackFoundNoValue(frame.diag_info_from_position(), "findIf"s));
                }
                if (m_size != m_array->size())
                {
                    runtime.__logmsg(logmessage::runtime::ArraySizeChanged(frame.diag_info_from_position(), m_size, m_array->size()));
                    m_size = m_array->size();
                }
                if (++m_index == m_size)
                {
                    runtime.context_active().push_value(-1);
                    return result::ok;
                }
                else
                {
                    runtime.context_active().clear_values();
                    frame.clear_value_scope();
                    frame["_x"] = m_array->at(m_index);
                    return result::seek_start;
                }
            };
        };

        auto r = left.data<d_array>();
        if (r->size() > 0)
        {
            frame f(runtime.default_value_scope(), right.data<d_code, instruction_set>(), std::make_shared<behavior_findif_exit>(r));
            f["_x"] = r->at(0);
            runtime.context_active().push_frame(f);
            return {};
        }
        else
        {
            return -1;
        }
    }
    value reverse_array(runtime& runtime, value::cref right)
    {
        right.data<d_array>()->reverse();
        return {};
    }
    value private_string(runtime& runtime, value::cref right)
    {
        runtime.context_active().current_frame().at(right.data<d_string, std::string>());
        return {};
    }
    value private_array(runtime& runtime, value::cref right)
    {
        auto arr = right.data<d_array>()->value();
        bool errflag = false;
        for (size_t i = 0; i < arr.size(); i++)
        {
            auto it = arr[i];
            if (!it.is<t_string>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), i, t_string(), it.type()));
                errflag = true;
            }
        }
        if (errflag)
        {
            return {};
        }
        for (auto& it : arr)
        {
            runtime.context_active().current_frame().at(it.data<d_string, std::string>());
        }
        return {};
    }
    value isnil_string(runtime& runtime, value::cref right)
    {
        auto varname = right.data<d_string, std::string>();
        auto val = runtime.context_active().get_variable(varname);
        if (!val.has_value())
        {
            val = runtime.context_active().current_frame().globals_value_scope()->try_get(varname);
        }
        return val.has_value() ? val->empty() : true;
    }
    value isnil_code(runtime& runtime, value::cref right)
    {
        class behavior_isnil_exit : public frame::behavior
        {
        private:
            size_t m_count;
        public:
            behavior_isnil_exit() : m_count(0) {}
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                m_count++;
                auto res = runtime.context_active().pop_value();
                if (res.has_value())
                {
                    auto value = res->data();
                    runtime.context_active().push_value(value.get() ? false : true);
                }
                else
                {
                    runtime.__logmsg(logmessage::runtime::CallstackFoundNoValue(frame.diag_info_from_position(), "isNil"s));
                }
                return result::ok;
            };
        };

        frame f(runtime.default_value_scope(), right.data<d_code, instruction_set>(), std::make_shared<behavior_isnil_exit>());
        runtime.context_active().push_frame(f);
        return {};
    }
    value hint_string(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_string, std::string>();
        runtime.__logmsg(err::InfoMessage(runtime.context_active().current_frame().diag_info_from_position(), "HINT"s, r));
        return {};
    }
    value hint_text(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_text, std::string>();
        runtime.__logmsg(err::InfoMessage(runtime.context_active().current_frame().diag_info_from_position(), "HINT"s, r));
        return {};
    }
    value systemchat_string(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_string, std::string>();
        runtime.__logmsg(err::InfoMessage(runtime.context_active().current_frame().diag_info_from_position(), "SYSTEM-CHAT"s, r));
        return {};
    }

    value switch_any(runtime& runtime, value::cref right)
    {
        return value(std::make_shared<d_switch>(right));
    }
    value do_switch_code(runtime& runtime, value::cref left, value::cref right)
    {
        class behavior_switch_exit : public frame::behavior
        {
        private:
            bool m_switched;
        public:
            behavior_switch_exit() : m_switched(false) {}
            virtual sqf::runtime::instruction_set get_instruction_set(sqf::runtime::frame& frame) override
            {
                return frame[d_switch::magic].data<d_switch>()->target_code();
            }
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                if (!m_switched)
                {
                    m_switched = true;
                    auto dswitch = frame[d_switch::magic].data_try<d_switch>();
                    return dswitch ? (dswitch->target_code().empty() ? result::ok : result::exchange) : result::fail;
                }
                else
                {
                    return result::ok;
                }
            };
        };

        frame f(runtime.default_value_scope(), right.data<d_code, instruction_set>(), std::make_shared<behavior_switch_exit>());
        f[d_switch::magic] = left;
        runtime.context_active().push_frame(f);
        return {};
    }
    value case_any(runtime& runtime, value::cref right)
    {
        auto valswtch = runtime.context_active().get_variable(d_switch::magic);
        if (!valswtch.has_value() || !valswtch->is<t_switch>())
        {
            runtime.__logmsg(err::MagicVariableTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), d_switch::magic, t_switch(), valswtch->type()));
            return {};
        }
        auto swtch = valswtch->data<d_switch>();

        if (right == swtch->value())
        {
            swtch->match_now(true);
        }
        return value(swtch);
    }
    value default_code(runtime& runtime, value::cref right)
    {
        auto valswtch = runtime.context_active().get_variable(d_switch::magic);
        if (!valswtch.has_value() || !valswtch->is<t_switch>())
        {
            runtime.__logmsg(err::MagicVariableTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), d_switch::magic, t_switch(), valswtch->type()));
            return {};
        }
        auto swtch = valswtch->data<d_switch>();
        if (!swtch->has_match())
        {
            swtch->target_code(right.data<d_code, instruction_set>());
        }
        return {};
    }
    value colon_switch_code(runtime& runtime, value::cref left, value::cref right)
    {
        auto valswtch = runtime.context_active().get_variable(d_switch::magic);
        if (!valswtch.has_value() || !valswtch->is<t_switch>())
        {
            runtime.__logmsg(err::MagicVariableTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), d_switch::magic, t_switch(), valswtch->type()));
            return {};
        }
        auto swtch = valswtch->data<d_switch>();
        if (!swtch->has_match() && swtch->match_now())
        {
            swtch->target_code(right.data<d_code, instruction_set>());
            swtch->match_now(false);
            swtch->has_match(true);
            runtime.context_active().current_frame().seek(0, frame::seekpos::end);
        }
        return {};
    }
    value apply_array_code(runtime& runtime, value::cref left, value::cref right)
    {
        class behavior_apply_exit : public frame::behavior
        {
        private:
            std::shared_ptr<d_array> m_array;
            std::vector<value> m_out;
            size_t m_index;
            size_t m_size;
        public:
            behavior_apply_exit(std::shared_ptr<d_array> arr) : m_array(arr), m_index(0), m_size(arr->size()) {}
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                auto res = runtime.context_active().pop_value();
                if (res.has_value())
                {
                    m_out.push_back(*res);
                }
                else
                {
                    runtime.__logmsg(logmessage::runtime::CallstackFoundNoValue(frame.diag_info_from_position(), "apply"s));
                }

                if (m_size != m_array->size())
                {
                    runtime.__logmsg(logmessage::runtime::ArraySizeChanged(frame.diag_info_from_position(), m_size, m_array->size()));
                    m_size = m_array->size();
                }
                if (++m_index == m_size)
                {
                    runtime.context_active().push_value(m_out);
                    return result::ok;
                }
                else
                {
                    runtime.context_active().clear_values();
                    frame.clear_value_scope();
                    frame["_x"] = m_array->at(m_index);
                    return result::seek_start;
                }
            };
        };

        auto arr = left.data<d_array>();
        if (arr->size() > 0)
        {
            frame f(runtime.default_value_scope(), right.data<d_code, instruction_set>(), std::make_shared<behavior_apply_exit>(arr));
            f["_x"] = arr->at(0);
            runtime.context_active().push_frame(f);
            return {};
        }
        return std::vector<value>();
    }
    value spawn_any_code(runtime& runtime, value::cref left, value::cref right)
    {
        auto context_weak = runtime.context_create();
        auto lock = context_weak.lock();
        lock->can_suspend(true);
        lock->weak_error_handling(true);
        auto scriptdata = std::make_shared<d_script>(context_weak);
        frame f(runtime.default_value_scope(), right.data<d_code, instruction_set>());
        f["_thisScript"] = scriptdata;
        f["_this"] = left;
        lock->push_frame(f);
        return scriptdata;
    }
    value scriptnull_(runtime& runtime)
    {
        auto scriptdata = std::make_shared<d_script>();
        return scriptdata;
    }
    value scriptdone_script(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_script>();
        return r->done();
    }
    value isnull_script(runtime& runtime, value::cref right)
    { // Due to implementation as of now, null-scripts are the "same" as done scripts.
        auto r = right.data<d_script>();
        return r->done();
    }
    value terminate_script(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_script>();
        if (r->done())
        {
            runtime.__logmsg(err::ScriptHandleAlreadyFinished(runtime.context_active().current_frame().diag_info_from_position()));
        }
        else
        {
            auto lock = r->value();

            if (lock->terminate())
            {
                runtime.__logmsg(err::ScriptHandleAlreadyTerminated(runtime.context_active().current_frame().diag_info_from_position()));
            }
            else
            {
                lock->terminate(true);
            }
        }
        return {};
    }
    value set_array_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto arr = left.data<d_array>();
        auto params = right.data<d_array>()->value();
        if (params.size() != 2)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 2, params.size()));
            return {};
        }
        if (!params[0].is<t_scalar>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 0, t_scalar(), params[0].type()));
            return {};
        }

        auto index = params[0].data<d_scalar, int>();
        if (index < 0)
        {
            runtime.__logmsg(err::NegativeIndex(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto val = params[1];
        if (static_cast<int>(arr->size()) <= index)
        {
            arr->resize(index + 1);
        }
        auto oldval = (*arr)[index];
        (*arr)[index] = val;
        if (!arr->recursion_test())
        {
            (*arr)[index] = oldval;
            runtime.__logmsg(err::ArrayRecursion(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        return {};
    }
    value plus_array(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_array>();
        return r->copy_deep();
    }
    value plus_array_array(runtime& runtime, value::cref left, value::cref right)
    {
        // create a copy of left array
        auto arr = left.data<d_array, std::vector<sqf::runtime::value>>();
        auto r = right.data<d_array>();
        arr.insert(arr.end(), r->begin(), r->end());
        return arr;
    }


    value minus_array_array(runtime& runtime, value::cref left, value::cref right)
    {
        // TODO: optimize (e.g. build a hash based set of the right array and check against it)
        auto l = left.data<d_array>();
        auto r = right.data<d_array>();
        std::vector<value> result;

        std::copy_if(l->begin(), l->end(), std::back_inserter(result), [&r](value::cref current) {

            auto found = std::find(r->begin(), r->end(), current);
            // We only want element in output if it doesn't exist in right arg
            return found == r->end();
            });

        return result;
    }
    value append_array_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto arr = left.data<d_array>();
        auto r = right.data<d_array>();
        arr->insert(arr->end(), r->begin(), r->end());
        return {};
    }
    value arrayintersect_array_array(runtime& runtime, value::cref left, value::cref right)
    {
        // TODO: optimize (e.g. use hash set for checking)
        auto l = left.data<d_array>();
        auto r = right.data<d_array>();
        std::vector<value> result;

        std::copy_if(l->begin(), l->end(), std::back_inserter(result), [&r, &result](value::cref current) {

            auto found = std::find(result.begin(), result.end(), current);

            // Result already contains the element. Don't add it (remove duplicates)
            if (found != result.end()) return false;

            found = std::find(r->begin(), r->end(), current);

            // Only add if right argument also contains the element
            return found != r->end();
            });

        return result;
    }
    value deleteat_array_scalar(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_array>();
        auto index = right.data<d_scalar, int>();
        if (index >= static_cast<int>(l->size()))
        {
            runtime.__logmsg(err::IndexOutOfRangeWeak(runtime.context_active().current_frame().diag_info_from_position(), l->size(), index));
            return {};
        }
        else if (index < 0)
        {
            runtime.__logmsg(err::NegativeIndexWeak(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }
        auto val = l->at(index);
        l->erase(l->begin() + index);
        return val;
    }
    value find_array_any(runtime& runtime, value::cref left, value::cref right)
    {
        auto l = left.data<d_array>();
        auto found = std::find(l->begin(), l->end(), right);

        if (found != l->end())
            return static_cast<int>(std::distance(l->begin(), found));

        return -1;
    }
    value selectmax_array(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_array>();
        double max = std::numeric_limits<float>::lowest();
        for (size_t i = r->size() - 1; i != ~(size_t)0; i--)
        {
            auto tmp = r->at(i);
            if (tmp.is<t_scalar>())
            {
                if (tmp.data<d_scalar, float>() > max)
                {
                    max = tmp.data<d_scalar, float>();
                }
            }
            else if (tmp.is<t_boolean>())
            {
                if (tmp.data<d_boolean, bool>() ? 1 : 0 > max)
                {
                    max = tmp.data<d_boolean, bool>() ? 1 : 0;
                }
            }
            else
            {
                if (0 > max)
                {
                    max = 0;
                }
                runtime.__logmsg(err::ExpectedArrayTypeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), i, std::array<type, 2>{t_scalar(), t_boolean()}, tmp.type()));
            }
        }
        return max;
    }
    value selectmin_array(runtime& runtime, value::cref right)
    {
        auto r = right.data<d_array>();
        double min = std::numeric_limits<float>::max();
        for (size_t i = r->size() - 1; i != ~(size_t)0; i--)
        {
            auto tmp = r->at(i);
            if (tmp.is<t_scalar>())
            {
                if (tmp.data<d_scalar, float>() < min)
                {
                    min = tmp.data<d_scalar, float>();
                }
            }
            else if (tmp.is<t_boolean>())
            {
                if (tmp.data<d_boolean, bool>() ? 1 : 0 < min)
                {
                    min = tmp.data<d_boolean, bool>() ? 1 : 0;
                }
            }
            else
            {
                if (0 < min)
                {
                    min = 0;
                }
                runtime.__logmsg(err::ExpectedArrayTypeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), i, std::array<type, 2>{t_scalar(), t_boolean()}, tmp.type()));
            }
        }
        return min;
    }

    std::string from_char_array(const char* arr, size_t max_size)
    {
        const char* start = arr;
        size_t i;
        for (i = 0; i < max_size && arr[i] != '\0'; i++);
        return std::string(start, arr + (i == max_size ? i - 1 : i));
    }

    std::shared_ptr<dlops> helpermethod_callextension_loadlibrary(runtime& runtime, std::string name)
    {
        std::string dlname = name;

        #ifdef ENVIRONMENT64
            dlname += "_x64";
        #endif

        #ifndef _WIN32 // Append .so for Linux
            dlname += ".so";
        #endif

        static char buffer[CALLEXTVERSIONBUFFSIZE + 1] = { 0 };
        for (auto it : runtime.storage<dlops, sqf::operators::dlops_storage>())
        {
            if (it->path() == dlname)
            {
                return it;
            }
        }

        auto dl = std::make_shared<dlops>(dlname);

        runtime.storage<dlops, sqf::operators::dlops_storage>().push_back(dl);
        void* sym = nullptr;
        if (dl->try_resolve("RVExtensionVersion", &sym))
        {
            reinterpret_cast<RVExtensionVersion>(sym)(buffer, CALLEXTVERSIONBUFFSIZE);
            if (buffer[CALLEXTVERSIONBUFFSIZE - 1] != '\0')
            {
                runtime.__logmsg(err::ExtensionNotTerminatingVersionString(runtime.context_active().current_frame().diag_info_from_position(), name));
            }
            runtime.__logmsg(err::ExtensionLoaded(runtime.context_active().current_frame().diag_info_from_position(), name, from_char_array(buffer, CALLEXTVERSIONBUFFSIZE)));
        }
        else
        {
            runtime.__logmsg(err::ExtensionLoaded(runtime.context_active().current_frame().diag_info_from_position(), name, "NOT AVAILABLE"));
        }
        if (dl->try_resolve("RVExtensionRegisterCallback", &sym))
        {
            // auto method = reinterpret_cast<RVExtensionRegisterCallback>(sym);
            // ToDo: Create a way to actually execute this callback
            // runtime->out() << "[RPT]\tRegistered 'ExtensionCallback' with '" << name << '\'' << std::endl;
        }
        return dl;
    }
    value callextension_string_string(runtime& runtime, value::cref left, value::cref right)
    {
        std::unique_ptr<char[]> buffer = std::make_unique<char[]>(CALLEXTBUFFSIZE);
        auto libname = left.data<d_string, std::string>();
        if (libname.find('/') != std::string::npos || libname.find('\\') != std::string::npos)
        {
            runtime.__logmsg(err::LibraryNameContainsPath(runtime.context_active().current_frame().diag_info_from_position(), libname));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return "";
        }
        try
        {
            auto dl = helpermethod_callextension_loadlibrary(runtime, libname);
#if defined(_WIN32) & !defined(_WIN64)
            auto method = reinterpret_cast<RVExtension>(dl->resolve("_RVExtension@12"));
#else
            auto method = reinterpret_cast<RVExtension>(dl->resolve("RVExtension"));
#endif
            method(buffer.get(), CALLEXTBUFFSIZE, right.data<d_string, std::string>().c_str());
            if (buffer[CALLEXTBUFFSIZE - 1] != '\0')
            {
                runtime.__logmsg(err::ExtensionNotTerminatingCallExtensionBufferString(runtime.context_active().current_frame().diag_info_from_position(), libname));
                buffer[CALLEXTBUFFSIZE - 1] = '\0';
            }
            // find end-of-string
            auto start = buffer.get();
            auto it = start;
            for (; (it - start) < CALLEXTBUFFSIZE + 1 && *it != '\0'; ++it);
            // return string
            return std::string(start, it);
        }
        catch (const std::runtime_error& ex)
        {
            runtime.__logmsg(err::ExtensionRuntimeError(runtime.context_active().current_frame().diag_info_from_position(), libname, ex.what()));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return "";
        }
    }
    value callextension_string_array(runtime& runtime, value::cref left, value::cref right)
    {
        std::unique_ptr<char[]> buffer = std::make_unique<char[]>(CALLEXTBUFFSIZE);
        auto libname = left.data<d_string, std::string>();
        if (libname.find('/') != std::string::npos || libname.find('\\') != std::string::npos)
        {
            runtime.__logmsg(err::LibraryNameContainsPath(runtime.context_active().current_frame().diag_info_from_position(), libname));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return "";
        }
        auto rvec = right.data<d_array>();
        if (rvec->size() < 2)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), 2, rvec->size()));
            runtime.__logmsg(err::ReturningErrorCode(runtime.context_active().current_frame().diag_info_from_position(), "PARAMS_ERROR_TOO_MANY_ARGS(201)"s));
            return std::vector<value> { "", 0, 201 };
        }
        else if (rvec->size() > 2)
        {
            runtime.__logmsg(err::ExpectedArraySizeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), 2, rvec->size()));
        }
        if (!rvec->at(0).is<t_string>())
        {
            runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 0, t_string(), rvec->at(0).type()));
            return {};
        }
        std::shared_ptr<d_array> arr;
        if (rvec->at(1).is<t_array>())
        {
            arr = rvec->at(1).data<d_array>();
            if (arr->size() > RVARGSLIMIT)
            {
                runtime.__logmsg(err::ExpectedArraySizeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), 0, RVARGSLIMIT, arr->size()));
                runtime.__logmsg(err::ReturningErrorCode(runtime.context_active().current_frame().diag_info_from_position(), "SYNTAX_ERROR_WRONG_PARAMS_SIZE(101)"s));
                return std::vector<value> { "", 0, 101 };
            }
        }
        else
        {
            arr = std::make_shared<d_array>();
            arr->push_back(rvec->at(1));
        }
        std::vector<std::string> argstringvec;

        for (size_t i = 0; i < arr->size(); i++)
        {
            const auto& at = arr->at(i);
            if (at.is<t_boolean>() || at.is<t_string>() || at.is<t_scalar>() || at.is<t_array>())
            {
                argstringvec.push_back(at.data()->to_string_sqf());
            }
            else
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), i, std::array<type, 4> {t_boolean(), t_string(), t_scalar(), t_array()}, at.type()));
                runtime.__logmsg(err::ReturningErrorCode(runtime.context_active().current_frame().diag_info_from_position(), "SYNTAX_ERROR_WRONG_PARAMS_TYPE(102)"s));
                return std::vector<value>{ "", 0, 102 };
            }
        }
        std::vector<const char*> argvec;
        argvec.reserve(argstringvec.size());
        for (auto& it : argstringvec)
        {
            argvec.push_back(it.c_str());
        }

        try
        {
            auto dl = helpermethod_callextension_loadlibrary(runtime, libname);
#if defined(_WIN32) & !defined(_WIN64)
            auto method = reinterpret_cast<RVExtensionArgs>(dl->resolve("_RVExtensionArgs@20"));
#else
            auto method = reinterpret_cast<RVExtensionArgs>(dl->resolve("RVExtensionArgs"));
#endif
            auto res = method(buffer.get(), CALLEXTBUFFSIZE, rvec->at(0).data<d_string, std::string>().c_str(), argvec.data(), static_cast<int>(argvec.size()));
            if (buffer[CALLEXTBUFFSIZE - 1] != '\0')
            {
                runtime.__logmsg(err::ExtensionNotTerminatingCallExtensionBufferString(runtime.context_active().current_frame().diag_info_from_position(), libname));
                buffer[CALLEXTBUFFSIZE - 1] = '\0';
            }
            // find end-of-string
            auto start = buffer.get();
            auto it = start;
            for (; (it - start) < CALLEXTBUFFSIZE + 1 && *it != '\0'; ++it);
            // return string
            return std::vector<value>{ std::string(start, it), res, 0 };
        }
        catch (const std::runtime_error& ex)
        {
            runtime.__logmsg(err::ExtensionRuntimeError(runtime.context_active().current_frame().diag_info_from_position(), libname, ex.what()));
            runtime.__logmsg(err::ReturningErrorCode(runtime.context_active().current_frame().diag_info_from_position(), "501"s));
            return std::vector<value>{ "", 0, 501 };
        }
    }

    value param_any_array(runtime& runtime, value::cref src, value::cref trgt)
    {
        // Transform source to d_array if type is not d_array
        std::shared_ptr<d_array> input_values;
        if (src.is<t_array>())
        {
            input_values = src.data<d_array>();
        }
        else
        {
            input_values = std::make_shared<d_array>();
            input_values->push_back(src);
        }


        auto params_descriptors = trgt.data<d_array>()->value(); //#TODO don't copy
        size_t i = 0;
        bool flag;

        if (!params_descriptors.empty())
        {
            // validation step
            if (params_descriptors.size() >= 1 && !params_descriptors.at(0).is<t_scalar>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 0, t_scalar(), params_descriptors.at(0).type()));
                return {};
            }
            else
            {
                i = params_descriptors.at(0).data<d_scalar, int>();
            }
            if (params_descriptors.size() >= 3 && !params_descriptors.at(2).is<t_array>())
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 0, t_array(), params_descriptors.at(2).type()));
                return {};
            }
            if (params_descriptors.size() >= 4 && (!params_descriptors.at(3).is<t_array>() && !params_descriptors.at(3).is<t_scalar>()))
            {
                runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 0, std::array<type, 2> { t_array(), t_scalar() }, params_descriptors.at(3).type()));
                return {};
            }
            else if (params_descriptors.size() >= 4 && !params_descriptors.at(3).is<t_array>())
            {
                auto tmp = params_descriptors.at(3).data<d_array>();
                flag = false;
                for (size_t j = 0; j < tmp->size(); j++)
                {
                    if (!tmp->at(j).is<t_scalar>())
                    {
                        runtime.__logmsg(err::ExpectedSubArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), std::array<size_t, 2> { 3, j }, t_scalar(), tmp->at(j).type()));
                        flag = true;
                        continue;
                    }
                }
                if (flag)
                {
                    return {};
                }
            }

            if (i < input_values->size())
            {
                auto current_input_value = input_values->at(i);
                if (params_descriptors.size() >= 3 && !params_descriptors.at(2).data<d_array>()->empty())
                {
                    auto tmp = params_descriptors.at(2).data<d_array>();

                    auto found = std::find_if(tmp->begin(), tmp->end(), [type = current_input_value.type()](value::cref val) {
                        return val.is(type);
                    });

                    flag = found != tmp->end();

                    if (!flag)
                    {
                        std::vector<type> types;
                        for (const auto& it : *tmp)
                        {
                            types.push_back(it.type());
                        }
                        runtime.__logmsg(err::ExpectedArrayTypeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), i, types, current_input_value.type()));
                        return params_descriptors.at(1);
                    }
                }
                if (params_descriptors.size() >= 4 && current_input_value.is<t_array>())
                {
                    flag = true;
                    auto tmp = params_descriptors.at(2).data<d_array>();

                    if (params_descriptors.at(2).is<t_array>())
                    {// Check available datatypes
                        auto found = std::find_if(tmp->begin(), tmp->end(), [type = current_input_value.type()](value::cref val) {
                            return val.is(type);
                        });
                        flag = found != tmp->end();
                    }
                    else if ((int)current_input_value.data<d_array>()->size() != params_descriptors.at(3).data<d_scalar, int>())
                    { // Check available datatypes
                        flag = false;
                    }
                    if (!flag)
                    {
                        std::vector<type> types;
                        for (const auto& it : *tmp)
                        {
                            types.push_back(it.type());
                        }
                        runtime.__logmsg(err::ExpectedArrayTypeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), i, types, current_input_value.type()));
                        return params_descriptors.at(1);
                    }
                }
                return current_input_value;
            }
            else
            {
                return params_descriptors.size() == 2 ? params_descriptors.at(1) : value();
            }
        }
        return {};
    }
    value param_array(runtime& runtime, value::cref right)
    {
        auto _this = runtime.context_active().get_variable("_this");
        if (!_this.has_value())
        {
            runtime.__logmsg(err::MagicVariableNotPresent(runtime.context_active().current_frame().diag_info_from_position(), "_this"));
            return {};
        }
        if (!_this->is<t_array>())
        {
            auto arr = std::make_shared<d_array>();
            arr->push_back(*_this);
            return param_any_array(runtime, arr, right);
        }
        return param_any_array(runtime, *_this, right);
    }
    value params_array_array(runtime& runtime, value::cref src, value::cref trgt)
    {
        auto elements = src.data<d_array>();
        auto format = trgt.data<d_array>();
        bool flag;
        for (size_t i = 0; i < format->size(); i++)
        {
            auto fel = format->at(i);
            std::vector<value> params_descriptors;
            if (fel.is<t_array>())
            {
                params_descriptors = fel.data<d_array>()->value();
            }
            else
            {
                params_descriptors = { fel };
            }
            //validation step
            if (params_descriptors.empty() || !params_descriptors.at(0).is<t_string>())
            {
                if (fel.is<t_array>())
                { // Empty Array
                    runtime.__logmsg(err::ExpectedArraySizeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), 1, 4, 0));
                }
                else
                { // Non-Valid value
                    runtime.__logmsg(err::ExpectedArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), i, std::array<type, 2> { t_string(), t_array() }, fel.type()));
                }
                continue;
            }
            if (params_descriptors.size() >= 3 && !params_descriptors.at(2).is<t_array>())
            {
                runtime.__logmsg(err::ExpectedSubArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), std::array<size_t, 2> { i, 2 }, t_array(), params_descriptors.at(2).type()));
                continue;
            }
            if (params_descriptors.size() >= 4 && (!params_descriptors.at(3).is<t_array>() && !params_descriptors.at(3).is<t_scalar>()))
            {
                runtime.__logmsg(err::ExpectedSubArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), std::array<size_t, 2> { i, 3 }, std::array<type, 2> { t_scalar(), t_array() }, params_descriptors.at(2).type()));
                continue;
            }
            else if (params_descriptors.size() >= 4 && params_descriptors.at(3).is<t_array>())
            {
                auto tmp = params_descriptors.at(3).data<d_array>();
                flag = false;
                for (size_t j = 0; j < tmp->size(); j++)
                {
                    if (!tmp->at(j).is<t_scalar>())
                    {
                        runtime.__logmsg(err::ExpectedSubArrayTypeMissmatch(runtime.context_active().current_frame().diag_info_from_position(), std::array<size_t, 3> { i, 3, j }, t_scalar(), tmp->at(j).type()));
                        flag = true;
                        continue;
                    }
                }
                if (flag)
                {
                    continue;
                }
            }

            if (i < elements->size())
            {
                auto current_input_value = elements->at(i);
                if (params_descriptors.size() >= 3 && !params_descriptors.at(2).data<d_array>()->empty())
                {
                    auto tmp = params_descriptors.at(2).data<d_array>();

                    auto found = std::find_if(tmp->begin(), tmp->end(), [type = current_input_value.type()](value::cref val) {
                        return val.is(type);
                    });

                    flag = found != tmp->end();

                    if (!flag)
                    {
                        std::vector<type> types;
                        for (const auto& it : *tmp)
                        {
                            types.push_back(it.type());
                        }
                        runtime.__logmsg(err::ExpectedArrayTypeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), i, types, current_input_value.type()));
                        current_input_value = params_descriptors.at(1);
                    }
                }
                if (params_descriptors.size() >= 4 && current_input_value.is<t_array>())
                {
                    flag = true;
                    auto tmp = params_descriptors.at(2).data<d_array>();
                    if (params_descriptors.at(2).is<t_array>())
                    {
                        auto found = std::find_if(tmp->begin(), tmp->end(), [type = current_input_value.type()](value::cref val) {
                            return val.is(type);
                        });

                        flag = found != tmp->end();
                    }
                    else if ((int)current_input_value.data<d_array>()->size() != params_descriptors.at(3).data<d_scalar, int>())
                    {
                        flag = false;
                    }
                    if (!flag)
                    {
                        std::vector<type> types;
                        for (const auto& it : *tmp)
                        {
                            types.push_back(it.type());
                        }
                        runtime.__logmsg(err::ExpectedArrayTypeMissmatchWeak(runtime.context_active().current_frame().diag_info_from_position(), i, types, current_input_value.type()));
                        current_input_value = params_descriptors.at(1);
                    }
                }
                runtime.context_active().current_frame()[params_descriptors.at(0).data<d_string, std::string>()] = current_input_value;
            }
            else
            {
                runtime.context_active().current_frame()[params_descriptors.at(0).data<d_string, std::string>()] = params_descriptors.size() >= 2 ? params_descriptors.at(1) : value();
            }
        }
        return {};
    }
    value params_array(runtime& runtime, value::cref right)
    {
        auto _this = runtime.context_active().get_variable("_this");
        if (!_this.has_value())
        {
            runtime.__logmsg(err::MagicVariableNotPresent(runtime.context_active().current_frame().diag_info_from_position(), "_this"));
            return {};
        }
        if (!_this->is<t_array>())
        {
            auto arr = std::make_shared<d_array>();
            arr->push_back(*_this);
            return params_array_array(runtime, arr, right);
        }
        return params_array_array(runtime, *_this, right);
    }



    value selectrandom_array(runtime& runtime, value::cref right)
    {
        auto arr = right.data<d_array>();
        return arr->at(rand() % arr->size());
    }
    value sleep_scalar(runtime& runtime, value::cref right)
    {
        if (!runtime.context_active().can_suspend())
        {
            runtime.__logmsg(err::SuspensionInUnscheduledEnvironment(runtime.context_active().current_frame().diag_info_from_position()));
            return {};
        }

        float f = right.data<d_scalar, float>();
        auto duration = std::chrono::duration<float>(f);
        auto durationCasted = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

        runtime.context_active().suspend(durationCasted);
        return {};
    }
    value cansuspend_(runtime& runtime)
    {
        return runtime.context_active().can_suspend();
    }
    value loadfile_string(runtime& runtime, value::cref right)
    {
        auto& fileio = runtime.fileio();
        auto pathinfo = fileio.get_info(right.data<d_string, std::string_view>(), {});
        if (pathinfo.has_value())
        {
            return fileio.read_file(*pathinfo);
        }
        else
        {
            runtime.__logmsg(err::FileNotFound(runtime.context_active().current_frame().diag_info_from_position(), right.data<d_string, std::string>()));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return "";
        }
    }
    value preprocessfile_string(runtime& runtime, value::cref right)
    {
        auto& fileio = runtime.fileio();
        auto pathinfo = fileio.get_info(right.data<d_string, std::string_view>(), {});
        if (pathinfo.has_value())
        {
            auto& preproc = runtime.parser_preprocessor();
            auto str = preproc.preprocess(runtime, *pathinfo);
            return str.has_value() ? *str : ""s;
        }
        else
        {
            runtime.__logmsg(err::FileNotFound(runtime.context_active().current_frame().diag_info_from_position(), right.data<d_string, std::string>()));
            runtime.__logmsg(err::ReturningEmptyString(runtime.context_active().current_frame().diag_info_from_position()));
            return "";
        }
    }
    value scopename_string(runtime& runtime, value::cref right)
    {
        auto str = right.data<d_string, std::string>();
        if (runtime.context_active().current_frame().scope_name().empty())
        {
            runtime.context_active().current_frame().scope_name(str);
        }
        else
        {
            runtime.__logmsg(err::ScopeNameAlreadySet(runtime.context_active().current_frame().diag_info_from_position()));
        }
        return {};
    }
    value scriptname_string(runtime& runtime, value::cref right)
    {
        auto str = right.data<d_string, std::string>();
        if (runtime.context_active().name().empty())
        {
            runtime.context_active().name(str);
        }
        else
        {
            runtime.__logmsg(err::ScriptNameAlreadySet(runtime.context_active().current_frame().diag_info_from_position()));
        }
        return {};
    }
    value in_any_array(runtime& runtime, value::cref left, value::cref right)
    {
        auto arr = right.data<d_array>();
        auto res = std::find(arr->begin(), arr->end(), left);
        return res != arr->end();
    }
    value in_string_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto needle = left.data<d_string, std::string>();
        auto haystack = right.data<d_string, std::string>();
        return haystack.find(needle) != std::string::npos;
    }
    value time_(runtime& runtime)
    {
        auto curtime = std::chrono::system_clock::now();
        auto starttime = runtime.runtime_timestamp();
        // Time is since beginning of game so long is fine.
        long r = static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(curtime - starttime).count());
        return (float)r * 0.001;
    }
    value throw_any(runtime& runtime, value::cref right)
    {
        auto res = std::find_if(runtime.context_active().frames_rbegin(), runtime.context_active().frames_rend(), [&](frame& f) -> bool {
            return f.can_recover_runtime_error();
            });
        if (res == runtime.context_active().frames_rend())
        {
            runtime.__logmsg(err::ErrorMessage(runtime.context_active().current_frame().diag_info_from_position(), "THROW", right.data()->to_string_sqf()));
        }
        else
        {
            std::vector<sqf::runtime::frame> stacktrace_frames(runtime.context_active().frames_rbegin(), runtime.context_active().frames_rend());
            sqf::runtime::diagnostics::stacktrace stacktrace(stacktrace_frames);
            stacktrace.value = right;
            auto valpos = runtime.context_active().values_size();
            runtime.context_active().push_value(stacktrace);
            if (res->recover_runtime_error(runtime) == frame::result::error)
            {
                if (valpos > 0)
                {
                    runtime.context_active().pop_value();
                }
                runtime.__logmsg(err::ErrorMessage(runtime.context_active().current_frame().diag_info_from_position(), "THROW", right.data()->to_string_sqf()));
                return {};
            }

            auto drop = res - runtime.context_active().frames_rbegin();
            while (drop-- != 0)
            {
                runtime.context_active().pop_frame();
            }
        }
        return {};
    }
    value throw_if_any(runtime& runtime, value::cref left, value::cref right)
    {
        if (left.data<d_boolean, bool>())
        {
            return throw_any(runtime, right);
        }
        return {};
    }
    value try_code(runtime& runtime, value::cref right)
    {
        return std::make_shared<d_exception>(right.data<d_code, sqf::runtime::instruction_set>());
    }
    value catch_exception_code(runtime& runtime, value::cref left, value::cref right)
    {
        class behavior_catch_exit : public frame::behavior
        {
        private:
            instruction_set m_set;
        public:
            behavior_catch_exit(instruction_set set) : m_set(set) {}
            virtual sqf::runtime::instruction_set get_instruction_set(sqf::runtime::frame& frame) override { return m_set; };
            virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
            {
                if (runtime.__runtime_error())
                {
                    return result::fail;
                }
                else
                {
                    auto val = runtime.context_active().pop_value();
                    runtime.context_active().clear_values();
                    frame.clear_value_scope();
                    if (val.has_value() && val->is<t_stacktrace>())
                    {
                        frame["_exception"] = val->data<d_stacktrace>()->value().value;
                    }
                    else
                    {
                        frame["_exception"] = {};
                    }
                    return result::exchange;
                }
            };
        };
        frame f(
            runtime.default_value_scope(),
            left.data<d_code, sqf::runtime::instruction_set>(),
            {},
            std::make_shared<behavior_catch_exit>(right.data<d_code, sqf::runtime::instruction_set>()));
        runtime.context_active().push_frame(f);
        return {};
    }
    value execvm_any_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto& fileio = runtime.fileio();
        auto pathinfo = fileio.get_info(right.data<d_string, std::string_view>(), {});
        if (pathinfo.has_value())
        {
            auto& preproc = runtime.parser_preprocessor();
            auto str = preproc.preprocess(runtime, *pathinfo);
            if (str.has_value())
            {
                auto r = right.data<d_string, std::string>();
                auto& parser = runtime.parser_sqf();
                auto res = parser.parse(runtime, r, runtime.context_active().current_frame().diag_info_from_position().path);
                if (res.has_value())
                {
                    auto context_weak = runtime.context_create();
                    auto lock = context_weak.lock();
                    auto scriptdata = std::make_shared<d_script>(context_weak);
                    frame f(runtime.default_value_scope(), res.value());
                    f["_thisScript"] = scriptdata;
                    f["_this"] = left;
                    lock->push_frame(f);
                    return scriptdata;
                }
                else
                {
                    runtime.__logmsg(err::ReturningEmptyScriptHandle(runtime.context_active().current_frame().diag_info_from_position()));
                    return std::make_shared<d_script>();
                }
            }
            else
            {
                runtime.__logmsg(err::ReturningEmptyScriptHandle(runtime.context_active().current_frame().diag_info_from_position()));
                return std::make_shared<d_script>();
            }
        }
        else
        {
            runtime.__logmsg(err::FileNotFound(runtime.context_active().current_frame().diag_info_from_position(), right.data<d_string, std::string>()));
            runtime.__logmsg(err::ReturningEmptyScriptHandle(runtime.context_active().current_frame().diag_info_from_position()));
            return std::make_shared<d_script>();
        }
    }
    value execvm_string(runtime& runtime, value::cref right)
    {
        return execvm_any_string(runtime, {}, right);
    }
    value breakout_any_string(runtime& runtime, value::cref left, value::cref right)
    {
        auto target_scope = right.data<d_string>()->value();
        auto& context = runtime.context_active();
        size_t pop_frame_count = 0;
        if (target_scope.empty())
        { // Empty just pops
            context.pop_frame();
            return left;
        }
        else
        {
            for (auto it = context.frames_rbegin(); it != context.frames_rend(); ++it, ++pop_frame_count)
            {
                if (it->scope_name() == target_scope)
                {
                    for (pop_frame_count++; pop_frame_count != 0; --pop_frame_count)
                    {
                        context.pop_frame();
                    }
                    return left;
                }
            }
            runtime.__logmsg(err::ScopeNameNotFound(runtime.context_active().current_frame().diag_info_from_position(), target_scope));
            return {};
        }
    }
    value breakout_string(runtime& runtime, value::cref right)
    {
        return breakout_any_string(runtime, {}, right);
    }
    value disableserialization(runtime& runtime)
    {
        return {};
    }
}
void sqf::operators::ops_generic(sqf::runtime::runtime& runtime)
{
    using namespace sqf::runtime::sqfop;

    runtime.register_sqfop(nular("time", "Returns time elapsed since virtualmachine start.", time_));
    runtime.register_sqfop(nular("nil", "Nil value. This value can be used to undefine existing variables.", nil_));
    runtime.register_sqfop(unary("call", t_code(), "Executes given set of compiled instructions.", call_code));
    runtime.register_sqfop(binary(4, "call", t_any(), t_code(), "Executes given set of compiled instructions with an option to pass arguments to the executed Code.", call_any_code));
    runtime.register_sqfop(unary("count", t_array(), "Can be used to count: the number of elements in array.", count_array));
    runtime.register_sqfop(binary(4, "count", t_code(), t_array(), "Can be used to count: the number of elements in array with condition.", count_code_array));
    runtime.register_sqfop(unary("compile", t_string(), "Compile expression.", compile_string));
    runtime.register_sqfop(unary("typeName", t_any(), "Returns the data type of an expression.", typename_any));
    runtime.register_sqfop(unary("str", t_any(), "Converts any value into a string.", str_any));
    runtime.register_sqfop(unary("comment", t_string(), "Define a comment. Mainly used in SQF Syntax, as you're able to introduce comment lines with semicolons in a SQS script.", comment_string));
    runtime.register_sqfop(unary("if", t_boolean(), "This operator creates a If Type which is used in conjunction with the 'then' command.", if_bool));
    runtime.register_sqfop(binary(4, "then", t_if(), t_array(), "First or second element of array is executed depending on left arg. Result of the expression executed is returned as a result (result may be Nothing).", then_if_array));
    runtime.register_sqfop(binary(4, "then", t_if(), t_code(), "If left arg is true, right arg is executed. Result of the expression executed is returned as a result (result may be Nothing).", then_if_code));
    runtime.register_sqfop(binary(5, "else", t_code(), t_code(), "Concats left and right element into a single, 2 element array.", else_code_code));
    runtime.register_sqfop(binary(4, "exitWith", t_if(), t_code(), "If condition evaluates to true, executes the code in a new scope and exits the current one afterwards.", exitwith_if_code));
    runtime.register_sqfop(unary("while", t_code(), "Marks code as WHILE type.", while_code));
    runtime.register_sqfop(unary("waitUntil", t_code(), "Suspends execution of scheduled script until the given condition satisfied. This command will loop and call the code inside {} until the code returns true.", waituntil_code));

    runtime.register_sqfop(binary(4, "do", t_while(), t_code(), "Executes provided code as long as while condition evaluates to true.", do_while_code));
    runtime.register_sqfop(unary("for", t_string(), "Creates a FOR type for usage in 'for <var> from <start> to <end> [ step <stepsize> ] do <code>' construct.", for_string));
    runtime.register_sqfop(binary(4, "from", t_for(), t_scalar(), "Sets the start index in a FOR type construct.", from_for_scalar));
    runtime.register_sqfop(binary(4, "to", t_for(), t_scalar(), "Sets the end index in a FOR type construct.", to_for_scalar));
    runtime.register_sqfop(binary(4, "step", t_for(), t_scalar(), "Sets the step size (default: 1) in a FOR type construct.", step_for_scalar));
    runtime.register_sqfop(binary(4, "do", t_for(), t_code(), "Executes provided code as long as the var is smaller then the end index.", do_for_code));
    runtime.register_sqfop(binary(4, "forEach", t_code(), t_array(), "Executes the given command(s) on every item of an array. The array items are represented by the magic variable _x. The array indices are represented by _forEachIndex.", foreach_code_array));

    runtime.register_sqfop(binary(4, "select", t_array(), t_scalar(), "Selects the element at provided index from array. If the index provided equals the array length, nil will be returned.", select_array_scalar));
    runtime.register_sqfop(binary(9, "#", t_array(), t_scalar(), "Selects the element at provided index from array. If the index provided equals the array length, nil will be returned.", select_array_scalar));
    runtime.register_sqfop(unary("selectRandom", t_array(), "Returns a random element from the given array.", selectrandom_array));

    runtime.register_sqfop(binary(4, "select", t_array(), t_boolean(), "Selects the first element if provided boolean is false, second element if it is true.", select_array_bool));
    runtime.register_sqfop(binary(4, "select", t_array(), t_array(), "Selects a range of elements in provided array, starting at element 0 index, ending at either end of the string or the provided element 1 length.", select_array_array));
    runtime.register_sqfop(binary(4, "select", t_array(), t_code(), "Selects elements from provided array matching provided condition. Current element will be placed in _x variable.", select_array_code));
    runtime.register_sqfop(binary(4, "resize", t_array(), t_scalar(), "Changes the size of the given array. The command does not return new array, it resizes the source array to the desired number of elements. If the new size is bigger than the current size, the new places are filled with nils.", resize_array_scalar));
    runtime.register_sqfop(binary(4, "deleterange", t_array(), t_array(), "Removes a range of array elements from the given array (modifies the original array, just like resize or set).", deleterange_array_array));

    runtime.register_sqfop(binary(4, "pushBack", t_array(), t_any(), "Insert an element to the back of the given array. This command modifies the original array. Returns the index of the newly added element.", pushback_array_any));
    runtime.register_sqfop(binary(4, "pushBackUnique", t_array(), t_any(), "Adds element to the back of the given array but only if it is unique to the array. The index of the added element is returned upon success, otherwise -1. This command modifies the original array.", pushbackunique_array_any));
    runtime.register_sqfop(binary(4, "findIf", t_array(), t_code(), "Searches for an element within array for which the code evaluates to true. Returns the 0 based index on success or -1 if not found. Code on the right side of the command is evaluated for each element of the array, processed element can be referenced in code as _x.", findif_array_code));
    runtime.register_sqfop(binary(4, "sort", t_array(), t_boolean(), "Attempts to sort given array either in ascending (true) or descending (false) order.", sort_array_boolean));
    runtime.register_sqfop(unary("reverse", t_array(), "Reverses given array by reference. Modifies the original array.", reverse_array));
    runtime.register_sqfop(unary("private", t_string(), "Sets a variable to the innermost scope.", private_string));
    runtime.register_sqfop(unary("private", t_array(), "Sets a bunch of variables to the innermost scope.", private_array));
    runtime.register_sqfop(unary("isNil", t_string(), "Tests whether the variable defined by the string argument is undefined.", isnil_string));
    runtime.register_sqfop(unary("isNil", t_code(), "Tests whether an expression result passed as code is undefined.", isnil_code));
    runtime.register_sqfop(unary("hint", t_string(), "Outputs a hint message.", hint_string));
    runtime.register_sqfop(unary("hint", t_text(), "Outputs a hint message.", hint_text));
    runtime.register_sqfop(unary("systemChat", t_string(), "Types text to the system radio channel.", systemchat_string));
    runtime.register_sqfop(nular("productVersion", "Returns basic info about the product.", productversion_));

    runtime.register_sqfop(unary("switch", t_any(), "Creates a SWITCH type that can be used in 'switch do {...}'.", switch_any));
    runtime.register_sqfop(binary(4, "do", t_switch(), t_code(), "Executes provided code and sets the magic switch variable.", do_switch_code));
    runtime.register_sqfop(unary("case", t_any(), "Command to create a case inside a switch do construct. Will check if argument matches the one provided in switch strict. Requires a magic variable to be set. Cannot be used outside of switch do codeblock!", case_any));
    runtime.register_sqfop(binary(4, ":", t_switch(), t_code(), "Checks if switch type has the case flag being set and executes provided code then. If another switch got executed already, nothing will be done.", colon_switch_code));
    runtime.register_sqfop(unary("default", t_code(), "Sets the code to be executed by default if no case matched.", default_code));
    runtime.register_sqfop(binary(4, "apply", t_array(), t_code(), "Applies given code to each element of the array and returns resulting array. The value of the current array element, to which the code will be applied, is stored in variable _x.", apply_array_code));
    runtime.register_sqfop(nular("scriptNull", "A non-existing Script or script that has finished.", scriptnull_));
    runtime.register_sqfop(binary(4, "spawn", t_any(), t_code(), "Adds given code to the scheduler. For SQF-runtime, every script is guaranteed to get the same ammount of instructions done before being suspended.", spawn_any_code));
    runtime.register_sqfop(unary("scriptDone", t_script(), "Check if a script is finished running using the Script_(Handle).", scriptdone_script));
    runtime.register_sqfop(unary("terminate", t_script(), "Terminates (aborts) spawned or execVMed script. "
        "The given script will not terminate immediately upon terminate command execution, it will do so the next time the script is processed by the scheduler", terminate_script));
    runtime.register_sqfop(unary("isNull", t_script(), "Checks if a given script is null.", isnull_script));



    runtime.register_sqfop(binary(4, "set", t_array(), t_array(), "Changes the element at the given (zero-based) index of the array. If the array size is smaller then the index provided, it is resized to allow for the index to be set.", set_array_array));
    runtime.register_sqfop(unary("+", t_array(), "Returns a copy of an array.", plus_array));
    runtime.register_sqfop(binary(6, "+", t_array(), t_array(), "Concatenates two arrays.", plus_array_array));
    runtime.register_sqfop(binary(6, "-", t_array(), t_array(), "Creates a new array with elements from the first parameter, that don't occur in the second one.", minus_array_array));
    runtime.register_sqfop(binary(4, "append", t_array(), t_array(), "Appends the second array's element to the first array.", append_array_array));
    runtime.register_sqfop(binary(4, "arrayIntersect", t_array(), t_array(), "Creates a new array that contains unique elements that occur in the first and the second array.", arrayintersect_array_array));
    runtime.register_sqfop(binary(4, "deleteAt", t_array(), t_scalar(), "Deletes the item at a given array position.", deleteat_array_scalar));
    runtime.register_sqfop(binary(4, "find", t_array(), t_any(), "Returns the index of an element in the array, or -1.", find_array_any));

    runtime.register_sqfop(unary("selectMax", t_array(), "Returns the array element with maximum numerical value. Therefore it is expected that supplied array consists of Numbers only. Booleans however are also supported and will be evaluated as Numbers: true - 1, false - 0. nil value treated as 0. Other non Number elements (not recommended) will be evaluated as 0 and Bad conversion: scalar message will be logged.", selectmax_array));
    runtime.register_sqfop(unary("selectMin", t_array(), "Returns the array element with minimum numerical value. Therefore it is expected that supplied array consists of Numbers only. Booleans however are also supported and will be evaluated as Numbers: true - 1, false - 0. nil value treated as 0. Other non Number elements (not recommended) will be evaluated as 0 and Bad conversion: scalar message will be logged.", selectmin_array));
    runtime.register_sqfop(binary(4, "execVM", t_any(), t_string(), "Compiles and adds SQF Script to the scheduler queue and returns script handle.", execvm_any_string));
    runtime.register_sqfop(unary("execVM", t_string(), "Compiles and adds SQF Script to the scheduler queue and returns script handle.", execvm_string));
    runtime.register_sqfop(binary(4, "callExtension", t_string(), t_string(), "See https://community.bistudio.com/wiki/callExtension", callextension_string_string));
    runtime.register_sqfop(binary(4, "callExtension", t_string(), t_array(), "See https://community.bistudio.com/wiki/callExtension", callextension_string_array));

    runtime.register_sqfop(unary("params", t_array(), "Parses arguments inside of _this into array of private variables.", params_array));
    runtime.register_sqfop(binary(4, "params", t_array(), t_array(), "Parses input argument into array of private variables.", params_array_array));
    runtime.register_sqfop(unary("param", t_array(), "Extracts a single value with given index from _this.", param_array));
    runtime.register_sqfop(binary(4, "param", t_any(), t_array(), "Extracts a single value with given index from input argument.", param_any_array));
    runtime.register_sqfop(unary("sleep", t_scalar(), "Suspends code execution for given time in seconds. The delay given is the minimal delay expected.", sleep_scalar));
    runtime.register_sqfop(unary("uiSleep", t_scalar(), "Suspends code execution for given time in seconds. The delay given is the minimal delay expected.", sleep_scalar));
    runtime.register_sqfop(nular("canSuspend", "Returns true if sleep, uiSleep or waitUntil commands can be used in current scope.", cansuspend_));
    runtime.register_sqfop(unary("loadFile", t_string(), "", loadfile_string));
    runtime.register_sqfop(unary("preprocessFileLineNumbers", t_string(), "Reads and processes the content of the specified file. Preprocessor is C-like, supports comments using // or /* and */ and PreProcessor Commands.", preprocessfile_string));
    runtime.register_sqfop(unary("preprocessFile", t_string(), "Reads and processes the content of the specified file. Preprocessor is C-like, supports comments using // or /* and */ and PreProcessor Commands.", preprocessfile_string));
    runtime.register_sqfop(unary("scopeName", t_string(), "Defines name of current scope. Name is visible in debugger, and name is also used as reference in some commands like breakOut and breakTo. Scope name should be defined only once per scope. Trying to set a different name on the scope that has already defined scope name will result in error.", scopename_string));
    runtime.register_sqfop(unary("scriptName", t_string(), "Assign a user friendly name to the runtime script this command is executed from. Once name is assigned, it cannot be changed.", scriptname_string));
    runtime.register_sqfop(unary("breakOut", t_string(), "Breaks the code execution out of the scope with the name as provided and previously set using `scopeName`.", breakout_string));
    runtime.register_sqfop(binary(4, "breakOut", t_any(), t_string(), "Breaks the code execution out of the scope with the name as provided and previously set using `scopeName`.", breakout_any_string));
    runtime.register_sqfop(binary(4, "in", t_any(), t_array(), "Checks whether value is in array. String values will be compared casesensitive.", in_any_array));
    runtime.register_sqfop(binary(4, "in", t_string(), t_string(), "Checks whether string is in string. Values will be compared casesensitive.", in_string_string));
    runtime.register_sqfop(unary("throw", t_any(), "Throws an exception. The exception is processed by first catch block. This command will terminate further execution of the code.", throw_any));
    runtime.register_sqfop(binary(4, "throw", t_if(), t_any(), "Throws an exception. The exception is processed by first catch block. This command will terminate further execution of the code.", throw_if_any));
    runtime.register_sqfop(unary("try", t_code(), "Defines a try-catch structure. This sets up an exception handling block.", try_code));
    runtime.register_sqfop(binary(4, "catch", t_exception(), t_code(), "Processes code when an exception is thrown in a try block. The exception caught can be found in the _exception variable.", catch_exception_code));
    runtime.register_sqfop(nular("disableSerialization", "Disable saving of script containing this command.", disableserialization));
}
