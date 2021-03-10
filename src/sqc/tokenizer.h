#pragma once
#include "../runtime/util.h"
#include <array>
#include <string>
#include <string_view>
#include <cctype>
namespace sqf::sqc
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

            t_be,
            t_case,
            t_catch,
            t_do,
            t_default,
            t_else,
            t_function,
            t_final,
            t_for,
            t_from,
            t_false,
            t_if,
            t_let,
            t_nil,
            t_params,
            t_private,
            t_return,
            t_step,
            t_switch,
            t_to,
            t_try,
            t_true,
            t_throw,
            t_while,
            t_this,
            t_break,

            s_curlyo,
            s_curlyc,
            s_roundo,
            s_roundc,
            s_edgeo,
            s_edgec,
            s_equalequalequal,
            s_equalequal,
            s_equal,
            s_greaterthenequal,
            s_greaterthengreaterthen,
            s_greaterthen,
            s_lessthenequal,
            s_lessthen,
            s_plus,
            s_plusplus,
            s_plusassign,
            s_minus,
            s_minusminus,
            s_minusassign,
            s_notequalequal,
            s_notequal,
            s_exclamationmark,
            s_percent,
            s_star,
            s_circumflex,
            s_starassign,
            s_slash,
            s_slashassign,
            s_andand,
            s_oror,
            s_questionmark,
            s_colon,
            s_semicolon,
            s_comma,
            s_dotdot,
            s_dot,

            t_string,
            t_ident,
            t_number,
            t_formatted_string_final,
            t_formatted_string_start,
            t_formatted_string_continue
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
            std::string path;
        };
        using iterator = std::string::iterator;
    private:
        iterator m_start;
        iterator m_current;
        iterator m_end;
        std::string m_path;
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
        bool is_match(iterator value) { return value < m_end&& is_match<TArgs...>(*value); }
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
            if (it < m_end && 
                (
                    ((char)std::tolower(*it) >= 'a' && (char)std::tolower(*it) <= 'z')
                    ||
                    ((char)std::tolower(*it) >= '0' && (char)std::tolower(*it) <= '9')
                    ||
                    *it == '_'
                ))
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
                            m_path = { start + 1, iter - 1 };
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

                case etoken::t_be:                      len = len_ident_match(iter, "be"); break;
                case etoken::t_break:                   len = len_ident_match(iter, "break"); break;
                case etoken::t_case:                    len = len_ident_match(iter, "case"); break;
                case etoken::t_catch:                   len = len_ident_match(iter, "catch"); break;
                case etoken::t_do:                      len = len_ident_match(iter, "do"); break;
                case etoken::t_default:                 len = len_ident_match(iter, "default"); break;
                case etoken::t_else:                    len = len_ident_match(iter, "else"); break;
                case etoken::t_function:                len = len_ident_match(iter, "function"); break;
                case etoken::t_from:                    len = len_ident_match(iter, "from"); break;
                case etoken::t_false:                   len = len_ident_match(iter, "false"); break;
                case etoken::t_for:                     len = len_ident_match(iter, "for"); break;
                case etoken::t_final:                   len = len_ident_match(iter, "final"); break;
                case etoken::t_let:                     len = len_ident_match(iter, "let"); break;;
                case etoken::t_if:                      len = len_ident_match(iter, "if"); break;
                case etoken::t_nil:                     len = len_ident_match(iter, "nil"); break;
                case etoken::t_params:                  len = len_ident_match(iter, "params"); break;
                case etoken::t_private:                 len = len_ident_match(iter, "private"); break;
                case etoken::t_return:                  len = len_ident_match(iter, "return"); break;
                case etoken::t_step:                    len = len_ident_match(iter, "step"); break;
                case etoken::t_switch:                  len = len_ident_match(iter, "switch"); break;
                case etoken::t_throw:                   len = len_ident_match(iter, "throw"); break;
                case etoken::t_to:                      len = len_ident_match(iter, "to"); break;
                case etoken::t_try:                     len = len_ident_match(iter, "try"); break;
                case etoken::t_this:                    len = len_ident_match(iter, "this"); break;
                case etoken::t_true:                    len = len_ident_match(iter, "true"); break;
                case etoken::t_while:                   len = len_ident_match(iter, "while"); break;

                case etoken::s_curlyo:                  len = is_match<'{'>(iter); break;
                case etoken::s_curlyc:                  len = is_match<'}'>(iter); break;
                case etoken::s_roundo:                  len = is_match<'('>(iter); break;
                case etoken::s_roundc:                  len = is_match<')'>(iter); break;
                case etoken::s_edgeo:                   len = is_match<'['>(iter); break;
                case etoken::s_edgec:                   len = is_match<']'>(iter); break;
                case etoken::s_equalequalequal:         len = is_match_repeated<3, '='>(iter) ? 3 : 0; break;
                case etoken::s_equalequal:              len = is_match_repeated<2, '='>(iter) ? 2 : 0; break;
                case etoken::s_equal:                   len = is_match<'='>(iter); break;
                case etoken::s_greaterthenequal:        len = is_match<'>'>(iter) && is_match<'='>(iter + 1) ? 2 : 0; break;
                case etoken::s_greaterthengreaterthen:  len = is_match_repeated<3, '>'>(iter) ? 3 : 0; break;
                case etoken::s_greaterthen:             len = is_match<'>'>(iter); break;
                case etoken::s_lessthenequal:           len = is_match<'<'>(iter) && is_match<'='>(iter + 1) ? 2 : 0; break;
                case etoken::s_lessthen:                len = is_match<'<'>(iter); break;
                case etoken::s_plus:                    len = is_match<'+'>(iter); break;
                case etoken::s_plusplus:                len = is_match_repeated<2, '+'>(iter) ? 2 : 0; break;
                case etoken::s_plusassign:              len = is_match<'+'>(iter) && is_match<'='>(iter + 1) ? 2 : 0; break;
                case etoken::s_minus:                   len = is_match<'-'>(iter); break;
                case etoken::s_minusminus:              len = is_match_repeated<2, '-'>(iter) ? 2 : 0; break;
                case etoken::s_minusassign:             len = is_match<'-'>(iter) && is_match<'='>(iter + 1) ? 2 : 0; break;
                case etoken::s_notequalequal:           len = is_match<'!'>(iter) && is_match_repeated<2, '='>(iter + 1) ? 3 : 0; break;
                case etoken::s_notequal:                len = is_match<'!'>(iter) && is_match<'='>(iter + 1) ? 2 : 0; break;
                case etoken::s_exclamationmark:         len = is_match<'!'>(iter); break;
                case etoken::s_percent:                 len = is_match<'%'>(iter); break;
                case etoken::s_starassign:              len = is_match<'*'>(iter) && is_match<'='>(iter + 1) ? 2 : 0; break;
                case etoken::s_star:                    len = is_match<'*'>(iter); break;
                case etoken::s_circumflex:              len = is_match<'^'>(iter); break;
                case etoken::s_slash:                   len = is_match<'/'>(iter); break;
                case etoken::s_slashassign:             len = is_match<'/'>(iter) && is_match<'='>(iter + 1) ? 2 : 0; break;
                case etoken::s_andand:                  len = is_match_repeated<2, '&'>(iter) ? 2 : 0; break;
                case etoken::s_oror:                    len = is_match_repeated<2, '|'>(iter) ? 2 : 0; break;
                case etoken::s_questionmark:            len = is_match<'?'>(iter); break;
                case etoken::s_colon:                   len = is_match<':'>(iter); break;
                case etoken::s_semicolon:               len = is_match<';'>(iter); break;
                case etoken::s_comma:                   len = is_match<','>(iter); break;
                case etoken::s_dotdot:                  len = is_match_repeated<2, '.'>(iter) ? 2 : 0; break;
                case etoken::s_dot:                     len = is_match<'.'>(iter); break;

                case etoken::t_string: {
                    ++iter;
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
                            len = 0;
                            break;
                        }
                        else
                        {
                            ++iter;
                        }
                    }
                    // set length
                    len = iter - m_current;
                } break;
                case etoken::t_formatted_string_start: {
                    if (!is_match<'"'>(iter + 1)) { break; }
                    ++iter;
                    ++iter;
                    // find string end
                    while (true)
                    {
                        if (is_match_repeated<2, '"'>(iter) || is_match_repeated<2, '{'>(iter) || is_match_repeated<2, '}'>(iter))
                        {
                            ++iter;
                        }
                        else if (is_match<'"'>(iter))
                        {
                            ++iter;
                            break;
                        }
                        else if (is_match<'{'>(iter))
                        {
                            ++iter;
                            m_mode = emode::formatable_string;
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
                            len = 0;
                            break;
                        }
                        else
                        {
                            ++iter;
                        }
                    }
                    // set length
                    len = iter - m_current;
                } break;
                case etoken::t_formatted_string_continue: {
                    if (m_mode != emode::formatable_string) { /* only available in formatable_string mode */ break; }

                    ++iter;
                    // find string end
                    while (true)
                    {
                        if (is_match_repeated<2, '"'>(iter) || is_match_repeated<2, '{'>(iter) || is_match_repeated<2, '}'>(iter))
                        {
                            ++iter;
                        }
                        else if (is_match<'"'>(iter))
                        {
                            // Retype as formatted_string final due to this the end
                            token_type = etoken::t_formatted_string_final;
                            m_mode = emode::normal;
                            ++iter;
                            break;
                        }
                        else if (is_match<'{'>(iter))
                        {
                            ++iter;
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
                            len = 0;
                            break;
                        }
                        else
                        {
                            ++iter;
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
                case etoken::t_number: {
                    size_t res = 0;
                    // match (optional) prefix
                    res = is_match<'-', '+'>(iter);
                    len += res; iter += res;

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
                    return t;
                }
            }
            return t;
        }

    public:
        tokenizer(iterator start, iterator end, std::string path) : m_start(start), m_current(start), m_end(end), m_mode(emode::normal), m_line(0), m_column(0) {}
        token next()
        {
            if (m_current == m_end) { return { etoken::eof, m_line, m_column, (size_t)(m_current - m_start), {} }; };
            switch (*m_current)
            {
            case 'a': case 'A': return try_match({ etoken::t_ident });
            case 'b': case 'B': return try_match({ etoken::t_be, etoken::t_break, etoken::t_ident });
            case 'c': case 'C': return try_match({ etoken::t_catch, etoken::t_case, etoken::t_ident });
            case 'd': case 'D': return try_match({ etoken::t_do, etoken::t_default, etoken::t_ident });
            case 'e': case 'E': return try_match({ etoken::t_else, etoken::t_ident });
            case 'f': case 'F': return try_match({ etoken::t_function, etoken::t_final, etoken::t_from, etoken::t_for, etoken::t_false, etoken::t_ident });
            case 'g': case 'G': return try_match({ etoken::t_ident });
            case 'h': case 'H': return try_match({ etoken::t_ident });
            case 'i': case 'I': return try_match({ etoken::t_if, etoken::t_ident });
            case 'j': case 'J': return try_match({ etoken::t_ident });
            case 'k': case 'K': return try_match({ etoken::t_ident });
            case 'l': case 'L': return try_match({ etoken::t_let, etoken::t_ident });
            case 'm': case 'M': return try_match({ etoken::t_ident });
            case 'n': case 'N': return try_match({ etoken::t_nil, etoken::t_ident });
            case 'o': case 'O': return try_match({ etoken::t_ident });
            case 'p': case 'P': return try_match({ etoken::t_private, etoken::t_params, etoken::t_ident });
            case 'q': case 'Q': return try_match({ etoken::t_ident });
            case 'r': case 'R': return try_match({ etoken::t_return, etoken::t_ident });
            case 's': case 'S': return try_match({ etoken::t_step, etoken::t_switch, etoken::t_ident });
            case 't': case 'T': return try_match({ etoken::t_throw, etoken::t_this, etoken::t_to, etoken::t_try, etoken::t_true, etoken::t_ident });
            case 'u': case 'U': return try_match({ etoken::t_ident });
            case 'v': case 'V': return try_match({ etoken::t_ident });
            case 'w': case 'W': return try_match({ etoken::t_while, etoken::t_ident });
            case 'x': case 'X': return try_match({ etoken::t_ident });
            case 'y': case 'Y': return try_match({ etoken::t_ident });
            case 'z': case 'Z': return try_match({ etoken::t_ident });
            case '_':           return try_match({ etoken::t_ident });
            case '0':           return try_match({ etoken::t_number });
            case '1':           return try_match({ etoken::t_number });
            case '2':           return try_match({ etoken::t_number });
            case '3':           return try_match({ etoken::t_number });
            case '4':           return try_match({ etoken::t_number });
            case '5':           return try_match({ etoken::t_number });
            case '6':           return try_match({ etoken::t_number });
            case '7':           return try_match({ etoken::t_number });
            case '8':           return try_match({ etoken::t_number });
            case '9':           return try_match({ etoken::t_number });
            case '+':           return try_match({ etoken::t_number, etoken::s_plusassign, etoken::s_plusplus, etoken::s_plus });
            case '-':           return try_match({ etoken::t_number, etoken::s_minusassign, etoken::s_minusminus, etoken::s_minus });
            case '/':           return try_match({ etoken::i_comment_line, etoken::i_comment_block, etoken::s_slashassign, etoken::s_slash });
            case '*':           return try_match({ etoken::s_starassign, etoken::s_star });
            case '(':           return try_match({ etoken::s_roundo });
            case ')':           return try_match({ etoken::s_roundc });
            case '[':           return try_match({ etoken::s_edgeo });
            case ']':           return try_match({ etoken::s_edgec });
            case '{':           return try_match({ etoken::s_curlyo });
            case '}':           return try_match({ etoken::t_formatted_string_continue, etoken::s_curlyc });
            case '&':           return try_match({ etoken::s_andand });
            case '$':           return try_match({ etoken::t_formatted_string_start });
            case '!':           return try_match({ etoken::s_notequalequal, etoken::s_notequal, etoken::s_exclamationmark });
            case '|':           return try_match({ etoken::s_oror });
            case '>':           return try_match({ etoken::s_greaterthenequal, etoken::s_greaterthengreaterthen, etoken::s_greaterthen });
            case '<':           return try_match({ etoken::s_lessthenequal, etoken::s_lessthen });
            case '"':           return try_match({ etoken::t_string });
            case '=':           return try_match({ etoken::s_equalequalequal, etoken::s_equalequal, etoken::s_equal });
            case '\'':          return try_match({ etoken::t_string });
            case '?':           return try_match({ etoken::s_questionmark });
            case ':':           return try_match({ etoken::s_colon });
            case ';':           return try_match({ etoken::s_semicolon });
            case ',':           return try_match({ etoken::s_comma });
            case '.':           return try_match({ etoken::s_dotdot, etoken::s_dot });
            case '%':           return try_match({ etoken::s_percent });
            case ' ':           return try_match({ etoken::i_whitespace });
            case '\r':          return try_match({ etoken::i_whitespace });
            case '\t':          return try_match({ etoken::i_whitespace });
            case '\n':          return try_match({ etoken::i_whitespace });
            case '#':           return try_match({ etoken::m_line });
            case '^':           return try_match({ etoken::s_circumflex });
            default:            return { etoken::invalid, m_line, m_column, (size_t)(m_current - m_start), {} };
            }
        }
        token create_token() const
        {
            return { etoken::invalid, m_line, m_column, (size_t)(m_current - m_start), m_path };
        }
        std::string_view to_string(etoken t) const
        {
            using namespace std::string_view_literals;
            switch (t)
            {
            default:                                return "UNKNOWN"sv;
            case etoken::eof:                       return "eof"sv;
            case etoken::invalid:                   return "invalid"sv;
            case etoken::i_comment_line:            return "comment_line"sv;
            case etoken::i_comment_block:           return "comment_block"sv;
            case etoken::i_whitespace:              return "whitespace"sv;

            case etoken::t_return:                  return "return"sv;
            case etoken::t_throw:                   return "throw"sv;
            case etoken::t_let:                     return "let"sv;
            case etoken::t_be:                      return "be"sv;
            case etoken::t_break:                   return "break"sv;
            case etoken::t_function:                return "function"sv;
            case etoken::t_final:                   return "final"sv;
            case etoken::t_if:                      return "if"sv;
            case etoken::t_else:                    return "else"sv;
            case etoken::t_from:                    return "from"sv;
            case etoken::t_to:                      return "to"sv;
            case etoken::t_step:                    return "step"sv;
            case etoken::t_while:                   return "while"sv;
            case etoken::t_do:                      return "do"sv;
            case etoken::t_try:                     return "try"sv;
            case etoken::t_this:                    return "this"sv;
            case etoken::t_catch:                   return "catch"sv;
            case etoken::t_switch:                  return "switch"sv;
            case etoken::t_case:                    return "case"sv;
            case etoken::t_default:                 return "default"sv;
            case etoken::t_nil:                     return "nil"sv;
            case etoken::t_true:                    return "true"sv;
            case etoken::t_false:                   return "false"sv;
            case etoken::t_for:                     return "for"sv;
            case etoken::t_private:                 return "private"sv;
            case etoken::t_params:                  return "params"sv;

            case etoken::s_curlyo:                  return "{"sv;
            case etoken::s_curlyc:                  return "}"sv;
            case etoken::s_roundo:                  return "("sv;
            case etoken::s_roundc:                  return ")"sv;
            case etoken::s_edgeo:                   return "["sv;
            case etoken::s_edgec:                   return "]"sv;
            case etoken::s_equalequalequal:         return "==="sv;
            case etoken::s_equalequal:              return "=="sv;
            case etoken::s_equal:                   return "="sv;
            case etoken::s_greaterthenequal:        return ">="sv;
            case etoken::s_greaterthengreaterthen:  return ">="sv;
            case etoken::s_greaterthen:             return ">"sv;
            case etoken::s_lessthenequal:           return "<="sv;
            case etoken::s_lessthen:                return "<"sv;
            case etoken::s_plus:                    return "+"sv;
            case etoken::s_plusassign:              return "+="sv;
            case etoken::s_minus:                   return "-"sv;
            case etoken::s_minusassign:             return "-="sv;
            case etoken::s_notequalequal:           return "!=="sv;
            case etoken::s_notequal:                return "!="sv;
            case etoken::s_exclamationmark:         return "!"sv;
            case etoken::s_percent:                 return "%"sv;
            case etoken::s_circumflex:              return "^"sv;
            case etoken::s_star:                    return "*"sv;
            case etoken::s_starassign:              return "*="sv;
            case etoken::s_slash:                   return "/"sv;
            case etoken::s_slashassign:             return "/="sv;
            case etoken::s_andand:                  return "&&"sv;
            case etoken::s_oror:                    return "||"sv;
            case etoken::s_questionmark:            return "?"sv;
            case etoken::s_colon:                   return ":"sv;
            case etoken::s_semicolon:               return ";"sv;
            case etoken::s_comma:                   return ","sv;
            case etoken::s_dotdot:                  return ".."sv;
            case etoken::s_dot:                     return "."sv;

            case etoken::t_string:                  return "string"sv;
            case etoken::t_ident:                   return "ident"sv;
            case etoken::t_number:                  return "number"sv;
            case etoken::t_formatted_string_final:    return "t_formatted_string"sv;
            case etoken::t_formatted_string_start:    return "formatted_string_start"sv;
            case etoken::t_formatted_string_continue: return "formatted_string_continue"sv;
            }
        }
    };
}