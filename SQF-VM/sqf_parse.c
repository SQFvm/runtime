#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef __linux
#include <alloca.h>
#endif // __linux
#ifdef _WIN32
#include <malloc.h>
#endif // _WIN32
#include <stdint.h>

#include "wstring_op.h"
#include "wstring_map.h"
#include "sqffull.h"
#include "tokenizer.h"

PSTRING parse_string(PVM vm, const wchar_t* str, unsigned int len)
{
	PSTRING value_string;
	int off;
	int i;
	wchar_t startchar;
	bool wasquotation = false;
	if (len == 1)
	{
		return string_create(0);
	}
	else
	{
		startchar = str[0];
		i = (str[len - 1] == '"' || str[len - 1] == '\'') ? 2 : 1;
		value_string = string_create(len - i);
		if (value_string->length == 0)
		{
			return value_string;
		}
		wcsncpy(value_string->val, str + 1, len - i);

		if (str[0] == startchar)
		{
			off = 0;
			for (i = 0; i < value_string->length; i++)
			{
				if (wasquotation)
				{
					wasquotation = false;

					if (value_string->val[i] == startchar)
					{
						off++;
					}
				}
				else
				{
					if (value_string->val[i] == startchar)
					{
						wasquotation = true;
					}
				}
				value_string->val[i - off] = value_string->val[i];
			}
			if (off > 0)
			{
				string_resize(value_string, -off);
			}
		}
		return value_string;
	}
}

int wcsncmpi(const wchar_t* left, int left_len, const wchar_t* right, int right_len)
{
	int i;
	if (left_len == -1)
	{
		left_len = wcslen(left);
	}
	if (right_len == -1)
	{
		right_len = wcslen(right);
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

void parse_block(PVM vm, PSTACK stack, const wchar_t* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter)
{
	int i, j = -1;
	int arraycount = 0;
	int codecount = 0;
	const wchar_t* str;
	TEXTRANGE range;
	if (arr_start == arr_end)
		return;
	for (i = arr_end - 1; i >= (int)arr_start; i--)
	{
		range = tr_arr_get(arr, i);
		str = code + range.start;
		if (arraycount > 0)
		{
			if (str[0] == ']')
			{
				arraycount++;
			}
			else if (str[0] == '[')
			{
				arraycount--;
			}
			continue;
		}
		if (codecount > 0)
		{
			if (str[0] == '}')
			{
				codecount++;
			}
			else if (str[0] == '{')
			{
				codecount--;
			}
			continue;
		}
		if (str[0] == '}')
		{
			codecount++;
		}
		else if (str[0] == ']')
		{
			arraycount++;
		}
		if (str[0] == ';' || str[0] == ',')
		{
			if (j == -1)
			{
				j = i;
			}
			else
			{
				parse_partial(vm, stack, code, arr, i, j, stack_counter);
				if (stack != 0)
				{
					push_stack(vm, stack, inst_clear_work());
				}
				(*stack_counter)++;
				j = i;
			}
		}
		else if (arr_end - 1 == i)
		{
			j = i + 1;
		}
	}
	if (j != 0)
	{
		parse_partial(vm, stack, code, arr, i + 1, j, stack_counter);
		if (stack != 0)
		{
			push_stack(vm, stack, inst_clear_work());
		}
		(*stack_counter)++;
	}
}

CPCMD fndcmd(PVM vm, const wchar_t* name, unsigned int len)
{
	wchar_t* name_buff = alloca(sizeof(wchar_t) * (len + 1));
	wcsncpy(name_buff, name, len);
	name_buff[len] = '\0';
	return find_command(vm, name_buff, 'C');
}
CPCMD fndcmd2(PVM vm, const wchar_t* name, unsigned int len, unsigned char filter)
{
	wchar_t* name_buff = alloca(sizeof(wchar_t) * (len + 1));
	wcsncpy(name_buff, name, len);
	name_buff[len] = '\0';
	CPCMD cmd = 0;
	if (ENUM_CMD_TYPE & filter && (cmd = find_command(vm, name_buff, 't')) != 0)
	{
		return cmd;
	}
	if (ENUM_CMD_BINARY & filter && (cmd = find_command(vm, name_buff, 'b')) != 0)
	{
		return cmd;
	}
	if (ENUM_CMD_UNARY & filter && (cmd = find_command(vm, name_buff, 'u')) != 0)
	{
		return cmd;
	}
	if (ENUM_CMD_NULLAR & filter && (cmd = find_command(vm, name_buff, 'n')) != 0)
	{
		return cmd;
	}
	return cmd;
}
void parse_form_code(PVM vm, PSTACK stack, const wchar_t* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter)
{
	TEXTRANGE range;
	PCODE pcode;
	unsigned int stack_size;
	range.start = tr_arr_get(arr, arr_start + 1).start;
	range.length = tr_arr_get(arr, arr_end).start - range.start;
	if (stack != 0)
	{
		pcode = code_create(code, range.start, range.length);
		stack_size = 0;
		parse_block(vm, 0, code, arr, arr_start + 1, arr_end, &stack_size);
		//parse_partial(vm, 0, code, arr, arr_start + 1, arr_end, &stack_size);
		resize_stack(vm, pcode->stack, stack_size + 1);
		parse_block(vm, pcode->stack, code, arr, arr_start + 1, arr_end, &stack_size);
		//parse_partial(vm, pcode->stack, code, arr, arr_start + 1, arr_end, &stack_size);
		push_stack(vm, stack, inst_value(value(CODE_TYPE(), base_voidptr(pcode))));
	}
	(*stack_counter)++;
}
void parse_form_array(PVM vm, PSTACK stack, const wchar_t* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter)
{
	int i, j = -1, k = -1;
	const wchar_t* str;
	TEXTRANGE range;
	int arrcount = 0;
	int codecount = 0;
	int bracecount = 0;
	wchar_t c;
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
			if (j == -1)
			{
				j = i;
			}
			arrcount++;
		}
		else if (str[0] == '}')
		{
			if (j == -1)
			{
				j = i;
			}
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
void parse_partial(PVM vm, PSTACK stack, const wchar_t* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter)
{
	const wchar_t* str;
	wchar_t* endptr;
	int i, j = -1, k;
	float f;
	TEXTRANGE range;
	CPCMD cmd = 0, smallest_cmd = 0;
	int arrcount = 0;
	int codecount = 0;
	int bracecount = 0;
	wchar_t c;
	int wasvariable = 0;
	if (arr_end - arr_start == 0)
	{
		return;
	}
	for (i = arr_start; i < (int)arr_end; i++)
	{
		range = tr_arr_get(arr, i);
		str = code + range.start;
		if (i == arr_start && range.length == 1 && (str[0] == ';' || str[0] == ','))
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
		if (arr_start == i || (tr_arr_get(arr, i - 1).length == 1 && (code + tr_arr_get(arr, i - 1).start)[0] == '=') || (cmd != 0 && cmd->type_code & (2 | 4)))
		{
			cmd = fndcmd2(vm, str, range.length, 4 | 8);
			if (cmd == 0)
			{
				cmd = fndcmd2(vm, str, range.length, 2);
			}
		}
		else
		{
			cmd = fndcmd(vm, str, range.length);
		}
		if (cmd == 0)
		{
			if (wasvariable)
			{
				if (stack != 0)
				{
					push_stack(vm, stack, inst_debug_info(range.line, range.col, range.start, range.length, code));
				}
				(*stack_counter)++;
				vm->error(vm, L"Syntax Error: Double non-command usage", stack);
			}
			wasvariable = 1;
			if (str[0] == '_' && smallest_cmd != 0 && wstr_cmpi(smallest_cmd->name, -1, L"private", -1) == 0)
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
			wasvariable = 0;
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
		push_stack(vm, stack, inst_debug_info(range.line, range.col, range.start, range.length, code));
	}
	(*stack_counter)++;
	i = j;
	if (smallest_cmd == 0)
	{
		str = code + range.start;
		if (str[0] == '"' || str[0] == '\'')
		{
			if (stack != 0)
			{
				push_stack(vm, stack, inst_value(value(STRING_TYPE(), base_voidptr(parse_string(vm, str, range.length)))));
			}
			(*stack_counter)++;
		}
		else if (str[0] == '(')
		{
			for (i = j, k = 1; i < (int)arr_end && k > 0;)
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
		}
		else if (str[0] == '[')
		{
			for (i = j, k = 1; i < (int)arr_end && k > 0;)
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
		}
		else if (str[0] == '{')
		{
			for (i = j, k = 1; i < (int)arr_end && k > 0;)
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
		}
		else
		{
			f = wcstof(str, &endptr);
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
				endptr = alloca(sizeof(wchar_t) * (range.length + 1));
				endptr[range.length] = '\0';
				wcsncpy(endptr, str, range.length);
				if (wcsncmpi(code + tr_arr_get(arr, j + 1).start, tr_arr_get(arr, j + 1).length, L"=", -1) == 0)
				{
					if (j > 0 && wcsncmpi(code + tr_arr_get(arr, j - 1).start, tr_arr_get(arr, j - 1).length, L"private", -1) == 0)
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
			if (smallest_cmd->type == 'u' && arr_end == i + 1)
			{
				cmd = fndcmd2(vm, smallest_cmd->name, smallest_cmd->name_len, 8);
				if (cmd != 0)
				{
					smallest_cmd = cmd;
				}
			}
			push_stack(vm, stack, inst_command(smallest_cmd));
		}
		(*stack_counter)++;
	}
	parse_partial(vm, stack, code, arr, arr_start, j, stack_counter);
	parse_partial(vm, stack, code, arr, i + 1, arr_end, stack_counter);
}
void parse(PVM vm, const wchar_t* code, bool createscope)
{
	TR_ARR* arr = tr_arr_create();
	unsigned int stack_counter = 0;
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
		parse_block(vm, vm->stack, code, arr, 0, arr->top, &stack_counter);
	}
	tr_arr_destroy(arr);
}

PCODE parse_into_code(PVM vm, const wchar_t* code)
{
	TR_ARR* arr = tr_arr_create();
	unsigned int stack_size = 0;
	PCODE pcode;
	if (code == 0)
	{
		return code_create(L"", 0, 0);
	}
	pcode = code_create(code, 0, wcslen(code));
	tokenize(arr, code);

	parse_block(vm, 0, code, arr, 0, arr->top, &stack_size);
	resize_stack(vm, pcode->stack, stack_size + 1);
	parse_block(vm, pcode->stack, code, arr, 0, arr->top, &stack_size);

	tr_arr_destroy(arr);
	return pcode;
}