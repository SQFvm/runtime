#include <cwctype>
#include <vector>
#include <string>
#include <sstream>
#include "astnode.h"
#include "compiletime.h"
#include "helper.h"
#include "parseconfig.h"
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


namespace sqf
{
	namespace parse
	{
		namespace config
		{
			void skip(const char *code, size_t &curoff)
			{
				while (true)
				{
					switch (code[curoff])
					{
					case ' ': curoff++; continue;
					case '\t': curoff++; continue;
					case '\r': curoff++; continue;
					case '\n': curoff++; continue;
					default: return;
					}
				}
			}
			void skip(const char *code, size_t &line, size_t &col, std::string& file, size_t &curoff)
			{
				while (true)
				{
					switch (code[curoff])
					{
						case ' ': curoff++; col++; continue;
						case '\t': curoff++; col++; continue;
						case '\r': curoff++; col++; continue;
						case '\n': curoff++; line++; col = 0; continue;
						case '#':
							if ((code[curoff + 1] == 'f' && code[curoff + 2] == 'i' && code[curoff + 3] == 'l' && code[curoff + 4] == 'e') ||
								(code[curoff + 1] == 'F' && code[curoff + 2] == 'I' && code[curoff + 3] == 'L' && code[curoff + 4] == 'E'))
							{
								curoff += 4;
								size_t start = curoff + 1;
								for (; code[curoff] != '\0' && code[curoff] != '\n'; curoff++);

								auto str = std::string(code + start, code + curoff);
								file = str;
								break;
							}
							else if ((code[curoff + 1] == 'l' && code[curoff + 2] == 'i' && code[curoff + 3] == 'n' && code[curoff + 4] == 'e') ||
								(code[curoff + 1] == 'L' && code[curoff + 2] == 'I' && code[curoff + 3] == 'N' && code[curoff + 4] == 'E'))
							{
								curoff += 4;
								size_t start = curoff + 1;
								for (; code[curoff] != '\0' && code[curoff] != '\n'; curoff++);
								auto str = std::string(code + start, code + curoff);
								line = (size_t)std::stoul(str.c_str());
								break;
							}
						default: return;
					}
				}
			}

			//endchr = [,;];
			size_t endchr(const char* code, size_t off) { return code[off] == ';' ? 1 : 0; }
			//identifier = [_a-zA-Z0-9]*;
			size_t identifier(const char* code, size_t off) { size_t i = off; for (i = off; (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z') || (code[i] >= '0' && code[i] <= '9') || code[i] == '_'; i++) {}; return i - off; }
			//operator_ = [-*+/a-zA-Z><=%_]+;
			//ToDo: Add clearer non-alphabetical checks (-- should not be detected as SINGLE operator but rather as two operators)
			size_t operator_(const char* code, size_t off) { size_t i; for (i = off; (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z') || code[i] == '+' || code[i] == '-' || code[i] == '*' || code[i] == '/' || code[i] == '>' || code[i] == '<' || code[i] == '=' || code[i] == '%' || code[i] == '_'; i++) {}; return i - off; }
			//hexadecimal = [0-9a-fA-F]+;
			size_t hexadecimal(const char* code, size_t off) { size_t i; for (i = off; (code[i] >= 'a' && code[i] <= 'f') || (code[i] >= 'A' && code[i] <= 'F') || (code[i] >= '0' && code[i] <= '9'); i++) {}; return i - off; }
			//scalarsub = [0-9]+;
			size_t numsub(const char* code, size_t off) { size_t i; for (i = off; code[i] >= '0' && code[i] <= '9'; i++) {}; return i - off; }
			//scalar = scalarsub(.scalarsub)?;
			size_t num(const char* code, size_t off) { size_t i = off + numsub(code, off); if (code[off] == '.') i += numsub(code, off); return i - off; }
			//anytext = (?![ \t\r\n;])+;
			size_t anytext(const char* code, size_t off) { size_t i; for (i = off; code[i] != ' ' && code[i] != '\t' && code[i] != '\r' && code[i] != '\n' && code[i] != ';'; i++) {}; return i - off; }


			//NODELIST = { NODE ';' { ';' } };
			bool NODELIST_start(const char* code, size_t curoff) { return true; }
			void NODELIST(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				//auto thisnode = astnode();
				//thisnode.offset = curoff;
				//thisnode.kind = configasttypes::NODELIST;
				skip(code, line, col, file, curoff);
				//Iterate over statements as long as it is an instruction start.
				while (NODE_start(code, curoff))
				{
					NODE(h, root, code, line, col, curoff, file, errflag);
					skip(code, line, col, file, curoff);
					//Make sure at least one endchr is available unless no statement follows
					if (!endchr(code, curoff) && NODE_start(code, curoff))
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected either ';' or ','." << std::endl;
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
							skip(code, line, col, file, curoff);
						}
					}
				}
				//thisnode.length = curoff - thisnode.offset;
				//root.children.push_back(thisnode);
			}
			//NODE = CONFIGNODE | VALUENODE;
			bool NODE_start(const char* code, size_t curoff) { return CONFIGNODE_start(code, curoff) || VALUENODE_start(code, curoff); }
			void NODE(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
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
					h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "No viable alternative for NODE." << std::endl;
					errflag = true;
				}
				//thisnode.length = curoff - thisnode.offset;
				//root.children.push_back(thisnode);
			}
			//CONFIGNODE = 'class' ident [ ':' ident ] '{' NODELIST '}'
			bool CONFIGNODE_start(const char* code, size_t curoff) { return str_cmpi(code + curoff, compiletime::strlen("class"), "class", compiletime::strlen("class")) == 0; }
			void CONFIGNODE(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				size_t len;
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = configasttypes::CONFIGNODE;

				curoff += compiletime::strlen("class");
				col += compiletime::strlen("class");
				skip(code, line, col, file, curoff);
				std::string ident;
				if ((len = identifier(code, curoff)) > 0)
				{
					ident = std::string(code + curoff, code + curoff + len);
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
					h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected identifier." << std::endl;
					errflag = true;
				}
				skip(code, line, col, file, curoff);
				if (code[curoff] == ':')
				{
					curoff++;
					col++;
					skip(code, line, col, file, curoff);
					if ((len = identifier(code, curoff)) > 0)
					{
						ident = std::string(code + curoff, code + curoff + len);
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
						h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected identifier." << std::endl;
						errflag = true;
					}
					skip(code, line, col, file, curoff);
				}
				if (code[curoff] == '{')
				{
					curoff++;
					col++;
					skip(code, line, col, file, curoff);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected '{'." << std::endl;
					errflag = true;
				}
				NODELIST(h, thisnode, code, line, col, curoff, file, errflag);
				if (code[curoff] == '}')
				{
					curoff++;
					col++;
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected '}'." << std::endl;
					errflag = true;
				}

				thisnode.length = curoff - thisnode.offset;
				root.children.push_back(thisnode);
			}
			//VALUENODE = ident ('=' (STRING | NUMBER | LOCALIZATION) | '[' ']' '=' ARRAY);
			bool VALUENODE_start(const char* code, size_t curoff) { return identifier(code, curoff) > 0; }
			void VALUENODE(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				size_t len;
				bool isarr = false;
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = configasttypes::VALUENODE;

				if ((len = identifier(code, curoff)) > 0)
				{
					auto ident = std::string(code + curoff, code + curoff + len);
					thisnode.content = ident;
					thisnode.line = line;
					thisnode.file = file;
					curoff += len;
					col += len;
					skip(code, line, col, file, curoff);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected identifier." << std::endl;
					errflag = true;
				}

				if (code[curoff] == '[')
				{
					isarr = true;
					curoff++;
					col++;
					skip(code, line, col, file, curoff);
					if (code[curoff] == ']')
					{
						curoff++;
						col++;
						skip(code, line, col, file, curoff);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected ']'." << std::endl;
						errflag = true;
					}
				}
				if (code[curoff] == '=')
				{
					curoff++;
					col++;
					skip(code, line, col, file, curoff);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected '='." << std::endl;
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
						h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected ARRAY." << std::endl;
						errflag = true;
					}
				}
				else
				{
					size_t tmp_line = line;
					size_t tmp_col = col;
					size_t tmp_off = curoff;
					bool was_handled = false;
					if (STRING_start(code, curoff))
					{
						was_handled = true;
						STRING(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else if (NUMBER_start(code, curoff))
					{
						was_handled = true;
						NUMBER(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else if (LOCALIZATION_start(code, curoff))
					{
						was_handled = true;
						LOCALIZATION(h, thisnode, code, line, col, curoff, file, errflag);
					}
					skip(code, line, col, file, curoff);
					if (code[curoff] != ';')
					{
						if (was_handled)
						{
							thisnode.children.pop_back();
						}
						while (code[curoff] != ';' && code[curoff] != '\0')
						{
							curoff++;
						}
						auto magicstringnode = astnode();
						magicstringnode.kind = configasttypes::STRING;
						magicstringnode.col = tmp_col;
						magicstringnode.line = tmp_line;
						magicstringnode.file = file;
						auto fullstring = std::string("\"").append(code + tmp_off, code + curoff).append("\"");
						magicstringnode.content = fullstring;
						magicstringnode.length = curoff - tmp_off;
						magicstringnode.offset = tmp_off;
						thisnode.children.push_back(magicstringnode);
					}
				}

				thisnode.length = curoff - thisnode.offset;
				root.children.push_back(thisnode);
			}
			//STRING = '"' { any | "\"\"" } '"' | '\'' { any | "''" } '\'';
			bool STRING_start(const char* code, size_t curoff) { return code[curoff] == '"' ||code[curoff] == '\''; }
			void STRING(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = configasttypes::STRING;
				thisnode.col = col;
				thisnode.line = line;
				thisnode.file = file;
				size_t i;
				auto startchr = code[curoff];
				col++;
				for (i = curoff + 1; code[i] != '\0' && (code[i] != startchr || code[i + 1] == startchr); i++)
				{
					if (code[i] == startchr)
					{
						col++;
						i++;
					}
					switch (code[i])
					{
					case '\n':
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
				auto fullstring = std::string(code + curoff, code + i);
				thisnode.content = fullstring;
				thisnode.length = i - curoff;
				thisnode.offset = curoff;
				curoff = i;
				root.children.push_back(thisnode);
			}
			//NUMBER = "0x" hexadecimal | [ '-' ]scalar;
			bool NUMBER_start(const char* code, size_t curoff) { return code[curoff] == '-' || (code[curoff] >= '0' && code[curoff] <= '9'); }
			void NUMBER(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = configasttypes::NUMBER;
				thisnode.col = col;
				thisnode.line = line;
				thisnode.file = file;
				if (code[curoff] == '0' && code[curoff + 1] == 'x')
				{
					thisnode.kind = configasttypes::HEXNUMBER;
					size_t i;
					for (i = curoff + 2; (code[i] >= '0' && code[i] <= '9') || (code[i] >= 'A' && code[i] <= 'F') || (code[i] >= 'a' && code[i] <= 'f'); i++);
					auto ident = std::string(code + curoff, code + i);
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
					if (code[i] == '-')
					{
						i++;
					}
					while (true)
					{
						if (code[i] >= '0' && code[i] <= '9')
						{
							i++;
							continue;
						}
						else if (!numhaddot && code[i] == '.')
						{
							i++;
							numhaddot = true;
							continue;
						}
						else if (numhadexp == 0 && (code[i] == 'e' || code[i] == 'E'))
						{
							i++;
							numhadexp++;
							continue;
						}
						else if (numhadexp == 1 && (code[i] == '+' || code[i] == '-'))
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
					auto ident = std::string(code + curoff, code + i);
					thisnode.content = ident;
					thisnode.offset = curoff;
					thisnode.length = i - curoff;
					col += i - curoff;
					curoff = i;
				}
				root.children.push_back(thisnode);
			}
			//LOCALIZATION = '$' ident;
			bool LOCALIZATION_start(const char* code, size_t curoff) { return code[curoff] == '$'; }
			void LOCALIZATION(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = configasttypes::LOCALIZATION;
				thisnode.file = file;
				curoff++;
				col++;
				auto len = identifier(code, curoff);
				auto ident = std::string(code + curoff, code + curoff + len);
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
			bool ARRAY_start(const char* code, size_t curoff) { return code[curoff] == '{'; }
			void ARRAY(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = configasttypes::ARRAY;
				thisnode.offset = curoff;
				thisnode.col = col;
				thisnode.line = line;
				thisnode.file = file;
				curoff++;
				col++;
				skip(code, line, col, file, curoff);
				if (VALUE_start(code, curoff))
				{
					VALUE(h, thisnode, code, line, col, curoff, file, errflag);
					skip(code, line, col, file, curoff);
					while (code[curoff] == ',')
					{
						col++;
						curoff++;
						skip(code, line, col, file, curoff);

						if (VALUE_start(code, curoff))
						{
							VALUE(h, thisnode, code, line, col, curoff, file, errflag);
							skip(code, line, col, file, curoff);
						}
						else
						{
							size_t i;
							for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
							h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected VALUE start." << std::endl;
							errflag = true;
						}
					}
				}
				if (code[curoff] == '}')
				{
					curoff++;
					col++;
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected '}'." << std::endl;
					errflag = true;
				}
				thisnode.length = curoff - thisnode.offset;
				thisnode.content = std::string(code + thisnode.offset, code + thisnode.offset + thisnode.length);
				root.children.push_back(thisnode);
			}
			//VALUE = STRING | NUMBER | LOCALIZATION | ARRAY;
			bool VALUE_start(const char* code, size_t curoff) { return STRING_start(code, curoff) || NUMBER_start(code, curoff) || LOCALIZATION_start(code, curoff) || ARRAY_start(code, curoff); }
			void VALUE(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
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
					h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "No viable alternative for VALUE." << std::endl;
					errflag = true;
				}
				//thisnode.length = curoff - thisnode.offset;
				//root.children.push_back(thisnode);
			}


			astnode parse_config(std::string codein, helper& h, bool &errflag)
			{
				const char *code = codein.c_str();
				size_t line = 1;
				size_t col = 0;
				size_t curoff = 0;
				astnode node;
				std::string file;
				node.kind = configasttypes::NODELIST;
				node.offset = 0;
				node.content = codein;
				NODELIST(h, node, code, line, col, curoff, "", errflag);
				skip(code, line, col, file, curoff);
				if (curoff != codein.length())
				{
					h.err() << h.dbgsegment(code, curoff, 1) << "[ERR][L" << line << "|C" << col << "]\t" << "Parsing config is done but not at EOF." << std::endl;
					errflag = true;
				}
				node.length = curoff;
				return node;
			}
			const char * astkindname(short id)
			{
				switch (id)
				{
				case configasttypes::NODELIST: return "NODELIST";
				case configasttypes::NODE: return "NODE";
				case configasttypes::CONFIGNODE: return "CONFIGNODE";
				case configasttypes::CONFIGNODE_PARENTIDENT: return "CONFIGNODE_PARENTIDENT";
				case configasttypes::VALUENODE: return "VALUENODE";
				case configasttypes::STRING: return "STRING";
				case configasttypes::NUMBER: return "NUMBER";
				case configasttypes::HEXNUMBER: return "HEXNUMBER";
				case configasttypes::LOCALIZATION: return "LOCALIZATION";
				case configasttypes::ARRAY: return "ARRAY";
				case configasttypes::VALUE: return "VALUE";
				default: return "NA";
				}
			}
		}
	}
}