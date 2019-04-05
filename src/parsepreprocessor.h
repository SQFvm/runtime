#pragma once
#include <string>

namespace sqf
{
	class virtualmachine;
	namespace parse
	{
		namespace preprocessor
		{
			class finfo
			{
			private:
				size_t last_col;
				bool is_in_string;
				bool is_in_block_comment;
				// Handles correct progression of line, col and off
				char _next()
				{
					if (off >= content.length())
					{
						return '\0';
					}
					char c = content[off++];
					switch (c)
					{
						case '\n':
							line++;
							last_col = col;
							col = 0;
							return c;
						case '\r':
							return _next();
						default:
							col++;
							return c;
					}
				}
			public:
				finfo()
				{
					last_col = 0;
					is_in_string = false;
					is_in_block_comment = false;
				}
				std::string content;
				size_t off = 0;
				size_t line = 1;
				size_t col = 0;
				std::string path;
				// Returns the next character.
				// Will not take into account to skip eg. comments or simmilar things!
				char peek(size_t len = 0)
				{
					if (off + len >= content.length())
					{
						return '\0';
					}
					return content[off + len];
				}
				// Will return the next character in the file.
				// Comments will be skipped automatically.
				char next()
				{
					char c = _next();
					if (!is_in_string && (c == '/' || is_in_block_comment))
					{
						if (c == '\n')
						{
							return c;
						}
						auto pc = peek();
						if (is_in_block_comment && c == '*' && pc == '/')
						{
							_next();
							c = _next();
							is_in_block_comment = false;
							return c;
						}
						else if (pc == '*' || is_in_block_comment)
						{
							if (!is_in_block_comment)
							{
								_next();
							}
							is_in_block_comment = true;
							while ((c = _next()) != '\0')
							{
								if (c == '\n')
								{
									break;
								}
								else if (c == '*' && peek() == '/')
								{
									_next();
									is_in_block_comment = false;
									c = _next();
									break;
								}
							}
						}
						else if (pc == '/')
						{
							while ((c = _next()) != '\0' && c != '\n');
						}
					}
					if (c == '"')
					{
						is_in_string = !is_in_string;
					}
					return c;
				}

				std::string get_word()
				{
					char c;
					size_t off_start = off;
					size_t off_end = off;
					while (
						(c = next()) != '\0' && (
						(c >= 'A' && c <= 'Z') ||
							(c >= 'a' && c <= 'z') ||
							(c >= '0' && c <= '9') ||
							c == '_'
							))
					{
						off_end = off;
					}
					move_back();
					return content.substr(off_start, off_end - off_start);
				}

				std::string get_line(bool catchEscapedNewLine)
				{
					char c;
					size_t off_start = off;
					bool escaped = false;
					bool exit = false;
					if (catchEscapedNewLine)
					{
						std::string outputString;
						outputString.reserve(64);
						while (!exit && (c = next()) != '\0')
						{
							switch (c)
							{
								case '\\':
									escaped = true;
									break;
								case '\n':
									if (!escaped)
									{
										exit = true;
									}
									escaped = false;
									break;
								default:
									if (escaped)
									{
										outputString.push_back('\\');
										escaped = false;
									}
									outputString.push_back(c);
									break;
							}
						}
						outputString.shrink_to_fit();
						return outputString;
					}
					else
					{
						while ((c = next()) != '\0' && c != '\n') {}
					}
					return content.substr(off_start, off - off_start);
				}
				// Moves one character backwards and updates
				// porgression of line, col and off according
				// col will only be tracked for one line!
				// Not supposed to be used more then once!
				void move_back()
				{
					if (off == 0)
					{
						return;
					}
					char c = content[--off];
					switch (c)
					{
						case '\n':
							line--;
							col = last_col;
							break;
						case '\r':
							move_back();
							break;
						default:
							col--;
							break;
					}
				}
			};
			class macro {
			public:
				std::string name;
				std::string content;
				std::vector<std::string> args;
				std::string filepath;
				size_t line;
				size_t column;
				bool hasargs;
				// Special method pointer that may be filled
				// to give this macro a special behavior rather
				// then a content.
				// Gets only applied if pointer is != nullptr
				std::string(*callback)(finfo fileinfo, std::vector<std::string> params);

				macro() : name(), content(), args(), filepath(), line(0), column(0), hasargs(false), callback(nullptr) {}
			};

			namespace settings
			{
				// Whether or not to warn on non-existing #undef
				// and already-existing #define
				extern bool disable_warn_define;
			};


			std::string parse(sqf::virtualmachine* vm, std::string input, bool &errflag, std::string filename);
		}
	}
}