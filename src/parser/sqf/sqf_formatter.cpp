#include "sqf_formatter.h"


::sqf::parser::sqf::formatter::formatter(::sqf::runtime::runtime& r, std::string c, ::sqf::runtime::fileio::pathinfo f) : runtime{ r }, contents{ c }, file{ f } {
    tokenizer t(contents.begin(), contents.end(), file.physical);
    ::sqf::parser::sqf::parser& dyn_cast = dynamic_cast<::sqf::parser::sqf::parser&>(this->runtime.parser_sqf());
    ::sqf::parser::sqf::bison::parser p(t, this->res, dyn_cast, this->runtime);
    this->hasError = p.parse() != 0;
}


void ::sqf::parser::sqf::formatter::formatter::prettify(const ::sqf::parser::sqf::bison::astnode& node, size_t depth, std::ostringstream& buff) {
    switch (node.kind) {
    case bison::astkind::EXP0:
    case bison::astkind::EXP1:
    case bison::astkind::EXP2:
    case bison::astkind::EXP3:
    case bison::astkind::EXP4:
    case bison::astkind::EXP5:
    case bison::astkind::EXP6:
    case bison::astkind::EXP7:
    case bison::astkind::EXP8:
    case bison::astkind::EXP9:
    {
        this->prettify(node.children[0], depth, buff);
        buff << " ";
        auto s = std::string(node.token.contents);
        std::transform(s.begin(), s.end(), s.begin(), [](char& c) { return (char)std::tolower((int)c); });
        buff << s;
        buff << " ";
        this->prettify(node.children[1], depth, buff);
    }
    break;
    case bison::astkind::EXPU:
    {
        auto s = std::string(node.token.contents);
        std::transform(s.begin(), s.end(), s.begin(), [](char& c) { return (char)std::tolower((int)c); });
        buff << s;
        buff << " ";

        if (s == "if" && node.children[0].token.contents != "!")
            buff << "(";
        else if (s == "!")
            buff << "(";

        this->prettify(node.children[0], depth, buff);

        if (s == "if" && node.children[0].token.contents != "!")
            buff << ")";
        else if (s == "!")
            buff << ")";
    }
    break;
    case bison::astkind::HEXNUMBER:
    {
        auto str = std::string(node.token.contents);
        if (str[0] == '$') { str = std::string("0x").append(str.substr(1)); }
        buff << str;
    }
    break;
    case bison::astkind::EXPN:
    case bison::astkind::IDENT:
    case bison::astkind::NUMBER:
    case bison::astkind::STRING:
    case bison::astkind::BOOLEAN_TRUE:
    case bison::astkind::BOOLEAN_FALSE:
    {
        buff << node.token.contents;
    }
    break;
    case bison::astkind::CODE:
    {
        buff << "{";

        if (!node.children.empty()) {
            buff << "\n";
            depth++;
        }

        for (size_t i = 0; i < node.children.size(); i++)
            this->prettify(node.children[i], depth, buff);

        if (!node.children.empty())
            depth--;

        buff << std::string(depth * 4, ' ');
        buff << "}";
    }
    break;
    case bison::astkind::ARRAY:
    {
        buff << "[";
        bool flag = false;
        for (auto& subnode : node.children) {
            if (flag) {
                buff << ", ";
            }
            else {
                flag = true;
            }
            this->prettify(subnode, depth, buff);
        }
        buff << "]";
    }
    break;
    case bison::astkind::ASSIGNMENT:
    {
        if (node.children[0].children.empty() && node.children[0].token.type == tokenizer::etoken::t_ident) {
            buff << node.children[0].token.contents;
            buff << " = ";
        }
        this->prettify(node.children[1], depth, buff);
    }
    break;
    case bison::astkind::ASSIGNMENT_LOCAL:
    {
        buff << "private ";
        buff << node.token.contents;
        buff << " = ";
        this->prettify(node.children[0], depth, buff);
    }
    break;
    default:
    {
        for (size_t i = 0; i < node.children.size(); i++) {
            if (node.kind != bison::astkind::NA)
                buff << std::string(depth * 4, ' ');
            this->prettify(node.children[i], depth, buff);
            if (node.kind != bison::astkind::NA)
                buff << ";\n";
        }
    }
    }
}
