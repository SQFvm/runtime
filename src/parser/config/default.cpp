#include "default.h"
#include "../../runtime/util.h"

#include "../../runtime/d_array.h"
#include "../../runtime/d_scalar.h"
#include "../../runtime/d_string.h"

#include <cwctype>
#include <utility>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>
/*
NODELIST = { NODE ';' { ';' } };
NODE = 'class' CONFIGNODE | VALUENODE;
CONFIGNODE = ident [ ':' ident ] '{' NODELIST '}'
VALUENODE = ident ('=' (STRING | NUMBER | LOCALIZATION) | '[' ']' '=' ARRAY);
STRING = '"' { any | "\"\"" } '"' | '\'' { any | "''" } '\'';
NUMBER = [ '-' ] num [ '.' num ];
LOCALIZATION = '$' ident;
ARRAY = '{' [ VALUE { ',' VALUE } ] '}'
VALUE = STRING | NUMBER | LOCALIZATION | ARRAY;
*/

namespace err = logmessage::config;
using namespace ::sqf::runtime::util;

void sqf::parser::config::default::instance::skip()
{
	while (true)
	{
		switch (m_contents[m_info.offset])
		{
		case ' ': m_info.offset++; m_info.column++; continue;
		case '\t': m_info.offset++; m_info.column++; continue;
		case '\r': m_info.offset++; m_info.column++; continue;
		case '\n': m_info.offset++; m_info.line++; m_info.column = 0; continue;
		case '#':
			if ((m_contents[m_info.offset + 1] == 'l' || m_contents[m_info.offset + 1] == 'L') &&
				(m_contents[m_info.offset + 2] == 'i' || m_contents[m_info.offset + 1] == 'I') &&
				(m_contents[m_info.offset + 3] == 'n' || m_contents[m_info.offset + 1] == 'N') &&
				(m_contents[m_info.offset + 4] == 'e' || m_contents[m_info.offset + 1] == 'E'))
			{
				m_info.offset += 6;
				size_t start = m_info.offset;
				for (; m_contents[m_info.offset] != '\0' && m_contents[m_info.offset] != '\n' && m_contents[m_info.offset] != ' '; m_info.offset++);
				auto str_tmp = std::string(m_contents.substr(start, m_info.offset - start));
				m_info.line = static_cast<size_t>(std::stoul(str_tmp));

				for (; m_contents[m_info.offset] != '\0' && m_contents[m_info.offset] != '\n' && m_contents[m_info.offset] == ' '; m_info.offset++);
				if (m_contents[m_info.offset] != '\0' && m_contents[m_info.offset] != '\n')
				{
					start = m_info.offset;
					for (; m_contents[m_info.offset] != '\0' && m_contents[m_info.offset] != '\n'; m_info.offset++);
					auto str = std::string(m_contents.substr(start, m_info.offset - start));
					m_info.path = { strip_quotes(str), {} };
				}
				break;
			}
		default: return;
		}
	}
}

//endchr = [,;];
size_t sqf::parser::config::default::instance::endchr(size_t off) { return m_contents[off] == ';' ? 1 : 0; }
//identifier = [_a-zA-Z0-9]*;
size_t sqf::parser::config::default::instance::identifier(size_t off) { size_t i = off; for (i = off; (m_contents[i] >= 'a' && m_contents[i] <= 'z') || (m_contents[i] >= 'A' && m_contents[i] <= 'Z') || (m_contents[i] >= '0' && m_contents[i] <= '9') || m_contents[i] == '_'; i++) {}; return i - off; }
//operator_ = [-*+/a-zA-Z><=%_]+;
//ToDo: Add clearer non-alphabetical checks (-- should not be detected as SINGLE operator but rather as two operators)
size_t sqf::parser::config::default::instance::operator_(size_t off) { size_t i; for (i = off; (m_contents[i] >= 'a' && m_contents[i] <= 'z') || (m_contents[i] >= 'A' && m_contents[i] <= 'Z') || m_contents[i] == '+' || m_contents[i] == '-' || m_contents[i] == '*' || m_contents[i] == '/' || m_contents[i] == '>' || m_contents[i] == '<' || m_contents[i] == '=' || m_contents[i] == '%' || m_contents[i] == '_'; i++) {}; return i - off; }
//hexadecimal = [0-9a-fA-F]+;
size_t sqf::parser::config::default::instance::hexadecimal(size_t off) { size_t i; for (i = off; (m_contents[i] >= 'a' && m_contents[i] <= 'f') || (m_contents[i] >= 'A' && m_contents[i] <= 'F') || (m_contents[i] >= '0' && m_contents[i] <= '9'); i++) {}; return i - off; }
//scalarsub = [0-9]+;
size_t sqf::parser::config::default::instance::numsub(size_t off) { size_t i; for (i = off; m_contents[i] >= '0' && m_contents[i] <= '9'; i++) {}; return i - off; }
//scalar = scalarsub(.scalarsub)?;
size_t sqf::parser::config::default::instance::num(size_t off) { size_t i = off + numsub(off); if (m_contents[off] == '.') i += numsub(off); return i - off; }
//anytext = (?![ \t\r\n;])+;
size_t sqf::parser::config::default::instance::anytext(size_t off) { size_t i; for (i = off; m_contents[i] != ' ' && m_contents[i] != '\t' && m_contents[i] != '\r' && m_contents[i] != '\n' && m_contents[i] != ';'; i++) {}; return i - off; }


// NODELIST = { NODE ';' { ';' } };
bool sqf::parser::config::default::instance::NODELIST_start(size_t off) { return true; }
void sqf::parser::config::default::instance::NODELIST(::sqf::parser::config::default::astnode& root, bool& errflag)
{
	//auto thisnode = astnode();
	//thisnode.offset = m_info.offset;
	//thisnode.kind = nodetype::NODELIST;
	skip();
	//Iterate over statements as long as it is an instruction start.
	while (NODE_start(m_info.offset))
	{
		NODE(root, errflag);
		skip();
		//Make sure at least one endchr is available unless no statement follows
		if (!endchr(m_info.offset) && NODE_start(m_info.offset))
		{
			owner.log(err::ExpectedStatementTerminator(m_info));
			errflag = true;
		}
		else
		{
			size_t endchrlen;
			//Add endchr up until no semicolon is left
			while ((endchrlen = endchr(m_info.offset)) > 0)
			{
				m_info.offset += endchrlen;
				m_info.column += endchrlen;
				skip();
			}
		}
	}
	//thisnode.length = m_info.offset - thisnode.offset;
	//root.children.push_back(thisnode);
}
// NODE = CONFIGNODE | VALUENODE | DELETENODE;
bool sqf::parser::config::default::instance::NODE_start(size_t off) { return CONFIGNODE_start(off) || VALUENODE_start(off); }
void sqf::parser::config::default::instance::NODE(::sqf::parser::config::default::astnode & root, bool& errflag)
{
	//auto thisnode = astnode();
	//thisnode.offset = m_info.offset;
	//thisnode.kind = nodetype::NODE;
	if (CONFIGNODE_start(m_info.offset))
	{
		CONFIGNODE(root, errflag);
	}
	else if (VALUENODE_start(m_info.offset))
	{
		VALUENODE(root, errflag);
	}
	else
	{
		owner.log(err::NoViableAlternativeNode(m_info));
		errflag = true;
	}
	//thisnode.length = m_info.offset - thisnode.offset;
	//root.children.push_back(thisnode);
}
// CONFIGNODE = 'class' ident [ ':' ident ] [ '{' NODELIST '}' ]
bool sqf::parser::config::default::instance::CONFIGNODE_start(size_t off)
{
	const char* txt = "class";
	return std::equal(
		m_contents.begin() + off, m_contents.end() + ::sqf::runtime::util::strlen(txt),
		txt, txt + ::sqf::runtime::util::strlen(txt));
}
void sqf::parser::config::default::instance::CONFIGNODE(::sqf::parser::config::default::astnode & root, bool& errflag)
{
	size_t len;
	auto thisnode = astnode();
	thisnode.offset = m_info.offset;
	thisnode.kind = nodetype::CONFIGNODE;

	m_info.offset += ::sqf::runtime::util::strlen("class");
	m_info.column += ::sqf::runtime::util::strlen("class");
	skip();
	std::string ident;
	if ((len = identifier(m_info.offset)) > 0)
	{
		ident = std::string(m_contents.substr(m_info.offset, len));
		thisnode.content = ident;
		thisnode.line = m_info.line;
		thisnode.path = m_info.path;
		m_info.offset += len;
		m_info.column += len;
	}
	else
	{
		owner.log(err::ExpectedIdentifier(m_info));
		errflag = true;
	}
	skip();
	if (m_contents[m_info.offset] == ':')
	{
		m_info.offset++;
		m_info.column++;
		skip();
		if ((len = identifier(m_info.offset)) > 0)
		{
			ident = std::string(m_contents.substr(m_info.offset, len));
			astnode parentidentnode;
			parentidentnode.offset = m_info.offset;
			parentidentnode.length = len;
			parentidentnode.line = m_info.line;
			parentidentnode.path = m_info.path;
			parentidentnode.content = ident;
			parentidentnode.kind = nodetype::CONFIGNODE_PARENTIDENT;
			m_info.offset += len;
			m_info.column += len;
			thisnode.children.push_back(parentidentnode);
		}
		else
		{
			owner.log(err::ExpectedIdentifier(m_info));
			errflag = true;
		}
		skip();
	}
	if (m_contents[m_info.offset] == '{')
	{
		m_info.offset++;;
		m_info.column++;
		skip();

		NODELIST(thisnode, errflag);
		if (m_contents[m_info.offset] == '}')
		{
			m_info.offset++;;
			m_info.column++;
		}
		else
		{
			owner.log(err::MissingCurlyClosingBracket(m_info));
			errflag = true;
		}

		thisnode.length = m_info.offset - thisnode.offset;
		root.children.push_back(thisnode);
	}
}
// DELETENODE = delete ident;
bool sqf::parser::config::default::instance::DELETENODE_start(size_t off)
{
	const char* txt = "delete";
	return std::equal(
		m_contents.begin() + off, m_contents.end() + ::sqf::runtime::util::strlen(txt),
		txt, txt + ::sqf::runtime::util::strlen(txt));
}
void sqf::parser::config::default::instance::DELETENODE(::sqf::parser::config::default::astnode & root, bool& errflag)
{
	size_t len;
	auto thisnode = astnode();
	thisnode.offset = m_info.offset;
	thisnode.kind = nodetype::DELETENODE;

	m_info.offset += ::sqf::runtime::util::strlen("delete");
	m_info.column += ::sqf::runtime::util::strlen("delete");
	skip();
	std::string ident;
	if ((len = identifier(m_info.offset)) > 0)
	{
		ident = std::string(m_contents.substr(m_info.offset, len));
		thisnode.content = ident;
		thisnode.line = m_info.line;
		thisnode.path = m_info.path;
		m_info.offset += len;
		m_info.column += len;
	}
	else
	{
		owner.log(err::ExpectedIdentifier(m_info));
		errflag = true;
	}
}
// VALUENODE = ident ('=' (STRING | NUMBER | LOCALIZATION) | '[' ']' ( '=' | '+=' ) ARRAY);
bool sqf::parser::config::default::instance::VALUENODE_start(size_t off) { return identifier(off) > 0; }
void sqf::parser::config::default::instance::VALUENODE(::sqf::parser::config::default::astnode & root, bool& errflag)
{
	size_t len;
	bool isarr = false;
	auto thisnode = astnode();
	thisnode.offset = m_info.offset;
	thisnode.kind = nodetype::VALUENODE;

	if ((len = identifier(m_info.offset)) > 0)
	{
		auto ident = std::string(m_contents.substr(m_info.offset, len));
		thisnode.content = ident;
		thisnode.line = m_info.line;
		thisnode.path = m_info.path;
		m_info.offset += len;
		m_info.column += len;
		skip();
	}
	else
	{
		owner.log(err::ExpectedIdentifier(m_info));
		errflag = true;
	}

	if (m_contents[m_info.offset] == '[')
	{
		isarr = true;
		m_info.offset++;;
		m_info.column++;
		skip();
		if (m_contents[m_info.offset] == ']')
		{
			m_info.offset++;;
			m_info.column++;
			skip();
		}
		else
		{
			owner.log(err::MissingSquareClosingBracket(m_info));
			errflag = true;
		}
		if (m_contents[m_info.offset] == '=')
		{
			m_info.offset++;
			m_info.column++;
			skip();
		}
		else if (m_contents[m_info.offset] == '+' && m_contents[m_info.offset + 1] == '=')
		{
			thisnode.kind = nodetype::VALUENODE_ADDARRAY;
			m_info.offset++;
			m_info.offset++;
			m_info.column++;
			m_info.column++;
			skip();
		}
		else
		{
			owner.log(err::MissingEqualSign(m_info));
			errflag = true;
		}
	}
	else
	{
		if (m_contents[m_info.offset] == '=')
		{
			m_info.offset++;;
			m_info.column++;
			skip();
		}
		else
		{
			owner.log(err::MissingEqualSign(m_info));
			errflag = true;
		}
	}

	if (isarr)
	{
		if (ARRAY_start(m_info.offset))
		{
			ARRAY(thisnode, errflag);
		}
		else
		{
			owner.log(err::ExpectedArray(m_info));
			errflag = true;
		}
	}
	else
	{
		size_t tmp_line = m_info.line;
		size_t tmp_col = m_info.column;
		size_t tmp_off = m_info.offset;
		bool was_handled = false;
		if (STRING_start(m_info.offset))
		{
			was_handled = true;
			STRING(thisnode, errflag);
		}
		else if (NUMBER_start(m_info.offset))
		{
			was_handled = true;
			NUMBER(thisnode, errflag);
		}
		else if (LOCALIZATION_start(m_info.offset))
		{
			was_handled = true;
			LOCALIZATION(thisnode, errflag);
		}
		skip();
		if (m_contents[m_info.offset] != ';')
		{
			if (was_handled)
			{
				thisnode.children.pop_back();
			}
			while (m_contents[m_info.offset] != ';' && m_contents[m_info.offset] != '\0')
			{
				m_info.offset++;;
			}
			auto magicstringnode = astnode();
			magicstringnode.kind = nodetype::STRING;
			magicstringnode.col = tmp_col;
			magicstringnode.line = tmp_line;
			magicstringnode.path = m_info.path;
			auto fullstring = std::string("\"").append(m_contents.substr(tmp_off, m_info.offset - tmp_off)).append("\"");
			magicstringnode.content = fullstring;
			magicstringnode.length = m_info.offset - tmp_off;
			magicstringnode.offset = tmp_off;
			thisnode.children.push_back(magicstringnode);
		}
	}

	thisnode.length = m_info.offset - thisnode.offset;
	root.children.push_back(thisnode);
}
//STRING = '"' { any | "\"\"" } '"' | '\'' { any | "''" } '\'';
bool sqf::parser::config::default::instance::STRING_start(size_t off) { return m_contents[off] == '"' || m_contents[off] == '\''; }
void sqf::parser::config::default::instance::STRING(::sqf::parser::config::default::astnode & root, bool& errflag)
{
	auto thisnode = astnode();
	thisnode.kind = nodetype::STRING;
	thisnode.col = m_info.column;
	thisnode.line = m_info.line;
	thisnode.path = m_file;
	size_t i;
	auto startchr = m_contents[m_info.offset];
	m_info.column++;
	for (i = m_info.offset + 1; m_contents[i] != '\0' && (m_contents[i] != startchr || m_contents[i + 1] == startchr); i++)
	{
		if (m_contents[i] == startchr)
		{
			m_info.column++;
			i++;
		}
		switch (m_contents[i])
		{
		case '\n':
			m_info.column = 0;
			m_info.line++;
			break;
		default:
			m_info.column++;
			break;
		}
	}
	i++;
	m_info.column++;
	auto fullstring = i - m_info.offset - 2 < 0 ? "" : std::string(m_contents.substr(m_info.offset + 1, i - m_info.offset - 2));
	thisnode.content = fullstring;
	thisnode.length = i - m_info.offset;
	thisnode.offset = m_info.offset;
	m_info.offset = i;
	root.children.push_back(thisnode);
}
//NUMBER = "0x" hexadecimal | [ '-' ]scalar;
bool sqf::parser::config::default::instance::NUMBER_start(size_t off) { return m_contents[off] == '-' || (m_contents[off] >= '0' && m_contents[off] <= '9'); }
void sqf::parser::config::default::instance::NUMBER(::sqf::parser::config::default::astnode & root, bool& errflag)
{
	auto thisnode = astnode();
	thisnode.kind = nodetype::NUMBER;
	thisnode.col = m_info.column;
	thisnode.line = m_info.line;
	thisnode.path = m_file;
	if (m_contents[m_info.offset] == '0' && m_contents[m_info.offset + 1] == 'x')
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
		if (m_contents[i] == '-')
		{
			i++;
		}
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
		auto ident = std::string(m_contents.substr(m_info.offset, i - m_info.offset));
		thisnode.content = ident;
		thisnode.offset = m_info.offset;
		thisnode.length = i - m_info.offset;
		m_info.column += i - m_info.offset;
		m_info.offset = i;
	}
	root.children.push_back(thisnode);
}
//LOCALIZATION = '$' ident;
bool sqf::parser::config::default::instance::LOCALIZATION_start(size_t off) { return m_contents[off] == '$'; }
void sqf::parser::config::default::instance::LOCALIZATION(::sqf::parser::config::default::astnode & root, bool& errflag)
{
	auto thisnode = astnode();
	thisnode.kind = nodetype::LOCALIZATION;
	thisnode.path = m_file;
	m_info.offset++;;
	m_info.column++;
	auto len = identifier(m_info.offset);
	auto ident = std::string(m_contents.substr(m_info.offset, len));
	thisnode.content = ident;
	thisnode.length = len;
	thisnode.offset = m_info.offset;
	thisnode.col = m_info.column;
	thisnode.line = m_info.line;

	m_info.offset += len;
	m_info.column += len;
	root.children.push_back(thisnode);
}
//ARRAY = '{' [ VALUE { ',' VALUE } ] '}'
bool sqf::parser::config::default::instance::ARRAY_start(size_t off) { return m_contents[off] == '{'; }
void sqf::parser::config::default::instance::ARRAY(::sqf::parser::config::default::astnode & root, bool& errflag)
{
	auto thisnode = astnode();
	thisnode.kind = nodetype::ARRAY;
	thisnode.offset = m_info.offset;
	thisnode.col = m_info.column;
	thisnode.line = m_info.line;
	thisnode.path = m_info.path;
	m_info.offset++;;
	m_info.column++;
	skip();
	if (VALUE_start(m_info.offset))
	{
		VALUE(thisnode, errflag);
		skip();
		while (m_contents[m_info.offset] == ',')
		{
			m_info.column++;
			m_info.offset++;;
			skip();

			if (VALUE_start(m_info.offset))
			{
				VALUE(thisnode, errflag);
				skip();
			}
			else
			{
				owner.log(err::ExpectedValue(m_info));
				errflag = true;
			}
		}
	}
	if (m_contents[m_info.offset] == '}')
	{
		m_info.offset++;;
		m_info.column++;
	}
	else
	{
		owner.log(err::MissingCurlyClosingBracket(m_info));
		errflag = true;
	}
	thisnode.length = m_info.offset - thisnode.offset;
	thisnode.content = std::string(m_contents.substr(thisnode.offset, thisnode.length));
	root.children.push_back(thisnode);
}
//VALUE = STRING | NUMBER | LOCALIZATION | ARRAY;
bool sqf::parser::config::default::instance::VALUE_start(size_t off) { return STRING_start(off) || NUMBER_start(off) || LOCALIZATION_start(off) || ARRAY_start(off); }
void sqf::parser::config::default::instance::VALUE(::sqf::parser::config::default::astnode & root, bool& errflag)
{
	//auto thisnode = astnode();
	//thisnode.offset = m_info.offset;
	//thisnode.kind = nodetype::VALUE;
	if (STRING_start(m_info.offset))
	{
		STRING(root, errflag);
	}
	else if (NUMBER_start(m_info.offset))
	{
		NUMBER(root, errflag);
	}
	else if (LOCALIZATION_start(m_info.offset))
	{
		LOCALIZATION(root, errflag);
	}
	else if (ARRAY_start(m_info.offset))
	{
		ARRAY(root, errflag);
	}
	else
	{
		owner.log(err::NoViableAlternativeValue(m_info));
		errflag = true;
	}
	//thisnode.length = m_info.offset - thisnode.offset;
	//root.children.push_back(thisnode);
}


sqf::parser::config::default::astnode sqf::parser::config::default::instance::parse(bool& errflag)
{
	astnode node;
	node.kind = nodetype::NODELIST;
	node.offset = 0;
	node.content = m_contents;
	NODELIST(node, errflag);
	skip();
	if (m_info.offset != m_contents.length())
	{
		owner.log(err::EndOfFileNotReached(m_info));
		errflag = true;
	}
	node.length = m_info.offset;
	return node;
}

bool sqf::parser::config::default::apply_to_confighost(sqf::parser::config::default::astnode& node, ::sqf::runtime::confighost& confighost, ::sqf::runtime::confighost::config& parent)
{
	// ToDo: Check if a corresponding config already exists in confighost before creating it to avoid duplicates

	switch (node.kind)
	{
	case nodetype::CONFIGNODE:
	{
		auto& curnode = confighost.create_new();
		parent.push_child(confighost, curnode);
		curnode.parent_logical(parent);
		curnode.name(node.content);

		if (!node.children.empty() && node.children.front().kind == nodetype::CONFIGNODE_PARENTIDENT)
		{
			auto inherited = confighost.find_traverse_upwards_logical(parent.iterator(confighost), node.children.front().content);
			if (inherited != confighost.end())
			{
				curnode.parent_inherited(*inherited);
			}
			for (size_t i = 1; i < node.children.size(); i++)
			{
				auto subnode = node.children[i];
				apply_to_confighost(subnode, confighost, curnode);
			}
		}
		else
		{
			for (auto subnode : node.children)
			{
				apply_to_confighost(subnode, confighost, curnode);
			}
		}
	} break;
	case nodetype::DELETENODE:
	{
		auto referenced = confighost.find_traverse_upwards_inherited(parent.iterator(confighost), node.content);
		if (referenced != confighost.end())
		{
			parent.push_deleted(*referenced);
		}
	} break;
	case nodetype::VALUENODE_ADDARRAY:
	case nodetype::VALUENODE:
	{
		auto& curnode = confighost.create_new();
		parent.push_child(confighost, curnode);
		curnode.parent_logical(parent);
		curnode.name(node.content);

		for (auto& subnode : node.children)
		{
			apply_to_confighost(subnode, confighost, curnode);
		}
		if (node.kind == nodetype::VALUENODE_ADDARRAY)
		{
			auto inherited = confighost.find_traverse_upwards_inherited(parent.iterator(confighost), node.content);
			if (inherited != confighost.end())
			{
				auto self =  curnode.value().data_try<sqf::types::d_array>();
				auto other = inherited->value().data_try<sqf::types::d_array>();
				if (self.get() && other.get())
				{
					self->insert(self->end(), other->begin(), other->end());
				}
			}
		}
	} break;
	case nodetype::STRING:
		parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_string>(node.content)));
		break;
	case nodetype::NUMBER:
		parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_scalar>(std::stod(node.content))));
		break;
	case nodetype::HEXNUMBER:
		parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_scalar>(std::stol(node.content, nullptr, 16))));
		break;
	case nodetype::LOCALIZATION:
		parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_string>(node.content)));
		break;
	case nodetype::ARRAY:
	{
		std::vector<sqf::runtime::value> values;
		for (auto& subnode : node.children)
		{
			apply_to_confighost(subnode, confighost, parent);
			values.push_back(parent.value());
		}
		parent.value(sqf::runtime::value(std::make_shared<sqf::types::d_array>(values)));
	} break;
	case nodetype::VALUE:
		break;
	default:
	{
		for (auto& subnode : node.children)
		{
			apply_to_confighost(subnode, confighost, parent);
		}
	}
	}
	return true;
}