#include "astnode.h"
#include "compiletime.h"
#include "helper.h"
#include "parsesqf.h"
#include "string_op.h"
#include <algorithm>
#include <cwctype>
#include <vector>
#include <string>
#include <sstream>

namespace sqf
{
	namespace parse
	{
		namespace sqf
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
								curoff += 5;
								size_t start = curoff + 1;
								for (; code[curoff] != '\0' && code[curoff] != '\n'; curoff++);

								auto str = std::string(code + start, code + curoff);
								file = str;
								break;
							}
							else if ((code[curoff + 1] == 'l' && code[curoff + 2] == 'i' && code[curoff + 3] == 'n' && code[curoff + 4] == 'e') ||
								(code[curoff + 1] == 'L' && code[curoff + 2] == 'I' && code[curoff + 3] == 'N' && code[curoff + 4] == 'E'))
							{
								curoff += 5;
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
			size_t endchr(const char* code, size_t off) { return code[off] == ';' || code[off] == ',' ? 1 : 0; }
			//identifier = [_a-zA-Z][_a-zA-Z0-9]*;
			size_t identifier(const char* code, size_t off) { size_t i = off; if (!((code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z') || code[i] == '_')) return 0; for (i = off + 1; (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z') || (code[i] >= '0' && code[i] <= '9') || code[i] == '_'; i++) {}; return i - off; }
			//identifier = [_a-zA-Z][_a-zA-Z0-9]+;
			size_t assidentifier(const char* code, size_t off)
			{
				size_t i = off;
				if (!((code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z') || code[i] == '_'))
				{
					return 0;
				}

				for (i++; (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z') || (code[i] >= '0' && code[i] <= '9') || code[i] == '_'; i++);
				return i - off;
			}
			//operator_ = [+-*/%^]|&&|\|\||==|[!<>][=]?|[a-zA-Z_]+;
			size_t operator_(const char* code, size_t off) {
				if (code[off] == '+' || code[off] == '-' || code[off] == '*' || code[off] == '/' || code[off] == '%' || code[off] == '^' || code[off] == ':' || code[off] == '#') return 1;
				if ((code[off] == '|' && code[off + 1] == '|') || (code[off] == '&' && code[off + 1] == '&') || (code[off] == '=' && code[off + 1] == '=') || (code[off] == '>' && code[off + 1] == '>')) return 2;
				if (code[off] == '<' || code[off] == '>' || code[off] == '!')
				{
					if (code[off + 1] == '=') return 2;
					return 1;
				}
				size_t i;
				for (i = off; (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z') || (code[i] >= '0' && code[i] <= '9') || code[i] == '_'; i++);
				return i - off;
			}
			//hexadecimal = [0-9a-fA-F]+;
			size_t hexadecimal(const char* code, size_t off) { size_t i; for (i = off; (code[i] >= 'a' && code[i] <= 'f') || (code[i] >= 'A' && code[i] <= 'F') || (code[i] >= '0' && code[i] <= '9'); i++) {}; return i - off; }
			//scalarsub = [0-9]+;
			size_t scalarsub(const char* code, size_t off) { size_t i; for (i = off; code[i] >= '0' && code[i] <= '9'; i++) {}; return i - off; }
			//scalar = scalarsub(.scalarsub)?;
			size_t scalar(const char* code, size_t off) { size_t i = off + scalarsub(code, off); if (code[off] == '.') i += scalarsub(code, off); return i - off; }
			//anytext = (?![ \t\r\n;])+;
			size_t anytext(const char* code, size_t off) { size_t i; for (i = off; code[i] != ' ' && code[i] != '\t' && code[i] != '\r' && code[i] != '\n' && code[i] != ';'; i++) {}; return i - off; }
			//SQF = [ STATEMENT { endchr { endchr } STATEMENT } ]
			bool SQF_start(helper &h, const char* code, size_t curoff) { return true; }
			void SQF(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				//auto thisnode = astnode();
				//thisnode.offset = curoff;
				//thisnode.kind = sqfasttypes::SQF;
				//thisnode.file = file;
				skip(code, line, col, file, curoff);
				size_t endchrlen;
				while ((endchrlen = endchr(code, curoff)) > 0)
				{
					curoff += endchrlen;
					col += endchrlen;
					skip(code, line, col, file, curoff);
				}
				//Iterate over statements as long as it is an instruction start.
				while (STATEMENT_start(h, code, curoff))
				{
					STATEMENT(h, root, code, line, col, curoff, file, errflag);
					skip(code, line, col, file, curoff);
					//Make sure at least one endchr is available unless no statement follows
					if (!endchr(code, curoff) && STATEMENT_start(h, code, curoff))
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected either ';' or ','." << std::endl; }
						else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected either ';' or ','." << std::endl; }
						errflag = true;
					}
					else
					{
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
			//STATEMENT = ASSIGNMENT | BINARYEXPRESSION;
			bool STATEMENT_start(helper &h, const char* code, size_t curoff) { return ASSIGNMENT_start(h, code, curoff) | BINARYEXPRESSION_start(h, code, curoff); }
			void STATEMENT(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				//auto thisnode = astnode();
				//thisnode.offset = curoff;
				//thisnode.kind = sqfasttypes::STATEMENT;
				//thisnode.file = file;
				if (ASSIGNMENT_start(h, code, curoff))
				{
					ASSIGNMENT(h, root, code, line, col, curoff, file, errflag);
				}
				else if (BINARYEXPRESSION_start(h, code, curoff))
				{
					BINARYEXPRESSION(h, root, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "No viable alternative for STATEMENT." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "No viable alternative for STATEMENT." << std::endl; }
					errflag = true;
				}
				//thisnode.length = curoff - thisnode.offset;
				//root.children.push_back(thisnode);
			}
			//ASSIGNMENT(2) = assidentifier '=' BINARYEXPRESSION | "private" assidentifier '=' BINARYEXPRESSION;
			bool ASSIGNMENT_start(helper &h, const char* code, size_t curoff)
			{
				size_t len;
				if (str_cmpi(code + curoff, compiletime::strlen("private"), "private", compiletime::strlen("private")) == 0)
				{
					curoff += compiletime::strlen("private");
					skip(code, curoff);
				}
				if ((len = assidentifier(code, curoff)) > 0)
				{
					curoff += len;
					skip(code, curoff);
					return code[curoff] == '=' && code[curoff + 1] != '=';
				}
				else
				{
					return false;
				}
			}
			void ASSIGNMENT(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = sqfasttypes::ASSIGNMENT;
				thisnode.offset = curoff;
				thisnode.file = file;
				size_t len;
				bool assignlocal = false;
				//check if prefixed by a 'private'
				if (str_cmpi(code + curoff, compiletime::strlen("private"), "private", compiletime::strlen("private")) == 0)
				{
					curoff += compiletime::strlen("private");
					col += compiletime::strlen("private");
					skip(code, line, col, file, curoff);
					assignlocal = true;
					thisnode.kind = sqfasttypes::ASSIGNMENTLOCAL;
				}
				//receive the ident
				len = assidentifier(code, curoff);
				auto ident = std::string(code + curoff, code + curoff + len);

				auto varnode = astnode();
				varnode.offset = curoff;
				varnode.length = len;
				varnode.content = ident;
				varnode.kind = sqfasttypes::VARIABLE;
				varnode.col = col;
				varnode.line = line;
				varnode.file = file;
				thisnode.children.emplace_back(std::move(varnode));

				if (assignlocal && ident[0] != '_')
				{
					h.err() << h.dbgsegment(code, curoff, len) << "[ERR][L" << line << "|C" << col << "]\t" << "Private variables need to be prefixed with an underscore '_'." << std::endl;
					errflag = true;
				}
				curoff += len;
				skip(code, line, col, file, curoff);
				//store the dbginf for the '='
				//size_t dbgstart = curoff;
				//size_t dbgcol = col;
				//size_t dbgline = line;
				thisnode.col = col;
				thisnode.line = line;
				//skip the '=' (is confirmed to be present in ASSIGNMENT_start)
				curoff++;
				col++;
				skip(code, line, col, file, curoff);

				if (BINARYEXPRESSION_start(h, code, curoff))
				{
					BINARYEXPRESSION(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				thisnode.length = curoff - thisnode.offset;
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
			void bexp_orderfix(astnode& root, astnode thisnode, short plevel)
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
			bool bexp10_start(helper &h, const char* code, size_t curoff) { return PRIMARYEXPRESSION_start(h, code, curoff); }
			void bexp10(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = sqfasttypes::BEXP10;
				thisnode.file = file;
                thisnode.line = line;
				size_t oplen;
				std::string op;
				skip(code, line, col, file, curoff);
				if (PRIMARYEXPRESSION_start(h, code, curoff))
				{
					PRIMARYEXPRESSION(h, thisnode, code, line, col, curoff, file, errflag);
				}

				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				skip(code, line, col, file, curoff);
				if ((oplen = operator_(code, curoff)) > 0 && h.contains_binary(op = std::string(code + curoff, code + curoff + oplen), 10))
				{
					auto node = astnode();
					node.content = op;
					node.offset = curoff;
					node.kind = sqfasttypes::BINARYOP;
					node.col = col;
					node.line = line;
					node.length = oplen;
					node.file = file;
                    thisnode.children.emplace_back(std::move(node));
					curoff += oplen;
					skip(code, line, col, file, curoff);
					if (bexp10_start(h, code, curoff))
					{
						bexp10(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Missing RARG for binary operator." << std::endl; }
						else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Missing RARG for binary operator." << std::endl; }
						errflag = true;
					}
					bexp_orderfix(root, std::move(thisnode), sqfasttypes::BEXP10);
				}
				else
				{
                    //We won't need this node anymore. Just move all children to root
                    root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
				}
			}
			bool bexp9_start(helper &h, const char* code, size_t curoff) { return PRIMARYEXPRESSION_start(h, code, curoff); }
			void bexp9(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = sqfasttypes::BEXP9;
				thisnode.file = file;
                thisnode.line = line;
				size_t oplen;
				std::string op;
				skip(code, line, col, file, curoff);
				if (bexp10_start(h, code, curoff))
				{
					bexp10(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				skip(code, line, col, file, curoff);
				if ((oplen = operator_(code, curoff)) > 0 && h.contains_binary(op = std::string(code + curoff, code + curoff + oplen), 9))
				{
					auto node = astnode();
					node.content = op;
					node.offset = curoff;
					node.kind = sqfasttypes::BINARYOP;
					node.col = col;
					node.line = line;
					node.length = oplen;
					node.file = file;
					thisnode.children.emplace_back(std::move(node));
					curoff += oplen;
					skip(code, line, col, file, curoff);
					if (bexp9_start(h, code, curoff))
					{
						bexp9(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Missing RARG for binary operator." << std::endl; }
						else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Missing RARG for binary operator." << std::endl; }
						errflag = true;
					}
					bexp_orderfix(root, std::move(thisnode), sqfasttypes::BEXP9);
				}
				else
				{
                    //We won't need this node anymore. Just move all children to root
                    root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
				}
			}
			bool bexp8_start(helper &h, const char* code, size_t curoff) { return PRIMARYEXPRESSION_start(h, code, curoff); }
			void bexp8(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = sqfasttypes::BEXP8;
				thisnode.file = file;
                thisnode.line = line;
				size_t oplen;
				std::string op;
				skip(code, line, col, file, curoff);
				if (bexp9_start(h, code, curoff))
				{
					bexp9(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				skip(code, line, col, file, curoff);
				if ((oplen = operator_(code, curoff)) > 0 && h.contains_binary(op = std::string(code + curoff, code + curoff + oplen), 8))
				{
					auto node = astnode();
					node.content = op;
					node.offset = curoff;
					node.kind = sqfasttypes::BINARYOP;
					node.col = col;
					node.line = line;
					node.length = oplen;
					node.file = file;
					thisnode.children.emplace_back(std::move(node));
					curoff += oplen;
					skip(code, line, col, file, curoff);
					if (bexp8_start(h, code, curoff))
					{
						bexp8(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Missing RARG for binary operator." << std::endl; }
						else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Missing RARG for binary operator." << std::endl; }
						errflag = true;
					}
					bexp_orderfix(root, std::move(thisnode), sqfasttypes::BEXP8);
				}
				else
				{
                    //We won't need this node anymore. Just move all children to root
                    root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
				}
			}
			bool bexp7_start(helper &h, const char* code, size_t curoff) { return PRIMARYEXPRESSION_start(h, code, curoff); }
			void bexp7(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = sqfasttypes::BEXP7;
				thisnode.file = file;
                thisnode.line = line;
				size_t oplen;
				std::string op;
				skip(code, line, col, file, curoff);
				if (bexp8_start(h, code, curoff))
				{
					bexp8(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				skip(code, line, col, file, curoff);
				if ((oplen = operator_(code, curoff)) > 0 && h.contains_binary(op = std::string(code + curoff, code + curoff + oplen), 7))
				{
					auto node = astnode();
					node.content = op;
					node.offset = curoff;
					node.kind = sqfasttypes::BINARYOP;
					node.col = col;
					node.line = line;
					node.length = oplen;
					node.file = file;
					thisnode.children.emplace_back(std::move(node));
					curoff += oplen;
					skip(code, line, col, file, curoff);
					if (bexp7_start(h, code, curoff))
					{
						bexp7(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Missing RARG for binary operator." << std::endl; }
						else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Missing RARG for binary operator." << std::endl; }
						errflag = true;
					}
					bexp_orderfix(root, std::move(thisnode), sqfasttypes::BEXP7);
				}
				else
				{
                    //We won't need this node anymore. Just move all children to root
                    root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
				}
			}
			bool bexp6_start(helper &h, const char* code, size_t curoff) { return PRIMARYEXPRESSION_start(h, code, curoff); }
			void bexp6(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = sqfasttypes::BEXP6;
				thisnode.file = file;
                thisnode.line = line;
				size_t oplen;
				std::string op;
				skip(code, line, col, file, curoff);
				if (bexp7_start(h, code, curoff))
				{
					bexp7(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				skip(code, line, col, file, curoff);
				if ((oplen = operator_(code, curoff)) > 0 && h.contains_binary(op = std::string(code + curoff, code + curoff + oplen), 6))
				{
					auto node = astnode();
					node.content = op;
					node.offset = curoff;
					node.kind = sqfasttypes::BINARYOP;
					node.col = col;
					node.line = line;
					node.length = oplen;
					node.file = file;
					thisnode.children.emplace_back(std::move(node));
					curoff += oplen;
					skip(code, line, col, file, curoff);
					if (bexp6_start(h, code, curoff))
					{
						bexp6(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Missing RARG for binary operator." << std::endl; }
						else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Missing RARG for binary operator." << std::endl; }
						errflag = true;
					}
					bexp_orderfix(root, std::move(thisnode), sqfasttypes::BEXP6);
				}
				else
				{
                    //We won't need this node anymore. Just move all children to root
                    root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
				}
			}
			bool bexp5_start(helper &h, const char* code, size_t curoff) { return PRIMARYEXPRESSION_start(h, code, curoff); }
			void bexp5(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = sqfasttypes::BEXP5;
				thisnode.file = file;
                thisnode.line = line;
				size_t oplen;
				std::string op;
				skip(code, line, col, file, curoff);
				if (bexp6_start(h, code, curoff))
				{
					bexp6(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				skip(code, line, col, file, curoff);
				if ((oplen = operator_(code, curoff)) > 0 && h.contains_binary(op = std::string(code + curoff, code + curoff + oplen), 5))
				{
					auto node = astnode();
					node.content = op;
					node.offset = curoff;
					node.kind = sqfasttypes::BINARYOP;
					node.col = col;
					node.line = line;
					node.length = oplen;
					node.file = file;
					thisnode.children.emplace_back(std::move(node));
					curoff += oplen;
					skip(code, line, col, file, curoff);
					if (bexp5_start(h, code, curoff))
					{
						bexp5(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Missing RARG for binary operator." << std::endl; }
						else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Missing RARG for binary operator." << std::endl; }
						errflag = true;
					}
					bexp_orderfix(root, std::move(thisnode), sqfasttypes::BEXP5);
				}
				else
				{
                    //We won't need this node anymore. Just move all children to root
                    root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
				}
			}
			bool bexp4_start(helper &h, const char* code, size_t curoff) { return PRIMARYEXPRESSION_start(h, code, curoff); }
			void bexp4(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = sqfasttypes::BEXP4;
				thisnode.file = file;
                thisnode.line = line;
				size_t oplen;
				std::string op;
				skip(code, line, col, file, curoff);
				if (bexp5_start(h, code, curoff))
				{
					bexp5(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				skip(code, line, col, file, curoff);
				if ((oplen = operator_(code, curoff)) > 0 && h.contains_binary(op = std::string(code + curoff, code + curoff + oplen), 4))
				{
					auto node = astnode();
					node.content = op;
					node.offset = curoff;
					node.kind = sqfasttypes::BINARYOP;
					node.col = col;
					node.line = line;
					node.length = oplen;
					node.file = file;
					thisnode.children.emplace_back(std::move(node));
					curoff += oplen;
					skip(code, line, col, file, curoff);
					if (bexp4_start(h, code, curoff))
					{
						bexp4(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Missing RARG for binary operator." << std::endl; }
						else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Missing RARG for binary operator." << std::endl; }
						errflag = true;
					}
					bexp_orderfix(root, std::move(thisnode), sqfasttypes::BEXP4);
				}
				else
				{
                    //We won't need this node anymore. Just move all children to root
                    root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
				}
			}
			bool bexp3_start(helper &h, const char* code, size_t curoff) { return PRIMARYEXPRESSION_start(h, code, curoff); }
			void bexp3(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = sqfasttypes::BEXP3;
				thisnode.file = file;
                thisnode.line = line;
				size_t oplen;
				std::string op;
				skip(code, line, col, file, curoff);
				if (bexp4_start(h, code, curoff))
				{
					bexp4(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				skip(code, line, col, file, curoff);
				if ((oplen = operator_(code, curoff)) > 0 && h.contains_binary(op = std::string(code + curoff, code + curoff + oplen), 3))
				{
					auto node = astnode();
					node.content = op;
					node.offset = curoff;
					node.kind = sqfasttypes::BINARYOP;
					node.col = col;
					node.line = line;
					node.length = oplen;
					node.file = file;
					thisnode.children.emplace_back(std::move(node));
					curoff += oplen;
					skip(code, line, col, file, curoff);
					if (bexp3_start(h, code, curoff))
					{
						bexp3(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Missing RARG for binary operator." << std::endl; }
						else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Missing RARG for binary operator." << std::endl; }
						errflag = true;
					}
					bexp_orderfix(root, std::move(thisnode), sqfasttypes::BEXP3);
				}
				else
				{
                    //We won't need this node anymore. Just move all children to root
                    root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
				}
			}
			bool bexp2_start(helper &h, const char* code, size_t curoff) { return PRIMARYEXPRESSION_start(h, code, curoff); }
			void bexp2(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = sqfasttypes::BEXP2;
                thisnode.file = file;
                thisnode.line = line;
				size_t oplen;
				std::string op;
				skip(code, line, col, file, curoff);
				if (bexp3_start(h, code, curoff))
				{
					bexp3(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				skip(code, line, col, file, curoff);
				if ((oplen = operator_(code, curoff)) > 0 && h.contains_binary(op = std::string(code + curoff, code + curoff + oplen), 2))
				{
					auto node = astnode();
					node.content = op;
					node.offset = curoff;
					node.kind = sqfasttypes::BINARYOP;
					node.col = col;
					node.line = line;
					node.length = oplen;
					node.file = file;
					thisnode.children.emplace_back(std::move(node));
					curoff += oplen;
					skip(code, line, col, file, curoff);
					if (bexp2_start(h, code, curoff))
					{
						bexp2(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Missing RARG for binary operator." << std::endl; }
						else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Missing RARG for binary operator." << std::endl; }
						errflag = true;
					}
					bexp_orderfix(root, std::move(thisnode), sqfasttypes::BEXP2);
				}
				else
				{
                    //We won't need this node anymore. Just move all children to root
                    root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
				}
			}
			bool bexp1_start(helper &h, const char* code, size_t curoff) { return PRIMARYEXPRESSION_start(h, code, curoff); }
			void bexp1(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.offset = curoff;
				thisnode.kind = sqfasttypes::BEXP1;
				thisnode.file = file;
                thisnode.line = line;
				size_t oplen;
				std::string op;
				skip(code, line, col, file, curoff);
				if (bexp2_start(h, code, curoff))
				{
					bexp2(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				skip(code, line, col, file, curoff);
				if ((oplen = operator_(code, curoff)) > 0 && h.contains_binary(op = std::string(code + curoff, code + curoff + oplen), 1))
				{
					auto node = astnode();
					node.content = op;
					node.offset = curoff;
					node.kind = sqfasttypes::BINARYOP;
					node.col = col;
					node.line = line;
					node.length = oplen;
					node.file = file;
					thisnode.children.emplace_back(std::move(node));
					curoff += oplen;
					skip(code, line, col, file, curoff);
					if (bexp1_start(h, code, curoff))
					{
						bexp1(h, thisnode, code, line, col, curoff, file, errflag);
					}
					else
					{
						size_t i;
						for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
						if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Missing RARG for binary operator." << std::endl; }
						else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Missing RARG for binary operator." << std::endl; }
						errflag = true;
					}
					bexp_orderfix(root, std::move(thisnode), sqfasttypes::BEXP1);
				}
				else
				{
                    //We won't need this node anymore. Just move all children to root
                    root.children.insert(root.children.end(), std::make_move_iterator(thisnode.children.begin()), std::make_move_iterator(thisnode.children.end()));
				}
			}
			bool BINARYEXPRESSION_start(helper &h, const char* code, size_t curoff) { return PRIMARYEXPRESSION_start(h, code, curoff); }
			void BINARYEXPRESSION(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				bexp1(h, root, code, line, col, curoff, file, errflag);
				//auto thisnode = astnode();
				//thisnode.offset = curoff;
				//thisnode.kind = sqfasttypes::BINARYSTATEMENT;
				//thisnode.file = file;
				//size_t oplen;
				//std::string op;
				////Get all nodes for current expression
				//while (true)
				//{
				//	skip(code, line, col, file, curoff);
				//	if (thisnode.children.size() % 2 == 1 && (oplen = operator_(code, curoff)) > 0 && h.contains_binary(op = std::string(code + curoff, code /+/ curoff + oplen)))
				//	{
				//		auto node = astnode();
				//		node.content = op;
				//		node.offset = curoff;
				//		node.kind = sqfasttypes::BINARYOP;
				//		node.col = col;
				//		node.line = line;
				//		node.length = oplen;
				//		node.file = file;
				//		thisnode.children.push_back(node);
				//		curoff += oplen;
				//	}
				//	else if (PRIMARYEXPRESSION_start(h, code, curoff))
				//	{
				//		PRIMARYEXPRESSION(h, thisnode, code, line, col, curoff, file, errflag);
				//	}
				//	else
				//	{
				//		break;
				//	}
				//}
				////Group the BinaryExpressions
				//auto vec = thisnode.children;
				//thisnode.children = std::vector<astnode>();
				//std::reverse(vec.begin(), vec.end());
				//auto curnode = astnode();
				//curnode.kind = sqfasttypes::BINARYEXPRESSION;
				//bool isOrig = true;
				//while (!vec.empty())
				//{
				//	auto it = vec.back();
				//	vec.pop_back();
				//
				//	curnode.children.push_back(it);
				//	if (curnode.children.size() == (isOrig ? 3 : 2))
				//	{
				//		if (curnode.children.size() == (isOrig ? 3 : 2) && curnode.children[(isOrig ? 1 : 0)].kind != sqfasttypes::BINARYOP)
				//		{
				//			size_t i;
				//			for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				//			h.err() << h.dbgsegment(code, curnode.offset, curnode.length) << "[ERR][L" << curnode.line << "|C" << curnode.col << "]\t" /<< /"Expected binary operator." << std::endl;
				//			errflag = true;
				//		}
				//		thisnode.children.push_back(curnode);
				//		curnode = astnode();
				//		curnode.kind = sqfasttypes::BINARYEXPRESSION;
				//		isOrig = false;
				//	}
				//}
				//if (curnode.children.size() == 1)
				//{
				//	auto tmp = curnode.children[0];
				//	curnode = tmp;
				//	thisnode.children.push_back(curnode);
				//}
				//else if (isOrig && curnode.children.size() == 2)
				//{
				//	size_t i;
				//	for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
				//	h.err() << h.dbgsegment(code, curnode.children.back().offset, curnode.children.back().length) << "[ERR][L" << curnode.line << "|C" << //curnode.col << "]\t" << "Missing RARG for binary operator." << std::endl;
				//	errflag = true;
				//}
				//else if (curnode.children.size() != 0)
				//{
				//	thisnode.children.push_back(curnode);
				//}
				//
				////Exit if no sorting is required
				//if (thisnode.children.size() <= 1)
				//{
				//	root.children.push_back(thisnode.children.size() == 1 ? thisnode.children.back() : thisnode);
				//	return;
				//}
				////Sort according to precedence
				//vec = thisnode.children;
				//thisnode.children = std::vector<astnode>();
				//std::reverse(vec.begin(), vec.end());
				//
				//curnode = vec.back();
				//vec.pop_back();
				//auto curprec = h.precedence(curnode.children[1].content);
				//while (!vec.empty())
				//{
				//	auto node = vec.back();
				//	vec.pop_back();
				//	auto prec = h.precedence(node.children[0].content);
				//	node.children.push_back(node.children[1]);
				//	node.children[1] = node.children[0];
				//	if (curprec < prec)
				//	{
				//		node.children[0] = curnode.children.back();
				//		curnode.children.back() = node;
				//	}
				//	else
				//	{
				//		node.children[0] = curnode;
				//		curnode = node;
				//	}
				//}
				//thisnode.children.push_back(curnode);
				//root.children.push_back(thisnode);
			}
			//BRACKETS = '(' BINARYEXPRESSION ')';
			bool BRACKETS_start(helper &h, const char* code, size_t curoff) { return code[curoff] == '('; }
			void BRACKETS(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = sqfasttypes::BRACKETS;
				thisnode.offset = curoff;
				thisnode.file = file;
				curoff++;
				col++;
				skip(code, line, col, file, curoff);
				if (BINARYEXPRESSION_start(h, code, curoff))
				{
					BINARYEXPRESSION(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of BINARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				skip(code, line, col, file, curoff);
				if (code[curoff] == ')')
				{
					curoff++;
					col++;
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected ')'." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected ')'." << std::endl; }
					errflag = true;
				}
				thisnode.length = curoff - thisnode.offset;
				root.children.emplace_back(std::move(thisnode));
			}
			//PRIMARYEXPRESSION = NUMBER | UNARYEXPRESSION | NULAREXPRESSION | VARIABLE | STRING | CODE | BRACKETS | ARRAY;
			bool PRIMARYEXPRESSION_start(helper &h, const char* code, size_t curoff) { return NUMBER_start(h, code, curoff) || UNARYEXPRESSION_start(h, code, curoff) || NULAREXPRESSION_start(h, code, curoff) || VARIABLE_start(h, code, curoff) || STRING_start(h, code, curoff) || CODE_start(h, code, curoff) || BRACKETS_start(h, code, curoff) || ARRAY_start(h, code, curoff); }
			void PRIMARYEXPRESSION(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				//auto thisnode = astnode();
				//thisnode.kind = sqfasttypes::PRIMARYEXPRESSION;
				//thisnode.offset = curoff;
				//thisnode.file = file;
				if (NUMBER_start(h, code, curoff))
				{
					NUMBER(h, root, code, line, col, curoff, file, errflag);
				}
				else if (UNARYEXPRESSION_start(h, code, curoff))
				{
					UNARYEXPRESSION(h, root, code, line, col, curoff, file, errflag);
				}
				else if (NULAREXPRESSION_start(h, code, curoff))
				{
					NULAREXPRESSION(h, root, code, line, col, curoff, file, errflag);
				}
				else if (VARIABLE_start(h, code, curoff))
				{
					VARIABLE(h, root, code, line, col, curoff, file, errflag);
				}
				else if (STRING_start(h, code, curoff))
				{
					STRING(h, root, code, line, col, curoff, file, errflag);
				}
				else if (CODE_start(h, code, curoff))
				{
					CODE(h, root, code, line, col, curoff, file, errflag);
				}
				else if (BRACKETS_start(h, code, curoff))
				{
					BRACKETS(h, root, code, line, col, curoff, file, errflag);
				}
				else if (ARRAY_start(h, code, curoff))
				{
					ARRAY(h, root, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "No viable alternative for PRIMARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "No viable alternative for PRIMARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				//thisnode.length = curoff - thisnode.offset;
				//root.children.push_back(thisnode);
			}
			//NULAREXPRESSION = operator;
			bool NULAREXPRESSION_start(helper &h, const char* code, size_t curoff) { auto oplen = operator_(code, curoff); return oplen > 0 ? h.contains_nular(std::string(code + curoff, code + curoff + oplen)) : false; }
			void NULAREXPRESSION(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = sqfasttypes::NULAROP;
				thisnode.file = file;
				auto len = operator_(code, curoff);
				auto ident = std::string(code + curoff, code + curoff + len);
				thisnode.content = ident;
				thisnode.length = len;
				thisnode.offset = curoff;
				thisnode.col = col;
				thisnode.line = line;
				curoff += len;
				col += len;
				root.children.emplace_back(std::move(thisnode));
			}
			//UNARYEXPRESSION = operator PRIMARYEXPRESSION;
			bool UNARYEXPRESSION_start(helper &h, const char* code, size_t curoff) { auto oplen = operator_(code, curoff); return oplen > 0 ? h.contains_unary(std::string(code + curoff, code + curoff + oplen)) : false; }
			void UNARYEXPRESSION(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = sqfasttypes::UNARYEXPRESSION;
				thisnode.offset = curoff;
				thisnode.file = file;
				//size_t dbgstart = curoff;
				//size_t dbgcol = col;
				//size_t dbgline = line;

				auto len = operator_(code, curoff);
				auto ident = std::string(code + curoff, code + curoff + len);
				auto opnode = astnode();
				opnode.kind = sqfasttypes::UNARYOP;
				opnode.offset = curoff;
				opnode.length = len;
				opnode.content = ident;
				opnode.col = col;
				opnode.file = file;
				opnode.line = line;
				thisnode.children.emplace_back(std::move(opnode));
				curoff += len;
				col += len;
				skip(code, line, col, file, curoff);

				if (PRIMARYEXPRESSION_start(h, code, curoff))
				{
					PRIMARYEXPRESSION(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected start of PRIMARYEXPRESSION." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected start of PRIMARYEXPRESSION." << std::endl; }
					errflag = true;
				}
				thisnode.length = curoff - thisnode.offset;
				root.children.emplace_back(std::move(thisnode));
			}
			//NUMBER = ("0x" | '$' | '.') hexadecimal | scalar;
			bool NUMBER_start(helper &h, const char* code, size_t curoff) { return code[curoff] == '$' || code[curoff] == '.' || (code[curoff] >= '0' && code[curoff] <= '9'); }
			void NUMBER(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = sqfasttypes::NUMBER;
				thisnode.col = col;
				thisnode.line = line;
				thisnode.file = file;
				if (code[curoff] == '$')
				{
					thisnode.kind = sqfasttypes::HEXNUMBER;
					size_t i;
					for (i = curoff + 1; (code[i] >= '0' && code[i] <= '9') || (code[i] >= 'A' && code[i] <= 'F') || (code[i] >= 'a' && code[i] <= 'f'); i++);
					auto ident = std::string(code + curoff + 1, code + i);
					thisnode.content = ident;
					thisnode.offset = curoff;
					thisnode.length = i - curoff;
					col += i - curoff;
					curoff = i;
				}
				else if (code[curoff] == '0' && code[curoff + 1] == 'x')
				{
					thisnode.kind = sqfasttypes::HEXNUMBER;
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
				if (thisnode.content.empty())
				{
					errflag = true;
					size_t i;
					for (i = thisnode.offset; i < thisnode.offset + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, thisnode.offset, i - thisnode.offset) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Empty Number." << std::endl; }
					else { h.err() << h.dbgsegment(code, thisnode.offset, i - thisnode.offset) << "[ERR][L" << line << "|C" << col << "]\t" << "Empty Number." << std::endl; }
				}
				root.children.emplace_back(std::move(thisnode));
			}
			//VARIABLE = identifier;
			bool VARIABLE_start(helper &h, const char* code, size_t curoff) { auto len = identifier(code, curoff); return len > 0 && !h.contains_binary(std::string(code + curoff, code + curoff + len), 0); }
			void VARIABLE(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = sqfasttypes::VARIABLE;
				thisnode.file = file;
				auto len = identifier(code, curoff);
				auto ident = std::string(code + curoff, code + curoff + len);
				thisnode.content = ident;
				thisnode.length = len;
				thisnode.offset = curoff;
				thisnode.col = col;
				thisnode.line = line;

				curoff += len;
				col += len;
				root.children.emplace_back(std::move(thisnode));
			}
			//STRING = '"' { any | "\"\"" } '"' | '\'' { any | "''" } '\'';
			bool STRING_start(helper &h, const char* code, size_t curoff) { return code[curoff] == '\'' || code[curoff] == '"'; }
			void STRING(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = sqfasttypes::STRING;
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
						col += 2;
						i++;
					}
					else if (code[i] == '\n')
					{
						col = 0;
						line++;
					}
					else
					{
						col++;
					}
				}
				i++;
				col++;
				auto fullstring = std::string(code + curoff, code + i);
				thisnode.content = fullstring;
				thisnode.length = i - curoff;
				thisnode.offset = curoff;
				curoff = i;
				root.children.emplace_back(std::move(thisnode));
			}
			//CODE = "{" SQF "}";
			bool CODE_start(helper &h, const char* code, size_t curoff) { return code[curoff] == '{'; }
			void CODE(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = sqfasttypes::CODE;
				thisnode.offset = curoff;
				thisnode.col = col;
				thisnode.line = line;
				thisnode.file = file;
				curoff++;
				col++;
				skip(code, line, col, file, curoff);

				if (SQF_start(h, code, curoff))
				{
					SQF(h, thisnode, code, line, col, curoff, file, errflag);
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected SQF start." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected SQF start." << std::endl; }
					errflag = true;
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
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected '}'." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected '}'." << std::endl; }
					errflag = true;
				}
				thisnode.length = curoff - thisnode.offset;
				root.children.emplace_back(std::move(thisnode));
			}
			//ARRAY = '[' [ BINARYEXPRESSION { ',' BINARYEXPRESSION } ] ']';
			bool ARRAY_start(helper &h, const char* code, size_t curoff) { return code[curoff] == '['; }
			void ARRAY(helper &h, astnode &root, const char* code, size_t &line, size_t &col, size_t &curoff, std::string file, bool &errflag)
			{
				auto thisnode = astnode();
				thisnode.kind = sqfasttypes::ARRAY;
				thisnode.offset = curoff;
				thisnode.col = col;
				thisnode.line = line;
				thisnode.file = file;
				curoff++;
				col++;
				skip(code, line, col, file, curoff);
				if (BINARYEXPRESSION_start(h, code, curoff))
				{
					BINARYEXPRESSION(h, thisnode, code, line, col, curoff, file, errflag);
					skip(code, line, col, file, curoff);
					while (code[curoff] == ',')
					{
						col++;
						curoff++;
						skip(code, line, col, file, curoff);

						if (BINARYEXPRESSION_start(h, code, curoff))
						{
							BINARYEXPRESSION(h, thisnode, code, line, col, curoff, file, errflag);
							skip(code, line, col, file, curoff);
						}
						else
						{
							size_t i;
							for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
							if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected BINARYEXPRESSION start." << std::endl; }
							else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected BINARYEXPRESSION start." << std::endl; }
							errflag = true;
						}
					}
				}
				if (code[curoff] == ']')
				{
					curoff++;
					col++;
				}
				else
				{
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Expected ']'." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Expected ']'." << std::endl; }
					errflag = true;
				}
				thisnode.length = curoff - thisnode.offset;
				thisnode.content = std::string(code + thisnode.offset, code + thisnode.offset + thisnode.length);
				root.children.emplace_back(std::move(thisnode));
			}

			astnode parse_sqf(const char* codein, helper& h, bool &errflag, std::string file)
			{
				const char *code = codein;
				size_t line = 1;
				size_t col = 0;
				size_t curoff = 0;
				astnode node;
				node.kind = sqfasttypes::SQF;
				node.offset = 0;
				node.content = codein;
				node.file = file;
				SQF(h, node, code, line, col, curoff, file, errflag);
				node.length = curoff;
				skip(codein, line, col, file, curoff);
				if (!errflag && codein[curoff] != '\0') {
					size_t i;
					for (i = curoff; i < curoff + 128 && std::iswalnum(code[i]); i++);
					if (!file.empty()) { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "|" << file << "]\t" << "Reached EOF before finishing parsing." << std::endl; }
					else { h.err() << h.dbgsegment(code, curoff, i - curoff) << "[ERR][L" << line << "|C" << col << "]\t" << "Reached EOF before finishing parsing." << std::endl; }
					errflag = true;
				}
				return node;
			}
			const char * astkindname(short id)
			{
				switch (id)
				{
					case sqfasttypes::SQF: return "SQF";
					case sqfasttypes::STATEMENT: return "STATEMENT";
					case sqfasttypes::ASSIGNMENT: return "ASSIGNMENT";
					case sqfasttypes::ASSIGNMENTLOCAL: return "ASSIGNMENTLOCA";
					case sqfasttypes::BEXP1: return "BEXP1";
					case sqfasttypes::BEXP2: return "BEXP2";
					case sqfasttypes::BEXP3: return "BEXP3";
					case sqfasttypes::BEXP4: return "BEXP4";
					case sqfasttypes::BEXP5: return "BEXP5";
					case sqfasttypes::BEXP6: return "BEXP6";
					case sqfasttypes::BEXP7: return "BEXP7";
					case sqfasttypes::BEXP8: return "BEXP8";
					case sqfasttypes::BEXP9: return "BEXP9";
					case sqfasttypes::BEXP10: return "BEXP10";
					case sqfasttypes::BINARYEXPRESSION: return "BINARYEXPRESSION";
					case sqfasttypes::BINARYOP: return "BINARYOP";
					case sqfasttypes::BRACKETS: return "BRACKETS";
					case sqfasttypes::UNARYOP: return "UNARYOP";
					case sqfasttypes::PRIMARYEXPRESSION: return "PRIMARYEXPRESSION";
					case sqfasttypes::NULAROP: return "NULAROP";
					case sqfasttypes::UNARYEXPRESSION: return "UNARYEXPRESSION";
					case sqfasttypes::HEXNUMBER: return "HEXNUMBER";
					case sqfasttypes::NUMBER: return "NUMBER";
					case sqfasttypes::VARIABLE: return "VARIABLE";
					case sqfasttypes::STRING: return "STRING";
					case sqfasttypes::CODE: return "CODE";
					case sqfasttypes::ARRAY: return "ARRAY";
					default: return "NA";
				}
			}
		}
	}
}
