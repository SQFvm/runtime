#pragma once

namespace sqf::parser::assembly::bison
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
        ASSIGN_TO,
        ASSIGN_TO_LOCAL,
        GET_VARIABLE,
        CALL_UNARY,
        CALL_NULAR,
        CALL_BINARY,
        PUSH,
        END_STATEMENT,
    };
    struct astnode
    {
        ::sqf::parser::assembly::tokenizer::token token;
        astkind kind;
        std::vector<astnode> children;

        astnode() : token(), kind(astkind::NA) { }
        astnode(astkind kind) : token(), kind(kind) { }
        astnode(::sqf::parser::assembly::tokenizer::token t) : token(t), kind(astkind::__TOKEN) { }
        astnode(astkind kind, ::sqf::parser::assembly::tokenizer::token t) : token(t), kind(kind) { }

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