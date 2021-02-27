#pragma once

namespace sqf::parser::sqf::bison
{
    enum class astkind
    {
        ENDOFFILE = -3,
        INVALID = -2,
        __TOKEN = -1,
        NA = 0,
        STATEMENTS,
        STATEMENT,
        IDENT,
        NUMBER,
        HEXNUMBER,
        STRING,
        BOOLEAN_TRUE,
        BOOLEAN_FALSE,
        EXPRESSION_LIST,
        CODE,
        ARRAY,
        ASSIGNMENT,
        ASSIGNMENT_LOCAL,
        EXPN,
        EXP0,
        EXP1,
        EXP2,
        EXP3,
        EXP4,
        EXP5,
        EXP6,
        EXP7,
        EXP8,
        EXP9,
        EXPU
    };
    struct astnode
    {
        ::sqf::parser::sqf::tokenizer::token token;
        astkind kind;
        std::vector<astnode> children;

        astnode() : token(), kind(astkind::NA) { }
        astnode(astkind kind) : token(), kind(kind) { }
        astnode(::sqf::parser::sqf::tokenizer::token t) : token(t), kind(astkind::__TOKEN) { }
        astnode(astkind kind, ::sqf::parser::sqf::tokenizer::token t) : token(t), kind(kind) { }

        void append(astnode node)
        {
            children.push_back(node);
        }
        void append_children(const astnode& other)
        { 
            for (auto node : other.children)
            {
                append(node); 
            } 
        }
    };
}