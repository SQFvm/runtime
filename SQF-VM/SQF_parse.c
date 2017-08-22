#include "textrange.h"
#include "SQF.h"
#include "SQF_types.h"
#include "SQF_parse.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>


void tokenize(TR_ARR* arr, const char* code)
{
	int i;
	int j;
	char c, s;
	int start = -1;
	for (i = 0; code[i] != '\0'; i++)
	{
		c = code[i];
		if (start == -1)
		{
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
			{
				continue;
			}
			start = i;
		}
		else
		{
			s = code[start];
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
			{
				tr_arr_push(arr, (TEXTRANGE)
				{
					.start = start, .length = i - start
				});
				start = -1;
			}
			else if (s == '(' || s == '{' || s == '[' || s == ')' || s == '}' || s == ']' || s == ';' || s == '+' || s == '-' || s == '*' || s == '/')
			{
				tr_arr_push(arr, (TEXTRANGE)
				{
					.start = start, .length = start - i
				});
				start = -1;
				i--;
			}
			else if (s >= '0' && s <= '9')
			{
				if (!(c >= '0' && c <= '9' || c == '.'))
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start
					});
					start = -1;
					i--;
				}
			}
			else if (s >= 'a' && s <= 'z' || s >= 'A' && s <= 'Z' || s == '_')
			{
				if (!(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9'))
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start
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
						.start = start, .length = i - start + 1
					});
					start = -1;
					i--;
				}
			}
			else if (s == '"')
			{
				if (c == '"' && code[i + 1] == '"')
				{
					i++;
				}
				else if (c == '"')
				{
					tr_arr_push(arr, (TEXTRANGE)
					{
						.start = start, .length = i - start + 1
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
						.start = start, .length = i - start + 1
					});
					start = -1;
				}
			}
			else
			{
				__asm int 3;
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
	int i;
	PCMD cmd;
	for (i = 0; i < vm->cmds_top; i++)
	{
		cmd = vm->cmds[i];
		if (strncmpi(cmd->name, cmd->name_len, name, len) == 0)
		{
			return cmd;
		}
	}
	return 0;
}
void parse_partial(PVM vm, PSTACK stack, const char* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end)
{
	char* str;
	char* endptr;
	int i, j = -1;
	float f;
	TEXTRANGE range;
	PSTRING value_string;
	CPCMD cmd, smallest_cmd = 0;
	if (arr_end - arr_start == 0)
	{
		return;
	}
	for (i = arr_start; i < arr_end; i++)
	{
		range = tr_arr_get(arr, i);
		str = code + range.start;
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
		}
		cmd = fndcmd(vm, str, range.length);
		if (cmd == 0)
		{
			if (str[0] == '_' && smallest_cmd != 0 && strcmpi(smallest_cmd->name, "private") == 0)
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
				if (cmd->type_code < smallest_cmd->type_code)
				{
					smallest_cmd = cmd;
					j = i;
				}
			}
		}
	}
	if (j == -1)
		return;
	if (smallest_cmd == 0)
	{
		str = code + tr_arr_get(arr, j).start;
		if (str[0] == '"' || str[0] == '\'')
		{
			value_string = string_create(tr_arr_get(arr, j).length - 2);
			strncpy(value_string->val, str + 1, tr_arr_get(arr, j).length - 2);
			push_stack(stack, inst_value(value(STRING_TYPE(), base_voidptr(value_string))));
		}
		else
		{
			f = strtof(str, &endptr);
			if (endptr != str)
			{
				push_stack(stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
			}
			else
			{
				endptr = alloca(sizeof(char) * (tr_arr_get(arr, j).length + 1));
				endptr[tr_arr_get(arr, j).length] = '\0';
				strncpy(endptr, str, tr_arr_get(arr, j).length);
				if (strncmpi(code + tr_arr_get(arr, j + 1).start, tr_arr_get(arr, j + 1).length, "=", -1) == 0)
				{
					if (j > 0 && strncmpi(code + tr_arr_get(arr, j - 1).start, tr_arr_get(arr, j - 1).length, "private", -1) == 0)
					{
						push_stack(stack, inst_store_var_local(endptr));
						arr_start++;
					}
					else
					{
						push_stack(stack, inst_store_var(endptr));
					}
				}
				else
				{
					push_stack(stack, inst_load_var(endptr));
				}
			}
		}
	}
	else
	{
		push_stack(stack, inst_command(smallest_cmd));
	}
	parse_partial(vm, stack, code, arr, arr_start, j);
	parse_partial(vm, stack, code, arr, j + 1, arr_end);
}
void parse(PVM vm, const char* code)
{
	TR_ARR* arr = tr_arr_create();
	int i, j = -1;
	char* str;
	TEXTRANGE range;
	tokenize(arr, code);
	push_stack(vm->stack, inst_scope("all"));
	for (i = arr->top - 1; i >= 0; i--)
	{
		range = tr_arr_get(arr, i);
		str = code + range.start;
		if (str[0] == ';' || i == 0)
		{
			if (j == -1)
			{
				j = i;
			}
			else
			{
				parse_partial(vm, vm->stack, code, arr, i, j);
				j = i;
			}
		}
		else if (arr->top - 1 == i)
		{
			j = i + 1;
		}
	}
}