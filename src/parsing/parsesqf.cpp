#include "astnode.h"
#include "compiletime.h"
#include "helper.h"
#include "parsesqf.h"
#include "string_op.h"
#include <algorithm>
#include <cwctype>
#include <utility>
#include <vector>
#include <string>
#include <sstream>

namespace err = logmessage::sqf;

void sqf::parse::sqf::skip(size_t &curoff)
{
	while (true)
	{
		switch (m_contents[curoff])
		{
			case ' ': curoff++; continue;
			case '\t': curoff++; continue;
			case '\r': curoff++; continue;
			case '\n': curoff++; continue;
			default: return;
		}
	}
}
void sqf::parse::sqf::skip(position_info& info)
{
	while (true)
	{
		switch (m_contents[info.offset])
		{
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
				auto str = std::string(m_contents.substr(start, info.offset - start));
				info.line = static_cast<size_t>(std::stoul(str));

				for (; m_contents[info.offset] != '\0' && m_contents[info.offset] != '\n' && m_contents[info.offset] == ' '; info.offset++);
				if (m_contents[info.offset] != '\0' && m_contents[info.offset] != '\n')
				{
					start = info.offset;
					for (; m_contents[info.offset] != '\0' && m_contents[info.offset] != '\n'; info.offset++);
					auto str = std::string(m_contents.substr(start, info.offset - start));
					info.file = str;
				}
				break;
			}
		default: return;
		}
	}
}

//endchr = [,;];
size_t sqf::parse::sqf::endchr(size_t off) { return m_contents[off] == ';' || m_contents[off] == ',' ? 1 : 0; }
//identifier = [_a-zA-Z][_a-zA-Z0-9]*;
size_t sqf::parse::sqf::identifier(size_t off) { size_t i = off; if (!((m_contents[i] >= 'a' && m_contents[i] <= 'z') || (m_contents[i] >= 'A' && m_contents[i] <= 'Z') || m_contents[i] == '_')) return 0; for (i = off + 1; (m_contents[i] >= 'a' && m_contents[i] <= 'z') || (m_contents[i] >= 'A' && m_contents[i] <= 'Z') || (m_contents[i] >= '0' && m_contents[i] <= '9') || m_contents[i] == '_'; i++) {}; return i - off; }
//identifier = [_a-zA-Z][_a-zA-Z0-9]+;
size_t sqf::parse::sqf::assidentifier(size_t off)
{
	size_t i = off;
	if (!((m_contents[i] >= 'a' && m_contents[i] <= 'z') || (m_contents[i] >= 'A' && m_contents[i] <= 'Z') || m_contents[i] == '_'))
	{
		return 0;
	}

	for (i++; (m_contents[i] >= 'a' && m_contents[i] <= 'z') || (m_contents[i] >= 'A' && m_contents[i] <= 'Z') || (m_contents[i] >= '0' && m_contents[i] <= '9') || m_contents[i] == '_'; i++);
	return i - off;
}
//operator_ = [+-*/%^]|&&|\|\||==|[!<>][=]?|[a-zA-Z_]+;
size_t sqf::parse::sqf::operator_(size_t off) {
	if (m_contents[off] == '+' || m_contents[off] == '-' || m_contents[off] == '*' || m_contents[off] == '/' || m_contents[off] == '%' || m_contents[off] == '^' || m_contents[off] == ':' || m_contents[off] == '#') return 1;
	if ((m_contents[off] == '|' && m_contents[off + 1] == '|') || (m_contents[off] == '&' && m_contents[off + 1] == '&') || (m_contents[off] == '=' && m_contents[off + 1] == '=') || (m_contents[off] == '>' && m_contents[off + 1] == '>')) return 2;
	if (m_contents[off] == '<' || m_contents[off] == '>' || m_contents[off] == '!')
	{
		if (m_contents[off + 1] == '=') return 2;
		return 1;
	}
	size_t i;
	for (i = off; (m_contents[i] >= 'a' && m_contents[i] <= 'z') || (m_contents[i] >= 'A' && m_contents[i] <= 'Z') || (m_contents[i] >= '0' && m_contents[i] <= '9') || m_contents[i] == '_'; i++);
	return i - off;
}
//hexadecimal = [0-9a-fA-F]+;
size_t sqf::parse::sqf::hexadecimal(size_t off) { size_t i; for (i = off; (m_contents[i] >= 'a' && m_contents[i] <= 'f') || (m_contents[i] >= 'A' && m_contents[i] <= 'F') || (m_contents[i] >= '0' && m_contents[i] <= '9'); i++) {}; return i - off; }
//scalarsub = [0-9]+;
size_t sqf::parse::sqf::scalarsub(size_t off) { size_t i; for (i = off; m_contents[i] >= '0' && m_contents[i] <= '9'; i++) {}; return i - off; }
//scalar = scalarsub(.scalarsub)?;
size_t sqf::parse::sqf::scalar(size_t off) { size_t i = off + scalarsub(off); if (m_contents[off] == '.') i += scalarsub(off); return i - off; }
//anytext = (?![ \t\r\n;])+;
size_t sqf::parse::sqf::anytext(size_t off) { size_t i; for (i = off; m_contents[i] != ' ' && m_contents[i] != '\t' && m_contents[i] != '\r' && m_contents[i] != '\n' && m_contents[i] != ';'; i++) {}; return i - off; }
//SQF = [ STATEMENT { endchr { endchr } STATEMENT } ]
bool sqf::parse::sqf::SQF_start(size_t curoff) { return true; }
void sqf::parse::sqf::SQF(astnode &root, position_info& info, bool &errflag)
{
	skip(info);
	size_t endchrlen;
	while ((endchrlen = endchr(info.offset)) > 0)
	{
		info.offset += endchrlen;
		info.column += endchrlen;
		skip(info);
	}
	//Iterate over statements as long as it is an instruction start.
	while (STATEMENT_start(info.offset))
	{
		STATEMENT(root, info, errflag);
		skip(info);
		//Make sure at least one endchr is available unless no statement follows
		if (!endchr(info.offset) && STATEMENT_start(info.offset))
		{
			log(err::ExpectedStatementTerminator(info));
			errflag = true;
		}
		else
		{
			//Add endchr up until no semicolon is left
			while ((endchrlen = endchr(info.offset)) > 0)
			{
				info.offset += endchrlen;
				info.column += endchrlen;
				skip(info);
			}
		}
	}
}
//STATEMENT = ASSIGNMENT | BINARYEXPRESSION;
bool sqf::parse::sqf::STATEMENT_start(size_t curoff) { return ASSIGNMENT_start(curoff) | BINARYEXPRESSION_start(curoff); }
void sqf::parse::sqf::STATEMENT(astnode &root, position_info& info, bool &errflag)
{
	if (ASSIGNMENT_start(info.offset))
	{
		ASSIGNMENT(root, info, errflag);
	}
	else if (BINARYEXPRESSION_start(info.offset))
	{
		BINARYEXPRESSION(root, info, errflag);
	}
	else
	{
		log(err::NoViableAlternativeStatement(info));
		errflag = true;
	}
	//thisnode.length = curoff - thisnode.offset;
	//root.children.push_back(thisnode);
}
//ASSIGNMENT(2) = assidentifier '=' BINARYEXPRESSION | "private" assidentifier '=' BINARYEXPRESSION;
bool sqf::parse::sqf::ASSIGNMENT_start(size_t curoff)
{
	size_t len;
	if (str_cmpi(m_contents.data() + curoff, compiletime::strlen("private"), "private", compiletime::strlen("private")) == 0)
	{
		curoff += compiletime::strlen("private");
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
void sqf::parse::sqf::ASSIGNMENT(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.kind = (short)asttype::sqf::ASSIGNMENT;
	thisnode.offset = info.offset;
	thisnode.file = info.file;
	size_t len;
	bool assignlocal = false;
	//check if prefixed by a 'private'
	if (str_cmpi(m_contents.data() + info.offset, compiletime::strlen("private"), "private", compiletime::strlen("private")) == 0)
	{
		info.offset += compiletime::strlen("private");
		info.column += compiletime::strlen("private");
		skip(info);
		assignlocal = true;
		thisnode.kind = (short)asttype::sqf::ASSIGNMENTLOCAL;
	}
	//receive the ident
	len = assidentifier(info.offset);
	auto ident = std::string(m_contents.substr(info.offset, len));

	auto varnode = astnode();
	varnode.offset = info.offset;
	varnode.length = len;
	varnode.content = ident;
	varnode.kind = (short)asttype::sqf::VARIABLE;
	varnode.col = info.column;
	varnode.line = info.line;
	varnode.file = info.file;
	thisnode.children.emplace_back(std::move(varnode));

	if (assignlocal && ident[0] != '_')
	{
		log(err::MissingUnderscoreOnPrivateVariable(info, ident));
	}
	info.offset += len;
	skip(info);
	thisnode.col = info.column;
	thisnode.line = info.line;
	//skip the '=' (is confirmed to be present in ASSIGNMENT_start)
	info.offset++;
	info.column++;
	skip(info);

	if (BINARYEXPRESSION_start(info.offset))
	{
		BINARYEXPRESSION(thisnode, info, errflag);
	}
	else
	{
		log(err::ExpectedBinaryExpression(info));
		errflag = true;
	}
	thisnode.length = info.offset - thisnode.offset;
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
void sqf::parse::sqf::bexp_orderfix(astnode& root, astnode thisnode, short plevel)
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
bool sqf::parse::sqf::bexp10_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void sqf::parse::sqf::bexp10(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.offset = info.offset;
	thisnode.kind = (short)asttype::sqf::BEXP10;
	thisnode.file = info.file;
    thisnode.line = info.line;
	size_t oplen;
	std::string op;
	skip(info);
	if (PRIMARYEXPRESSION_start(info.offset))
	{
		PRIMARYEXPRESSION(thisnode, info, errflag);
	}

	else
	{
		log(err::ExpectedBinaryExpression(info));
		errflag = true;
	}
	skip(info);
	if ((oplen = operator_(info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(info.offset, oplen)), 10))
	{
		auto node = astnode();
		node.content = op;
		node.offset = info.offset;
		node.kind = (short)asttype::sqf::BINARYOP;
		node.col = info.column;
		node.line = info.line;
		node.length = oplen;
		node.file = info.file;
        thisnode.children.emplace_back(std::move(node));
		info.offset += oplen;
		skip(info);
		if (bexp10_start(info.offset))
		{
			bexp10(thisnode, info, errflag);
		}
		else
		{
			log(err::MissingRightArgument(info, op));
			errflag = true;
		}
		bexp_orderfix(root, std::move(thisnode), (short)asttype::sqf::BEXP10);
	}
	else
	{
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
	}
}
bool sqf::parse::sqf::bexp9_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void sqf::parse::sqf::bexp9(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.offset = info.offset;
	thisnode.kind = (short)asttype::sqf::BEXP9;
	thisnode.file = info.file;
    thisnode.line = info.line;
	size_t oplen;
	std::string op;
	skip(info);
	if (bexp10_start(info.offset))
	{
		bexp10(thisnode, info, errflag);
	}
	else
	{
		log(err::ExpectedBinaryExpression(info));
		errflag = true;
	}
	skip(info);
	if ((oplen = operator_(info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(info.offset, oplen)), 9))
	{
		auto node = astnode();
		node.content = op;
		node.offset = info.offset;
		node.kind = (short)asttype::sqf::BINARYOP;
		node.col = info.column;
		node.line = info.line;
		node.length = oplen;
		node.file = info.file;
		thisnode.children.emplace_back(std::move(node));
		info.offset += oplen;
		skip(info);
		if (bexp9_start(info.offset))
		{
			bexp9(thisnode, info, errflag);
		}
		else
		{
			log(err::MissingRightArgument(info, op));
			errflag = true;
		}
		bexp_orderfix(root, std::move(thisnode), (short)asttype::sqf::BEXP9);
	}
	else
	{
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
	}
}
bool sqf::parse::sqf::bexp8_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void sqf::parse::sqf::bexp8(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.offset = info.offset;
	thisnode.kind = (short)asttype::sqf::BEXP8;
	thisnode.file = info.file;
    thisnode.line = info.line;
	size_t oplen;
	std::string op;
	skip(info);
	if (bexp9_start(info.offset))
	{
		bexp9(thisnode, info, errflag);
	}
	else
	{
		log(err::ExpectedBinaryExpression(info));
		errflag = true;
	}
	skip(info);
	if ((oplen = operator_(info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(info.offset, oplen)), 8))
	{
		auto node = astnode();
		node.content = op;
		node.offset = info.offset;
		node.kind = (short)asttype::sqf::BINARYOP;
		node.col = info.column;
		node.line = info.line;
		node.length = oplen;
		node.file = info.file;
		thisnode.children.emplace_back(std::move(node));
		info.offset += oplen;
		skip(info);
		if (bexp8_start(info.offset))
		{
			bexp8(thisnode, info, errflag);
		}
		else
		{
			log(err::MissingRightArgument(info, op));
			errflag = true;
		}
		bexp_orderfix(root, std::move(thisnode), (short)asttype::sqf::BEXP8);
	}
	else
	{
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
	}
}
bool sqf::parse::sqf::bexp7_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void sqf::parse::sqf::bexp7(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.offset = info.offset;
	thisnode.kind = (short)asttype::sqf::BEXP7;
	thisnode.file = info.file;
    thisnode.line = info.line;
	size_t oplen;
	std::string op;
	skip(info);
	if (bexp8_start(info.offset))
	{
		bexp8(thisnode, info, errflag);
	}
	else
	{
		log(err::ExpectedBinaryExpression(info));
		errflag = true;
	}
	skip(info);
	if ((oplen = operator_(info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(info.offset, oplen)), 7))
	{
		auto node = astnode();
		node.content = op;
		node.offset = info.offset;
		node.kind = (short)asttype::sqf::BINARYOP;
		node.col = info.column;
		node.line = info.line;
		node.length = oplen;
		node.file = info.file;
		thisnode.children.emplace_back(std::move(node));
		info.offset += oplen;
		skip(info);
		if (bexp7_start(info.offset))
		{
			bexp7(thisnode, info, errflag);
		}
		else
		{
			log(err::MissingRightArgument(info, op));
			errflag = true;
		}
		bexp_orderfix(root, std::move(thisnode), (short)asttype::sqf::BEXP7);
	}
	else
	{
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
	}
}
bool sqf::parse::sqf::bexp6_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void sqf::parse::sqf::bexp6(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.offset = info.offset;
	thisnode.kind = (short)asttype::sqf::BEXP6;
	thisnode.file = info.file;
    thisnode.line = info.line;
	size_t oplen;
	std::string op;
	skip(info);
	if (bexp7_start(info.offset))
	{
		bexp7(thisnode, info, errflag);
	}
	else
	{
		log(err::ExpectedBinaryExpression(info));
		errflag = true;
	}
	skip(info);
	if ((oplen = operator_(info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(info.offset, oplen)), 6))
	{
		auto node = astnode();
		node.content = op;
		node.offset = info.offset;
		node.kind = (short)asttype::sqf::BINARYOP;
		node.col = info.column;
		node.line = info.line;
		node.length = oplen;
		node.file = info.file;
		thisnode.children.emplace_back(std::move(node));
		info.offset += oplen;
		skip(info);
		if (bexp6_start(info.offset))
		{
			bexp6(thisnode, info, errflag);
		}
		else
		{
			log(err::MissingRightArgument(info, op));
			errflag = true;
		}
		bexp_orderfix(root, std::move(thisnode), (short)asttype::sqf::BEXP6);
	}
	else
	{
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
	}
}
bool sqf::parse::sqf::bexp5_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void sqf::parse::sqf::bexp5(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.offset = info.offset;
	thisnode.kind = (short)asttype::sqf::BEXP5;
	thisnode.file = info.file;
    thisnode.line = info.line;
	size_t oplen;
	std::string op;
	skip(info);
	if (bexp6_start(info.offset))
	{
		bexp6(thisnode, info, errflag);
	}
	else
	{
		log(err::ExpectedBinaryExpression(info));
		errflag = true;
	}
	skip(info);
	if ((oplen = operator_(info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(info.offset, oplen)), 5))
	{
		auto node = astnode();
		node.content = op;
		node.offset = info.offset;
		node.kind = (short)asttype::sqf::BINARYOP;
		node.col = info.column;
		node.line = info.line;
		node.length = oplen;
		node.file = info.file;
		thisnode.children.emplace_back(std::move(node));
		info.offset += oplen;
		skip(info);
		if (bexp5_start(info.offset))
		{
			bexp5(thisnode, info, errflag);
		}
		else
		{
			log(err::MissingRightArgument(info, op));
			errflag = true;
		}
		bexp_orderfix(root, std::move(thisnode), (short)asttype::sqf::BEXP5);
	}
	else
	{
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
	}
}
bool sqf::parse::sqf::bexp4_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void sqf::parse::sqf::bexp4(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.offset = info.offset;
	thisnode.kind = (short)asttype::sqf::BEXP4;
	thisnode.file = info.file;
    thisnode.line = info.line;
	size_t oplen;
	std::string op;
	skip(info);
	if (bexp5_start(info.offset))
	{
		bexp5(thisnode, info, errflag);
	}
	else
	{
		log(err::ExpectedBinaryExpression(info));
		errflag = true;
	}
	skip(info);
	if ((oplen = operator_(info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(info.offset, oplen)), 4))
	{
		auto node = astnode();
		node.content = op;
		node.offset = info.offset;
		node.kind = (short)asttype::sqf::BINARYOP;
		node.col = info.column;
		node.line = info.line;
		node.length = oplen;
		node.file = info.file;
		thisnode.children.emplace_back(std::move(node));
		info.offset += oplen;
		skip(info);
		if (bexp4_start(info.offset))
		{
			bexp4(thisnode, info, errflag);
		}
		else
		{
			log(err::MissingRightArgument(info, op));
			errflag = true;
		}
		bexp_orderfix(root, std::move(thisnode), (short)asttype::sqf::BEXP4);
	}
	else
	{
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
	}
}
bool sqf::parse::sqf::bexp3_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void sqf::parse::sqf::bexp3(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.offset = info.offset;
	thisnode.kind = (short)asttype::sqf::BEXP3;
	thisnode.file = info.file;
    thisnode.line = info.line;
	size_t oplen;
	std::string op;
	skip(info);
	if (bexp4_start(info.offset))
	{
		bexp4(thisnode, info, errflag);
	}
	else
	{
		log(err::ExpectedBinaryExpression(info));
		errflag = true;
	}
	skip(info);
	if ((oplen = operator_(info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(info.offset, oplen)), 3))
	{
		auto node = astnode();
		node.content = op;
		node.offset = info.offset;
		node.kind = (short)asttype::sqf::BINARYOP;
		node.col = info.column;
		node.line = info.line;
		node.length = oplen;
		node.file = info.file;
		thisnode.children.emplace_back(std::move(node));
		info.offset += oplen;
		skip(info);
		if (bexp3_start(info.offset))
		{
			bexp3(thisnode, info, errflag);
		}
		else
		{
			log(err::MissingRightArgument(info, op));
			errflag = true;
		}
		bexp_orderfix(root, std::move(thisnode), (short)asttype::sqf::BEXP3);
	}
	else
	{
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
	}
}
bool sqf::parse::sqf::bexp2_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void sqf::parse::sqf::bexp2(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.offset = info.offset;
	thisnode.kind = (short)asttype::sqf::BEXP2;
    thisnode.file = info.file;
    thisnode.line = info.line;
	size_t oplen;
	std::string op;
	skip(info);
	if (bexp3_start(info.offset))
	{
		bexp3(thisnode, info, errflag);
	}
	else
	{
		log(err::ExpectedBinaryExpression(info));
		errflag = true;
	}
	skip(info);
	if ((oplen = operator_(info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(info.offset, oplen)), 2))
	{
		auto node = astnode();
		node.content = op;
		node.offset = info.offset;
		node.kind = (short)asttype::sqf::BINARYOP;
		node.col = info.column;
		node.line = info.line;
		node.length = oplen;
		node.file = info.file;
		thisnode.children.emplace_back(std::move(node));
		info.offset += oplen;
		skip(info);
		if (bexp2_start(info.offset))
		{
			bexp2(thisnode, info, errflag);
		}
		else
		{
			log(err::MissingRightArgument(info, op));
			errflag = true;
		}
		bexp_orderfix(root, std::move(thisnode), (short)asttype::sqf::BEXP2);
	}
	else
	{
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
	}
}
bool sqf::parse::sqf::bexp1_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void sqf::parse::sqf::bexp1(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.offset = info.offset;
	thisnode.kind = (short)asttype::sqf::BEXP1;
	thisnode.file = info.file;
    thisnode.line = info.line;
	size_t oplen;
	std::string op;
	skip(info);
	if (bexp2_start(info.offset))
	{
		bexp2(thisnode, info, errflag);
	}
	else
	{
		log(err::ExpectedBinaryExpression(info));
		errflag = true;
	}
	skip(info);
	if ((oplen = operator_(info.offset)) > 0 && m_contains_binary(op = std::string(m_contents.substr(info.offset, oplen)), 1))
	{
		auto node = astnode();
		node.content = op;
		node.offset = info.offset;
		node.kind = (short)asttype::sqf::BINARYOP;
		node.col = info.column;
		node.line = info.line;
		node.length = oplen;
		node.file = info.file;
		thisnode.children.emplace_back(std::move(node));
		info.offset += oplen;
		skip(info);
		if (bexp1_start(info.offset))
		{
			bexp1(thisnode, info, errflag);
		}
		else
		{
			log(err::MissingRightArgument(info, op));
			errflag = true;
		}
		bexp_orderfix(root, std::move(thisnode), (short)asttype::sqf::BEXP1);
	}
	else
	{
        //We won't need this node anymore. Just move all children to root
        root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
	}
}
bool sqf::parse::sqf::BINARYEXPRESSION_start(size_t curoff) { return PRIMARYEXPRESSION_start(curoff); }
void sqf::parse::sqf::BINARYEXPRESSION(astnode &root, position_info& info, bool &errflag)
{
	bexp1(root, info, errflag);
}
//BRACKETS = '(' BINARYEXPRESSION ')';
bool sqf::parse::sqf::BRACKETS_start(size_t curoff) { return m_contents[curoff] == '('; }
void sqf::parse::sqf::BRACKETS(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.kind = (short)asttype::sqf::BRACKETS;
	thisnode.offset = info.offset;
	thisnode.file = info.file;
	info.offset++;
	info.column++;
	skip(info);
	if (BINARYEXPRESSION_start(info.offset))
	{
		BINARYEXPRESSION(thisnode, info, errflag);
	}
	else
	{
		log(err::ExpectedBinaryExpression(info));
		errflag = true;
	}
	skip(info);
	if (m_contents[info.offset] == ')')
	{
		info.offset++;
		info.column++;
	}
	else
	{
		log(err::MissingRoundClosingBracket(info));
		errflag = true;
	}
	thisnode.length = info.offset - thisnode.offset;
	root.children.emplace_back(std::move(thisnode));
}
//PRIMARYEXPRESSION = NUMBER | UNARYEXPRESSION | NULAREXPRESSION | VARIABLE | STRING | CODE | BRACKETS | ARRAY;
bool sqf::parse::sqf::PRIMARYEXPRESSION_start(size_t curoff) { return NUMBER_start(curoff) || UNARYEXPRESSION_start(curoff) || NULAREXPRESSION_start(curoff) || VARIABLE_start(curoff) || STRING_start(curoff) || CODE_start(curoff) || BRACKETS_start(curoff) || ARRAY_start(curoff); }
void sqf::parse::sqf::PRIMARYEXPRESSION(astnode &root, position_info& info, bool &errflag)
{
	if (NUMBER_start(info.offset))
	{
		NUMBER(root, info, errflag);
	}
	else if (UNARYEXPRESSION_start(info.offset))
	{
		UNARYEXPRESSION(root, info, errflag);
	}
	else if (NULAREXPRESSION_start(info.offset))
	{
		NULAREXPRESSION(root, info, errflag);
	}
	else if (VARIABLE_start(info.offset))
	{
		VARIABLE(root, info, errflag);
	}
	else if (STRING_start(info.offset))
	{
		STRING(root, info, errflag);
	}
	else if (CODE_start(info.offset))
	{
		CODE(root, info, errflag);
	}
	else if (BRACKETS_start(info.offset))
	{
		BRACKETS(root, info, errflag);
	}
	else if (ARRAY_start(info.offset))
	{
		ARRAY(root, info, errflag);
	}
	else
	{
		log(err::NoViableAlternativePrimaryExpression(info));
		errflag = true;
	}
	//thisnode.length = curoff - thisnode.offset;
	//root.children.push_back(thisnode);
}
//NULAREXPRESSION = operator;
bool sqf::parse::sqf::NULAREXPRESSION_start(size_t curoff) { auto oplen = operator_(curoff); return oplen > 0 ? m_contains_nular(std::string(m_contents.substr(curoff, oplen))) : false; }
void sqf::parse::sqf::NULAREXPRESSION(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.kind = (short)asttype::sqf::NULAROP;
	thisnode.file = info.file;
	auto len = operator_(info.offset);
	auto ident = std::string(m_contents.substr(info.offset, len));
	thisnode.content = ident;
	thisnode.length = len;
	thisnode.offset = info.offset;
	thisnode.col = info.column;
	thisnode.line = info.line;
	info.offset += len;
	info.column += len;
	root.children.emplace_back(std::move(thisnode));
}
//UNARYEXPRESSION = operator PRIMARYEXPRESSION;
bool sqf::parse::sqf::UNARYEXPRESSION_start(size_t curoff) { auto oplen = operator_(curoff); return oplen > 0 ? m_contains_unary(std::string(m_contents.substr(curoff, oplen))) : false; }
void sqf::parse::sqf::UNARYEXPRESSION(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.kind = (short)asttype::sqf::UNARYEXPRESSION;
	thisnode.offset = info.offset;
	thisnode.file = info.file;
    thisnode.col = info.column;
    thisnode.line = info.line;

	auto len = operator_(info.offset);
	auto operatorname = std::string(m_contents.substr(info.offset, len));
	auto opnode = astnode();
	opnode.kind = (short)asttype::sqf::UNARYOP;
	opnode.offset = info.offset;
	opnode.length = len;
	opnode.content = operatorname;
	opnode.col = info.column;
	opnode.file = info.file;
	opnode.line = info.line;
	thisnode.children.emplace_back(std::move(opnode));
	info.offset += len;
	info.column += len;
	skip(info);

	if (PRIMARYEXPRESSION_start(info.offset))
	{
		PRIMARYEXPRESSION(thisnode, info, errflag);
	}
	else
	{
		log(err::MissingRightArgument(info, operatorname));
		errflag = true;
	}
	thisnode.length = info.offset - thisnode.offset;
	root.children.emplace_back(std::move(thisnode));
}
//NUMBER = ("0x" | '$' | '.') hexadecimal | scalar;
bool sqf::parse::sqf::NUMBER_start(size_t curoff)
{
	return  m_contents[curoff] == '$' ||
			(
				m_contents[curoff] == '.' &&
				m_contents[curoff + 1] >= '0' &&
				m_contents[curoff + 1] <= '9'
			) ||
			(m_contents[curoff] >= '0' && m_contents[curoff] <= '9'); }
void sqf::parse::sqf::NUMBER(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.kind = (short)asttype::sqf::NUMBER;
	thisnode.col = info.column;
	thisnode.line = info.line;
	thisnode.file = info.file;
	if (m_contents[info.offset] == '$')
	{
		thisnode.kind = (short)asttype::sqf::HEXNUMBER;
		size_t i;
		for (i = info.offset + 1; (m_contents[i] >= '0' && m_contents[i] <= '9') || (m_contents[i] >= 'A' && m_contents[i] <= 'F') || (m_contents[i] >= 'a' && m_contents[i] <= 'f'); i++);
		auto ident = std::string(m_contents.substr(info.offset + 1, i));
		thisnode.content = ident;
		thisnode.offset = info.offset;
		thisnode.length = i - info.offset;
		info.column += i - info.offset;
		info.offset = i;
	}
	else if (m_contents[info.offset] == '0' && m_contents[info.offset + 1] == 'x')
	{
		thisnode.kind = (short)asttype::sqf::HEXNUMBER;
		size_t i;
		for (i = info.offset + 2; (m_contents[i] >= '0' && m_contents[i] <= '9') || (m_contents[i] >= 'A' && m_contents[i] <= 'F') || (m_contents[i] >= 'a' && m_contents[i] <= 'f'); i++);
		auto ident = std::string(m_contents.substr(info.offset, i));
		thisnode.content = ident;
		thisnode.offset = info.offset;
		thisnode.length = i - info.offset;
		info.column += i - info.offset;
		info.offset = i;
	}
	else
	{
		size_t i = info.offset;
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
		auto ident = std::string(m_contents.substr(info.offset, i));
		thisnode.content = ident;
		thisnode.offset = info.offset;
		thisnode.length = i - info.offset;
		info.column += i - info.offset;
		info.offset = i;
	}
	if (thisnode.content.empty())
	{
		log(err::EmptyNumber(info));
		errflag = true;
	}
	root.children.emplace_back(std::move(thisnode));
}
//VARIABLE = identifier;
bool sqf::parse::sqf::VARIABLE_start(size_t curoff) { auto len = identifier(curoff); return len > 0 && !m_contains_binary(std::string(m_contents.substr(curoff, len)), 0); }
void sqf::parse::sqf::VARIABLE(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.kind = (short)asttype::sqf::VARIABLE;
	thisnode.file = info.file;
	auto len = identifier(info.offset);
	auto ident = std::string(m_contents.substr(info.offset, len));
	thisnode.content = ident;
	thisnode.length = len;
	thisnode.offset = info.offset;
	thisnode.col = info.column;
	thisnode.line = info.line;

	info.offset += len;
	info.column += len;
	root.children.emplace_back(std::move(thisnode));
}
//STRING = '"' { any | "\"\"" } '"' | '\'' { any | "''" } '\'';
bool sqf::parse::sqf::STRING_start(size_t curoff) { return m_contents[curoff] == '\'' || m_contents[curoff] == '"'; }
void sqf::parse::sqf::STRING(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.kind = (short)asttype::sqf::STRING;
	thisnode.col = info.column;
	thisnode.line = info.line;
	thisnode.file = info.file;
	size_t i;
	auto startchr = m_contents[info.offset];
	info.column++;
	for (i = info.offset + 1; m_contents[i] != '\0' && (m_contents[i] != startchr || m_contents[i + 1] == startchr); i++)
	{
		if (m_contents[i] == startchr)
		{
			info.column += 2;
			i++;
		}
		else if (m_contents[i] == '\n')
		{
			info.column = 0;
			info.line++;
		}
		else
		{
			info.column++;
		}
	}
	i++;
	info.column++;
	auto fullstring = std::string(m_contents.substr(info.offset, i));
	thisnode.content = fullstring;
	thisnode.length = i - info.offset;
	thisnode.offset = info.offset;
	info.offset = i;
	root.children.emplace_back(std::move(thisnode));
}
//CODE = "{" SQF "}";
bool sqf::parse::sqf::CODE_start(size_t curoff) { return m_contents[curoff] == '{'; }
void sqf::parse::sqf::CODE(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.kind = (short)asttype::sqf::CODE;
	thisnode.offset = info.offset;
	thisnode.col = info.column;
	thisnode.line = info.line;
	thisnode.file = info.file;
	info.offset++;
	info.column++;
	skip(info);

	if (SQF_start(info.offset))
	{
		SQF(thisnode, info, errflag);
	}
	else
	{
		log(err::ExpectedSQF(info));
		errflag = true;
	}

	if (m_contents[info.offset] == '}')
	{
		info.offset++;
		info.column++;
	}
	else
	{
		log(err::MissingCurlyClosingBracket(info));
		errflag = true;
	}
	thisnode.length = info.offset - thisnode.offset;
	root.children.emplace_back(std::move(thisnode));
}
//ARRAY = '[' [ BINARYEXPRESSION { ',' BINARYEXPRESSION } ] ']';
bool sqf::parse::sqf::ARRAY_start(size_t curoff) { return m_contents[curoff] == '['; }
void sqf::parse::sqf::ARRAY(astnode &root, position_info& info, bool &errflag)
{
	auto thisnode = astnode();
	thisnode.kind = (short)asttype::sqf::ARRAY;
	thisnode.offset = info.offset;
	thisnode.col = info.column;
	thisnode.line = info.line;
	thisnode.file = info.file;
	info.offset++;
	info.column++;
	skip(info);
	if (BINARYEXPRESSION_start(info.offset))
	{
		BINARYEXPRESSION(thisnode, info, errflag);
		skip(info);
		while (m_contents[info.offset] == ',')
		{
			info.column++;
			info.offset++;
			skip(info);

			if (BINARYEXPRESSION_start(info.offset))
			{
				BINARYEXPRESSION(thisnode, info, errflag);
				skip(info);
			}
			else
			{
				log(err::ExpectedBinaryExpression(info));
				errflag = true;
			}
		}
	}
	if (m_contents[info.offset] == ']')
	{
		info.offset++;
		info.column++;
	}
	else
	{
		log(err::MissingSquareClosingBracket(info));
		errflag = true;
	}
	thisnode.length = info.offset - thisnode.offset;
	thisnode.content = std::string(m_contents.substr(thisnode.offset, thisnode.length));
	root.children.emplace_back(std::move(thisnode));
}

::sqf::parse::astnode sqf::parse::sqf::parse_sqf(bool &errflag)
{
	position_info info = { 1, 0, 0, m_file };
	astnode node;
	node.kind = (short)asttype::sqf::SQF;
	node.offset = 0;
	node.content = m_contents;
	node.file = m_file;
	SQF(node, info, errflag);
	node.length = info.offset;
	skip(info);
	if (!errflag && m_contents[info.offset] != '\0') {
		log(err::EndOfFile(info));
		errflag = true;
	}
	return node;
}
const char * sqf::parse::sqf::astkindname(short id)
{
	switch (id)
	{
		case (short)asttype::sqf::SQF: return "SQF";
		case (short)asttype::sqf::STATEMENT: return "STATEMENT";
		case (short)asttype::sqf::ASSIGNMENT: return "ASSIGNMENT";
		case (short)asttype::sqf::ASSIGNMENTLOCAL: return "ASSIGNMENTLOCA";
		case (short)asttype::sqf::BEXP1: return "BEXP1";
		case (short)asttype::sqf::BEXP2: return "BEXP2";
		case (short)asttype::sqf::BEXP3: return "BEXP3";
		case (short)asttype::sqf::BEXP4: return "BEXP4";
		case (short)asttype::sqf::BEXP5: return "BEXP5";
		case (short)asttype::sqf::BEXP6: return "BEXP6";
		case (short)asttype::sqf::BEXP7: return "BEXP7";
		case (short)asttype::sqf::BEXP8: return "BEXP8";
		case (short)asttype::sqf::BEXP9: return "BEXP9";
		case (short)asttype::sqf::BEXP10: return "BEXP10";
		case (short)asttype::sqf::BINARYEXPRESSION: return "BINARYEXPRESSION";
		case (short)asttype::sqf::BINARYOP: return "BINARYOP";
		case (short)asttype::sqf::BRACKETS: return "BRACKETS";
		case (short)asttype::sqf::UNARYOP: return "UNARYOP";
		case (short)asttype::sqf::PRIMARYEXPRESSION: return "PRIMARYEXPRESSION";
		case (short)asttype::sqf::NULAROP: return "NULAROP";
		case (short)asttype::sqf::UNARYEXPRESSION: return "UNARYEXPRESSION";
		case (short)asttype::sqf::HEXNUMBER: return "HEXNUMBER";
		case (short)asttype::sqf::NUMBER: return "NUMBER";
		case (short)asttype::sqf::VARIABLE: return "VARIABLE";
		case (short)asttype::sqf::STRING: return "STRING";
		case (short)asttype::sqf::CODE: return "m_contents";
		case (short)asttype::sqf::ARRAY: return "ARRAY";
		default: return "NA";
	}
}