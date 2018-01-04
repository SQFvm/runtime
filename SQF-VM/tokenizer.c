#include <wchar.h>
#include <stdbool.h>

#include "textrange.h"
#include "tokenizer.h"


void tokenize(TR_ARR* arr, const wchar_t* code)
{
	int i;
	wchar_t c, s;
	int start = -1;
	int line = 1;
	int col = 0;
	bool in_string = false;
	bool in_line_comment_mode = false;
	bool in_block_comment_mode = false;
	for (i = 0; code[i] != '\0'; i++, col++)
	{
		c = code[i];
		if (in_line_comment_mode)
		{
			if (c == '\n')
			{
				in_line_comment_mode = false;
			}
			continue;
		}
		if (in_block_comment_mode)
		{
			if (c == '/' && code[i - 1] == '*')
			{
				in_block_comment_mode = false;
			}
			continue;
		}
		if (!in_string && c == '/' && code[i + 1] == '/')
		{
			in_line_comment_mode = true;
			start = -1;
			continue;
		}
		else if (!in_string && c == '/' &&  code[i + 1] == '*')
		{
			in_block_comment_mode = true;
			start = -1;
			continue;
		}
		else if (start == -1)
		{
			if (c == ' ' || c == '\t' || c == '\r')
			{
				continue;
			}
			else if (c == '\n')
			{
				line++;
				col = 0;
				continue;
			}
			start = i;
		}
		else
		{
			s = code[start];
			if (s == '"')
			{
				in_string = true;
				if (c == '"' && code[i + 1] == '"')
				{
					i++;
				}
				else if (c == '"')
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start + 1, .line = line, .col = col - (i - start + 1)
					});
					start = -1;
					in_string = false;
				}
			}
			else if (s == '\'')
			{
				in_string = true;
				if (c == '\'' && code[i + 1] == '\'')
				{
					i++;
				}
				else if (c == '\'')
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start + 1, .line = line, .col = col - (i - start + 1)
					});
					start = -1;
					in_string = false;
				}
			}
			else if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
			{
				tr_arr_push(arr, (TEXTRANGE)
				{
					.start = start, .length = i - start, .line = line, .col = col - (i - start)
				});
				start = -1;
				if (c == '\n')
				{
					line++;
					col = 0;
					continue;
				}
			}
			else if (s == '(' || s == '{' || s == '[' || s == ')' || s == '}' || s == ']' || s == ':' || s == ';' || s == '+' || s == '-' || s == '*' || s == '/' || s == ',' || s == '%' || s == '^' || s == '$')
			{
				tr_arr_push(arr, (TEXTRANGE)
				{
					.start = start, .length = i - start, .line = line, .col = col - (i - start)
				});
				start = -1;
				i--;
				col--;
			}
			else if (s >= '0' && s <= '9')
			{
				if (!((c >= '0' && c <= '9') || c == '.'))
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start, .line = line, .col = col - (i - start)
					});
					start = -1;
					i--;
					col--;
				}
			}
			else if ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || s == '_')
			{
				if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9')))
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start, .line = line, .col = col - (i - start)
					});
					start = -1;
					i--;
					col--;
				}
			}
			else if (s == '=' || s == '>' || s == '<')
			{
				if (!(c == '>' || c == '<' || c == '='))
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start, .line = line, .col = col - (i - start)
					});
					start = -1;
					i--;
					col--;
				}
			}
			else if (s == '&' || s == '|')
			{
				if (!(c == '&' || c == '|'))
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start + 1, .line = line, .col = col - (i - start + 1)
					});
					start = -1;
					i--;
					col--;
				}
			}
			else
			{
			}
		}
	}
	if (start != -1)
	{
		tr_arr_push(arr, (TEXTRANGE)
		{
			.start = start, .length = i - start, .line = line, .col = col - (i - start + 1)
		});
		start = -1;
	}
}
