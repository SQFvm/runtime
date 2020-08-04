#include "default.h"

#include "../../opcodes/assign_to.h"
#include "../../opcodes/assign_to_local.h"
#include "../../opcodes/call_binary.h"
#include "../../opcodes/call_nular.h"
#include "../../opcodes/call_unary.h"
#include "../../opcodes/end_statement.h"
#include "../../opcodes/get_variable.h"
#include "../../opcodes/make_array.h"
#include "../../opcodes/push.h"

#include "../../runtime/d_string.h"
#include "../../runtime/d_scalar.h"
#include "../../runtime/d_code.h"

//#include "string_op.h"



#include <algorithm>
#include <cwctype>
#include <utility>
#include <vector>
#include <string>
#include <sstream>


namespace err = logmessage::sqf;

using namespace ::sqf::runtime::util;
using namespace ::sqf::parser::util;

bool sqf::parser::sqf::default::instance::m_contains_nular(std::string_view view)
{
    auto s = std::string(view);
    std::transform(s.begin(), s.end(), s.begin(), [](char& c) { return std::tolower(c); });
    return m_runtime.sqfop_exists(::sqf::runtime::sqfop_nular::key{ s });
}

bool sqf::parser::sqf::default::instance::m_contains_unary(std::string_view view)
{
    auto s = std::string(view);
    std::transform(s.begin(), s.end(), s.begin(), [](char& c) { return std::tolower(c); });
    return m_runtime.sqfop_exists_unary(s);
}

bool sqf::parser::sqf::default::instance::m_contains_binary(std::string_view view, short precedence)
{
    auto s = std::string(view);
    std::transform(s.begin(), s.end(), s.begin(), [](char& c) { return std::tolower(c); });
    if (!m_runtime.sqfop_exists_binary(s)) { return false; }
    auto& res = m_runtime.sqfop_binary_by_name(s);
    if (precedence == 0) { return !res.empty(); }
    for (auto& it : res)
    {
        if (it.get().precedence() == precedence)
        {
            return true;
        }
    }
    return false;
}

short sqf::parser::sqf::default::instance::m_precedence(std::string_view view)
{
    auto s = std::string(view);
    std::transform(s.begin(), s.end(), s.begin(), [](char& c) { return std::tolower(c); });
    auto& res = m_runtime.sqfop_binary_by_name(s);
    return res.empty() ? 0 : res.front().get().precedence();
}

void ::sqf::parser::sqf::default::instance::skip(size_t& curoff)
{
    while (true)
    {
        switch (m_contents[curoff])
        {
        case '\0': return;
        case ' ': curoff++; continue;
        case '\t': curoff++; continue;
        case '\r': curoff++; continue;
        case '\n': curoff++; continue;
        default: return;
        }
    }
}
void ::sqf::parser::sqf::default::instance::skip(::sqf::runtime::diagnostics::diag_info& info)
{
    while (true)
    {
        switch (m_contents[info.offset])
        {
        case '\0': return;
        case ' ': info.offset++; info.column++; continue;
        case '\t': info.offset++; info.column++; continue;
        case '\r': info.offset++; info.column++; continue;
        case '\n': info.offset++; info.line++; info.column = 0; continue;
        case '#':
            if ((m_contents[info.offset + 1] == 'l' || m_contents[info.offset + 1] == 'L') &&
                (m_contents[info.offset + 2] == 'i' || m_contents[info.offset + 1] == 'I') &&
                (m_contents[info.offset + 3] == 'n' || m_contents[info.offset + 1] == 'N') &&
                (m_contents[info.offset + 4] == 'e' || m_contents[info.offset + 1] == 'E'))
            {
                info.offset += 6;
                size_t start = info.offset;
                for (; m_contents[info.offset] != '\0' && m_contents[info.offset] != '\n' && m_contents[info.offset] != ' '; info.offset++);
                auto str_tmp = std::string(m_contents.substr(start, info.offset - start));
                info.line = static_cast<size_t>(std::stoul(str_tmp));

                for (; m_contents[info.offset] != '\0' && m_contents[info.offset] != '\n' && m_contents[info.offset] == ' '; info.offset++);
                if (m_contents[info.offset] != '\0' && m_contents[info.offset] != '\n')
                {
                    start = info.offset;
                    for (; m_contents[info.offset] != '\0' && m_contents[info.offset] != '\n'; info.offset++);
                    auto str = std::string(m_contents.substr(start, info.offset - start));
                    info.path = { strip_quotes(str), {} };
                }
                break;
            }
        default: return;
        }
    }
}

//endchr = [,;];
size_t sqf::parser::sqf::default::instance::endchr(size_t off) { return util::is_match<';', ','>(m_contents[off]) ? 1 : 0; }
//identifier = [_a-zA-Z][_a-zA-Z0-9]*;
size_t sqf::parser::sqf::default::instance::identifier(size_t off)
{
    if (!util::is_match<
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '_'>(m_contents[off]))
    {
        return 0;
    }
    return util::len_match<
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_'>(m_contents + off);
}
//identifier = [_a-zA-Z0-9]+;
size_t sqf::parser::sqf::default::instance::assidentifier(size_t off)
{
    return util::len_match<
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_'>(m_contents + off);
}
//operator_ = [+-*/%^]|&&|\|\||==|[!<>][=]?|[a-zA-Z_]+;
size_t sqf::parser::sqf::default::instance::operator_(size_t off)
{
    if (util::is_match<'+', '-', '*', '/', '%', '^', ':', '#'>(m_contents[off])) { return 1; }
    if (util::is_match_x<2, '|'>(m_contents + off) ||
        util::is_match_x<2, '&'>(m_contents + off) ||
        util::is_match_x<2, '='>(m_contents + off) ||
        util::is_match_x<2, '>'>(m_contents + off) ||
        util::is_match_x<2, '<'>(m_contents + off)) { return 2; }

    if (util::is_match<'<', '!', '>'>(m_contents[off]))
    {
        if (util::is_match<'='>(m_contents[off + 1])) return 2;
        return 1;
    }
    return util::len_match<
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_'>(m_contents + off);
}
// hexadecimal = [0-9a-fA-F]+;
size_t sqf::parser::sqf::default::instance::hexadecimal(size_t off)
{
    return util::len_match<
        'a', 'b', 'c', 'd', 'e', 'f',
        'A', 'B', 'C', 'D', 'E', 'F',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_'>(m_contents + off);
}
// scalarsub = [0-9]+;
size_t sqf::parser::sqf::default::instance::scalarsub(size_t off)
{
    return util::len_match<'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'>(m_contents + off);
}
// scalar = scalarsub(.scalarsub)?;
size_t sqf::parser::sqf::default::instance::scalar(size_t off)
{
    size_t i = off + scalarsub(off);
    if (util::is_match<'='>(m_contents[off]) == '.')
    {
        i += scalarsub(off);
    }
    return i - off;
}
// anytext = (?![ \t\r\n;])+;
size_t sqf::parser::sqf::default::instance::anytext(size_t off)
{
    const char* begin = m_contents + off;
    const char* it = begin;
    for (; !util::is_match<' ', '\t', '\r', '\n', ';', '\0'>(*it); ++it) {};
    return it - begin;
}
//SQF = [ STATEMENT { endchr { endchr } STATEMENT } ]
bool ::sqf::parser::sqf::default::instance::SQF_start(size_t curoff) { return true; }
void ::sqf::parser::sqf::default::instance::SQF(astnode& root, bool& errflag)
{
    skip(m_info);
    size_t endchrlen;
    while ((endchrlen = endchr(m_info.offset)) > 0)
    {
        m_info.offset += endchrlen;
        m_info.column += endchrlen;
        skip(m_info);
    }
    //Iterate over statements as long as it is an instruction start.
    while (STATEMENT_start(m_info.offset))
    {
        STATEMENT(root, errflag);
        skip(m_info);
        //Make sure at least one endchr is available unless no statement follows
        if (!endchr(m_info.offset) && STATEMENT_start(m_info.offset))
        {
            m_owner.log(err::ExpectedStatementTerminator(m_info));
            errflag = true;
        }
        else
        {
            //Add endchr up until no semicolon is left
            while ((endchrlen = endchr(m_info.offset)) > 0)
            {
                m_info.offset += endchrlen;
                m_info.column += endchrlen;
                skip(m_info);
            }
        }
    }
}
//STATEMENT = ASSIGNMENT | BINARYEXPRESSION;
bool ::sqf::parser::sqf::default::instance::STATEMENT_start(size_t curoff) { return ASSIGNMENT_start(curoff) | BINARYEXPRESSION_start(curoff); }
void ::sqf::parser::sqf::default::instance::STATEMENT(astnode& root, bool& errflag)
{
    if (ASSIGNMENT_start(m_info.offset))
    {
        ASSIGNMENT(root, errflag);
    }
    else if (BINARYEXPRESSION_start(m_info.offset))
    {
        BINARYEXPRESSION(root, errflag);
    }
    else
    {
        m_owner.log(err::NoViableAlternativeStatement(m_info));
        errflag = true;
    }
    //thisnode.length = curoff - thisnode.offset;
    //root.children.push_back(thisnode);
}
//ASSIGNMENT(2) = assidentifier '=' BINARYEXPRESSION | "private" assidentifier '=' BINARYEXPRESSION;
bool ::sqf::parser::sqf::default::instance::ASSIGNMENT_start(size_t curoff)
{
    size_t len;
#if defined(SQFVM_ARMA2_SYNTAX)
    const char* priv = "local";
#else
    const char* priv = "private";
#endif

    if (std::equal(
        m_contents + curoff, m_contents + curoff + ::sqf::runtime::util::strlen(priv),
        priv, priv + ::sqf::runtime::util::strlen(priv),
        [](char l, char r) { return std::tolower(l) == std::tolower(r); }))
    {
        curoff += ::sqf::runtime::util::strlen(priv);
        skip(curoff);
    }
    if ((len = assidentifier(curoff)) > 0)
    {
        curoff += len;
        skip(curoff);
        return m_contents[curoff] == '=' && m_contents[curoff + 1] != '=';
    }
    else
    {
        return false;
    }
}
void ::sqf::parser::sqf::default::instance::ASSIGNMENT(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.kind = nodetype::ASSIGNMENT;
    thisnode.offset = m_info.offset;
    thisnode.path = m_info.path;
    size_t len;
    bool assignlocal = false;
#if defined(SQFVM_ARMA2_SYNTAX)
    const char* priv = "local";
#else
    const char* priv = "private";
#endif

    if (std::equal(
        m_contents + m_info.offset, m_contents + m_info.offset + ::sqf::runtime::util::strlen(priv),
        priv, priv + ::sqf::runtime::util::strlen(priv),
        [](char l, char r) { return std::tolower(l) == std::tolower(r); }))
    {
        m_info.offset += ::sqf::runtime::util::strlen(priv);
        m_info.column += ::sqf::runtime::util::strlen(priv);
        skip(m_info);
        assignlocal = true;
        thisnode.kind = nodetype::ASSIGNMENTLOCAL;
    }
    //receive the ident
    len = assidentifier(m_info.offset);
    auto ident = std::string(m_contents.substr(m_info.offset, len));
    thisnode.length = len;
    thisnode.line = m_info.line;
    thisnode.col = m_info.column;

    auto varnode = astnode();
    varnode.offset = m_info.offset;
    varnode.length = len;
    varnode.content = ident;
    varnode.kind = nodetype::VARIABLE;
    varnode.col = m_info.column;
    varnode.line = m_info.line;
    varnode.path = m_info.path;
    thisnode.children.emplace_back(std::move(varnode));

    if (assignlocal && ident[0] != '_')
    {
        m_owner.log(err::MissingUnderscoreOnPrivateVariable(m_info, ident));
    }
    if (ident[0] >= '0' && ident[0] <= '9')
    {
        m_owner.log(err::InvalidStartOfGlobalVariable(m_info, ident));
    }
    m_info.offset += len;
    m_info.column += len;
    skip(m_info);
    //skip the '=' (is confirmed to be present in ASSIGNMENT_start)
    m_info.offset++;
    m_info.column++;
    skip(m_info);

    if (BINARYEXPRESSION_start(m_info.offset))
    {
        BINARYEXPRESSION(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::ExpectedBinaryExpression(m_info));
        errflag = true;
    }
    thisnode.length = m_info.offset - thisnode.offset;
    root.children.emplace_back(std::move(thisnode));
}
//BINARYEXPRESSION = BEXP1;
//BEXP1 = BEXP2 [ boperator BEXP10 ];
//BEXP2 = BEXP3 [ boperator BEXP10 ];
//BEXP3 = BEXP4 [ boperator BEXP10 ];
//BEXP4 = BEXP5 [ boperator BEXP10 ];
//BEXP5 = BEXP6 [ boperator BEXP10 ];
//BEXP6 = BEXP7 [ boperator BEXP10 ];
//BEXP7 = BEXP8 [ boperator BEXP10 ];
//BEXP8 = BEXP9 [ boperator BEXP10 ];
//BEXP9 = BEXP10 [ boperator BEXP10 ];
//BEXP10 = PRIMARYEXPRESSION [ boperator BEXP10 ];
void ::sqf::parser::sqf::default::instance::bexp_orderfix(astnode& root, astnode thisnode, nodetype plevel)
{
    auto& othernodeRef = thisnode.children.back();
    if (othernodeRef.children.size() == 3 && othernodeRef.kind == plevel)
    {
        auto othernode = thisnode.children.back();
        astnode* ptr = &othernode.children.front();
        astnode* lptr = &othernode;
        while (ptr->children.size() == 3 && ptr->kind == plevel)
        {
            lptr = ptr;
            ptr = &ptr->children.front();
        }

        thisnode.children.pop_back();
        thisnode.children.push_back(*ptr);
        lptr->children.front() = thisnode;
        root.children.push_back(othernode);
    }
    else
    {
        root.children.emplace_back(std::move(thisnode));
    }
}
bool ::sqf::parser::sqf::default::instance::bexp10_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void ::sqf::parser::sqf::default::instance::bexp10(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.offset = m_info.offset;
    thisnode.kind = nodetype::BEXP10;
    thisnode.path = m_info.path;
    thisnode.line = m_info.line;
    size_t oplen;
    std::string op;
    skip(m_info);
    if (PRIMARYEXPRESSION_start(m_info.offset))
    {
        PRIMARYEXPRESSION(thisnode, errflag);
    }

    else
    {
        m_owner.log(err::ExpectedBinaryExpression(m_info));
        errflag = true;
    }
    skip(m_info);
    if ((oplen = operator_(m_info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(m_info.offset, oplen)), 10))
    {
        auto node = astnode();
        node.content = op;
        node.offset = m_info.offset;
        node.kind = nodetype::BINARYOP;
        node.col = m_info.column;
        node.line = m_info.line;
        node.length = oplen;
        node.path = m_info.path;
        thisnode.children.emplace_back(std::move(node));
        m_info.offset += oplen;
        m_info.column += oplen;
        skip(m_info);
        if (bexp10_start(m_info.offset))
        {
            bexp10(thisnode, errflag);
        }
        else
        {
            m_owner.log(err::MissingRightArgument(m_info, op));
            errflag = true;
        }
        bexp_orderfix(root, std::move(thisnode), nodetype::BEXP10);
    }
    else
    {
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
    }
}
bool ::sqf::parser::sqf::default::instance::bexp9_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void ::sqf::parser::sqf::default::instance::bexp9(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.offset = m_info.offset;
    thisnode.kind = nodetype::BEXP9;
    thisnode.path = m_info.path;
    thisnode.line = m_info.line;
    size_t oplen;
    std::string op;
    skip(m_info);
    if (bexp10_start(m_info.offset))
    {
        bexp10(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::ExpectedBinaryExpression(m_info));
        errflag = true;
    }
    skip(m_info);
    if ((oplen = operator_(m_info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(m_info.offset, oplen)), 9))
    {
        auto node = astnode();
        node.content = op;
        node.offset = m_info.offset;
        node.kind = nodetype::BINARYOP;
        node.col = m_info.column;
        node.line = m_info.line;
        node.length = oplen;
        node.path = m_info.path;
        thisnode.children.emplace_back(std::move(node));
        m_info.offset += oplen;
        m_info.column += oplen;
        skip(m_info);
        if (bexp9_start(m_info.offset))
        {
            bexp9(thisnode, errflag);
        }
        else
        {
            m_owner.log(err::MissingRightArgument(m_info, op));
            errflag = true;
        }
        bexp_orderfix(root, std::move(thisnode), nodetype::BEXP9);
    }
    else
    {
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
    }
}
bool ::sqf::parser::sqf::default::instance::bexp8_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void ::sqf::parser::sqf::default::instance::bexp8(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.offset = m_info.offset;
    thisnode.kind = nodetype::BEXP8;
    thisnode.path = m_info.path;
    thisnode.line = m_info.line;
    size_t oplen;
    std::string op;
    skip(m_info);
    if (bexp9_start(m_info.offset))
    {
        bexp9(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::ExpectedBinaryExpression(m_info));
        errflag = true;
    }
    skip(m_info);
    if ((oplen = operator_(m_info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(m_info.offset, oplen)), 8))
    {
        auto node = astnode();
        node.content = op;
        node.offset = m_info.offset;
        node.kind = nodetype::BINARYOP;
        node.col = m_info.column;
        node.line = m_info.line;
        node.length = oplen;
        node.path = m_info.path;
        thisnode.children.emplace_back(std::move(node));
        m_info.offset += oplen;
        m_info.column += oplen;
        skip(m_info);
        if (bexp8_start(m_info.offset))
        {
            bexp8(thisnode, errflag);
        }
        else
        {
            m_owner.log(err::MissingRightArgument(m_info, op));
            errflag = true;
        }
        bexp_orderfix(root, std::move(thisnode), nodetype::BEXP8);
    }
    else
    {
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
    }
}
bool ::sqf::parser::sqf::default::instance::bexp7_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void ::sqf::parser::sqf::default::instance::bexp7(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.offset = m_info.offset;
    thisnode.kind = nodetype::BEXP7;
    thisnode.path = m_info.path;
    thisnode.line = m_info.line;
    size_t oplen;
    std::string op;
    skip(m_info);
    if (bexp8_start(m_info.offset))
    {
        bexp8(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::ExpectedBinaryExpression(m_info));
        errflag = true;
    }
    skip(m_info);
    if ((oplen = operator_(m_info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(m_info.offset, oplen)), 7))
    {
        auto node = astnode();
        node.content = op;
        node.offset = m_info.offset;
        node.kind = nodetype::BINARYOP;
        node.col = m_info.column;
        node.line = m_info.line;
        node.length = oplen;
        node.path = m_info.path;
        thisnode.children.emplace_back(std::move(node));
        m_info.offset += oplen;
        m_info.column += oplen;
        skip(m_info);
        if (bexp7_start(m_info.offset))
        {
            bexp7(thisnode, errflag);
        }
        else
        {
            m_owner.log(err::MissingRightArgument(m_info, op));
            errflag = true;
        }
        bexp_orderfix(root, std::move(thisnode), nodetype::BEXP7);
    }
    else
    {
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
    }
}
bool ::sqf::parser::sqf::default::instance::bexp6_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void ::sqf::parser::sqf::default::instance::bexp6(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.offset = m_info.offset;
    thisnode.kind = nodetype::BEXP6;
    thisnode.path = m_info.path;
    thisnode.line = m_info.line;
    size_t oplen;
    std::string op;
    skip(m_info);
    if (bexp7_start(m_info.offset))
    {
        bexp7(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::ExpectedBinaryExpression(m_info));
        errflag = true;
    }
    skip(m_info);
    if ((oplen = operator_(m_info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(m_info.offset, oplen)), 6))
    {
        auto node = astnode();
        node.content = op;
        node.offset = m_info.offset;
        node.kind = nodetype::BINARYOP;
        node.col = m_info.column;
        node.line = m_info.line;
        node.length = oplen;
        node.path = m_info.path;
        thisnode.children.emplace_back(std::move(node));
        m_info.offset += oplen;
        m_info.column += oplen;
        skip(m_info);
        if (bexp6_start(m_info.offset))
        {
            bexp6(thisnode, errflag);
        }
        else
        {
            m_owner.log(err::MissingRightArgument(m_info, op));
            errflag = true;
        }
        bexp_orderfix(root, std::move(thisnode), nodetype::BEXP6);
    }
    else
    {
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
    }
}
bool ::sqf::parser::sqf::default::instance::bexp5_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void ::sqf::parser::sqf::default::instance::bexp5(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.offset = m_info.offset;
    thisnode.kind = nodetype::BEXP5;
    thisnode.path = m_info.path;
    thisnode.line = m_info.line;
    size_t oplen;
    std::string op;
    skip(m_info);
    if (bexp6_start(m_info.offset))
    {
        bexp6(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::ExpectedBinaryExpression(m_info));
        errflag = true;
    }
    skip(m_info);
    if ((oplen = operator_(m_info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(m_info.offset, oplen)), 5))
    {
        auto node = astnode();
        node.content = op;
        node.offset = m_info.offset;
        node.kind = nodetype::BINARYOP;
        node.col = m_info.column;
        node.line = m_info.line;
        node.length = oplen;
        node.path = m_info.path;
        thisnode.children.emplace_back(std::move(node));
        m_info.offset += oplen;
        m_info.column += oplen;
        skip(m_info);
        if (bexp5_start(m_info.offset))
        {
            bexp5(thisnode, errflag);
        }
        else
        {
            m_owner.log(err::MissingRightArgument(m_info, op));
            errflag = true;
        }
        bexp_orderfix(root, std::move(thisnode), nodetype::BEXP5);
    }
    else
    {
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
    }
}
bool ::sqf::parser::sqf::default::instance::bexp4_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void ::sqf::parser::sqf::default::instance::bexp4(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.offset = m_info.offset;
    thisnode.kind = nodetype::BEXP4;
    thisnode.path = m_info.path;
    thisnode.line = m_info.line;
    size_t oplen;
    std::string op;
    skip(m_info);
    if (bexp5_start(m_info.offset))
    {
        bexp5(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::ExpectedBinaryExpression(m_info));
        errflag = true;
    }
    skip(m_info);
    if ((oplen = operator_(m_info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(m_info.offset, oplen)), 4))
    {
        auto node = astnode();
        node.content = op;
        node.offset = m_info.offset;
        node.kind = nodetype::BINARYOP;
        node.col = m_info.column;
        node.line = m_info.line;
        node.length = oplen;
        node.path = m_info.path;
        thisnode.children.emplace_back(std::move(node));
        m_info.offset += oplen;
        m_info.column += oplen;
        skip(m_info);
        if (bexp4_start(m_info.offset))
        {
            bexp4(thisnode, errflag);
        }
        else
        {
            m_owner.log(err::MissingRightArgument(m_info, op));
            errflag = true;
        }
        bexp_orderfix(root, std::move(thisnode), nodetype::BEXP4);
    }
    else
    {
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
    }
}
bool ::sqf::parser::sqf::default::instance::bexp3_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void ::sqf::parser::sqf::default::instance::bexp3(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.offset = m_info.offset;
    thisnode.kind = nodetype::BEXP3;
    thisnode.path = m_info.path;
    thisnode.line = m_info.line;
    size_t oplen;
    std::string op;
    skip(m_info);
    if (bexp4_start(m_info.offset))
    {
        bexp4(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::ExpectedBinaryExpression(m_info));
        errflag = true;
    }
    skip(m_info);
    if ((oplen = operator_(m_info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(m_info.offset, oplen)), 3))
    {
        auto node = astnode();
        node.content = op;
        node.offset = m_info.offset;
        node.kind = nodetype::BINARYOP;
        node.col = m_info.column;
        node.line = m_info.line;
        node.length = oplen;
        node.path = m_info.path;
        thisnode.children.emplace_back(std::move(node));
        m_info.offset += oplen;
        m_info.column += oplen;
        skip(m_info);
        if (bexp3_start(m_info.offset))
        {
            bexp3(thisnode, errflag);
        }
        else
        {
            m_owner.log(err::MissingRightArgument(m_info, op));
            errflag = true;
        }
        bexp_orderfix(root, std::move(thisnode), nodetype::BEXP3);
    }
    else
    {
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
    }
}
bool ::sqf::parser::sqf::default::instance::bexp2_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void ::sqf::parser::sqf::default::instance::bexp2(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.offset = m_info.offset;
    thisnode.kind = nodetype::BEXP2;
    thisnode.path = m_info.path;
    thisnode.line = m_info.line;
    size_t oplen;
    std::string op;
    skip(m_info);
    if (bexp3_start(m_info.offset))
    {
        bexp3(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::ExpectedBinaryExpression(m_info));
        errflag = true;
    }
    skip(m_info);
    if ((oplen = operator_(m_info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(m_info.offset, oplen)), 2))
    {
        auto node = astnode();
        node.content = op;
        node.offset = m_info.offset;
        node.kind = nodetype::BINARYOP;
        node.col = m_info.column;
        node.line = m_info.line;
        node.length = oplen;
        node.path = m_info.path;
        thisnode.children.emplace_back(std::move(node));
        m_info.offset += oplen;
        m_info.column += oplen;
        skip(m_info);
        if (bexp2_start(m_info.offset))
        {
            bexp2(thisnode, errflag);
        }
        else
        {
            m_owner.log(err::MissingRightArgument(m_info, op));
            errflag = true;
        }
        bexp_orderfix(root, std::move(thisnode), nodetype::BEXP2);
    }
    else
    {
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
    }
}
bool ::sqf::parser::sqf::default::instance::bexp1_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void ::sqf::parser::sqf::default::instance::bexp1(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.offset = m_info.offset;
    thisnode.kind = nodetype::BEXP1;
    thisnode.path = m_info.path;
    thisnode.line = m_info.line;
    size_t oplen;
    std::string op;
    skip(m_info);
    if (bexp2_start(m_info.offset))
    {
        bexp2(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::ExpectedBinaryExpression(m_info));
        errflag = true;
    }
    skip(m_info);
    if ((oplen = operator_(m_info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(m_info.offset, oplen)), 1))
    {
        auto node = astnode();
        node.content = op;
        node.offset = m_info.offset;
        node.kind = nodetype::BINARYOP;
        node.col = m_info.column;
        node.line = m_info.line;
        node.length = oplen;
        node.path = m_info.path;
        thisnode.children.emplace_back(std::move(node));
        m_info.offset += oplen;
        m_info.column += oplen;
        skip(m_info);
        if (bexp1_start(m_info.offset))
        {
            bexp1(thisnode, errflag);
        }
        else
        {
            m_owner.log(err::MissingRightArgument(m_info, op));
            errflag = true;
        }
        bexp_orderfix(root, std::move(thisnode), nodetype::BEXP1);
    }
    else
    {
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
    }
}
bool ::sqf::parser::sqf::default::instance::BINARYEXPRESSION_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void ::sqf::parser::sqf::default::instance::BINARYEXPRESSION(astnode& root, bool& errflag)
{
    bexp1(root, errflag);
}
//BRACKETS = '(' BINARYEXPRESSION ')';
bool ::sqf::parser::sqf::default::instance::BRACKETS_start(size_t curoff) {  return m_contents[curoff] == '('; }
void ::sqf::parser::sqf::default::instance::BRACKETS(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.kind = nodetype::BRACKETS;
    thisnode.offset = m_info.offset;
    thisnode.path = m_info.path;
    m_info.offset++;
    m_info.column++;
    skip(m_info);
    if (BINARYEXPRESSION_start(m_info.offset))
    {
        BINARYEXPRESSION(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::ExpectedBinaryExpression(m_info));
        errflag = true;
    }
    skip(m_info);
    if (m_contents[m_info.offset] == ')')
    {
        m_info.offset++;
        m_info.column++;
    }
    else
    {
        m_owner.log(err::MissingRoundClosingBracket(m_info));
        errflag = true;
    }
    thisnode.length = m_info.offset - thisnode.offset;
    root.children.emplace_back(std::move(thisnode));
}
//PRIMARYEXPRESSION = NUMBER | UNARYEXPRESSION | NULAREXPRESSION | VARIABLE | STRING | CODE | BRACKETS | ARRAY;
bool ::sqf::parser::sqf::default::instance::PRIMARYEXPRESSION_start(size_t curoff) { return NUMBER_start(curoff) || UNARYEXPRESSION_start(curoff) || NULAREXPRESSION_start(curoff) || VARIABLE_start(curoff) || STRING_start(curoff) || CODE_start(curoff) || BRACKETS_start(curoff) || ARRAY_start(curoff); }
void ::sqf::parser::sqf::default::instance::PRIMARYEXPRESSION(astnode& root, bool& errflag)
{
    if (NUMBER_start(m_info.offset))
    {
        NUMBER(root, errflag);
    }
    else if (UNARYEXPRESSION_start(m_info.offset))
    {
        UNARYEXPRESSION(root, errflag);
    }
    else if (NULAREXPRESSION_start(m_info.offset))
    {
        NULAREXPRESSION(root, errflag);
    }
    else if (VARIABLE_start(m_info.offset))
    {
        VARIABLE(root, errflag);
    }
    else if (STRING_start(m_info.offset))
    {
        STRING(root, errflag);
    }
    else if (CODE_start(m_info.offset))
    {
        CODE(root, errflag);
    }
    else if (BRACKETS_start(m_info.offset))
    {
        BRACKETS(root, errflag);
    }
    else if (ARRAY_start(m_info.offset))
    {
        ARRAY(root, errflag);
    }
    else
    {
        m_owner.log(err::NoViableAlternativePrimaryExpression(m_info));
        errflag = true;
    }
    //thisnode.length = curoff - thisnode.offset;
    //root.children.push_back(thisnode);
}
//NULAREXPRESSION = operator;
bool ::sqf::parser::sqf::default::instance::NULAREXPRESSION_start(size_t curoff) { auto oplen = operator_(curoff); return oplen > 0 ? m_contains_nular(std::string(m_contents.substr(curoff, oplen))) : false; }
void ::sqf::parser::sqf::default::instance::NULAREXPRESSION(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.kind = nodetype::NULAROP;
    thisnode.path = m_info.path;
    auto len = operator_(m_info.offset);
    auto ident = std::string(m_contents.substr(m_info.offset, len));
    thisnode.content = ident;
    thisnode.length = len;
    thisnode.offset = m_info.offset;
    thisnode.col = m_info.column;
    thisnode.line = m_info.line;
    m_info.offset += len;
    m_info.column += len;
    root.children.emplace_back(std::move(thisnode));
}
//UNARYEXPRESSION = operator PRIMARYEXPRESSION;
bool ::sqf::parser::sqf::default::instance::UNARYEXPRESSION_start(size_t curoff) { auto oplen = operator_(curoff); return oplen > 0 ? m_contains_unary(std::string(m_contents.substr(curoff, oplen))) : false; }
void ::sqf::parser::sqf::default::instance::UNARYEXPRESSION(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.kind = nodetype::UNARYEXPRESSION;
    thisnode.offset = m_info.offset;
    thisnode.path = m_info.path;
    thisnode.col = m_info.column;
    thisnode.line = m_info.line;

    auto len = operator_(m_info.offset);
    auto operatorname = std::string(m_contents.substr(m_info.offset, len));
    auto opnode = astnode();
    opnode.kind = nodetype::UNARYOP;
    opnode.offset = m_info.offset;
    opnode.length = len;
    opnode.content = operatorname;
    opnode.col = m_info.column;
    opnode.path = m_info.path;
    opnode.line = m_info.line;
    thisnode.children.emplace_back(std::move(opnode));
    m_info.offset += len;
    m_info.column += len;
    skip(m_info);

    if (PRIMARYEXPRESSION_start(m_info.offset))
    {
        PRIMARYEXPRESSION(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::MissingRightArgument(m_info, operatorname));
        errflag = true;
    }
    thisnode.length = m_info.offset - thisnode.offset;
    root.children.emplace_back(std::move(thisnode));
}
//NUMBER = ("0x" | '$' | '.') hexadecimal | scalar;
bool ::sqf::parser::sqf::default::instance::NUMBER_start(size_t curoff)
{
    return  m_contents[curoff] == '$' ||
        (
            m_contents[curoff] == '.' &&
            m_contents[curoff + 1] >= '0' &&
            m_contents[curoff + 1] <= '9'
            ) ||
        (m_contents[curoff] >= '0' && m_contents[curoff] <= '9');
}
void ::sqf::parser::sqf::default::instance::NUMBER(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.kind = nodetype::NUMBER;
    thisnode.col = m_info.column;
    thisnode.line = m_info.line;
    thisnode.path = m_info.path;
    if (m_contents[m_info.offset] == '$')
    {
        thisnode.kind = nodetype::HEXNUMBER;
        size_t i;
        for (i = m_info.offset + 1; (m_contents[i] >= '0' && m_contents[i] <= '9') || (m_contents[i] >= 'A' && m_contents[i] <= 'F') || (m_contents[i] >= 'a' && m_contents[i] <= 'f'); i++);
        auto ident = std::string(m_contents.substr(m_info.offset + 1, i - m_info.offset));
        thisnode.content = ident;
        thisnode.offset = m_info.offset;
        thisnode.length = i - m_info.offset;
        m_info.column += i - m_info.offset;
        m_info.offset = i;
    }
    else if (m_contents[m_info.offset] == '0' && m_contents[m_info.offset + 1] == 'x')
    {
        thisnode.kind = nodetype::HEXNUMBER;
        size_t i;
        for (i = m_info.offset + 2; (m_contents[i] >= '0' && m_contents[i] <= '9') || (m_contents[i] >= 'A' && m_contents[i] <= 'F') || (m_contents[i] >= 'a' && m_contents[i] <= 'f'); i++);
        auto ident = std::string(m_contents.substr(m_info.offset, i - m_info.offset));
        thisnode.content = ident;
        thisnode.offset = m_info.offset;
        thisnode.length = i - m_info.offset;
        m_info.column += i - m_info.offset;
        m_info.offset = i;
    }
    else
    {
        size_t i = m_info.offset;
        bool numhaddot = false;
        unsigned short numhadexp = 0;
        while (true)
        {
            if (m_contents[i] >= '0' && m_contents[i] <= '9')
            {
                i++;
                continue;
            }
            else if (!numhaddot && m_contents[i] == '.')
            {
                i++;
                numhaddot = true;
                continue;
            }
            else if (numhadexp == 0 && (m_contents[i] == 'e' || m_contents[i] == 'E'))
            {
                i++;
                numhadexp++;
                continue;
            }
            else if (numhadexp == 1 && (m_contents[i] == '+' || m_contents[i] == '-'))
            {
                i++;
                numhadexp++;
                continue;
            }
            else
            {
                break;
            }

        }
        auto ident = std::string(m_contents.substr(m_info.offset, i));
        thisnode.content = ident;
        thisnode.offset = m_info.offset;
        thisnode.length = i - m_info.offset;
        m_info.column += i - m_info.offset;
        m_info.offset = i;
    }
    if (thisnode.content.empty())
    {
        m_owner.log(err::EmptyNumber(m_info));
        errflag = true;
    }
    root.children.emplace_back(std::move(thisnode));
}
//VARIABLE = identifier;
bool ::sqf::parser::sqf::default::instance::VARIABLE_start(size_t curoff) { auto len = identifier(curoff); return len > 0 && !m_contains_binary(std::string(m_contents.substr(curoff, len)), 0); }
void ::sqf::parser::sqf::default::instance::VARIABLE(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.kind = nodetype::VARIABLE;
    thisnode.path = m_info.path;
    auto len = identifier(m_info.offset);
    auto ident = std::string(m_contents.substr(m_info.offset, len));
    thisnode.content = ident;
    thisnode.length = len;
    thisnode.offset = m_info.offset;
    thisnode.col = m_info.column;
    thisnode.line = m_info.line;

    m_info.offset += len;
    m_info.column += len;
    root.children.emplace_back(std::move(thisnode));
}
//STRING = '"' { any | "\"\"" } '"' | '\'' { any | "''" } '\'';
bool ::sqf::parser::sqf::default::instance::STRING_start(size_t curoff) { return m_contents[curoff] == '\'' || m_contents[curoff] == '"'; }
void ::sqf::parser::sqf::default::instance::STRING(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.kind = nodetype::STRING;
    thisnode.col = m_info.column;
    thisnode.line = m_info.line;
    thisnode.path = m_info.path;
    size_t i;
    auto startchr = m_contents[m_info.offset];
    m_info.column++;
    for (i = m_info.offset + 1; (m_contents[i] != startchr || m_contents[i + 1] == startchr) && m_contents[i] != '\0'; i++)
    {
        if (m_contents[i] == startchr)
        {
            m_info.column += 2;
            i++;
        }
        else if (m_contents[i] == '\n')
        {
            m_info.column = 0;
            m_info.line++;
        }
        else
        {
            m_info.column++;
        }
    }
    if (m_contents[i] != startchr)
    {
        m_owner.log(err::MissingStringTermination(m_info));
    }
    i++;
    m_info.column++;
    auto fullstring = std::string(m_contents.substr(m_info.offset, i - m_info.offset));
    thisnode.content = fullstring;
    thisnode.length = i - m_info.offset;
    thisnode.offset = m_info.offset;
    m_info.offset = i;
    root.children.emplace_back(std::move(thisnode));
}
//CODE = "{" SQF "}";
bool ::sqf::parser::sqf::default::instance::CODE_start(size_t curoff) { return m_contents[curoff] == '{'; }
void ::sqf::parser::sqf::default::instance::CODE(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.kind = nodetype::CODE;
    thisnode.offset = m_info.offset;
    thisnode.col = m_info.column;
    thisnode.line = m_info.line;
    thisnode.path = m_info.path;
    auto copy = m_info;
    m_info.offset++;
    m_info.column++;
    skip(m_info);

    if (SQF_start(m_info.offset))
    {
        SQF(thisnode, errflag);
    }
    else
    {
        m_owner.log(err::ExpectedSQF(m_info));
        errflag = true;
    }

    if (m_contents[m_info.offset] == '}')
    {
        m_info.offset++;
        m_info.column++;
    }
    else
    {
        m_owner.log(err::MissingCurlyClosingBracket(copy));
        errflag = true;
    }
    thisnode.length = m_info.offset - thisnode.offset;
    root.children.emplace_back(std::move(thisnode));
}
//ARRAY = '[' [ BINARYEXPRESSION { ',' BINARYEXPRESSION } ] ']';
bool ::sqf::parser::sqf::default::instance::ARRAY_start(size_t curoff) { return m_contents[curoff] == '['; }
void ::sqf::parser::sqf::default::instance::ARRAY(astnode& root, bool& errflag)
{
    auto thisnode = astnode();
    thisnode.kind = nodetype::ARRAY;
    thisnode.offset = m_info.offset;
    thisnode.col = m_info.column;
    thisnode.line = m_info.line;
    thisnode.path = m_info.path;
    m_info.offset++;
    m_info.column++;
    skip(m_info);
    if (BINARYEXPRESSION_start(m_info.offset))
    {
        BINARYEXPRESSION(thisnode, errflag);
        skip(m_info);
        while (m_contents[m_info.offset] == ',')
        {
            m_info.column++;
            m_info.offset++;
            skip(m_info);

            if (BINARYEXPRESSION_start(m_info.offset))
            {
                BINARYEXPRESSION(thisnode, errflag);
                skip(m_info);
            }
            else
            {
                m_owner.log(err::ExpectedBinaryExpression(m_info));
                errflag = true;
            }
        }
    }
    if (m_contents[m_info.offset] == ']')
    {
        m_info.offset++;
        m_info.column++;
    }
    else
    {
        m_owner.log(err::MissingSquareClosingBracket(m_info));
        errflag = true;
    }
    thisnode.length = m_info.offset - thisnode.offset;
    thisnode.content = std::string(m_contents.substr(thisnode.offset, thisnode.length));
    root.children.emplace_back(std::move(thisnode));
}

sqf::parser::sqf::default::astnode sqf::parser::sqf::default::instance::parse(bool& errflag)
{
    astnode node;
    node.kind = nodetype::SQF;
    node.offset = 0;
    node.content = m_contents.substr();
    node.path = m_file;
    SQF(node, errflag);
    node.length = m_info.offset;
    skip(m_info);
    if (!errflag && m_info.offset < m_contents.size()) {
        m_owner.log(err::EndOfFile(m_info));
        errflag = true;
    }
    return node;
}


bool sqf::parser::sqf::default::instance::to_assembly(::sqf::parser::sqf::default::astnode& node, std::vector<::sqf::runtime::instruction::sptr>& set)
{
    // execute_parsing_callbacks(full, node, evaction::enter);
    switch (node.kind)
    {
    case nodetype::BEXP1:
    case nodetype::BEXP2:
    case nodetype::BEXP3:
    case nodetype::BEXP4:
    case nodetype::BEXP5:
    case nodetype::BEXP6:
    case nodetype::BEXP7:
    case nodetype::BEXP8:
    case nodetype::BEXP9:
    case nodetype::BEXP10:
    case nodetype::BINARYEXPRESSION:
    {
        to_assembly(node.children[0], set);
        to_assembly(node.children[2], set);
        auto s = std::string(node.children[1].content);
        std::transform(s.begin(), s.end(), s.begin(), [](char& c) { return std::tolower(c); });
        auto inst = std::make_shared<::sqf::opcodes::call_binary>(s, (short)(((short)node.kind - (short)nodetype::BEXP1) + 1));
        inst->diag_info({ node.children[1].line, node.children[1].col, node.children[1].offset, node.children[1].path, create_code_segment(m_contents, node.children[1].offset, node.children[1].length) });
        set.push_back(inst);
    }
    break;
    case nodetype::UNARYEXPRESSION:
    {
        to_assembly(node.children[1], set);
        auto s = std::string(node.children[0].content);
        std::transform(s.begin(), s.end(), s.begin(), [](char& c) { return std::tolower(c); });
        auto inst = std::make_shared<::sqf::opcodes::call_unary>(s);
        inst->diag_info({ node.children[0].line, node.children[0].col, node.children[0].offset, node.children[0].path, create_code_segment(m_contents, node.children[0].offset, node.children[0].length) });
        set.push_back(inst);
    }
    break;
    case nodetype::NULAROP:
    {
        auto s = std::string(node.content);
        std::transform(s.begin(), s.end(), s.begin(), [](char& c) { return std::tolower(c); });
        auto inst = std::make_shared<::sqf::opcodes::call_nular>(s);
        inst->diag_info({ node.line, node.col, node.offset, node.path, create_code_segment(m_contents, node.offset, node.length) });
        set.push_back(inst);
    }
    break;
    case nodetype::HEXNUMBER:
    {
        try
        {
            auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>(std::stol(node.content, nullptr, 16))));
            inst->diag_info({ node.line, node.col, node.offset, node.path, create_code_segment(m_contents, node.offset, node.length) });
            set.push_back(inst);
        }
        catch (std::out_of_range&)
        {
            auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>(std::nanf(""))));
            inst->diag_info({ node.line, node.col, node.offset, node.path, create_code_segment(m_contents, node.offset, node.length) });
            m_owner.log(logmessage::assembly::NumberOutOfRange(inst->diag_info()));
            set.push_back(inst);
        }
    }
    break;
    case nodetype::NUMBER:
    {
        try
        {
            auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>(std::stod(node.content))));
            inst->diag_info({ node.line, node.col, node.offset, node.path, create_code_segment(m_contents, node.offset, node.length) });
            set.push_back(inst);
        }
        catch (std::out_of_range&)
        {
            auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_scalar>(std::nanf(""))));
            inst->diag_info({ node.line, node.col, node.offset, node.path, create_code_segment(m_contents, node.offset, node.length) });
            m_owner.log(logmessage::assembly::NumberOutOfRange(inst->diag_info()));
            set.push_back(inst);
        }
    }
    break;
    case nodetype::STRING:
    {
        auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_string>(::sqf::types::d_string::from_sqf(node.content))));
        inst->diag_info({ node.line, node.col, node.offset, node.path, create_code_segment(m_contents, node.offset, node.length) });
        set.push_back(inst);
    }
    break;
    case nodetype::CODE:
    {
        std::vector<::sqf::runtime::instruction::sptr> tmp_set;
        astnode previous_node;
        for (size_t i = 0; i < node.children.size(); i++)
        {
            if (i != 0)
            {
                auto inst = std::make_shared<::sqf::opcodes::end_statement>();
                inst->diag_info({ previous_node.line, previous_node.col + previous_node.length, previous_node.offset, previous_node.path, create_code_segment(m_contents, previous_node.offset, previous_node.length) });
                tmp_set.push_back(inst);
            }
            previous_node = node.children[i];
            to_assembly(previous_node, tmp_set);
        }
        auto inst_set = ::sqf::runtime::instruction_set(tmp_set);
        auto inst = std::make_shared<::sqf::opcodes::push>(::sqf::runtime::value(std::make_shared<::sqf::types::d_code>(inst_set)));
        inst->diag_info({ node.line, node.col, node.offset, node.path, create_code_segment(m_contents, node.offset, node.length) });
        set.push_back(inst);
    }
    break;
    case nodetype::ARRAY:
    {
        for (auto& subnode : node.children)
        {
            to_assembly(subnode, set);
        }
        auto inst = std::make_shared<::sqf::opcodes::make_array>(node.children.size());
        inst->diag_info({ node.line, node.col, node.offset, node.path, create_code_segment(m_contents, node.offset, node.length) });
        set.push_back(inst);
    }
    break;
    case nodetype::ASSIGNMENT:
    {
        to_assembly(node.children[1], set);
        auto inst = std::make_shared<::sqf::opcodes::assign_to>(node.children[0].content);
        inst->diag_info({ node.line, node.col, node.offset, node.path, create_code_segment(m_contents, node.offset, node.length) });
        set.push_back(inst);
    }
    break;
    case nodetype::ASSIGNMENTLOCAL:
    {
        to_assembly(node.children[1], set);
        auto inst = std::make_shared<::sqf::opcodes::assign_to_local>(node.children[0].content);
        inst->diag_info({ node.line, node.col, node.offset, node.path, create_code_segment(m_contents, node.offset, node.length) });
        set.push_back(inst);
    }
    break;
    case nodetype::VARIABLE:
    {
        auto inst = std::make_shared<::sqf::opcodes::get_variable>(node.content);
        inst->diag_info({ node.line, node.col, node.offset, node.path, create_code_segment(m_contents, node.offset, node.length) });
        set.push_back(inst);
    }
    break;
    default:
    {
        astnode previous_node;
        for (size_t i = 0; i < node.children.size(); i++)
        {
            if (i != 0)
            {
                auto inst = std::make_shared<::sqf::opcodes::end_statement>();
                inst->diag_info({ previous_node.line, previous_node.col + previous_node.length, previous_node.offset, previous_node.path, create_code_segment(m_contents, previous_node.offset, previous_node.length) });
                set.push_back(inst);
            }
            previous_node = node.children[i];
            to_assembly(previous_node, set);
        }
    }
    }
    // execute_parsing_callbacks(full, node, evaction::exit);

    return true;
}