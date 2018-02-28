#include <cwctype>
#include <vector>
#include <string>
#include <sstream>
#include "astnode.h"
#include "compiletime.h"
#include "helper.h"
#include "parseconfig.h"
#include "wstring_op.h"
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


namespace sqf
{
	namespace parse
	{
		namespace config
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
			size_t endchr(const wchar_t* code, size_t off) { return code[off] == L';' ? 1 : 0; }
			//identifier = [_a-zA-Z0-9]*;
			size_t identifier(const wchar_t* code, size_t off) { size_t i = off; for (i = off; (code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z') || (code[i] >= L'0' && code[i] <= L'9') || code[i] == L'_'; i++); return i - off; }
			//operator_ = [-*+/a-zA-Z><=%_]+;
			//ToDo: Add clearer non-alphabetical checks (-- should not be detected as SINGLE operator but rather as two operators)
			size_t operator_(const wchar_t* code, size_t off) { size_t i; for (i = off; (code[i] >= L'a' && code[i] <= L'z') || (code[i] >= L'A' && code[i] <= L'Z') || code[i] == L'+' || code[i] == L'-' || code[i] == L'*' || code[i] == L'/' || code[i] == L'>' || code[i] == L'<' || code[i] == L'=' || code[i] == L'%' || code[i] == L'_'; i++); return i - off; }
			//hexadecimal = [0-9a-fA-F]+;
			size_t hexadecimal(const wchar_t* code, size_t off) { size_t i; for (i = off; (code[i] >= L'a' && code[i] <= L'f') || (code[i] >= L'A' && code[i] <= L'F') || (code[i] >= L'0' && code[i] <= L'9'); i++); return i - off; }
			//scalarsub = [0-9]+;
			size_t numsub(const wchar_t* code, size_t off) { size_t i; for (i = off; code[i] >= L'0' && code[i] <= L'9'; i++); return i - off; }
			//scalar = scalarsub(.scalarsub)?;
			size_t num(const wchar_t* code, size_t off) { size_t i = off + numsub(code, off); if (code[off] == L'.') i += numsub(code, off); return i - off; }
			//anytext = (?![ \t\r\n;])+;
			size_t anytext(const wchar_t* code, size_t off) { size_t i; for (i = off; code[i] != L' ' && code[i] != L'\t' && code[i] != L'\r' && code[i] != L'\n' && code[i] != L';'; i++); return i - off; }


			//NODELIST = { NODE ';' { ';' } };
			bool NODELIST_start(const wchar_t* code, size_t curoff) { return true; }
			void NODELIST(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag)
			{
				//auto thisnode = astnode();
				//thisnode.offset = curoff;
				//thisnode.kind = configasttypes::NODELIST;
				skip(code, line, col, curoff);
				//Iterate over statements as long as it is an instruction start.
				while (NODE_start(code, curoff))
				{
					NODE(h, root, code, line, col, curoff, file, errflag);
					skip(code, line, col, curoff);
					//Make sure at least one endchr is available unless no statement follows
					if (!endchr(code, curoff) && NODE_start(code, curoff))
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected either ';' or ','." << std::endl;
						errflag = true;
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
			//NODE = CONFIGNODE | VALUENODE;
			bool NODE_start(const wchar_t* code, size_t curoff) { return CONFIGNODE_start(code, curoff) || VALUENODE_start(code, curoff); }
			void NODE(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag)
			{
				//auto thisnode = astnode();
				//thisnode.offset = curoff;
				//thisnode.kind = configasttypes::NODE;
				if (CONFIGNODE_start(code, curoff))
				{
					CONFIGNODE(h, root, code, line, col, curoff, file, errflag);
				}
				else if (VALUENODE_start(code, curoff))
				{
					VALUENODE(h, root, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"No viable alternative for NODE." << std::endl;
					errflag = true;
				}
				//thisnode.length = curoff - thisnode.offset;
				//root.children.push_back(thisnode);
			}
			//CONFIGNODE = 'class' ident [ ':' ident ] '{' NODELIST '}'
			bool CONFIGNODE_start(const wchar_t* code, size_t curoff) { return wstr_cmpi(code + curoff, compiletime::strlen(L"class"), L"class", compiletime::strlen(L"class")) == 0; }
			void CONFIGNODE(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag)
			{
				size_t len;
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = configasttypes::CONFIGNODE;

				curoff += compiletime::strlen(L"class");
				col += compiletime::strlen(L"class");
				skip(code, line, col, curoff);
				std::wstring ident;
				if ((len = identifier(code, curoff)) > 0)
				{
					ident = std::wstring(code + curoff, code + curoff + len);
					thisnode.content = ident;
					thisnode.line = line;
					thisnode.file = file;
					curoff += len;
					col += len;
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected identifier." << std::endl;
					errflag = true;
				}
				skip(code, line, col, curoff);
				if (code[curoff] == L':')
				{
					curoff++;
					col++;
					skip(code, line, col, curoff);
					if ((len = identifier(code, curoff)) > 0)
					{
						ident = std::wstring(code + curoff, code + curoff + len);
						astnode parentidentnode;
						parentidentnode.offset = curoff;
						parentidentnode.length = len;
						parentidentnode.line = line;
						parentidentnode.file = file;
						parentidentnode.content = ident;
						parentidentnode.kind = configasttypes::CONFIGNODE_PARENTIDENT;
						curoff += len;
						col += len;
						thisnode.children.push_back(parentidentnode);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected identifier." << std::endl;
						errflag = true;
					}
					skip(code, line, col, curoff);
				}
				if (code[curoff] == L'{')
				{
					curoff++;
					col++;
					skip(code, line, col, curoff);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected '{'." << std::endl;
					errflag = true;
				}
				NODELIST(h, thisnode, code, line, col, curoff, file, errflag);
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
					errflag = true;
				}

				thisnode.length = curoff - thisnode.offset;
				root.children.push_back(thisnode);
			}
			//VALUENODE = ident ('=' (STRING | NUMBER | LOCALIZATION) | '[' ']' '=' ARRAY);
			bool VALUENODE_start(const wchar_t* code, size_t curoff) { return identifier(code, curoff) > 0; }
			void VALUENODE(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag)
			{
				size_t len;
				bool isarr = false;
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = configasttypes::VALUENODE;

				if ((len = identifier(code, curoff)) > 0)
				{
					auto ident = std::wstring(code + curoff, code + curoff + len);
					thisnode.content = ident;
					thisnode.line = line;
					thisnode.file = file;
					curoff += len;
					col += len;
					skip(code, line, col, curoff);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected identifier." << std::endl;
					errflag = true;
				}

				if (code[curoff] == L'[')
				{
					isarr = true;
					curoff++;
					col++;
					skip(code, line, col, curoff);
					if (code[curoff] == L']')
					{
						curoff++;
						col++;
						skip(code, line, col, curoff);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected ']'." << std::endl;
						errflag = true;
					}
				}
				if (code[curoff] == L'=')
				{
					curoff++;
					col++;
					skip(code, line, col, curoff);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected '='." << std::endl;
					errflag = true;
				}
				
				if (isarr)
				{
					if (ARRAY_start(code, curoff))
					{
						ARRAY(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected ARRAY." << std::endl;
						errflag = true;
					}
				}
				else
				{
					if (STRING_start(code, curoff))
					{
						STRING(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else if (NUMBER_start(code, curoff))
					{
						NUMBER(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else if (LOCALIZATION_start(code, curoff))
					{
						LOCALIZATION(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected STRING or NUMBER or LOCALIZATION." << std::endl;
						errflag = true;
					}
				}

				thisnode.length = curoff - thisnode.offset;
				root.children.push_back(thisnode);
			}
			//STRING = '"' { any | "\"\"" } '"' | '\'' { any | "''" } '\'';
			bool STRING_start(const wchar_t* code, size_t curoff) { return code[curoff] == L'"' ||code[curoff] == L'\''; }
			void STRING(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = configasttypes::STRING;
				thisnode.col = col;
				thisnode.line = line;
				thisnode.file = file;
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
			//NUMBER = "0x" hexadecimal | [ '-' ]scalar;
			bool NUMBER_start(const wchar_t* code, size_t curoff) { return code[curoff] == L'-' || (code[curoff] >= L'0' && code[curoff] <= L'9'); }
			void NUMBER(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = configasttypes::NUMBER;
				thisnode.col = col;
				thisnode.line = line;
				thisnode.file = file;
				if (code[curoff] == L'0' && code[curoff + 1] == L'x')
				{
					thisnode.kind = configasttypes::HEXNUMBER;
					size_t i;
					for (i = curoff + 2; (code[i] >= L'0' && code[i] <= L'9') || (code[i] >= L'A' && code[i] <= L'F') || (code[i] >= L'a' && code[i] <= L'f'); i++);
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
					if (code[i] == L'-')
					{
						i++;
					}
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
						else if (numhadexp == 0 && (code[i] == L'e' || code[i] == L'E'))
						{
							i++;
							numhadexp++;
							continue;
						}
						else if (numhadexp == 1 && (code[i] == L'+' || code[i] == L'-'))
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
			//LOCALIZATION = '$' ident;
			bool LOCALIZATION_start(const wchar_t* code, size_t curoff) { return code[curoff] == L'$'; }
			void LOCALIZATION(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = configasttypes::LOCALIZATION;
				thisnode.file = file;
				curoff++;
				col++;
				auto len = identifier(code, curoff);
				auto ident = std::wstring(code + curoff, code + curoff + len);
				thisnode.content = ident;
				thisnode.length = len;
				thisnode.offset = curoff;
				thisnode.col = col;
				thisnode.line = line;

				curoff += len;
				col += len;
				root.children.push_back(thisnode);
			}
			//ARRAY = '{' [ VALUE { ',' VALUE } ] '}'
			bool ARRAY_start(const wchar_t* code, size_t curoff) { return code[curoff] == L'{'; }
			void ARRAY(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = configasttypes::ARRAY;
				thisnode.offset = curoff;
				thisnode.col = col;
				thisnode.line = line;
				thisnode.file = file;
				curoff++;
				col++;
				skip(code, line, col, curoff);
				if (VALUE_start(code, curoff))
				{
					VALUE(h, thisnode, code, line, col, curoff, file, errflag);
					skip(code, line, col, curoff);
					while (code[curoff] == L',')
					{
						col++;
						curoff++;
						skip(code, line, col, curoff);

						if (VALUE_start(code, curoff))
						{
							VALUE(h, thisnode, code, line, col, curoff, file, errflag);
							skip(code, line, col, curoff);
						}
						else
						{
							size_t i;
							for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
							h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"Expected VALUE start." << std::endl;
							errflag = true;
						}
					}
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
					errflag = true;
				}
				thisnode.length = curoff - thisnode.offset;
				thisnode.content = std::wstring(code + thisnode.offset, code + thisnode.offset + thisnode.length);
				root.children.push_back(thisnode);
			}
			//VALUE = STRING | NUMBER | LOCALIZATION | ARRAY;
			bool VALUE_start(const wchar_t* code, size_t curoff) { return true; }
			void VALUE(helper &h, astnode &root, const wchar_t* code, size_t &line, size_t &col, size_t &curoff, const wchar_t* file, bool &errflag)
			{
				//auto thisnode = astnode();
				//thisnode.offset = curoff;
				//thisnode.kind = configasttypes::VALUE;
				if (STRING_start(code, curoff))
				{
					STRING(h, root, code, line, col, curoff, file, errflag);
				}
				else if (NUMBER_start(code, curoff))
				{
					NUMBER(h, root, code, line, col, curoff, file, errflag);
				}
				else if (LOCALIZATION_start(code, curoff))
				{
					LOCALIZATION(h, root, code, line, col, curoff, file, errflag);
				}
				else if (ARRAY_start(code, curoff))
				{
					ARRAY(h, root, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << L"[ERR][L" << line << L"|C" << col << L"]\t" << L"No viable alternative for VALUE." << std::endl;
					errflag = true;
				}
				//thisnode.length = curoff - thisnode.offset;
				//root.children.push_back(thisnode);
			}


			astnode parse_config(const wchar_t* codein, helper& h, bool &errflag)
			{
				const wchar_t *code = codein;
				size_t line = 0;
				size_t col = 0;
				size_t curoff = 0;
				astnode node;
				node.kind = configasttypes::NODELIST;
				node.offset = 0;
				node.content = codein;
				NODELIST(h, node, code, line, col, curoff, L"", errflag);
				node.length = curoff;
				return node;
			}
			const wchar_t * astkindname(short id)
			{
				switch (id)
				{
				case configasttypes::NODELIST: return L"NODELIST";
				case configasttypes::NODE: return L"NODE";
				case configasttypes::CONFIGNODE: return L"CONFIGNODE";
				case configasttypes::CONFIGNODE_PARENTIDENT: return L"CONFIGNODE_PARENTIDENT";
				case configasttypes::VALUENODE: return L"VALUENODE";
				case configasttypes::STRING: return L"STRING";
				case configasttypes::NUMBER: return L"NUMBER";
				case configasttypes::HEXNUMBER: return L"HEXNUMBER";
				case configasttypes::LOCALIZATION: return L"LOCALIZATION";
				case configasttypes::ARRAY: return L"ARRAY";
				case configasttypes::VALUE: return L"VALUE";
				default: return L"NA";
				}
			}
		}
	}
}