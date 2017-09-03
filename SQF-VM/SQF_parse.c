#include "basetype.h"
#include "string_map.h"
#include "string_op.h"
#include "textrange.h"
#include "SQF.h"
#include "SQF_types.h"
#include "SQF_parse.h"
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef __linux
#include <alloca.h>
#endif // __linux
#ifdef _WIN32
#include <malloc.h>
#endif // _WIN32


void tokenize(TR_ARR* arr, const char* code)
{
	int i;
	char c, s;
	int start = -1;
	int line = 1;
	int col = 0;
	unsigned char in_line_comment_mode = 0;
	unsigned char in_block_comment_mode = 0;
	for (i = 0; code[i] != '\0'; i++, col++)
	{
		c = code[i];
		if (in_line_comment_mode)
		{
			if (c == '\n')
			{
				in_line_comment_mode = 0;
			}
			continue;
		}
		if(in_block_comment_mode)
		{
			if (c == '/' && code[i - 1] == '*')
			{
				in_block_comment_mode = 0;
			}
			continue;
		}
		if (c == '/' && code[i + 1] == '/')
		{
			in_line_comment_mode = 1;
			start = -1;
			continue;
		}
		else if (c == '/' &&  code[i + 1] == '*')
		{
			in_block_comment_mode = 1;
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
				if (c == '"' && code[i + 1] == '"')
				{
					i++;
				}
				else if (c == '"')
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start + 1, .line = line, .col = col
					});
					start = -1;
				}
			}
			else if (s == '\'')
			{
				if (c == '\'')
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start + 1, .line = line, .col = col
					});
					start = -1;
				}
			}
			else if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
			{
				tr_arr_push(arr, (TEXTRANGE)
				{
					.start = start, .length = i - start, .line = line, .col = col
				});
				start = -1;
				if (c == '\n')
				{
					line++;
					col = 0;
					continue;
				}
			}
			else if (s == '(' || s == '{' || s == '[' || s == ')' || s == '}' || s == ']' || s == ';' || s == '+' || s == '-' || s == '*' || s == '/' || s == ',' || s == '%' || s == '^')
			{
				tr_arr_push(arr, (TEXTRANGE)
				{
					.start = start, .length = i - start, .line = line, .col = col
				});
				start = -1;
				i--;
			}
			else if (s >= '0' && s <= '9')
			{
				if (!((c >= '0' && c <= '9') || c == '.'))
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start, .line = line, .col = col
					});
					start = -1;
					i--;
				}
			}
			else if ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || s == '_')
			{
				if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9')))
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start, .line = line, .col = col
					});
					start = -1;
					i--;
				}
			}
			else if (s == '=' || s == '>' || s == '<')
			{
				if (!(c == '>' || c == '<' || c == '='))
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start, .line = line, .col = col
					});
					start = -1;
					i--;
				}
			}
			else if (s == '&' || s == '|')
			{
				if (!(c == '&' || c == '|'))
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start + 1, .line = line, .col = col
					});
					start = -1;
					i--;
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
			.start = start, .length = i - start
		});
		start = -1;
	}
}

int strncmpi(const char* left, int left_len, const char* right, int right_len)
{
	int i;
	if (left_len == -1)
	{
		left_len = strlen(left);
	}
	if (right_len == -1)
	{
		right_len = strlen(right);
	}
	if (left_len != right_len)
		return -1;
	for (i = 0; i < left_len; i++)
	{
		if (toupper(left[i]) != toupper(right[i]))
			return left[i] < right[i] ? -1 : 1;
	}
	return 0;
}


CPCMD fndcmd(PVM vm, const char* name, unsigned int len)
{
	char* name_buff = alloca(sizeof(char) * (len + 1));
	strncpy(name_buff, name, len);
	name_buff[len] = '\0';
	return find_command(vm, name_buff, 'C');
}
CPCMD fndcmd2(PVM vm, const char* name, unsigned int len, unsigned char filter)
{
	char* name_buff = alloca(sizeof(char) * (len + 1));
	strncpy(name_buff, name, len);
	name_buff[len] = '\0';
	CPCMD cmd = 0;
	if (ENUM_CMD_NULLAR & filter && (cmd = find_command(vm, name_buff, 'n')) != 0)
	{
		return cmd;
	}
	if (ENUM_CMD_UNARY & filter && (cmd = find_command(vm, name_buff, 'u')) != 0)
	{
		return cmd;
	}
	if (ENUM_CMD_BINARY & filter && (cmd = find_command(vm, name_buff, 'b')) != 0)
	{
		return cmd;
	}
	if (ENUM_CMD_TYPE & filter && (cmd = find_command(vm, name_buff, 't')) != 0)
	{
		return cmd;
	}
	return cmd;
}
void parse_form_code(PVM vm, PSTACK stack, const char* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter)
{
	TEXTRANGE range;
	PCODE pcode;
	int stack_size;
	range.start = tr_arr_get(arr, arr_start + 1).start;
	range.length = tr_arr_get(arr, arr_end).start - range.start;
	if (stack != 0)
	{
		pcode = code_create(code, range.start, range.length);
		stack_size = 0;
		parse_partial(vm, 0, code, arr, arr_start + 1, arr_end - 1, &stack_size);
		resize_stack(vm, pcode->stack, stack_size + 1);
		parse_partial(vm, pcode->stack, code, arr, arr_start + 1, arr_end - 1, &stack_size);
		push_stack(vm, stack, inst_value(value(CODE_TYPE(), base_voidptr(pcode))));
	}
	(*stack_counter)++;
}
void parse_form_array(PVM vm, PSTACK stack, const char* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter)
{
	int i, j = -1, k = -1;
	const char* str;
	TEXTRANGE range;
	int arrcount = 0;
	int codecount = 0;
	int bracecount = 0;
	char c;
	for (i = arr_end - 1; i >= (int)arr_start; i--)
	{
		range = tr_arr_get(arr, i);
		str = code + range.start;

		if (arrcount > 0)
		{
			if (str[0] == '[')
			{
				arrcount--;
			}
			else if (str[0] == ']')
			{
				arrcount++;
			}
			if (arrcount == 0)
			{
				if (stack != 0)
				{
					push_stack(vm, stack, inst_arr_push());
				}
				(*stack_counter)++;
				parse_form_array(vm, stack, code, arr, i, j, stack_counter);
				k = j;
				j = -1;
			}
			continue;
		}
		if (codecount > 0)
		{
			if (str[0] == '{')
			{
				codecount--;
			}
			else if (str[0] == '}')
			{
				codecount++;
			}
			continue;
		}
		if (bracecount > 0)
		{
			if (str[0] == '(')
			{
				codecount--;
			}
			else if (str[0] == ')')
			{
				codecount++;
			}
			continue;
		}
		if (str[0] == ']')
		{
			j = i;
			arrcount++;
		}
		else if (str[0] == '}')
		{
			j = i;
			codecount++;
		}
		else if (j == -1)
		{
			j = i;
		}
		else if (str[0] == ',' || str[0] == '[')
		{
			if (k >= 0)
			{
				c = (code + tr_arr_get(arr, k).start)[0];
			}
			else
			{
				c = '\0';
			}
			if (c != ']')
			{
				if (c == '}' && (code + tr_arr_get(arr, i + 1).start)[0] == '{')
				{
					if (stack != 0)
					{
						push_stack(vm, stack, inst_arr_push());
					}
					(*stack_counter)++;
					parse_form_code(vm, stack, code, arr, i + 1, k, stack_counter);
				}
				else
				{
					if (stack != 0)
					{
						push_stack(vm, stack, inst_arr_push());
					}
					(*stack_counter)++;
					parse_partial(vm, stack, code, arr, i + 1, j + 1, stack_counter);
				}
			}
			j = -1;
			k = -1;
		}
	}
	if (stack != 0)
	{
		push_stack(vm, stack, inst_value(value(ARRAY_TYPE(), base_voidptr(array_create()))));
	}
	(*stack_counter)++;
}
void parse_partial(PVM vm, PSTACK stack, const char* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter)
{
	const char* str;
	char* endptr;
	int i, j = -1, k;
	float f;
	TEXTRANGE range;
	PSTRING value_string;
	CPCMD cmd = 0, smallest_cmd = 0;
	int arrcount = 0;
	int codecount = 0;
	int bracecount = 0;
	char c;
	if (arr_end - arr_start == 0)
	{
		return;
	}
	for (i = arr_start; i < arr_end; i++)
	{
		range = tr_arr_get(arr, i);
		str = code + range.start;
		if (i == arr_start && range.length == 1 && str[0] == ';')
		{
			arr_start++;
			continue;
		}
		if (arrcount > 0)
		{
			if (str[0] == '[')
			{
				arrcount++;
			}
			else if (str[0] == ']')
			{
				arrcount--;
			}
			continue;
		}
		if (codecount > 0)
		{
			if (str[0] == '{')
			{
				codecount++;
			}
			else if (str[0] == '}')
			{
				codecount--;
			}
			continue;
		}
		if (bracecount > 0)
		{
			if (str[0] == '(')
			{
				bracecount++;
			}
			else if (str[0] == ')')
			{
				bracecount--;
			}
			continue;
		}
		if (range.length == 1)
		{
			if (str[0] == '=')
			{
				if (j != -1)
				{
					break;
				}
				else
				{
					continue;
				}
			}
			else if (str[0] == ';')
			{
				continue;
			}
			else if (str[0] == '[')
			{
				arrcount++;
			}
			else if (str[0] == '{')
			{
				codecount++;
			}
			else if (str[0] == '(')
			{
				bracecount++;
			}
		}
		if (arr_start == i || (cmd != 0 && cmd->type_code & (2 | 4)))
		{
			cmd = fndcmd2(vm, str, range.length, 4 | 8);
		}
		else
		{
			cmd = fndcmd(vm, str, range.length);
		}
		if (cmd == 0)
		{
			if (str[0] == '_' && smallest_cmd != 0 && str_cmpi(smallest_cmd->name, -1, "private", -1) == 0)
			{
				smallest_cmd = 0;
				j = -1;
			}
			if (j == -1)
			{
				j = i;
			}
		}
		else
		{
			if (smallest_cmd == 0)
			{
				smallest_cmd = cmd;
				j = i;
			}
			else
			{
				if (cmd->type_code < smallest_cmd->type_code || (cmd->type_code == smallest_cmd->type_code && smallest_cmd->type == 'b' && cmd->precedence_level <= smallest_cmd->precedence_level))
				{
					smallest_cmd = cmd;
					j = i;
				}
			}
		}
	}
	if (j == -1)
		return;
	range = tr_arr_get(arr, j);
	if (stack != 0)
	{
		push_stack(vm, stack, inst_debug_info(range.line, range.col, range.start, range.length));
	}
	(*stack_counter)++;
	if (smallest_cmd == 0)
	{
		str = code + range.start;
		if (str[0] == '"' || str[0] == '\'')
		{
			if (stack != 0)
			{
				value_string = string_create(range.length - 2);
				strncpy(value_string->val, str + 1, range.length - 2);
				push_stack(vm, stack, inst_value(value(STRING_TYPE(), base_voidptr(value_string))));
			}
			(*stack_counter)++;
		}
		else if (str[0] == '(')
		{
			i = j;
			for (i = j, k = 1; i < arr_end && k > 0;)
			{
				i++;
				c = (code + tr_arr_get(arr, i).start)[0];
				if (c == '(')
				{
					k++;
				}
				else if (c == ')')
				{
					k--;
				}
			}
			parse_partial(vm, stack, code, arr, j + 1, i, stack_counter);
			return;
		}
		else if (str[0] == '[')
		{
			i = j;
			for (i = j, k = 1; i < arr_end && k > 0;)
			{
				i++;
				c = (code + tr_arr_get(arr, i).start)[0];
				if (c == '[')
				{
					k++;
				}
				else if (c == ']')
				{
					k--;
				}
			}
			parse_form_array(vm, stack, code, arr, j, i, stack_counter);
			return;
		}
		else if (str[0] == '{')
		{
			i = j;
			for (i = j, k = 1; i < arr_end && k > 0;)
			{
				i++;
				c = (code + tr_arr_get(arr, i).start)[0];
				if (c == '{')
				{
					k++;
				}
				else if (c == '}')
				{
					k--;
				}
			}
			parse_form_code(vm, stack, code, arr, j, i, stack_counter);
			return;
		}
		else
		{
			f = strtof(str, &endptr);
			if (endptr != str)
			{
				if (stack != 0)
				{
					push_stack(vm, stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
				}
				(*stack_counter)++;
			}
			else
			{
				endptr = alloca(sizeof(char) * (range.length + 1));
				endptr[range.length] = '\0';
				strncpy(endptr, str, range.length);
				if (strncmpi(code + tr_arr_get(arr, j + 1).start, tr_arr_get(arr, j + 1).length, "=", -1) == 0)
				{
					if (j > 0 && strncmpi(code + tr_arr_get(arr, j - 1).start, tr_arr_get(arr, j - 1).length, "private", -1) == 0)
					{
						if (stack != 0)
						{
							push_stack(vm, stack, inst_store_var_local(endptr));
						}
						(*stack_counter)++;
						arr_start++;
					}
					else
					{
						if (stack != 0)
						{
							push_stack(vm, stack, inst_store_var(endptr));
						}
						(*stack_counter)++;
					}
				}
				else
				{
					if (stack != 0)
					{
						push_stack(vm, stack, inst_load_var(endptr));
					}
					(*stack_counter)++;
				}
			}
		}
	}
	else
	{
		if (stack != 0)
		{
			push_stack(vm, stack, inst_command(smallest_cmd));
		}
		(*stack_counter)++;
	}
	parse_partial(vm, stack, code, arr, arr_start, j, stack_counter);
	parse_partial(vm, stack, code, arr, j + 1, arr_end, stack_counter);
}
void parse(PVM vm, const char* code, unsigned char createscope)
{
	TR_ARR* arr = tr_arr_create();
	int i, j = -1;
	int codecount = 0;
	const char* str;
	unsigned int stack_counter = 0;
	TEXTRANGE range;
	tokenize(arr, code);
	if (createscope)
	{
		push_stack(vm, vm->stack, inst_scope(NULL));
	}
	if (arr->top == 1)
	{
		parse_partial(vm, vm->stack, code, arr, 0, 1, &stack_counter);
	}
	else if(arr->top != 0)
	{
		for (i = arr->top - 1; i >= 0; i--)
		{
			range = tr_arr_get(arr, i);
			str = code + range.start;
			if (codecount > 0)
			{
				if (str[0] == '}')
				{
					codecount++;
				}
				else if(str[0] == '{')
				{
					codecount--;
				}
				continue;
			}
			if (str[0] == '}')
			{
				codecount++;
			}
			if (str[0] == ';' || i == 0)
			{
				if (j == -1)
				{
					j = i;
				}
				else
				{
					parse_partial(vm, vm->stack, code, arr, i, j, &stack_counter);
					push_stack(vm, vm->stack, inst_clear_work());
					j = i;
				}
			}
			else if (arr->top - 1 == i)
			{
				j = i + 1;
			}
		}
		if (j != 0)
		{
			parse_partial(vm, vm->stack, code, arr, i + 1, j, &stack_counter);
			push_stack(vm, vm->stack, inst_clear_work());
		}
	}
	tr_arr_destroy(arr);
}
