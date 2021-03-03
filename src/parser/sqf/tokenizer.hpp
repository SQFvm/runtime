#pragma once
#include "../../runtime/util.h"
#include <array>
#include <string>
#include <string_view>
#include <cctype>
#include <vector>
namespace sqf::parser::sqf
{
    class tokenizer
    {
    public:
        enum class etoken
        {
            eof,
            invalid,
            m_line,
            i_comment_line,
            i_comment_block,
            i_whitespace,

            t_true,
            t_false,
            t_private,

            s_curlyo,
            s_curlyc,
            s_roundo,
            s_roundc,
            s_edgeo,
            s_edgec,
            s_semicolon,
            s_comma,
            s_equal,

            t_operator,
            t_string_double,
            t_string_single,
            t_ident,
            t_number,
            t_hexadecimal
        };
        enum class emode
        {
            normal,
            formatable_string
        };
        struct token
        {
            etoken type;
            size_t line;
            size_t column;
            size_t offset;
            std::string_view contents;
            std::string* path;
        };
        using iterator = std::string::iterator;
    private:
        std::vector<std::string*> m_strings;
        iterator m_start;
        iterator m_current;
        iterator m_end;
        emode m_mode;

        size_t m_line;
        size_t m_column;

        template<typename = void>
        inline bool is_match(char value) { return false; }
        template<char TArg, char ... TArgs>
        bool is_match(char value)
        {
            switch (value)
            {
            case TArg: return true;
            default: return is_match<TArgs...>(value);
            }
        }
        template<char ... TArgs>
        bool is_match(iterator value) { return value < m_end && is_match<TArgs...>(*value); }
        template<size_t len, char ... TArgs>
        bool is_match_repeated(iterator value)
        {
            size_t i = 0;
            while (value < m_end && is_match<TArgs...>(*value++)) { ++i; }
            return len <= i;
        }

        template<char ... TArgs>
        size_t len_match(iterator str)
        {
            iterator it = str;
            while (it < m_end && is_match<TArgs...>(*it)) { ++it; }
            return it - str;
        }
        size_t len_match(iterator start, const char* against)
        {
            auto it = start;
            auto len = ::sqf::runtime::util::strlen(against);
            for (size_t i = 0; i < len && it < m_end; i++, ++it)
            {
                if ((char)std::tolower(*it) != against[i]) { return 0; }
            }
            return it - start;
        }
        size_t len_ident_match(iterator start, const char* against)
        {
            auto it = start;
            auto len = ::sqf::runtime::util::strlen(against);
            for (size_t i = 0; i < len && it < m_end; i++, ++it)
            {
                if ((char)std::tolower(*it) != against[i]) { return 0; }
            }
            if (it < m_end && ((char)std::tolower(*it) >= 'a' && (char)std::tolower(*it) <= 'z'))
            {
                return 0;
            }
            return it - start;
        }

        token try_match(std::initializer_list<etoken> tokens)
        {
            token t = create_token();
            for (auto token_type : tokens)
            {
                auto iter = m_current;
                size_t len = 0;
                switch (token_type)
                {
                default: return { etoken::invalid, m_line, m_column, (size_t)(m_current - m_start), {} };

                case etoken::m_line: /* ToDo: Properly handle #line instruction */ {
                    // Check if line comment start
                    if (len_ident_match(iter, "#line"))
                    {
                        iter += 6;

                        // Read in line num
                        auto start = iter;
                        for (; iter != m_end && *iter != '\n' && *iter != ' '; iter++);
                        std::string str_tmp(start, iter);
                        m_line = static_cast<size_t>(std::stoul(str_tmp));

                        // Try skip to file
                        iter += len_match<' ', '\t'>(iter);
                        start = iter;
                        for (; iter != m_end && *iter != '\n'; iter++);
                        if (iter != m_end && iter - start >= 2)
                        {
                            // Read-in file
                            m_strings.push_back(new std::string(start + 1, iter - 1));
                        }

                        // update column
                        m_column = 0;

                        // set length
                        len = iter - m_current;
                    }
                } break;
                case etoken::i_comment_line: {
                    // Check if line comment start
                    if (is_match_repeated<2, '/'>(iter))
                    {
                        // find line comment end
                        while (!is_match<'\n'>(++iter));

                        // update position info
                        m_line++;
                        m_column = 0;

                        // set length
                        len = iter - m_current;
                    }
                } break;
                case etoken::i_comment_block: {
                    // Check if block comment start
                    if (is_match<'/'>(iter) && is_match<'*'>(iter + 1))
                    {
                        ++iter;
                        ++iter;
                        // find block comment end
                        while (!(is_match<'*'>(iter) && is_match<'/'>(iter + 1)))
                        {
                            // update position info
                            if (!is_match<'\n'>(iter))
                            {
                                m_column++;
                            }
                            else
                            {
                                m_line++;
                                m_column = 0;
                            }
                            ++iter;
                        }

                        // EOF check
                        if (is_match<'/'>(iter) && is_match<'/'>(iter + 1))
                        {
                            ++iter;
                            ++iter;
                        }
                        // set length
                        len = iter - m_current;
                    }
                } break;
                case etoken::i_whitespace: {
                    while (is_match<' ', '\n', '\r', '\t'>(iter))
                    {
                        // update position info
                        if (!is_match<'\n'>(iter))
                        {
                            m_column++;
                        }
                        else
                        {
                            m_line++;
                            m_column = 0;
                        }
                        ++iter;
                    }
                    // set length
                    len = iter - m_current;
                } break;

                case etoken::t_false:            len = len_ident_match(iter, "false"); break;
                case etoken::t_true:             len = len_ident_match(iter, "true"); break;
                case etoken::t_private:          len = len_ident_match(iter, "private"); break;

                case etoken::s_curlyo:           len = is_match<'{'>(iter); break;
                case etoken::s_curlyc:           len = is_match<'}'>(iter); break;
                case etoken::s_roundo:           len = is_match<'('>(iter); break;
                case etoken::s_roundc:           len = is_match<')'>(iter); break;
                case etoken::s_edgeo:            len = is_match<'['>(iter); break;
                case etoken::s_edgec:            len = is_match<']'>(iter); break;
                case etoken::s_equal:            len = is_match<'='>(iter); break;
                case etoken::t_operator:
                    len =
                        is_match_repeated<2, '='>(iter) ? 2 : 
                        is_match<'<'>(iter) && is_match<'='>(iter + 1) ? 2 :
                        is_match<'<'>(iter) ? 1 :
                        is_match<'>'>(iter) && is_match<'='>(iter + 1) ? 2 :
                        is_match_repeated<2, '>'>(iter) ? 2 :
                        is_match<'>'>(iter) ? 1 :
                        is_match<'+'>(iter) ? 1 :
                        is_match<'-'>(iter) ? 1 :
                        is_match<'/'>(iter) ? 1 :
                        is_match<'*'>(iter) ? 1 :
                        is_match<'%'>(iter) ? 1 :
                        is_match<'^'>(iter) ? 1 :
                        is_match<'!'>(iter) && is_match<'='>(iter + 1) ? 2 :
                        is_match<'!'>(iter) ? 1 :
                        is_match<':'>(iter) ? 1 :
                        is_match<'#'>(iter) ? 1 :
                        is_match_repeated<2, '|'>(iter) ? 2 :
                        is_match_repeated<2, '&'>(iter) ? 2 :
                        0
                        ;
                    break;
                case etoken::s_semicolon:        len = is_match<';'>(iter); break;
                case etoken::s_comma:            len = is_match<','>(iter); break;

                case etoken::t_string_single: {
                    ++iter;
                    m_column++;
                    // find string end
                    while (true)
                    {
                        if (is_match<'\''>(iter) && is_match<'\''>(iter + 1))
                        {
                            ++iter;
                        }
                        else if (is_match<'\''>(iter))
                        {
                            ++iter;
                            m_column++;
                            break;
                        }
                        // update position info
                        if (!is_match<'\n'>(iter))
                        {
                            m_column++;
                        }
                        else
                        {
                            m_line++;
                            m_column = 0;
                        }
                        if (iter == m_end)
                        {
                            break;
                        }
                        else
                        {
                            iter++;
                        }
                    }
                    // set length
                    len = iter - m_current;
                } break;
                case etoken::t_string_double: {
                    ++iter;
                    m_column++;
                    // find string end
                    while (true)
                    {
                        if (is_match<'"'>(iter) && is_match<'"'>(iter + 1))
                        {
                            ++iter;
                        }
                        else if (is_match<'"'>(iter))
                        {
                            ++iter;
                            m_column++;
                            break;
                        }
                        // update position info
                        if (!is_match<'\n'>(iter))
                        {
                            m_column++;
                        }
                        else
                        {
                            m_line++;
                            m_column = 0;
                        }
                        if (iter == m_end)
                        {
                            break;
                        }
                        else
                        {
                            iter++;
                        }
                    }
                    // set length
                    len = iter - m_current;
                } break;
                case etoken::t_ident: {
                    len = len_match<
                        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_'>(iter);
                } break;
                case etoken::t_hexadecimal: {
                    if (*iter == '$')
                    {
                        ++iter;

                        size_t res = len_match<'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                            'A', 'B', 'C', 'D', 'E', 'F',
                            'a', 'b', 'c', 'd', 'e', 'f'>(iter);
                        if (res == 0) { len = 0; break; }
                        len = (iter + res) - m_current;
                    }
                    else
                    {
                        ++iter;
                        size_t res = 0;
                        res = is_match<'x'>(iter);
                        if (res == 0) { len = 0; break; }
                        ++iter;

                        res = len_match<'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                            'A', 'B', 'C', 'D', 'E', 'F',
                            'a', 'b', 'c', 'd', 'e', 'f'>(iter);
                        if (res == 0) { len = 0; break; }
                        len = (iter + res) - m_current;
                    }
                } break;
                case etoken::t_number: {
                    if (!is_match<'.'>(iter))
                    {
                        // match first part of number
                        auto res = len_match<'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'>(iter);
                        if (res == 0) { len = 0; break; }
                        else { iter += res; }
                    }
                    if (is_match<'.'>(iter))
                    {
                        ++iter;
                        // match second part of number
                        auto res = len_match<'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'>(iter);
                        if (res == 0) { --iter; }
                        else { iter += res; }
                    }
                    if (is_match<'e', 'E'>(iter))
                    {
                        ++iter;
                        if (is_match<'+', '-'>(iter)) { ++iter; }
                        // match second part of number
                        auto res = len_match<'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'>(iter);
                        if (res == 0) { --iter; }
                        else { iter += res; }
                    }
                    len = iter - m_current;
                } break;
                }

                if (len > 0)
                {
                    t.type = token_type;
                    t.contents = { &*m_current,  len }; // Dirty hack thanks to std::string_view not accepting iterators ...
                    m_current += len;
                    switch (token_type)
                    {
                    case etoken::eof:
                    case etoken::invalid:
                    case etoken::m_line:
                    case etoken::i_comment_line:
                    case etoken::i_comment_block:
                    case etoken::i_whitespace:
                    case etoken::t_string_double:
                    case etoken::t_string_single:
                        break;
                    default:
                        m_column += len;
                        break;
                    }
                    return t;
                }
            }
            return t;
        }

    public:
        tokenizer(iterator start, iterator end, std::string path) :
            m_start(start),
            m_current(start),
            m_end(end),
            m_mode(emode::normal),
            m_line(0),
            m_column(0)
        {
            m_strings.push_back(new std::string(path));
        }
        ~tokenizer()
        {
            for (auto ptr : m_strings)
            {
                delete ptr;
            }
        }
        token next()
        {
            if (m_current == m_end) { return create_token(etoken::eof); };
            switch (*m_current)
            {
            case 'a': case 'A': return try_match({ etoken::t_ident });
            case 'b': case 'B': return try_match({ etoken::t_ident });
            case 'c': case 'C': return try_match({ etoken::t_ident });
            case 'd': case 'D': return try_match({ etoken::t_ident });
            case 'e': case 'E': return try_match({ etoken::t_ident });
            case 'f': case 'F': return try_match({ etoken::t_false, etoken::t_ident });
            case 'g': case 'G': return try_match({ etoken::t_ident });
            case 'h': case 'H': return try_match({ etoken::t_ident });
            case 'i': case 'I': return try_match({ etoken::t_ident });
            case 'j': case 'J': return try_match({ etoken::t_ident });
            case 'k': case 'K': return try_match({ etoken::t_ident });
            case 'l': case 'L': return try_match({ etoken::t_ident });
            case 'm': case 'M': return try_match({ etoken::t_ident });
            case 'n': case 'N': return try_match({ etoken::t_ident });
            case 'o': case 'O': return try_match({ etoken::t_ident });
            case 'p': case 'P': return try_match({ etoken::t_private, etoken::t_ident });
            case 'q': case 'Q': return try_match({ etoken::t_ident });
            case 'r': case 'R': return try_match({ etoken::t_ident });
            case 's': case 'S': return try_match({ etoken::t_ident });
            case 't': case 'T': return try_match({ etoken::t_true, etoken::t_ident });
            case 'u': case 'U': return try_match({ etoken::t_ident });
            case 'v': case 'V': return try_match({ etoken::t_ident });
            case 'w': case 'W': return try_match({ etoken::t_ident });
            case 'x': case 'X': return try_match({ etoken::t_ident });
            case 'y': case 'Y': return try_match({ etoken::t_ident });
            case 'z': case 'Z': return try_match({ etoken::t_ident });
            case '_':           return try_match({ etoken::t_ident });
            case '0':           return try_match({ etoken::t_hexadecimal, etoken::t_number });
            case '1':           return try_match({ etoken::t_number });
            case '2':           return try_match({ etoken::t_number });
            case '3':           return try_match({ etoken::t_number });
            case '4':           return try_match({ etoken::t_number });
            case '5':           return try_match({ etoken::t_number });
            case '6':           return try_match({ etoken::t_number });
            case '7':           return try_match({ etoken::t_number });
            case '8':           return try_match({ etoken::t_number });
            case '9':           return try_match({ etoken::t_number });
            case '+':           return try_match({ etoken::t_number, etoken::t_operator });
            case '-':           return try_match({ etoken::t_number, etoken::t_operator });
            case '/':           return try_match({ etoken::i_comment_line, etoken::i_comment_block, etoken::t_operator });
            case '*':           return try_match({ etoken::t_operator });
            case '(':           return try_match({ etoken::s_roundo });
            case ')':           return try_match({ etoken::s_roundc });
            case '[':           return try_match({ etoken::s_edgeo });
            case ']':           return try_match({ etoken::s_edgec });
            case '{':           return try_match({ etoken::s_curlyo });
            case '}':           return try_match({ etoken::s_curlyc });
            case '%':           return try_match({ etoken::t_operator });
            case '&':           return try_match({ etoken::t_operator });
            case '$':           return try_match({ etoken::t_hexadecimal });
            case '!':           return try_match({ etoken::t_operator });
            case '|':           return try_match({ etoken::t_operator });
            case '>':           return try_match({ etoken::t_operator });
            case '<':           return try_match({ etoken::t_operator });
            case '"':           return try_match({ etoken::t_string_double });
            case '=':           return try_match({ etoken::t_operator, etoken::s_equal });
            case '\'':          return try_match({ etoken::t_string_single });
            case '?':           return try_match({ });
            case ':':           return try_match({ etoken::t_operator });
            case '^':           return try_match({ etoken::t_operator });
            case ';':           return try_match({ etoken::s_semicolon });
            case ',':           return try_match({ etoken::s_comma });
            case '.':           return try_match({ etoken::t_number });
            case ' ':           return try_match({ etoken::i_whitespace });
            case '\r':          return try_match({ etoken::i_whitespace });
            case '\t':          return try_match({ etoken::i_whitespace });
            case '\n':          return try_match({ etoken::i_whitespace });
            case '#':           return try_match({ etoken::m_line, etoken::t_operator });
            default:            return create_token();
            }
        }
        token create_token(etoken token_type = etoken::invalid) const
        {
            return
            {
                token_type,
                m_line,
                m_column,
                (size_t)(m_current - m_start),
                {},
                m_strings.back()
            };
        }
        std::string_view to_string(etoken t) const
        {
            using namespace std::string_view_literals;
            switch (t)
            {
            default:                         return "UNKNOWN"sv;
            case etoken::eof:                return "eof"sv;
            case etoken::invalid:            return "invalid"sv;
            case etoken::i_comment_line:     return "comment_line"sv;
            case etoken::i_comment_block:    return "comment_block"sv;
            case etoken::i_whitespace:       return "whitespace"sv;

            case etoken::t_true:             return "true"sv;
            case etoken::t_false:            return "false"sv;
            case etoken::t_private:          return "private"sv;

            case etoken::s_curlyo:           return "{"sv;
            case etoken::s_curlyc:           return "}"sv;
            case etoken::s_roundo:           return "("sv;
            case etoken::s_roundc:           return ")"sv;
            case etoken::s_edgeo:            return "["sv;
            case etoken::s_edgec:            return "]"sv;
            case etoken::s_semicolon:        return ";"sv;
            case etoken::s_comma:            return ","sv;
            case etoken::s_equal:            return "="sv;

            case etoken::t_operator:         return "operator"sv;
            case etoken::t_string_double:    return "dq string"sv;
            case etoken::t_string_single:    return "q string"sv;
            case etoken::t_ident:            return "ident"sv;
            case etoken::t_number:           return "number"sv;
            case etoken::t_hexadecimal:      return "hexadecimal"sv;
            }
        }
    };
}