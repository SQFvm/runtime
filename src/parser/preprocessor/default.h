#pragma once

#include "../../runtime/parser/preprocessor.h"
#include "../../runtime/logging.h"
#include "../../runtime/diagnostics/diag_info.h"
#include "../../runtime/fileio.h"

#include <cassert>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>


namespace sqf::parser::preprocessor {
    class impl_default : public ::sqf::runtime::parser::preprocessor, public CanLog {

    private:
        std::unordered_map<std::string, ::sqf::runtime::parser::macro> m_macros;
        std::unordered_map<std::string, ::sqf::runtime::parser::pragma> m_pragmas;
        struct condition_scope {
            bool allow_write{};
            ::sqf::runtime::diagnostics::diag_info info_if;
            ::sqf::runtime::diagnostics::diag_info info_else;
        };
        struct file_scope {
            ::sqf::runtime::fileio::pathinfo path;
            std::vector<condition_scope> conditions;
        };

        class instance : public CanLog {
        public:
            instance(
                    impl_default *owner,
                    Logger &logger,
                    std::unordered_map<std::string, ::sqf::runtime::parser::macro> macros)
                    : CanLog(logger),
                      m_owner(owner),
                      m_macros(std::move(macros)) {};
            std::vector<file_scope> m_file_scopes;
            std::unordered_set<std::string> m_visited;
            bool m_errflag = false;
            impl_default *m_owner;
            std::unordered_map<std::string, ::sqf::runtime::parser::macro> m_macros;

            void replace_stringify(
                    ::sqf::runtime::runtime &runtime,
                    context &local_fileinfo,
                    context &original_fileinfo,
                    const ::sqf::runtime::parser::macro &m,
                    std::vector<std::string> &params,
                    std::stringstream &sstream,
                    const std::unordered_map<std::string, std::string> &param_map,
                    std::vector<const ::sqf::runtime::parser::macro *> &macro_stack);

            void replace_concat(
                    ::sqf::runtime::runtime &runtime,
                    context &local_fileinfo,
                    context &original_fileinfo,
                    const ::sqf::runtime::parser::macro &m,
                    std::vector<std::string> &params,
                    std::stringstream &sstream,
                    const std::unordered_map<std::string, std::string> &param_map,
                    std::vector<const ::sqf::runtime::parser::macro *> &macro_stack);

            std::string handle_macro(
                    ::sqf::runtime::runtime &runtime,
                    context &local_fileinfo,
                    context &original_fileinfo,
                    const ::sqf::runtime::parser::macro &m,
                    const std::unordered_map<std::string, std::string> &param_map,
                    std::vector<const ::sqf::runtime::parser::macro *> &macro_stack);

            std::string replace(
                    ::sqf::runtime::runtime &runtime,
                    context &fileinfo,
                    const ::sqf::runtime::parser::macro &m,
                    std::vector<std::string> &params,
                    std::vector<const ::sqf::runtime::parser::macro *> &macro_stack);

            std::string handle_arg(
                    ::sqf::runtime::runtime &runtime,
                    context &local_fileinfo,
                    context &original_fileinfo,
                    size_t endindex,
                    const std::unordered_map<std::string, std::string> &param_map,
                    std::vector<const ::sqf::runtime::parser::macro *> &macro_stack);

            std::string parse_ppinstruction(::sqf::runtime::runtime &runtime, context &file_context);

            std::string parse_file(::sqf::runtime::runtime &runtime, context &file_context);

            size_t replace_find_wordend(::sqf::runtime::runtime &runtime, context fileinfo);

            void replace_skip(
                    ::sqf::runtime::runtime &runtime,
                    context &fileinfo,
                    std::stringstream &sstream);

            [[nodiscard]] bool allow_write() const {
                return m_file_scopes.back().conditions.empty() || m_file_scopes.back().conditions.back().allow_write;
            }

            [[nodiscard]] bool errflag() const { return m_errflag; }

            file_scope &current_file_scope() { return m_file_scopes.back(); }

            void push_path(const ::sqf::runtime::fileio::pathinfo &pathinfo);

            void pop_path(context &preprocessorfileinfo);

            [[nodiscard]] std::optional<::sqf::runtime::parser::macro> try_get_macro(
                    const std::string &macro_name) const {
                auto res = m_macros.find(macro_name);
                if (res == m_macros.end()) {
                    return {};
                }
                return res->second;
            }
        };

    public:
        explicit impl_default(Logger &logger);

        std::optional<std::string> preprocess(
                ::sqf::runtime::runtime &runtime,
                std::string_view view,
                ::sqf::runtime::fileio::pathinfo pathinfo,
                std::vector<std::string> *out_included,
                std::vector<::sqf::runtime::parser::macro> *out_macros);

        void push_back(::sqf::runtime::parser::macro m) override {
            auto name = std::string(m.name());
            m_macros[name] = m;
            assert(m_macros[std::string(name.begin(), name.end())].name() == name);
            assert(m_macros.find(std::string(name.begin(), name.end())) != m_macros.end());
        };

        void push_back(::sqf::runtime::parser::pragma p) override {
            auto name = std::string(p.name());
            m_pragmas[name] = p;
            assert(m_pragmas[name].name() == name);
            assert(m_pragmas.find(std::string(name.begin(), name.end())) != m_pragmas.end());
        };

        ~impl_default() override = default;

        std::optional<std::string> preprocess(::sqf::runtime::runtime &runtime, std::string_view view,
                                              ::sqf::runtime::fileio::pathinfo pathinfo) override {
            return preprocess(runtime, view, pathinfo, nullptr, nullptr);
        }

        [[nodiscard]] std::optional<::sqf::runtime::parser::macro> try_get_macro(const std::string &name) const {
            auto res = m_macros.find(name);
            if (res == m_macros.end()) {
                return {};
            }
            return res->second;
        }

        [[nodiscard]] std::optional<::sqf::runtime::parser::pragma> try_get_pragma(const std::string &name) const {
            auto res = m_pragmas.find(name);
            if (res == m_pragmas.end()) {
                return {};
            }
            return res->second;
        }
    };
}
