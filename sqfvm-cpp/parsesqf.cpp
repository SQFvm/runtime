#include <cwctype>
#include <vector>
#include <string>
#include <sstream>
#include "astnode.h"
#include "sqfasttypes.h"
#include "compiletime.h"
#include "parsesqf.h"
extern "C"
{
#include "wstring_op.h"
}

namespace sqf
{
	namespace parse
	{

		void skip(const wchar_t *code, size_t &curoff)
		{
			while (1)
			{
				switch (code[curoff])
				{
				case L' ': curoff++; continue;
				case L'\t': curoff++; continue;
				case L'\r': curoff++; continue;
				case L'\n': curoff++; continue;
				default: return;
				}
			}
		}
		void skip(const wchar_t *code, size_t &line, size_t &col, size_t &curoff)
		{
			while (1)
			{
				switch (code[curoff])
				{
				case L' ': curoff++; col++; continue;
				case L'\t': curoff++; col++; continue;
				case L'\r': curoff++; col++; continue;
				case L'\n': curoff++; line++; col = 0; continue;
				default: return;
				}
			}
		}

		//endchr = [,;];
		size_t endchr(const wchar_t* code, size_t off) { return code[off] == L';' || code[off] == L',' ? 1 : 0; }
		//identifier = [_a-zA-Z][_a-zA-Z0-9]*;
		size_t identifier(const wchar_t* code, size_t off) { size_t i = off; if (!((code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z') || code[i] == L'_')) return 0; for (i = off + 1; (code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z') || (code[i] >= L'0' && code[i] <= L'9') || code[i] == L'_'; i++); return i - off; }
		//operator_ = [-*+/a-zA-Z><=%_]+;
		//ToDo: Add clearer non-alphabetical checks (-- should not be detected as SINGLE operator but rather as two operators)
		size_t operator_(const wchar_t* code, size_t off) { size_t i; for (i = off; (code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z') || code[i] == L'+' || code[i] == L'-' || code[i] == L'*' || code[i] == L'/' || code[i] == L'>' || code[i] == L'<' || code[i] == L'=' || code[i] == L'%' || code[i] == L'_'; i++); return i - off; }
		//hexadecimal = [0-9a-fA-F]+;
		size_t hexadecimal(const wchar_t* code, size_t off) { size_t i; for (i = off; (code[i] >= L'a' && code[i] <= L'f') || (code[i] >= L'A' && code[i] <= L'F') || (code[i] >= L'0' && code[i] <= L'9'); i++); return i - off; }
		//scalarsub = [0-9]+;
		size_t scalarsub(const wchar_t* code, size_t off) { size_t i; for (i = off; code[i] >= L'0' && code[i] <= L'9'; i++); return i - off; }
		//scalar = scalarsub(.scalarsub)?;
		size_t scalar(const wchar_t* code, size_t off) { size_t i = off + scalarsub(code, off); if (code[off] == L'.') i += scalarsub(code, off); return i - off; }
		//anytext = (?![ \t\r\n;])+;
		size_t anytext(const wchar_t* code, size_t off) { size_t i; for (i = off; code[i] != L' ' && code[i] != L'\t' && code[i] != L'\r' && code[i] != L'\n' && code[i] != L';'; i++); return i - off; }
		//SQF = [ STATEMENT { endchr { endchr } STATEMENT } ]
		bool SQF_start(helper &h, const wchar_t* code, size_t curoff) { return STATEMENT_start(h, code, curoff); }
		void SQF(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file)
		{
			//auto thisnode = astnode();
			//thisnode.offset = curoff;
			//thisnode.kind = sqfasttypes::SQF;
			skip(code, line, col, curoff);
			//Iterate over statements as long as it is an instruction start.
			while (STATEMENT_start(h, code, curoff))
			{
				STATEMENT(h, root, code, line, col, curoff, file);
				skip(code, line, col, curoff);
				//Make sure at least one endchr is available unless no statement follows
				if (!endchr(code, curoff) && STATEMENT_start(h, code, curoff))
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected either ';' or ','." << std::endl;
				}
				else
				{
					size_t endchrlen;
					//Add endchr up until no semicolon is left
					while ((endchrlen = endchr(code, curoff)) > 0)
					{
						curoff += endchrlen;
						col += endchrlen;
						skip(code, line, col, curoff);
					}
				}
			}
			//thisnode.length = curoff - thisnode.offset;
			//root.children.push_back(thisnode);
		}
		//STATEMENT = ASSIGNMENT | BINARYEXPRESSION;
		bool STATEMENT_start(helper &h, const wchar_t* code, size_t curoff) { return ASSIGNMENT_start(h, code, curoff) | BINARYEXPRESSION_start(h, code, curoff); }
		void STATEMENT(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file)
		{
			//auto thisnode = astnode();
			//thisnode.offset = curoff;
			//thisnode.kind = sqfasttypes::STATEMENT;
			if (ASSIGNMENT_start(h, code, curoff))
			{
				ASSIGNMENT(h, root, code, line, col, curoff, file);
			}
			else if (BINARYEXPRESSION_start(h, code, curoff))
			{
				BINARYEXPRESSION(h, root, code, line, col, curoff, file);
			}
			else
			{
				size_t i;
				for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"No viable alternative for STATEMENT." << std::endl;
			}
			//thisnode.length = curoff - thisnode.offset;
			//root.children.push_back(thisnode);
		}
		//ASSIGNMENT(2) = identifier '=' BINARYEXPRESSION | "private" identifier '=' BINARYEXPRESSION;
		bool ASSIGNMENT_start(helper &h, const wchar_t* code, size_t curoff)
		{
			size_t len;
			if (wstr_cmpi(code + curoff, compiletime::strlen(L"private"), L"private", compiletime::strlen(L"private")) == 0)
			{
				curoff += compiletime::strlen(L"private");
				skip(code, curoff);
			}
			if ((len = identifier(code, curoff)) > 0)
			{
				curoff += len;
				skip(code, curoff);
				return code[curoff] == L'=';
			}
			else
			{
				return false;
			}
		}
		void ASSIGNMENT(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file)
		{
			auto thisnode = astnode();
			thisnode.kind = sqfasttypes::ASSIGNMENT;
			thisnode.offset = curoff;
			size_t len;
			bool assignlocal = false;
			//check if prefixed by a 'private'
			if (wstr_cmpi(code + curoff, compiletime::strlen(L"private"), L"private", compiletime::strlen(L"private")) == 0)
			{
				curoff += compiletime::strlen(L"private");
				col += compiletime::strlen(L"private");
				skip(code, line, col, curoff);
				assignlocal = true;
				thisnode.kind = sqfasttypes::ASSIGNMENTLOCAL;
			}
			//receive the ident
			len = identifier(code, curoff);
			auto ident = std::wstring(code + curoff, code + curoff + len);

			auto varnode = astnode();
			varnode.offset = curoff;
			varnode.length = len;
			varnode.content = ident;
			varnode.kind = sqfasttypes::VARIABLE;
			thisnode.children.push_back(varnode);

			if (assignlocal && ident[0] != L'_')
			{
				h.err() << h.dbgsegment(code, curoff, len) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Private variables need to be prefixed with an underscore '_'." << std::endl;
			}
			curoff += len;
			skip(code, line, col, curoff);
			//store the dbginf for the '='
			size_t dbgstart = curoff;
			size_t dbgcol = col;
			size_t dbgline = line;
			//skip the '=' (is confirmed to be present in ASSIGNMENT_start)
			curoff++;
			col++;
			skip(code, line, col, curoff);

			if (BINARYEXPRESSION_start(h, code, curoff))
			{
				BINARYEXPRESSION(h, thisnode, code, line, col, curoff, file);
			}
			else
			{
				size_t i;
				for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected start of BINARYEXPRESSION." << std::endl;
			}
			thisnode.length = curoff - thisnode.offset;
			root.children.push_back(thisnode);
		}
		//BINARYEXPRESSION = PRIMARYEXPRESSION [ boperator BINARYEXPRESSION ];
		bool BINARYEXPRESSION_start(helper &h, const wchar_t* code, size_t curoff) { return PRIMARYEXPRESSION_start(h, code, curoff); }
		void BINARYEXPRESSION(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, short* calleeprec)
		{
			auto thisnode = astnode();
			thisnode.offset = curoff;
			thisnode.kind = sqfasttypes::BINARYEXPRESSION;
			PRIMARYEXPRESSION(h, thisnode, code, line, col, curoff, file);
			skip(code, line, col, curoff);
			auto opidentlen = operator_(code, curoff);
			if (opidentlen > 0)
			{
				auto opident = std::wstring(code + curoff, code + curoff + opidentlen);
				auto curnode = astnode();
				curnode.offset = curoff;
				curnode.kind = sqfasttypes::BINARYOP;
				curnode.length = opidentlen;
				curnode.content = opident;
				thisnode.children.push_back(curnode);
				auto curprec = h.precedence(opident);
				if (calleeprec != 0)
				{
					*calleeprec = curprec;
				}
				short otherprec = 0;
				col += opidentlen;
				curoff += opidentlen;
				skip(code, line, col, curoff);
				if (BINARYEXPRESSION_start(h, code, curoff))
				{
					BINARYEXPRESSION(h, thisnode, code, line, col, curoff, file, &otherprec);
					skip(code, line, col, curoff);
					if (otherprec <= curprec)
					{
						auto othernode = thisnode.children.back();
						if (!othernode.children.empty())
						{
							thisnode.children.pop_back();
							thisnode.children.push_back(othernode.children.front());
							othernode.children.front() = thisnode;
							thisnode = othernode;
							if (calleeprec != 0)
							{
								*calleeprec = otherprec;
							}
						}
					}
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected start of PRIMARYEXPRESSION.";
				}
				root.children.push_back(thisnode);
			}
			else
			{
				root.children.push_back(thisnode.children.back());
			}
		}
		//BRACKETS = '(' BINARYEXPRESSION ')';
		bool BRACKETS_start(helper &h, const wchar_t* code, size_t curoff) { return code[curoff] == L'('; }
		void BRACKETS(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file)
		{
			auto thisnode = astnode();
			thisnode.kind = sqfasttypes::BRACKETS;
			thisnode.offset = curoff;
			curoff++;
			col++;
			skip(code, line, col, curoff);
			if (BINARYEXPRESSION_start(h, code, curoff))
			{
				BINARYEXPRESSION(h, thisnode, code, line, col, curoff, file);
			}
			else
			{
				size_t i;
				for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected start of BINARYEXPRESSION." << std::endl;
			}
			skip(code, line, col, curoff);
			if (code[curoff] == L')')
			{
				curoff++;
				col++;
			}
			else
			{
				size_t i;
				for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected ')'.";
			}
			thisnode.length = curoff - thisnode.offset;
			root.children.push_back(thisnode);
		}
		//PRIMARYEXPRESSION = NUMBER | UNARYEXPRESSION | NULAREXPRESSION | VARIABLE | STRING | CODE | BRACKETS | ARRAY;
		bool PRIMARYEXPRESSION_start(helper &h, const wchar_t* code, size_t curoff) { return NUMBER_start(h, code, curoff) || UNARYEXPRESSION_start(h, code, curoff) || NULAREXPRESSION_start(h, code, curoff) || VARIABLE_start(h, code, curoff) || STRING_start(h, code, curoff) || CODE_start(h, code, curoff) || BRACKETS_start(h, code, curoff) || ARRAY_start(h, code, curoff); }
		void PRIMARYEXPRESSION(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file)
		{
			//auto thisnode = astnode();
			//thisnode.kind = sqfasttypes::PRIMARYEXPRESSION;
			//thisnode.offset = curoff;
			if (NUMBER_start(h, code, curoff))
			{
				NUMBER(h, root, code, line, col, curoff, file);
			}
			else if (UNARYEXPRESSION_start(h, code, curoff))
			{
				UNARYEXPRESSION(h, root, code, line, col, curoff, file);
			}
			else if (NULAREXPRESSION_start(h, code, curoff))
			{
				NULAREXPRESSION(h, root, code, line, col, curoff, file);
			}
			else if (VARIABLE_start(h, code, curoff))
			{
				VARIABLE(h, root, code, line, col, curoff, file);
			}
			else if (STRING_start(h, code, curoff))
			{
				STRING(h, root, code, line, col, curoff, file);
			}
			else if (CODE_start(h, code, curoff))
			{
				CODE(h, root, code, line, col, curoff, file);
			}
			else if (BRACKETS_start(h, code, curoff))
			{
				BRACKETS(h, root, code, line, col, curoff, file);
			}
			else if (ARRAY_start(h, code, curoff))
			{
				ARRAY(h, root, code, line, col, curoff, file);
			}
			else
			{
				size_t i;
				for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"No viable alternative for PRIMARYEXPRESSION." << std::endl;
			}
			//thisnode.length = curoff - thisnode.offset;
			//root.children.push_back(thisnode);
		}
		//NULAREXPRESSION = operator;
		bool NULAREXPRESSION_start(helper &h, const wchar_t* code, size_t curoff) { auto oplen = operator_(code, curoff); return oplen > 0 ? h.contains_nular(std::wstring(code + curoff, code + curoff + oplen)) : false; }
		void NULAREXPRESSION(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file)
		{
			auto thisnode = astnode();
			thisnode.kind = sqfasttypes::NULAROP;
			auto len = operator_(code, curoff);
			auto ident = std::wstring(code + curoff, code + curoff + len);
			thisnode.content = ident;
			thisnode.length = len;
			thisnode.offset = curoff;
			curoff += len;
			col += len;
			root.children.push_back(thisnode);
		}
		//UNARYEXPRESSION = operator PRIMARYEXPRESSION;
		bool UNARYEXPRESSION_start(helper &h, const wchar_t* code, size_t curoff) { auto oplen = operator_(code, curoff); return oplen > 0 ? h.contains_unary(std::wstring(code + curoff, code + curoff + oplen)) : false; }
		void UNARYEXPRESSION(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file)
		{
			auto thisnode = astnode();
			thisnode.kind = sqfasttypes::UNARYEXPRESSION;
			thisnode.offset = curoff;
			size_t dbgstart = curoff;
			size_t dbgcol = col;
			size_t dbgline = line;

			auto len = operator_(code, curoff);
			auto ident = std::wstring(code + curoff, code + curoff + len);
			auto opnode = astnode();
			opnode.kind = sqfasttypes::UNARYOP;
			opnode.offset = curoff;
			opnode.length = len;
			opnode.content = ident;
			thisnode.children.push_back(opnode);
			curoff += len;
			col += len;
			skip(code, line, col, curoff);

			if (PRIMARYEXPRESSION_start(h, code, curoff))
			{
				PRIMARYEXPRESSION(h, thisnode, code, line, col, curoff, file);
			}
			else
			{
				size_t i;
				for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected start of PRIMARYEXPRESSION." << std::endl;
			}
			thisnode.length = curoff - thisnode.offset;
			root.children.push_back(thisnode);
		}
		//NUMBER = ("0x" | '$') hexadecimal | scalar;
		bool NUMBER_start(helper &h, const wchar_t* code, size_t curoff) { return code[curoff] == L'$' || (code[curoff] >= L'0' && code[curoff] <= L'9'); }
		void NUMBER(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file)
		{
			auto thisnode = astnode();
			thisnode.kind = sqfasttypes::NUMBER;
			double number = 0;
			if (code[curoff] == L'$')
			{
				thisnode.kind = sqfasttypes::HEXNUMBER;
				size_t i;
				for (i = curoff + 1; code[i] >= L'0' && code[i] <= L'9' || code[i] >= L'A' && code[i] <= L'F' || code[i] >= L'a' && code[i] <= L'f'; i++);
				auto ident = std::wstring(code + curoff, code + i);
				thisnode.content = ident;
				thisnode.offset = curoff;
				thisnode.length = i - curoff;
				col += i - curoff;
				curoff = i;
			}
			else if (code[curoff] == L'0' && code[curoff + 1] == L'x')
			{
				thisnode.kind = sqfasttypes::HEXNUMBER;
				size_t i;
				for (i = curoff + 2; code[i] >= L'0' && code[i] <= L'9' || code[i] >= L'A' && code[i] <= L'F' || code[i] >= L'a' && code[i] <= L'f'; i++);
				auto ident = std::wstring(code + curoff, code + i);
				thisnode.content = ident;
				thisnode.offset = curoff;
				thisnode.length = i - curoff;
				col += i - curoff;
				curoff = i;
			}
			else
			{
				size_t i = curoff;
				bool numhaddot = false;
				unsigned short numhadexp = 0;
				while (1)
				{
					if (code[i] >= L'0' && code[i] <= L'9')
					{
						i++;
						continue;
					}
					else if (!numhaddot && code[i] == L'.')
					{
						i++;
						numhaddot = true;
						continue;
					}
					else if (numhadexp == 0 && code[i] == L'e' || code[i] == L'E')
					{
						i++;
						numhadexp++;
						continue;
					}
					else if (numhadexp == 1 && code[i] == L'+' || code[i] == L'-')
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
				auto ident = std::wstring(code + curoff, code + i);
				thisnode.content = ident;
				thisnode.offset = curoff;
				thisnode.length = i - curoff;
				col += i - curoff;
				curoff = i;
			}
			root.children.push_back(thisnode);
		}
		//VARIABLE = identifier;
		bool VARIABLE_start(helper &h, const wchar_t* code, size_t curoff) { return identifier(code, curoff) > 0; }
		void VARIABLE(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file)
		{
			auto thisnode = astnode();
			thisnode.kind = sqfasttypes::VARIABLE;
			auto len = identifier(code, curoff);
			auto ident = std::wstring(code + curoff, code + curoff + len);
			thisnode.content = ident;
			thisnode.length = len;
			thisnode.offset = curoff;

			curoff += len;
			col += len;
			root.children.push_back(thisnode);
		}
		//STRING = '"' { any | "\"\"" } '"' | '\'' { any | "''" } '\'';
		bool STRING_start(helper &h, const wchar_t* code, size_t curoff) { return code[curoff] == L'\'' || code[curoff] == L'"'; }
		void STRING(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file)
		{
			auto thisnode = astnode();
			thisnode.kind = sqfasttypes::STRING;
			size_t i;
			auto startchr = code[curoff];
			col++;
			for (i = curoff + 1; code[i] != L'\0' && (code[i] != startchr || code[i + 1] == startchr); i++)
			{
				switch (code[i])
				{
				case L'\'':
				case L'"':
					col += 2;
					i++;
					break;
				case L'\n':
					col = 0;
					line++;
					break;
				default:
					col++;
					break;
				}
			}
			i++;
			col++;
			auto fullstring = std::wstring(code + curoff, code + i);
			thisnode.content = fullstring;
			thisnode.length = i - curoff;
			thisnode.offset = curoff;
			curoff = i;
			root.children.push_back(thisnode);
		}
		//CODE = "{" SQF "}";
		bool CODE_start(helper &h, const wchar_t* code, size_t curoff) { return code[curoff] == L'{'; }
		void CODE(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file)
		{
			auto thisnode = astnode();
			thisnode.kind = sqfasttypes::CODE;
			thisnode.offset = curoff;
			curoff++;
			col++;
			skip(code, line, col, curoff);

			if (SQF_start(h, code, curoff))
			{
				BINARYEXPRESSION(h, thisnode, code, line, col, curoff, file);
			}
			else
			{
				size_t i;
				for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected SQF start." << std::endl;
			}

			if (code[curoff] == L'}')
			{
				curoff++;
				col++;
			}
			else
			{
				size_t i;
				for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected '}'." << std::endl;
			}
			thisnode.length = curoff - thisnode.offset;
			root.children.push_back(thisnode);
		}
		//ARRAY = '[' [ BINARYEXPRESSION { ',' BINARYEXPRESSION } ] ']';
		bool ARRAY_start(helper &h, const wchar_t* code, size_t curoff) { return code[curoff] == L'['; }
		void ARRAY(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file)
		{
			auto thisnode = astnode();
			thisnode.kind = sqfasttypes::ARRAY;
			thisnode.offset = curoff;
			curoff++;
			col++;
			skip(code, line, col, curoff);
			if (BINARYEXPRESSION_start(h, code, curoff))
			{
				BINARYEXPRESSION(h, thisnode, code, line, col, curoff, file);
				skip(code, line, col, curoff);
				while (code[curoff] == L',')
				{
					col++;
					curoff++;
					skip(code, line, col, curoff);

					if (BINARYEXPRESSION_start(h, code, curoff))
					{
						BINARYEXPRESSION(h, thisnode, code, line, col, curoff, file);
						skip(code, line, col, curoff);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected BINARYEXPRESSION start." << std::endl;
					}
				}
			}
			if (code[curoff] == L']')
			{
				curoff++;
				col++;
			}
			else
			{
				size_t i;
				for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected ']'." << std::endl;
			}
			thisnode.length = curoff - thisnode.offset;
			thisnode.content = std::wstring(code + thisnode.offset, code + thisnode.offset + thisnode.length);
			root.children.push_back(thisnode);
		}

		astnode parse_sqf(const wchar_t* codein, helper& h)
		{
			const wchar_t *code = codein;
			size_t line = 0;
			size_t col = 0;
			size_t curoff = 0;
			astnode node;
			node.kind = sqfasttypes::SQF;
			node.offset = 0;
			node.content = codein;
			SQF(h, node, code, line, col, curoff, L"");
			node.length = curoff;
			return node;
		}
	}
}
