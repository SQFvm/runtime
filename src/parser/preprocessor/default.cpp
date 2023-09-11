#include "default.h"
#include "../../runtime/fileio.h"
#include "../../runtime/value.h"
#include "../../runtime/util.h"
#include "../../runtime/runtime.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <utility>
#include <vector>
#include <string>
#include <optional>
#include <iostream>

#if defined(DF__SQF_PREPROC__TRACE_MACRO_RESOLVE) || defined(DF__SQF_PREPROC__TRACE_MACRO_PARSE)
#include <iostream>
#endif // DF__SQF_PREPROC__TRACE_MACRO_RESOLVE


namespace err = logmessage::preprocessor;
using namespace std::string_literals;
using namespace sqf::runtime::util;

void sqf::parser::preprocessor::impl_default::instance::replace_stringify(
        ::sqf::runtime::runtime &runtime,
        context &local_fileinfo,
        context &original_fileinfo,
        const ::sqf::runtime::parser::macro &m,
        std::vector<std::string> &params,
        std::stringstream &sstream,
        const std::unordered_map<std::string, std::string> &param_map,
        const std::vector<const ::sqf::runtime::parser::macro *> &macro_stack) {
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
    auto ___begin = sstream.tellp();
#endif
    char c;
    replace_skip(runtime, local_fileinfo, sstream);
    c = local_fileinfo.peek();
    if (c == '#') {
        local_fileinfo.next();
        replace_concat(runtime, local_fileinfo, original_fileinfo, m, params, sstream, param_map, macro_stack);
    } else {
        auto word_end = replace_find_wordend(runtime, local_fileinfo);
        std::string word;
        word.resize(word_end);
        for (size_t i = 0; i < word_end; i++) {
            word[i] = local_fileinfo.next();
        }
        auto param_res = std::find_if(
                m.args().begin(),
                m.args().end(),
                [word](const std::string &s) -> bool {
                    return s == word;
                }
        );
        if (param_res != m.args().end()) {
            auto index = param_res - m.args().begin();
            sstream << '"' << params[index] << '"';
        } else {
            auto macro_res = std::find_if(
                    m_macros.begin(),
                    m_macros.end(),
                    [word](const std::unordered_map<std::string, ::sqf::runtime::parser::macro>::value_type &m) -> bool {
                        return m.first == word;
                    }
            );
            if (macro_res == m_macros.end()) {
                sstream << '"' << word << '"';
            } else {
                sstream << '"' << handle_macro(runtime, local_fileinfo, original_fileinfo, macro_res->second, param_map,
                                               macro_stack)
                        << '"';
            }
        }
    }
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
    std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
        "        " <<
        "        " <<
        "    " << "\x1B[36mreplace_stringify(runtime, preprocessorfileinfo, preprocessorfileinfo, macro: " << m.name() << ", params: ";
    std::cout << "{ ";
    bool ___first = false;
    for (auto& it : params)
    {
        if (___first)
        {
            std::cout << ", ";
        }
        else
        {
            ___first = true;
        }
        std::cout << it;
    }
    std::cout << " }";

    std::cout << ", stringstream, unordered_map<string, string>: ";
    std::cout << "{ ";
    ___first = false;
    for (auto& it : param_map)
    {
        if (___first)
        {
            std::cout << ", ";
        }
        else
        {
            ___first = true;
        }
        std::cout << "{ " << it.first << ", " << it.second << " }";
    }
    std::cout << " }";
    std::cout << ")\033[0m:" << sstream.str().substr(___begin) << std::endl;
#endif
}

void sqf::parser::preprocessor::impl_default::instance::replace_concat(
        ::sqf::runtime::runtime &runtime,
        context &local_fileinfo,
        context &original_fileinfo,
        const ::sqf::runtime::parser::macro &m,
        std::vector<std::string> &params,
        std::stringstream &sstream,
        const std::unordered_map<std::string, std::string> &param_map,
        std::vector<const ::sqf::runtime::parser::macro *> macro_stack) {
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
    auto ___begin = sstream.tellp();
#endif
    char c;
    replace_skip(runtime, local_fileinfo, sstream);
    c = local_fileinfo.peek();
    auto word_end = replace_find_wordend(runtime, local_fileinfo);
    std::string word;
    word.resize(word_end);
    for (size_t i = 0; i < word_end; i++) {
        word[i] = local_fileinfo.next();
    }
    auto param_res = std::find_if(
            m.args().begin(),
            m.args().end(),
            [word](const std::string &s) -> bool {
                return s == word;
            }
    );
    if (param_res != m.args().end()) {
        auto index = param_res - m.args().begin();
        sstream << params[index];
    } else {
        auto macro_res = std::find_if(
                m_macros.begin(),
                m_macros.end(),
                [word](const std::unordered_map<std::string, ::sqf::runtime::parser::macro>::value_type &m) -> bool {
                    return m.first == word;
                }
        );
        if (macro_res == m_macros.end()) {
            sstream << word;
        } else {
            sstream << handle_macro(
                    runtime,
                    local_fileinfo,
                    original_fileinfo,
                    macro_res->second,
                    param_map,
                    std::move(macro_stack));
        }
    }
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
    std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
        "        " <<
        "        " <<
        "    " << "\x1B[36mreplace_concat(runtime, preprocessorfileinfo, preprocessorfileinfo, macro: " << m.name() << ", params: ";
    std::cout << "{ ";
    bool ___first = false;
    for (auto& it : params)
    {
        if (___first)
        {
            std::cout << ", ";
        }
        else
        {
            ___first = true;
        }
        std::cout << it;
    }
    std::cout << " }";

    std::cout << ", stringstream, unordered_map<string, string>: ";
    std::cout << "{ ";
    ___first = false;
    for (auto& it : param_map)
    {
        if (___first)
        {
            std::cout << ", ";
        }
        else
        {
            ___first = true;
        }
        std::cout << "{ " << it.first << ", " << it.second << " }";
    }
    std::cout << " }";
    std::cout << ")\033[0m:" << sstream.str().substr(___begin) << std::endl;
#endif
}

size_t sqf::parser::preprocessor::impl_default::instance::replace_find_wordend(
        ::sqf::runtime::runtime &runtime,
        context fileinfo) {
    auto currentOffset = fileinfo.file_offset;
    size_t res;
    while (true) {
        char c = fileinfo.next();
        switch (c) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '_':
                continue;
            case '\0':
                res = fileinfo.file_offset - currentOffset;
                goto exit;
            default:
                res = fileinfo.file_offset - currentOffset - 1;
                goto exit;
        }
    }
    exit:
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
    std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
        "        " <<
        "        " <<
        "    " << "\x1B[36mreplace_find_wordend(runtime, preprocessorfileinfo)\033[0m: " << res << std::endl;
#endif
    return res;
}

void sqf::parser::preprocessor::impl_default::instance::replace_skip(
        ::sqf::runtime::runtime &runtime,
        context &fileinfo,
        std::stringstream &sstream) {
    bool flag = true;
    bool in_string = false;
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
    auto ___begin = sstream.tellp();
#endif
    while (flag) {
        if (in_string) {
            char c = fileinfo.next();
            if (c == '"') {
                in_string = false;
            }
            sstream << c;
        } else {
            char c = fileinfo.peek();
            switch (c) {
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                case 'g':
                case 'h':
                case 'i':
                case 'j':
                case 'k':
                case 'l':
                case 'm':
                case 'n':
                case 'o':
                case 'p':
                case 'q':
                case 'r':
                case 's':
                case 't':
                case 'u':
                case 'v':
                case 'w':
                case 'x':
                case 'y':
                case 'z':
                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                case 'G':
                case 'H':
                case 'I':
                case 'J':
                case 'K':
                case 'L':
                case 'M':
                case 'N':
                case 'O':
                case 'P':
                case 'Q':
                case 'R':
                case 'S':
                case 'T':
                case 'U':
                case 'V':
                case 'W':
                case 'X':
                case 'Y':
                case 'Z':
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '_':
                case '\n':
                case '\\':
                case '#':
                case '\0':
                    flag = false;
                    break;
                case '\r':
                    break;
                case '"':
                    in_string = true;
                default:
                    sstream << fileinfo.next();
            }
        }
    }
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
    std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
        "        " <<
        "        " <<
        "    " << "\x1B[36mreplace_skip(runtime, preprocessorfileinfo, stringstream)\033[0m: " << sstream.str().substr(___begin) << std::endl;
#endif
}

std::string sqf::parser::preprocessor::impl_default::instance::replace(
        ::sqf::runtime::runtime &runtime,
        context &original_fileinfo,
        const ::sqf::runtime::parser::macro &m,
        std::vector<std::string> &params,
        const std::vector<const ::sqf::runtime::parser::macro *> &macro_stack) {
    if (m.args().size() != params.size()) {
        m_errflag = true;
        log(err::ArgCountMissmatch(original_fileinfo.to_diag_info(), m.diag_info(), m.args().size(), params.size(), std::string(m.name().begin(), m.name().end())));
        return "";
    }
    context local_fileinfo(m.diag_info());
    local_fileinfo.content = m.content();
    local_fileinfo.line = m.diag_info().line;
    if (m.has_callback()) {
        return m(original_fileinfo, original_fileinfo, params, runtime);
    }

    std::unordered_map<std::string, std::string> parammap;
    for (size_t i = 0; i < params.size(); i++) {
        parammap[m.args()[i]] = params[i];
    }

    std::stringstream sstream;

    char c;
    while (true) {
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
        std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
            "        " <<
            "        " <<
            "    " << "\x1B[36mreplace(...)\033[0m: Remaining: " << local_fileinfo.content.substr(local_fileinfo.off) << std::endl;
#endif
        replace_skip(runtime, local_fileinfo, sstream);
        c = local_fileinfo.peek();

        if (c == '#') {
            local_fileinfo.next();
            replace_stringify(runtime, local_fileinfo, original_fileinfo, m, params, sstream, parammap, macro_stack);
        } else if (c == '\n' || c == '\0') {
            local_fileinfo.next();
            break;
        } else {
            auto word_end = replace_find_wordend(runtime, local_fileinfo);
            // Check if another wordend can be found
            if (word_end == 0) { // no other word was found
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
                std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
                    "        " <<
                    "        " <<
                    "    " << "\x1B[36mreplace(...)\033[0m: Appending end: " << local_fileinfo.next() << std::endl;
#endif
                sstream << local_fileinfo.next();
            } else { // Receive next word
                std::string word;
                word.resize(word_end);
                for (size_t i = 0; i < word_end; i++) {
                    word[i] = local_fileinfo.next();
                }

                // Check if word matches any parameter
                auto param_res = std::find(
                        m.args().begin(),
                        m.args().end(),
                        word);
                if (param_res != m.args().end()) { // word matches a parameter, replacee
                    auto index = param_res - m.args().begin();
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
                    std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
                        "        " <<
                        "        " <<
                        "    " << "\x1B[36mreplace(...)\033[0m: Inserting parameter[" << index << "]: " << params[index] << std::endl;
#endif
                    sstream << params[index];
                } else { // Check if word matches any macro
                    auto macro_res = m_macros.find(word);
                    if (macro_res == m_macros.end()) { // word matches no macro, append to sstream
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
                        std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
                            "        " <<
                            "        " <<
                            "    " << "\x1B[36mreplace(...)\033[0m: Adding word: " << word << std::endl;
#endif
                        sstream << word;
                    } else { // word matches macro, handle it
                        auto res = handle_macro(
                                runtime,
                                local_fileinfo,
                                original_fileinfo,
                                macro_res->second,
                                parammap,
                                macro_stack);
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
                        std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
                            "        " <<
                            "        " <<
                            "    " << "\x1B[36mreplace(...)\033[0m: Adding macro result: " << res << std::endl;
#endif
                        sstream << res;
                    }
                }
            }
        }
    }

#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
    std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
        "        " <<
        "        " <<
        "    " << "\x1B[36mreplace(runtime, preprocessorfileinfo, const macro: " << m.name() << ", params: ";
    std::cout << "{ ";
    bool ___first = false;
    for (auto& it : params)
    {
        if (___first)
        {
            std::cout << ", ";
        }
        else
        {
            ___first = true;
        }
        std::cout << it;
    }
    std::cout << " }";
    std::cout << ")\033[0m: " << sstream.str() << std::endl;
#endif
    return sstream.str();
}

std::string sqf::parser::preprocessor::impl_default::instance::handle_arg(
        ::sqf::runtime::runtime &runtime,
        context &local_fileinfo,
        context &original_fileinfo,
        size_t endindex,
        const std::unordered_map<std::string, std::string> &param_map,
        const std::vector<const ::sqf::runtime::parser::macro *> &macro_stack) {
    size_t word_start = local_fileinfo.file_offset;
    bool inside_word = false;
    bool string_mode = false;
    bool part_of_word = false;
    std::stringstream sstream;
    char c;
    while (local_fileinfo.file_offset != endindex && (c = local_fileinfo.next()) != '\0') {
        if (string_mode) {
            if (c == '"') {
                string_mode = false;
            }
            sstream << c;
            continue;
        }
        switch (c) {
            case '"':
                string_mode = true;
                sstream << c;
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '_':
                if (!inside_word) {
                    inside_word = true;
                    word_start = local_fileinfo.file_offset - 1;
                }
                if (local_fileinfo.file_offset != endindex) {
                    break;
                } // Intended conditional fallthrough
                part_of_word = true;
            default:
                if (inside_word) {
                    inside_word = false;
                    auto word = local_fileinfo.content.substr(word_start, local_fileinfo.file_offset - word_start -
                                                                          (!part_of_word ? 1 : 0));
                    auto res = try_get_macro(word);
                    if (res.has_value()) {
                        if (res.value().is_callable()) {
                            local_fileinfo.move_back();
                        }
                        auto handled = handle_macro(
                                runtime,
                                local_fileinfo,
                                original_fileinfo,
                                res.value(),
                                param_map,
                                macro_stack);
                        if (m_errflag) {
                            return "";
                        }
                        sstream << handled;
                        if (!res.value().is_callable() && !part_of_word) {
                            local_fileinfo.move_back();
                        }
                    } else if (param_map.find(word) != param_map.end()) {
                        sstream << param_map.at(word);
                        if (!part_of_word) {
                            local_fileinfo.move_back();
                        }
                    } else {
                        sstream << word;
                        if (!part_of_word) {
                            local_fileinfo.move_back();
                        }
                    }
                } else {
                    sstream << c;
                }
                part_of_word = false;
                break;
        }
    }
    return sstream.str();
}

std::string sqf::parser::preprocessor::impl_default::instance::handle_macro(
        ::sqf::runtime::runtime &runtime,
        context &local_fileinfo,
        context &original_fileinfo,
        const ::sqf::runtime::parser::macro &m,
        const std::unordered_map<std::string, std::string> &param_map,
        std::vector<const ::sqf::runtime::parser::macro *> macro_stack) {
    // Check if macro is already in stack
    {
        std::vector<std::vector<const ::sqf::runtime::parser::macro *>::iterator> occurences;
        for (auto it = macro_stack.begin(); it != macro_stack.end(); ++it) {
            if (*it == &m) {
                occurences.push_back(it);
            }
        }
        if (occurences.size() >= 3) {
            for (size_t i = 0; i < occurences.size() - 3; i++) {
                auto begin = occurences[i];
                auto middle = occurences[i + 1];
                auto end = occurences[i + 2];
                auto distance_begin_middle = std::distance(begin, middle);
                auto distance_middle_end = std::distance(middle, end);
                if (distance_begin_middle != distance_middle_end)
                    continue;

                bool match = true;
                for (size_t j = 0; j < distance_begin_middle; j++) {
                    if (*(begin + j) != *(middle + j)) {
                        match = false;
                        break;
                    }
                }
                if (match) {
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
                    std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36mhandle_macro(runtime, preprocessorfileinfo, preprocessorfileinfo, macro: " << m.name() << ", unordered_map<string, string>: ";
            std::cout << "{ ";
            bool ___first = false;
            for (auto& it : param_map)
            {
                if (___first)
                {
                    std::cout << ", ";
                }
                else
                {
                    ___first = true;
                }
                std::cout << "{ " << it.first << ", " << it.second << " }";
            }
            std::cout << " }";
            std::cout << ")\033[0m found macro recursion" << std::endl;
#endif
                    log(err::RecursiveMacro(original_fileinfo.to_diag_info()));
                    return {m.name().begin(), m.name().end()};
                }
            }
        }
    }
    macro_stack.push_back(&m);
    // Needs to handle 'NAME(ARG1, ARG2, ARGN)' not more, not less!
    std::vector<std::string> params;

    if (!m.is_callable()) {
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
        std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
            "        " <<
            "        " <<
            "    " << "\x1B[36mhandle_macro(runtime, preprocessorfileinfo, preprocessorfileinfo, macro: " << m.name() << ", unordered_map<string, string>: ";
        std::cout << "{ ";
        bool ___first = false;
        for (auto& it : param_map)
        {
            if (___first)
            {
                std::cout << ", ";
            }
            else
            {
                ___first = true;
            }
            std::cout << "{ " << it.first << ", " << it.second << " }";
        }
        std::cout << " }";
        std::cout << ")\033[0m parsing empty macro" << std::endl;
#endif
    } else {
        if (local_fileinfo.peek() != '(') {
            return std::string(m.name());
        }
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
        std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
            "        " <<
            "        " <<
            "    " << "\x1B[36mhandle_macro(runtime, preprocessorfileinfo, preprocessorfileinfo, macro: " << m.name() << ", unordered_map<string, string>: ";
        std::cout << "{ ";
        bool ___first = false;
        for (auto& it : param_map)
        {
            if (___first)
            {
                std::cout << ", ";
            }
            else
            {
                ___first = true;
            }
            std::cout << "{ " << it.first << ", " << it.second << " }";
        }
        std::cout << " }";
        std::cout << ")\033[0m parsing macro with args" << std::endl;
#endif
        local_fileinfo.next(); // Skip the initial '('
        size_t rb_counter = 0;
        size_t cb_counter = 0;
        size_t eb_counter = 0;
        size_t lastargstart = local_fileinfo.file_offset;
        bool exit = false;
        char c;
        bool in_string = false;
        while (!exit && (c = local_fileinfo.next()) != '\0') {
            if (in_string) {
                if (c == '"') {
                    in_string = false;
                }
                continue;
            }
            switch (c) {
                default:
                    // empty
                    break;
                case '[':
                    eb_counter++;
                    break;
                case ']':
                    eb_counter--;
                    break;
                case '{':
                    cb_counter++;
                    break;
                case '}':
                    cb_counter--;
                    break;
                case '(':
                    rb_counter++;
                    break;
                case '"':
                    in_string = true;
                    break;
                case ')':
                    if (rb_counter != 0) {
                        rb_counter--;
                        break;
                    } else { exit = true; /* goto case ',' */ }
                case ',':
                    if (rb_counter == 0 && eb_counter == 0 && cb_counter == 0) {
                        local_fileinfo.move_back();
                        if (local_fileinfo.file_offset - lastargstart > 0) {
                            context copy = local_fileinfo;
                            copy.file_offset = lastargstart;
                            auto handled_param = handle_arg(
                                    runtime,
                                    copy,
                                    original_fileinfo,
                                    local_fileinfo.file_offset,
                                    param_map,
                                    macro_stack);
                            params.emplace_back(std::move(handled_param));
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
                            std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
                                "        " <<
                                "        " <<
                                "    " << "\x1B[36mhandle_macro(...)\033[0m found param: " << params.back() << std::endl;
#endif
                        } else if (!m.args().empty()) {
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
                            std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
                                "        " <<
                                "        " <<
                                "    " << "\x1B[36mhandle_macro(...)\033[0m found empty param: " << params.back() << std::endl;
#endif
                            params.emplace_back("");
                            log(err::EmptyArgument(original_fileinfo.to_diag_info()));
                        }
                        local_fileinfo.next();
                        lastargstart = local_fileinfo.file_offset;
                    }
                    break;
            }
        }
    }
#ifdef DF__SQF_PREPROC__TRACE_MACRO_RESOLVE
    std::cout << "\x1B[33m[PREPROCESSOR-RS]\033[0m" <<
        "        " <<
        "        " <<
        "    " << "\x1B[36mhandle_macro(...)\033[0m starting replace." << std::endl;
#endif
    return replace(runtime, original_fileinfo, m, params, macro_stack);
}

std::string sqf::parser::preprocessor::impl_default::instance::parse_ppinstruction(
        ::sqf::runtime::runtime &runtime,
        context &file_context) {
    auto inst = file_context.get_word();
    std::string line{trim(file_context.get_line(true))};
    std::transform(inst.begin(), inst.end(), inst.begin(), [](char &c) { return (char) std::toupper((int) c); });
    if (inst == "INCLUDE") { // #include "file/path"
        // Trim
        if (!(current_file_scope().conditions.empty() || current_file_scope().conditions.back().allow_write)) {
            return "\n";
        }
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](char c) -> bool {
            return c != '"';
        }));
        auto endIter = std::find_if(line.begin(), line.end(), [](char c) -> bool {
            return c == '"';
        });
        if (std::distance(endIter, line.end()) > 1)
            log(err::UnexpectedDataAfterInclude(file_context.to_diag_info()));
        line.erase(endIter, line.end());
        try {
            auto include_path_info = runtime.fileio().get_info(line, file_context);
            if (!include_path_info.has_value()) {
                m_errflag = true;
                log(err::IncludeFailed(file_context.to_diag_info(), line, "FileIO returned no file."));
                return "";
            }
            const auto &physical = include_path_info->physical;
            auto res = std::find_if(m_file_scopes.begin(), m_file_scopes.end(),
                                    [physical](file_scope &parent) -> bool {
                                        return parent.path.physical == physical;
                                    });
            if (res != m_file_scopes.end()) {
                m_errflag = true;
                std::stringstream includeTree;
                for (size_t i = 0; i < m_file_scopes.size(); i++) {
                    includeTree << i << ". " << m_file_scopes[i].path.physical << " [" << m_file_scopes[i].path.virtual_
                                << "]\n";
                }
                log(err::RecursiveInclude(file_context.to_diag_info(), includeTree.str()));
                return "";
            }
            context otherfinfo(*include_path_info);
            otherfinfo.content = runtime.fileio().read_file(*include_path_info);


            std::string output;
            auto lineInfo = std::to_string(file_context.line - 1);
            auto parsedFile = parse_file(runtime, otherfinfo);
            output.reserve(
                    ::sqf::runtime::util::strlen("#line 1 \"") + physical.size() +
                    ::sqf::runtime::util::strlen("\"\n") +
                    parsedFile.size() + ::sqf::runtime::util::strlen("\n") +
                    ::sqf::runtime::util::strlen("#line ") + lineInfo.size() + ::sqf::runtime::util::strlen(" \"") +
                    file_context.path.physical.size() + ::sqf::runtime::util::strlen("\"\n")
            );
            output.append("#line 1 \"");
            output.append(physical);
            output.append("\"\n");
            output.append(parsedFile);
            output.append("\n");
            output.append("#line ");
            output.append(lineInfo);
            output.append(" \"");
            output.append(file_context.path.physical);
            output.append("\"\n");
            return output;
        }
        catch (const std::runtime_error &ex) {
            m_errflag = true;
            log(err::IncludeFailed(file_context.to_diag_info(), line, ex));
            return "";
        }
    } else if (inst == "DEFINE") { // #define TEST(A, B, C) A #B##C
        if (!(current_file_scope().conditions.empty() || current_file_scope().conditions.back().allow_write)) {
#ifdef DF__SQF_PREPROC__TRACE_MACRO_PARSE
            std::cout << "\x1B[33m[PP-DEFINE-PARSE]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36m" << line << "\033[0m skipped due to m_allowwrite == 0" << std::endl;
#endif // DF__SQF_PREPROC__TRACE_MACRO_PARSE
            return "\n";
        }
        auto bracketsIndex = line.find('(');
        auto spaceIter = std::find_if(line.begin(), line.end(), [](char ch) {
            return !(ch == '_' ||
                     (ch >= 'a' && ch <= 'z') ||
                     (ch >= 'A' && ch <= 'Z') ||
                     (ch >= '0' && ch <= '9'));
        });
        auto spaceIndex = spaceIter == line.end() ? std::string::npos : std::distance(line.begin(), spaceIter);
        if (bracketsIndex == std::string::npos && spaceIndex == std::string::npos) { // Empty define
            if (try_get_macro(line).has_value()) {
                log(err::MacroDefinedTwice(file_context.to_diag_info(), line));
            }
            m_macros[line] = {file_context, line};
#ifdef DF__SQF_PREPROC__TRACE_MACRO_PARSE
            std::cout << "\x1B[33m[PP-DEFINE-PARSE]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36m" << line << "\033[0m Declared Empty Macro" << std::endl;
#endif // DF__SQF_PREPROC__TRACE_MACRO_PARSE
        } else {
            if (spaceIndex < bracketsIndex || bracketsIndex ==
                                              std::string::npos) // std::string::npos does not need to be catched as bracketsIndex always < npos here
            { // First bracket was found after first space OR is not existing thus we have a simple define with a replace value here
                auto name_tmp = line.substr(0, spaceIndex);
                if (try_get_macro(name_tmp).has_value()) {
                    log(err::MacroDefinedTwice(file_context.to_diag_info(), name_tmp));
                }
                std::string content(trim(line.substr(
                        line[spaceIndex] == ' ' ? spaceIndex + 1 : spaceIndex))); // Special magic for '#define macro\'
                m_macros[name_tmp] = {file_context, name_tmp, content};
#ifdef DF__SQF_PREPROC__TRACE_MACRO_PARSE
                std::cout << "\x1B[33m[PP-DEFINE-PARSE]\033[0m" <<
                    "        " <<
                    "        " <<
                    "    " << "\x1B[36m" << line << "\033[0m Declared Macro with Content" << std::endl;
#endif // DF__SQF_PREPROC__TRACE_MACRO_PARSE
            } else { // We got a define with arguments here
                auto name_tmp = line.substr(0, bracketsIndex);
                if (try_get_macro(name_tmp).has_value()) {
                    log(err::MacroDefinedTwice(file_context.to_diag_info(), name_tmp));
                }
                auto bracketsEndIndex = line.find(')');
                auto argumentsString = line.substr(bracketsIndex + 1, bracketsEndIndex);
                std::string content;
                std::vector<std::string> args;

                if (bracketsIndex + 1 != bracketsEndIndex) { // Non-Empty args list
                    size_t arg_index = bracketsIndex;
                    size_t arg_start_index = bracketsIndex + 1;
                    bool ended = false;
                    while (!ended) {
                        arg_index = line.find(',', arg_start_index);
                        if (arg_index == std::string::npos || arg_index > bracketsEndIndex) {
                            ended = true;
                            arg_index = bracketsEndIndex;
                        }
                        std::string arg{trim(line.substr(arg_start_index, arg_index - arg_start_index))};
                        if (!arg.empty()) {
                            args.emplace_back(std::move(arg));
                            arg_start_index = arg_index + 1;
                        }
                    }
                    // Special magic for '#define macro\'
                    content = (trim(line.substr(line[arg_start_index] == ' ' ? arg_start_index + 1 : arg_start_index)));
                } else { // Empty args list
                    // Special magic for '#define macro\'
                    content = (trim(line.substr(
                            line[bracketsEndIndex + 1] == ' ' ? bracketsEndIndex + 2 : bracketsEndIndex + 1)));
                }

                m_macros[name_tmp] = {file_context, name_tmp, args, content};
#ifdef DF__SQF_PREPROC__TRACE_MACRO_PARSE
                std::cout << "\x1B[33m[PP-DEFINE-PARSE]\033[0m" <<
                    "        " <<
                    "        " <<
                    "    " << "\x1B[36m" << line << "\033[0m Declared Macro with Args and Content" << std::endl;
#endif // DF__SQF_PREPROC__TRACE_MACRO_PARSE
            }
        }
        return "\n";
    } else if (inst == "UNDEF") { // #undef TEST
        if (!(current_file_scope().conditions.empty() || current_file_scope().conditions.back().allow_write)) {
            return "\n";
        }

        auto res = m_macros.find(static_cast<std::string>(line));
        if (res == m_macros.end()) {
            log(err::MacroNotFound(file_context.to_diag_info(), line));
        } else {
            m_macros.erase(res);
        }
        return "\n";
    } else if (inst == "IFDEF") { // #ifdef TEST
        if (!current_file_scope().conditions.empty()) {
            log(err::UnexpectedIfdef(file_context.to_diag_info()));
        }
        auto res = m_macros.find(static_cast<std::string>(line));
        current_file_scope().conditions.push_back({res != m_macros.end(), file_context, file_context});
        return "\n";
    } else if (inst == "IFNDEF") { // #ifndef TEST
        if (!current_file_scope().conditions.empty()) {
            log(err::UnexpectedIfndef(file_context.to_diag_info()));
        }
        auto res = m_macros.find(static_cast<std::string>(line));
        current_file_scope().conditions.push_back({res == m_macros.end(), file_context, file_context});
        return "\n";
    } else if (inst == "ELSE") { // #else
        if (current_file_scope().conditions.empty()) {
            m_errflag = true;
            log(err::UnexpectedElse(file_context.to_diag_info()));
            return "";
        }
        current_file_scope().conditions.back().allow_write = !current_file_scope().conditions.back().allow_write;
        return "\n";
    } else if (inst == "ENDIF") { // #endif
        if (current_file_scope().conditions.empty()) {
            m_errflag = true;
            log(err::UnexpectedEndif(file_context.to_diag_info()));
            return "";
        }
        current_file_scope().conditions.pop_back();
        return "\n";
    } else if (inst == "PRAGMA") { // #endif
        if (!(current_file_scope().conditions.empty() || current_file_scope().conditions.back().allow_write)) {
#ifdef DF__SQF_PREPROC__TRACE_MACRO_PARSE
            std::cout << "\x1B[33m[PP-DEFINE-PARSE]\033[0m" <<
                "        " <<
                "        " <<
                "    " << "\x1B[36m" << line << "\033[0m skipped due to m_allowwrite == 0" << std::endl;
#endif // DF__SQF_PREPROC__TRACE_MACRO_PARSE
            return "\n";
        }
        auto spaceIter = std::find_if(line.begin(), line.end(), [](char ch) {
            return !(ch == '_' ||
                     (ch >= 'a' && ch <= 'z') ||
                     (ch >= 'A' && ch <= 'Z') ||
                     (ch >= '0' && ch <= '9'));
        });
        auto spaceIndex = spaceIter == line.end() ? std::string::npos : std::distance(line.begin(), spaceIter);
        if (spaceIndex == std::string::npos) { // Empty define
            auto p = m_owner->try_get_pragma(line);
            if (p.has_value()) {
#ifdef DF__SQF_PREPROC__TRACE_MACRO_PARSE
                std::cout << "\x1B[33m[PP-DEFINE-PARSE]\033[0m" <<
                    "        " <<
                    "        " <<
                    "    " << "\x1B[36m" << line << "\033[0m PRAGMA " << line << std::endl;
#endif // DF__SQF_PREPROC__TRACE_MACRO_PARSE
                auto result = p.value()(runtime, file_context, {});
                if (result.has_value())
                    return result.value();
            } else {
                log(err::UnknownPragma(file_context.to_diag_info(), line));
            }

        } else {
            auto name_tmp = line.substr(0, spaceIndex);
            auto p = m_owner->try_get_pragma(name_tmp);
            if (p.has_value()) {
#ifdef DF__SQF_PREPROC__TRACE_MACRO_PARSE
                std::cout << "\x1B[33m[PP-DEFINE-PARSE]\033[0m" <<
                    "        " <<
                    "        " <<
                    "    " << "\x1B[36m" << line << "\033[0m PRAGMA " << line << std::endl;
#endif // DF__SQF_PREPROC__TRACE_MACRO_PARSE
                std::string content(trim(line.substr(
                        line[spaceIndex] == ' ' ? spaceIndex + 1 : spaceIndex))); // Special magic for '#define macro\'
                auto result = p.value()(runtime, file_context, content);
                if (result.has_value())
                    return result.value();
            } else {
                log(err::UnknownPragma(file_context.to_diag_info(), name_tmp));
            }
        }
        return "\n";
    } else {
        m_errflag = true;
        log(err::UnknownInstruction(file_context.to_diag_info(), inst));
        return "";
    }
}

std::string sqf::parser::preprocessor::impl_default::instance::parse_file(
        ::sqf::runtime::runtime &runtime,
        context &file_context) {

    push_path(file_context.path);
    char c;
    std::stringstream sstream;
    std::stringstream wordstream;
    std::unordered_map<std::string, std::string> empty_parammap;
    sstream << "#line 0 \"" << file_context.path.physical << "\"\n";
    bool was_new_line = true;
    bool is_in_string = false;

    while ((c = file_context.next()) != '\0') {
        if (is_in_string) {
            if (c == '"') {
                is_in_string = false;
            }
            if (current_file_scope().conditions.empty() || current_file_scope().conditions.back().allow_write)
                sstream << c;
            continue;
        }
        switch (c) {
            case '"': {
                is_in_string = true;
                auto word = wordstream.str();
                wordstream.str("");
                if (current_file_scope().conditions.empty() || current_file_scope().conditions.back().allow_write)
                    sstream << word << c;
            }
                break;
            case '\n': {
                was_new_line = true;
                // Fall Through
            }
            case '#': {
                if (c == '#' && was_new_line) {
                    auto res = parse_ppinstruction(runtime, file_context);
                    if (m_errflag) {
                        return res;
                    }
                    sstream << res;
                    break;
                }
            }
            default: {
                if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                    was_new_line = false;
                }
                if (current_file_scope().conditions.empty() || current_file_scope().conditions.back().allow_write) {
                    if (wordstream.tellp() > 0) {
                        auto word = wordstream.str();
                        wordstream.str("");
                        auto m = try_get_macro(word);
                        if (m.has_value()) {
                            file_context.move_back();
                            assert(file_context.file_offset >= word.length());
                            macro_resolved_data macro_offset_start {
                                    file_context.line,
                                    file_context.col - word.length(),
                                    file_context.file_offset - word.length(),
                            };
                            std::vector<const ::sqf::runtime::parser::macro *> macro_stack;
                            auto res = handle_macro(
                                    runtime,
                                    file_context,
                                    file_context,
                                    m.value(),
                                    empty_parammap,
                                    macro_stack);
                            if (m_errflag) {
                                return res;
                            }
                            macro_resolved_data macro_offset_end {
                                   file_context.line,
                                   file_context.col,
                                   file_context.file_offset,
                            };
                            auto replacement_offset_start = sstream.tellp();
                            sstream << res;
                            auto replacement_offset_end = sstream.tellp();
                            if (m_macro_resolved_callback) {
                                m_macro_resolved_callback(
                                        macro_offset_start,
                                        macro_offset_end,
                                        replacement_offset_start,
                                        replacement_offset_end,
                                        runtime,
                                        file_context,
                                        file_context,
                                        m.value(),
                                        empty_parammap);
                            }
                        } else {
                            sstream << word << c;
                        }
                    } else {
                        sstream << c;
                    }
                } else if (c == '\n') {
                    sstream << c;
                }
            }
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '_': {
                if (current_file_scope().conditions.empty() || current_file_scope().conditions.back().allow_write)
                    wordstream << c;
                was_new_line = false;
            }
                break;
        }
    }

    auto word = wordstream.str();
    if (!word.empty()) {
        auto m = try_get_macro(word);
        if (m.has_value()) {
            file_context.move_back();
            assert(file_context.file_offset >= word.length());
            macro_resolved_data macro_offset_start {
                    file_context.line,
                    file_context.col - word.length(),
                    file_context.file_offset - word.length(),
            };
            std::vector<const ::sqf::runtime::parser::macro *> macro_stack;
            auto res = handle_macro(runtime, file_context, file_context, m.value(), empty_parammap, macro_stack);
            if (m_errflag) {
                return res;
            }
            macro_resolved_data macro_offset_end {
                    file_context.line,
                    file_context.col,
                    file_context.file_offset,
            };
            auto replacement_offset_start = sstream.tellp();
            sstream << res;
            auto replacement_offset_end = sstream.tellp();
            if (m_macro_resolved_callback) {
                m_macro_resolved_callback(
                        macro_offset_start,
                        macro_offset_end,
                        replacement_offset_start,
                        replacement_offset_end,
                        runtime,
                        file_context,
                        file_context,
                        m.value(),
                        empty_parammap);
            }
        } else {
            sstream << word;
        }
    }
    pop_path(file_context);
    return sstream.str();
}

std::string line_macro_callback(
        const ::sqf::runtime::parser::macro &m,
        const ::sqf::runtime::diagnostics::diag_info &dinf,
        const ::sqf::runtime::fileio::pathinfo &local,
        const std::vector<std::string> &params,
        ::sqf::runtime::runtime &runtime) {
    return std::to_string(dinf.line);
}

std::string file_macro_callback(
        const ::sqf::runtime::parser::macro &m,
        const ::sqf::runtime::diagnostics::diag_info &dinf,
        const ::sqf::runtime::fileio::pathinfo &local,
        const std::vector<std::string> &params,
        ::sqf::runtime::runtime &runtime) {
    return "\""s + dinf.path.physical + "\""s;
}

std::string eval_macro_callback(
        const ::sqf::runtime::parser::macro &m,
        const ::sqf::runtime::diagnostics::diag_info &dinf,
        const ::sqf::runtime::fileio::pathinfo &local,
        const std::vector<std::string> &params,
        ::sqf::runtime::runtime &runtime) {
    if (params.empty()) {
        return "";
    }
    bool success = false;
    // Cannot wait for stop here as either we are most of the time already 
    // running inside the VM and thus cannot give way for the evaluate_expression method.
    // ToDo: Fix "edge case" where the user uses a running VM to preprocess a file that contains __EVAL to not break the SQF-VM execution.
    auto res = runtime.evaluate_expression(params[0], success, false);
    return success ? res.data()->to_string_sqf() : "";
}

static int counter_ = 0;

std::string counter_macro_callback(
        const ::sqf::runtime::parser::macro &m,
        const ::sqf::runtime::diagnostics::diag_info &dinf,
        const ::sqf::runtime::fileio::pathinfo &local,
        const std::vector<std::string> &params,
        ::sqf::runtime::runtime &runtime) {
    return std::to_string(counter_++);
}

std::string counter_reset_macro_callback(
        const ::sqf::runtime::parser::macro &m,
        const ::sqf::runtime::diagnostics::diag_info &dinf,
        const ::sqf::runtime::fileio::pathinfo &local,
        const std::vector<std::string> &params,
        ::sqf::runtime::runtime &runtime) {
    counter_ = 0;
    return "";
}


void sqf::parser::preprocessor::impl_default::instance::push_path(const ::sqf::runtime::fileio::pathinfo &pathinfo) {
    m_file_scopes.push_back({pathinfo, {}});
    m_visited.insert(pathinfo.physical);
}

void sqf::parser::preprocessor::impl_default::instance::pop_path(context &preprocessorfileinfo) {
    if (!m_file_scopes.back().conditions.empty()) {
        log(logmessage::preprocessor::MissingEndif(preprocessorfileinfo.to_diag_info()));
        m_errflag = true;
    }
    m_file_scopes.pop_back();
}

sqf::parser::preprocessor::impl_default::impl_default(Logger &logger) : CanLog(logger) {
    // m_macros["__DATE_ARR__"s] = { "__DATE_ARR__"s, counter_macro_callback }; // 2020,10,28,15,17,42
    // m_macros["__DATE_STR__"s] = { "__DATE_STR__"s, counter_macro_callback }; // "2020/10/28, 15:17:42"
    // m_macros["__DATE_STR_ISO8601__"s] = { "__DATE_STR_ISO8601__"s, counter_macro_callback }; // "2020-10-28T14:17:42Z"
    // m_macros["__TIME__"s] = { "__TIME__"s, counter_macro_callback }; // 15:17:42
    // m_macros["__TIME_UTC__"s] = { "__TIME_UTC__"s, counter_macro_callback }; // 14:17:42
    // m_macros["__RAND_INT8__"s] = { "__RAND_INT8__"s, counter_macro_callback };
    // m_macros["__RAND_UINT8__"s] = { "__RAND_UINT8__"s, counter_macro_callback };
    // m_macros["__RAND_INT16__"s] = { "__RAND_INT16__"s, counter_macro_callback };
    // m_macros["__RAND_UINT16__"s] = { "__RAND_UINT16__"s, counter_macro_callback };
    // m_macros["__RAND_INT32__"s] = { "__RAND_INT32__"s, counter_macro_callback };
    // m_macros["__RAND_UINT32__"s] = { "__RAND_UINT32__"s, counter_macro_callback };
    // m_macros["__RAND_INT64__"s] = { "__RAND_INT64__"s, counter_macro_callback };
    // m_macros["__RAND_UINT64__"s] = { "__RAND_UINT64__"s, counter_macro_callback };
    m_macros["__GAME_VER__"s] = {"__GAME_VER__"s,
                                 STR(SQFVM_RUNTIME_VERSION_MAJOR) "." STR(SQFVM_RUNTIME_VERSION_MINOR) "." STR(
                                         SQFVM_RUNTIME_VERSION_REVISION)};
    m_macros["__GAME_VER_MAJ__"s] = {"__GAME_VER_MAJ__"s, STR(SQFVM_RUNTIME_VERSION_MAJOR)};
    m_macros["__GAME_VER_MIN__"s] = {"__GAME_VER_MIN__"s, STR(SQFVM_RUNTIME_VERSION_MINOR)};
    m_macros["__GAME_BUILD__"s] = {"__GAME_BUILD__"s, STR(SQFVM_RUNTIME_VERSION_REVISION)};
    m_macros["__COUNTER__"s] = {"__COUNTER__"s, counter_macro_callback};
    m_macros["__COUNTER_RESET__"s] = {"__COUNTER_RESET__"s, counter_reset_macro_callback};
    m_macros["__FILE__"s] = {"__FILE__"s, file_macro_callback};
    m_macros["__LINE__"s] = {"__LINE__"s, line_macro_callback};
    m_macros["__EXEC"s] = {"__EXEC"s, {"EXPRESSION"s}, eval_macro_callback};
    m_macros["__EVAL"s] = {"__EVAL"s, {"EXPRESSION"s}, eval_macro_callback};
    m_macros["_SQFVM"s] = {"_SQFVM"s};
    m_macros["_SQFVM_RUNTIME_VERSION_MAJOR"s] = {"_SQFVM_RUNTIME_VERSION_MAJOR"s, STR(SQFVM_RUNTIME_VERSION_MAJOR)};
    m_macros["_SQFVM_RUNTIME_VERSION_MINOR"s] = {"_SQFVM_RUNTIME_VERSION_MINOR"s, STR(SQFVM_RUNTIME_VERSION_MINOR)};
    m_macros["_SQFVM_RUNTIME_VERSION_REVISION"s] = {"_SQFVM_RUNTIME_VERSION_REVISION"s,
                                                    STR(SQFVM_RUNTIME_VERSION_REVISION)};
#if defined(_DEBUG)
    m_macros["_SQFVM_DEBUG"s] = {"_DEBUG"s};
#endif

}

std::optional<std::string> sqf::parser::preprocessor::impl_default::preprocess(
        ::sqf::runtime::runtime &runtime,
        std::string_view view,
        ::sqf::runtime::fileio::pathinfo pathinfo,
        std::vector<std::string> *out_included,
        std::vector<::sqf::runtime::parser::macro> *out_macros) {
    context fileinfo(std::move(pathinfo));
    fileinfo.content = view;
    instance i(this, get_logger(), m_macros);
    i.m_macro_resolved_callback = m_macro_resolved_callback;
    auto res = i.parse_file(runtime, fileinfo);
    if (out_included) {
        for (const auto &entry: i.m_visited) {
            out_included->push_back(entry);
        }
    }
    if (out_macros) {
        for (const auto &entry: i.m_macros) {
            out_macros->push_back(entry.second);
        }
    }
    if (i.errflag()) {
        return {};
    }
    return res;
}
