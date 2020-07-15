#include <cwctype>
#include <utility>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#include "astnode.h"
#include "compiletime.h"
#include "parseconfig.h"
#include "stringmanip.h"
#include "string_op.h"
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

namespace sqf::parse
{
	void config::skip()
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
					auto str = std::string(m_contents_actual.substr(start, m_info.offset - start));
					m_info.line = static_cast<size_t>(std::stoul(str));

					for (; m_contents[m_info.offset] != '\0' && m_contents[m_info.offset] != '\n' && m_contents[m_info.offset] == ' '; m_info.offset++);
					if (m_contents[m_info.offset] != '\0' && m_contents[m_info.offset] != '\n')
					{
						start = m_info.offset;
						for (; m_contents[m_info.offset] != '\0' && m_contents[m_info.offset] != '\n'; m_info.offset++);
						auto str = std::string(m_contents_actual.substr(start, m_info.offset - start));
						m_info.file = strip_quotes(str);
					}
					break;
				}
			default: return;
			}
		}
	}

	//endchr = [,;];
	size_t config::endchr(size_t off) { return m_contents[off] == ';' ? 1 : 0; }
	//identifier = [_a-zA-Z0-9]*;
	size_t config::identifier(size_t off) { size_t i = off; for (i = off; (m_contents[i] >= 'a' && m_contents[i] <= 'z') || (m_contents[i] >= 'A' && m_contents[i] <= 'Z') || (m_contents[i] >= '0' && m_contents[i] <= '9') || m_contents[i] == '_'; i++) {}; return i - off; }
	//operator_ = [-*+/a-zA-Z><=%_]+;
	//ToDo: Add clearer non-alphabetical checks (-- should not be detected as SINGLE operator but rather as two operators)
	size_t config::operator_(size_t off) { size_t i; for (i = off; (m_contents[i] >= 'a' && m_contents[i] <= 'z') || (m_contents[i] >= 'A' && m_contents[i] <= 'Z') || m_contents[i] == '+' || m_contents[i] == '-' || m_contents[i] == '*' || m_contents[i] == '/' || m_contents[i] == '>' || m_contents[i] == '<' || m_contents[i] == '=' || m_contents[i] == '%' || m_contents[i] == '_'; i++) {}; return i - off; }
	//hexadecimal = [0-9a-fA-F]+;
	size_t config::hexadecimal(size_t off) { size_t i; for (i = off; (m_contents[i] >= 'a' && m_contents[i] <= 'f') || (m_contents[i] >= 'A' && m_contents[i] <= 'F') || (m_contents[i] >= '0' && m_contents[i] <= '9'); i++) {}; return i - off; }
	//scalarsub = [0-9]+;
	size_t config::numsub(size_t off) { size_t i; for (i = off; m_contents[i] >= '0' && m_contents[i] <= '9'; i++) {}; return i - off; }
	//scalar = scalarsub(.scalarsub)?;
	size_t config::num(size_t off) { size_t i = off + numsub(off); if (m_contents[off] == '.') i += numsub(off); return i - off; }
	//anytext = (?![ \t\r\n;])+;
	size_t config::anytext(size_t off) { size_t i; for (i = off; m_contents[i] != ' ' && m_contents[i] != '\t' && m_contents[i] != '\r' && m_contents[i] != '\n' && m_contents[i] != ';'; i++) {}; return i - off; }


	// NODELIST = { NODE ';' { ';' } };
	bool config::NODELIST_start(size_t off) { return true; }
	void config::NODELIST(astnode & root, bool& errflag)
	{
		//auto thisnode = astnode();
		//thisnode.offset = m_info.offset;
		//thisnode.kind = (short)asttype::config::NODELIST;
		skip();
		//Iterate over statements as long as it is an instruction start.
		while (NODE_start(m_info.offset))
		{
			NODE(root, errflag);
			skip();
			//Make sure at least one endchr is available unless no statement follows
			if (!endchr(m_info.offset) && NODE_start(m_info.offset))
			{
				log(err::ExpectedStatementTerminator(m_info));
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
	// NODE = CONFIGNODE | VALUENODE;
	bool config::NODE_start(size_t off) { return CONFIGNODE_start(off) || VALUENODE_start(off); }
	void config::NODE(astnode & root, bool& errflag)
	{
		//auto thisnode = astnode();
		//thisnode.offset = m_info.offset;
		//thisnode.kind = (short)asttype::config::NODE;
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
			log(err::NoViableAlternativeNode(m_info));
			errflag = true;
		}
		//thisnode.length = m_info.offset - thisnode.offset;
		//root.children.push_back(thisnode);
	}
	// CONFIGNODE = 'class' ident [ ':' ident ] [ '{' NODELIST '}' ]
	bool config::CONFIGNODE_start(size_t off) { return str_cmpi(m_contents + off, compiletime::strlen("class"), "class", compiletime::strlen("class")) == 0; }
	void config::CONFIGNODE(astnode & root, bool& errflag)
	{
		size_t len;
		auto thisnode = astnode();
		thisnode.offset = m_info.offset;
		thisnode.kind = (short)asttype::config::CONFIGNODE;

		m_info.offset += compiletime::strlen("class");
		m_info.column += compiletime::strlen("class");
		skip();
		std::string ident;
		if ((len = identifier(m_info.offset)) > 0)
		{
			ident = std::string(m_contents_actual.substr(m_info.offset, len));
			thisnode.content = ident;
			thisnode.line = m_info.line;
			thisnode.file = m_info.file;
			m_info.offset += len;
			m_info.column += len;
		}
		else
		{
			log(err::ExpectedIdentifier(m_info));
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
				ident = std::string(m_contents_actual.substr(m_info.offset, len));
				astnode parentidentnode;
				parentidentnode.offset = m_info.offset;
				parentidentnode.length = len;
				parentidentnode.line = m_info.line;
				parentidentnode.file = m_info.file;
				parentidentnode.content = ident;
				parentidentnode.kind = (short)asttype::config::CONFIGNODE_PARENTIDENT;
				m_info.offset += len;
				m_info.column += len;
				thisnode.children.push_back(parentidentnode);
			}
			else
			{
				log(err::ExpectedIdentifier(m_info));
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
				log(err::MissingCurlyClosingBracket(m_info));
				errflag = true;
			}

			thisnode.length = m_info.offset - thisnode.offset;
			root.children.push_back(thisnode);
		}
		else
		{
		}
	}
	//VALUENODE = ident ('=' (STRING | NUMBER | LOCALIZATION) | '[' ']' '=' ARRAY);
	bool config::VALUENODE_start(size_t off) { return identifier(off) > 0; }
	void config::VALUENODE(astnode & root, bool& errflag)
	{
		size_t len;
		bool isarr = false;
		auto thisnode = astnode();
		thisnode.offset = m_info.offset;
		thisnode.kind = (short)asttype::config::VALUENODE;

		if ((len = identifier(m_info.offset)) > 0)
		{
			auto ident = std::string(m_contents_actual.substr(m_info.offset, len));
			thisnode.content = ident;
			thisnode.line = m_info.line;
			thisnode.file = m_info.file;
			m_info.offset += len;
			m_info.column += len;
			skip();
		}
		else
		{
			log(err::ExpectedIdentifier(m_info));
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
				log(err::MissingSquareClosingBracket(m_info));
				errflag = true;
			}
		}
		if (m_contents[m_info.offset] == '=')
		{
			m_info.offset++;;
			m_info.column++;
			skip();
		}
		else
		{
			log(err::MissingEqualSign(m_info));
			errflag = true;
		}

		if (isarr)
		{
			if (ARRAY_start(m_info.offset))
			{
				ARRAY(thisnode, errflag);
			}
			else
			{
				log(err::ExpectedArray(m_info));
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
				magicstringnode.kind = (short)asttype::config::STRING;
				magicstringnode.col = tmp_col;
				magicstringnode.line = tmp_line;
				magicstringnode.file = m_info.file;
				auto fullstring = std::string("\"").append(m_contents_actual.substr(tmp_off, m_info.offset - tmp_off)).append("\"");
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
	bool config::STRING_start(size_t off) { return m_contents[off] == '"' || m_contents[off] == '\''; }
	void config::STRING(astnode & root, bool& errflag)
	{
		auto thisnode = astnode();
		thisnode.kind = (short)asttype::config::STRING;
		thisnode.col = m_info.column;
		thisnode.line = m_info.line;
		thisnode.file = m_file;
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
		auto fullstring = i - m_info.offset - 2 < 0 ? "" : std::string(m_contents_actual.substr(m_info.offset + 1, i - m_info.offset - 2));
		thisnode.content = fullstring;
		thisnode.length = i - m_info.offset;
		thisnode.offset = m_info.offset;
		m_info.offset = i;
		root.children.push_back(thisnode);
	}
	//NUMBER = "0x" hexadecimal | [ '-' ]scalar;
	bool config::NUMBER_start(size_t off) { return m_contents[off] == '-' || (m_contents[off] >= '0' && m_contents[off] <= '9'); }
	void config::NUMBER(astnode & root, bool& errflag)
	{
		auto thisnode = astnode();
		thisnode.kind = (short)asttype::config::NUMBER;
		thisnode.col = m_info.column;
		thisnode.line = m_info.line;
		thisnode.file = m_file;
		if (m_contents[m_info.offset] == '0' && m_contents[m_info.offset + 1] == 'x')
		{
			thisnode.kind = (short)asttype::config::HEXNUMBER;
			size_t i;
			for (i = m_info.offset + 2; (m_contents[i] >= '0' && m_contents[i] <= '9') || (m_contents[i] >= 'A' && m_contents[i] <= 'F') || (m_contents[i] >= 'a' && m_contents[i] <= 'f'); i++);
			auto ident = std::string(m_contents_actual.substr(m_info.offset, i - m_info.offset));
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
			auto ident = std::string(m_contents_actual.substr(m_info.offset, i - m_info.offset));
			thisnode.content = ident;
			thisnode.offset = m_info.offset;
			thisnode.length = i - m_info.offset;
			m_info.column += i - m_info.offset;
			m_info.offset = i;
		}
		root.children.push_back(thisnode);
	}
	//LOCALIZATION = '$' ident;
	bool config::LOCALIZATION_start(size_t off) { return m_contents[off] == '$'; }
	void config::LOCALIZATION(astnode & root, bool& errflag)
	{
		auto thisnode = astnode();
		thisnode.kind = (short)asttype::config::LOCALIZATION;
		thisnode.file = m_file;
		m_info.offset++;;
		m_info.column++;
		auto len = identifier(m_info.offset);
		auto ident = std::string(m_contents_actual.substr(m_info.offset, len));
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
	bool config::ARRAY_start(size_t off) { return m_contents[off] == '{'; }
	void config::ARRAY(astnode & root, bool& errflag)
	{
		auto thisnode = astnode();
		thisnode.kind = (short)asttype::config::ARRAY;
		thisnode.offset = m_info.offset;
		thisnode.col = m_info.column;
		thisnode.line = m_info.line;
		thisnode.file = m_info.file;
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
					log(err::ExpectedValue(m_info));
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
			log(err::MissingCurlyClosingBracket(m_info));
			errflag = true;
		}
		thisnode.length = m_info.offset - thisnode.offset;
		thisnode.content = std::string(m_contents_actual.substr(thisnode.offset, thisnode.length));
		root.children.push_back(thisnode);
	}
	//VALUE = STRING | NUMBER | LOCALIZATION | ARRAY;
	bool config::VALUE_start(size_t off) { return STRING_start(off) || NUMBER_start(off) || LOCALIZATION_start(off) || ARRAY_start(off); }
	void config::VALUE(astnode & root, bool& errflag)
	{
		//auto thisnode = astnode();
		//thisnode.offset = m_info.offset;
		//thisnode.kind = (short)asttype::config::VALUE;
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
			log(err::NoViableAlternativeValue(m_info));
			errflag = true;
		}
		//thisnode.length = m_info.offset - thisnode.offset;
		//root.children.push_back(thisnode);
	}


	astnode config::parse(bool& errflag)
	{
		astnode node;
		node.kind = (short)asttype::config::NODELIST;
		node.offset = 0;
		node.content = m_contents;
		NODELIST(node, errflag);
		skip();
		if (m_info.offset != std::strlen(m_contents))
		{
			log(err::EndOfFileNotReached(m_info));
			errflag = true;
		}
		node.length = m_info.offset;
		return node;
	}
}