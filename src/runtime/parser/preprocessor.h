#pragma once

#include "../fileio.h"
#include "../diagnostics/diag_info.h"

#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <functional>


namespace sqf {
    namespace runtime {
        class runtime;
        namespace parser {
            class macro;
            class pragma;

            class preprocessor {
            public:
                class context {
                private:
                    size_t last_col;
                    bool is_in_string;
                    bool is_in_block_comment;

                    // Handles correct progression of line, col and off
                    char next_internal() {
                        if (file_offset >= content.length()) {
                            return '\0';
                        }
                        char c = content[file_offset++];
                        switch (c) {
                            case '\n':
                                line++;
                                last_col = col;
                                col = 0;
                                return c;
                            case '\r':
                                return next_internal();
                            default:
                                col++;
                                return c;
                        }
                    }

                public:
                    explicit context(::sqf::runtime::fileio::pathinfo pathinfo)
                            : path(std::move(pathinfo)) {
                        last_col = 0;
                        is_in_string = false;
                        is_in_block_comment = false;
                    }

                    explicit context(const ::sqf::runtime::diagnostics::diag_info& diag_info)
                            : path(diag_info.path) {
                        last_col = 0;
                        is_in_string = false;
                        is_in_block_comment = false;
                    }

                    std::string content;
                    size_t file_offset = 0;
                    size_t line = 1;
                    size_t col = 0;
                    ::sqf::runtime::fileio::pathinfo path;

                    // Returns the next character.
                    // Will not take into account to skip eg. comments or similar things!
                    char peek(size_t len = 0) {
                        if (file_offset + len >= content.length()) {
                            return '\0';
                        }
                        return content[file_offset + len];
                    }

                    // Will return the next character in the file.
                    // Comments will be skipped automatically.
                    char next() {
                        char c = next_internal();
                        if (!is_in_string && (c == '/' || is_in_block_comment)) {
                            if (c == '\n') {
                                return c;
                            }
                            auto pc = peek();
                            if (is_in_block_comment && c == '*' && pc == '/') {
                                next_internal();
                                is_in_block_comment = false;
                                c = next();
                                return c;
                            } else if (pc == '*' || is_in_block_comment) {
                                if (!is_in_block_comment) {
                                    next_internal();
                                }
                                is_in_block_comment = true;
                                while ((c = next_internal()) != '\0') {
                                    if (c == '\n') {
                                        break;
                                    } else if (c == '*' && peek() == '/') {
                                        next_internal();
                                        is_in_block_comment = false;
                                        c = next();
                                        break;
                                    }
                                }
                            } else if (pc == '/') {
                                while ((c = next_internal()) != '\0' && c != '\n');
                            }
                        }
                        if (c == '\\') {
                            auto pc1 = peek(0);
                            auto pc2 = peek(1);
                            if ((pc1 == '\r' && pc2 == '\n') || pc1 == '\n') {
                                next_internal();
                                return next();
                            }
                        }
                        if (c == '"') {
                            is_in_string = !is_in_string;
                        }
                        return c;
                    }

                    std::string get_word() {
                        char c;
                        size_t off_start = file_offset;
                        size_t off_end = file_offset;
                        while (
                                (c = next()) != '\0' && (
                                        (c >= 'A' && c <= 'Z') ||
                                        (c >= 'a' && c <= 'z') ||
                                        (c >= '0' && c <= '9') ||
                                        c == '_'
                                )) {
                            off_end = file_offset;
                        }
                        move_back();
                        return content.substr(off_start, off_end - off_start);
                    }

                    std::string get_line(bool catchEscapedNewLine) {
                        char c;
                        size_t off_start = file_offset;
                        bool escaped = false;
                        bool exit = false;
                        if (catchEscapedNewLine) {
                            std::string outputString;
                            outputString.reserve(64);
                            while (!exit && (c = next()) != '\0') {
                                switch (c) {
                                    case '\\':
                                        escaped = true;
                                        break;
                                    case '\n':
                                        if (!escaped) {
                                            exit = true;
                                        }
                                        escaped = false;
                                        break;
                                    default:
                                        if (escaped) {
                                            outputString.push_back('\\');
                                            escaped = false;
                                        }
                                        outputString.push_back(c);
                                        break;
                                }
                            }
                            outputString.shrink_to_fit();
                            return outputString;
                        } else {
                            while ((c = next()) != '\0' && c != '\n') {}
                        }
                        return content.substr(off_start, file_offset - off_start);
                    }

                    // Moves one character backwards and updates
                    // progression of line, col and off according
                    // col will only be tracked for one line!
                    // Not supposed to be used more than once!
                    void move_back() {
                        if (file_offset == 0) {
                            return;
                        }
                        char c = content[--file_offset];
                        switch (c) {
                            case '\n':
                                line--;
                                col = last_col;
                                break;
                            case '\r':
                                move_back();
                                break;
                            default:
                                col--;
                                break;
                        }
                    }

                    [[nodiscard]] ::sqf::runtime::diagnostics::diag_info to_diag_info() const { return {line, col, file_offset, path, {}}; }

                    [[nodiscard]] ::sqf::runtime::fileio::pathinfo to_pathinfo() const { return path; }

                    operator ::sqf::runtime::diagnostics::diag_info() const { return to_diag_info(); }

                    operator ::sqf::runtime::fileio::pathinfo() const { return path; }
                };
                virtual void push_back(::sqf::runtime::parser::macro m) = 0;

                virtual void push_back(::sqf::runtime::parser::pragma p) = 0;

                virtual ~preprocessor() = default;

                virtual std::optional<std::string> preprocess(
                        ::sqf::runtime::runtime &runtime,
                        ::std::string_view view,
                        ::sqf::runtime::fileio::pathinfo pathinfo) = 0;

                std::optional<std::string>
                preprocess(::sqf::runtime::runtime &runtime, ::sqf::runtime::fileio::pathinfo pathinfo);
            };

            class macro {
            public:
                // Special method pointer that may be filled
                // to give this macro a special behavior rather
                // than a content.
                // Gets only applied if pointer is != nullptr
                // m -> the original macro
                // local_fileinfo -> the location, where the macro is called locally (lowest level)
                // original_fileinfo -> the most upper file in the macro chain
                // params -> the passed parameters
                using callback = std::string(
                        const macro &m,
                        const ::sqf::runtime::diagnostics::diag_info dinf,
                        const ::sqf::runtime::fileio::pathinfo location,
                        const std::vector<std::string> &params,
                        ::sqf::runtime::runtime &runtime
                );
            private:
                std::string m_name;
                std::string m_content;
                std::vector<std::string> m_args;
                bool m_is_callable;
                std::function<callback> m_callback{};

                ::sqf::runtime::diagnostics::diag_info m_diag_info;
            public:
                macro() = default;

                macro(diagnostics::diag_info diag_info, std::string name, std::vector<std::string> args,
                      bool is_callable, std::string content, std::function<callback> callback) :
                        m_name(std::move(name)),
                        m_content(std::move(content)),
                        m_args(std::move(args)),
                        m_is_callable(is_callable),
                        m_callback(std::move(callback)),
                        m_diag_info(std::move(diag_info)) {}

                macro(diagnostics::diag_info diag_info, std::string name)
                        : macro(std::move(diag_info), std::move(name), {}, false, {}, {}) {}

                macro(diagnostics::diag_info diag_info, std::string name, std::string content)
                        : macro(std::move(diag_info), std::move(name), {}, false, std::move(content), {}) {}

                macro(diagnostics::diag_info diag_info, std::string name, std::vector<std::string> args)
                        : macro(std::move(diag_info), std::move(name), std::move(args), true, {}, {}) {}

                macro(diagnostics::diag_info diag_info, std::string name, std::vector<std::string> args,
                      std::string content)
                        : macro(std::move(diag_info), std::move(name), std::move(args), true, std::move(content), {}) {}

                macro(std::string name)
                        : macro({}, std::move(name), {}, false, {}, nullptr) {}

                macro(std::string name, std::string content)
                        : macro({}, std::move(name), {}, false, std::move(content), {}) {}

                macro(std::string name, std::function<callback> callback)
                        : macro({}, std::move(name), {}, false, {}, std::move(callback)) {}

                macro(std::string name, std::vector<std::string> args, std::string content)
                        : macro({}, std::move(name), std::move(args), true, std::move(content), {}) {}

                macro(std::string name, std::vector<std::string> args, std::function<callback> callback)
                        : macro({}, std::move(name), std::move(args), true, {}, std::move(callback)) {}

                [[nodiscard]] std::string_view name() const { return m_name; }

                [[nodiscard]] std::string_view content() const { return m_content; }

                [[nodiscard]] const std::vector<std::string> &args() const { return m_args; }

                [[nodiscard]] ::sqf::runtime::diagnostics::diag_info diag_info() const { return m_diag_info; }

                [[nodiscard]] bool has_callback() const { return static_cast<bool>(m_callback); }

                [[nodiscard]] bool is_callable() const { return m_is_callable; }

                std::string operator()(
                        const ::sqf::runtime::diagnostics::diag_info dinf,
                        const ::sqf::runtime::fileio::pathinfo location,
                        const std::vector<std::string> &params,
                        ::sqf::runtime::runtime &runtime) const {
                    return m_callback(*this, dinf, location, params, runtime);
                }
            };

            class pragma {
            public:
                // Special method pointer that may be filled
                // to give this pragma its behavior.
                // self -> the original pragma
                // file_context -> the context of the file, where the pragma is called
                // data -> the data available after the name
                using callback = std::optional<std::string>(
                        const ::sqf::runtime::parser::pragma &self,
                        ::sqf::runtime::runtime &runtime,
                        ::sqf::runtime::parser::preprocessor::context &file_context,
                        const std::string &data
                );
            private:
                std::string m_name;
                std::function<callback> m_callback;

            public:
                pragma() = default;

                pragma(std::string name, std::function<callback> cb) :
                        m_name(std::move(name)),
                        m_callback(std::move(cb)) {
                }

                [[nodiscard]] std::string_view name() const { return m_name; }

                std::optional<std::string> operator()(
                        ::sqf::runtime::runtime &runtime,
                        preprocessor::context &file_context,
                        const std::string &data) const {
                    return m_callback(*this, runtime, file_context, data);
                }
            };
        }
    }
    namespace parser::preprocessor {
        class passthrough : public ::sqf::runtime::parser::preprocessor {
        public:
            void push_back(::sqf::runtime::parser::macro m) override {};

            void push_back(::sqf::runtime::parser::pragma p) override {};

            ~passthrough() override {};

            std::optional<std::string> preprocess(::sqf::runtime::runtime &runtime, ::std::string_view view,
                                                  ::sqf::runtime::fileio::pathinfo pathinfo) override;
        };
    }
}
