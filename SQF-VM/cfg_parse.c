#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <stdbool.h>
#include <stdint.h>

#include "wstring_op.h"
#include "wstring_map.h"
#include "sqffull.h"

/*
NODELIST = { NODE ';' { ';' } };
NODE = 'class' CONFIGNODE | VALUENODE;
CONFIGNODE = ident [ ':' ident ] '{' NODELIST '}'
VALUENODE = ident ('=' (STRING | NUMBER | LOCALIZATION) | '[' ']' '=' ARRAY);
STRING = '"' anything '"' | '\'' anything '\'';
NUMBER = [ '-' ] num [ '.' num ];
LOCALIZATION = '$' ident;
ARRAY = '{' [ VALUE { ',' VALUE } ] '}'
VALUE = STRING | NUMBER | LOCALIZATION | ARRAY;
*/

bool cfgparse_isident(const wchar_t* str, unsigned int len)
{
	unsigned int i;
	wchar_t c;
	for (i = 0; i < len; i++)
	{
		c = str[0];
		if (!((c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z') || (c >= L'0' && c <= L'9') || (c == '/' || c == '\\' || c == '_')))
			return false;
	}
	return true;
}

TEXTRANGE get_ident_range(const wchar_t* code, TR_ARR* arr, unsigned int* index)
{
	TEXTRANGE output = tr_arr_get(arr, *index);
	TEXTRANGE range;
	unsigned int tmpindex = *index;
	wchar_t c;
	do
	{
		c = ((range = tr_arr_get(arr, tmpindex++)).start + code)[0];
	} while (range.length != 0 && ((c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z') || (c >= L'0' && c <= L'9') || (c == '/' || c == '\\' || c == '_')));
	tmpindex--;
	output.length = 0;
	for (; *index < tmpindex; (*index)++)
	{
		range = tr_arr_get(arr, *index);
		output.length += range.length;
	}
	(*index)--;
	return output;
}

//NODELIST = { NODE ';' { ';' } };
void cfgparse_nodelist(PVM vm, PCONFIG root, TR_ARR* arr, const wchar_t* code, unsigned int *index)
{
	TEXTRANGE range;
	const wchar_t* str;

	//Exit if not start of NODE
	range = tr_arr_get(arr, *index);
	str = code + range.start;
	if (!cfgparse_isident(str, range.length) && !wstr_cmpi(str, range.length, L"class", 5) == 0)
	{
		return;
	}

	//{
	while (*index < arr->top)
	{
		//NODE
		cfgparse_node(vm, root, arr, code, index);

		//L';'
		range = tr_arr_get(arr, *index);
		str = code + range.start;
		if (str[0] == L';' && range.length == 1)
		{
			//{
			do
			{
				//L';'
				(*index)++;
				range = tr_arr_get(arr, *index);
				str = code + range.start;
				//}
			} while (str[0] == L';' && range.length == 1);
		}
		else
		{
			vm->print(vm, L"L%u;C%u;", range.line, range.col);
			vm->error(vm, L"Expected ';'.", 0);
		}
		//}

		//Exit if not start of NODE
		range = tr_arr_get(arr, *index);
		str = code + range.start;
		if (!cfgparse_isident(str, range.length) && !wstr_cmpi(str, range.length, L"class" , 5) == 0)
		{
			break;
		}
	}
}
//NODE = 'class' CONFIGNODE | VALUENODE;
void cfgparse_node(PVM vm, PCONFIG root, TR_ARR* arr, const wchar_t* code, unsigned int *index)
{
	TEXTRANGE range;
	const wchar_t* str;

	//L'class'
	range = tr_arr_get(arr, *index);
	str = code + range.start;
	if (wstr_cmpi(str, range.length, L"class", 5) == 0)
	{
		(*index)++;
		//CONFIGNODE
		cfgparse_confignode(vm, root, arr, code, index);
	}
	//|
	else
	{
		//VALUENODE
		cfgparse_valuenode(vm, root, arr, code, index);
	}
}
//CONFIGNODE = ident[':' ident] '{' NODELIST '}'
void cfgparse_confignode(PVM vm, PCONFIG root, TR_ARR* arr, const wchar_t* code, unsigned int *index)
{
	TEXTRANGE range;
	const wchar_t* str;
	PCONFIG node;

	//ident
	range = get_ident_range(code, arr, index);
	str = code + range.start;

	node = config_create_node(str, range.length, range.length);
	node->parent = root;
	config_push_node(root, node);
	(*index)++;
	//[ L':'
	range = tr_arr_get(arr, *index);
	str = code + range.start;
	if (str[0] == L':' && range.length == 1)
	{
		(*index)++;
		//ident
		range = get_ident_range(code, arr, index);
		str = code + range.start;
		node->inheritingident = malloc(sizeof(wchar_t) * (range.length + 1));
		node->inheritingident = wcsncpy(node->inheritingident, str, range.length);
		node->inheritingident[range.length] = L'\0';
		node->inheritingident_length = range.length;
		(*index)++;
		range = tr_arr_get(arr, *index);
		str = code + range.start;
	}
	//]
	//L'{'
	if (str[0] == L'{' && range.length == 1)
	{
		(*index)++;
		range = tr_arr_get(arr, *index);
		str = code + range.start;
	}
	else
	{
		vm->print(vm, L"L%u;C%u;", range.line, range.col);
		vm->error(vm, L"Expected '{'.", 0);
	}
	cfgparse_nodelist(vm, node, arr, code, index);
	range = tr_arr_get(arr, *index);
	str = code + range.start;
	if (str[0] == L'}' && range.length == 1)
	{
		(*index)++;
	}
	else
	{
		vm->print(vm, L"L%u;C%u;", range.line, range.col);
		vm->error(vm, L"Expected '}'.", 0);
	}
	//L'}'
}
//VALUENODE = ident ('=' (STRING | NUMBER | LOCALIZATION) | '[' ']' '=' ARRAY);
void cfgparse_valuenode(PVM vm, PCONFIG root, TR_ARR* arr, const wchar_t* code, unsigned int *index)
{
	TEXTRANGE range;
	const wchar_t* str;
	TEXTRANGE identrange;
	VALUE val;
	PCONFIG node;
	bool error = false;
	//ident
	identrange = get_ident_range(code, arr, index);
	//L'=' | L'['
	(*index)++;
	range = tr_arr_get(arr, *index);
	str = code + range.start;
	//L'='
	if (str[0] == L'=' && range.length == 1)
	{
		//STRING | NUMBER | LOCALIZATION
		(*index)++;
		range = tr_arr_get(arr, *index);
		str = code + range.start;
		//STRING
		if (cfgparse_string_start(str, range.length))
		{
			cfgparse_string(vm, &val, arr, code, index);
		}
		//NUMBER
		else if (cfgparse_number_start(str, range.length))
		{
			cfgparse_number(vm, &val, arr, code, index);
		}
		//LOCALIZATION
		else if (cfgparse_localization_start(str, range.length))
		{
			cfgparse_localization(vm, &val, arr, code, index);
		}
		else
		{
			vm->print(vm, L"L%u;C%u;", range.line, range.col);
			vm->error(vm, L"Expected ''' or '\"' or '-' or NUMBER or '$'.", 0);
			error = true;
		}
	}
	//L'['
	else if (str[0] == L'[' && range.length == 1)
	{
		//L']'
		(*index)++;
		range = tr_arr_get(arr, *index);
		str = code + range.start;
		if (str[0] != L']' || range.length != 1)
		{
			vm->print(vm, L"L%u;C%u;", range.line, range.col);
			vm->error(vm, L"Expected ']'.", 0);
		}

		//L'='
		(*index)++;
		range = tr_arr_get(arr, *index);
		str = code + range.start;
		if (str[0] != L'=' || range.length != 1)
		{
			vm->print(vm, L"L%u;C%u;", range.line, range.col);
			vm->error(vm, L"Expected '='.", 0);
		}
		//ARRAY
		(*index)++;
		range = tr_arr_get(arr, *index);
		str = code + range.start;
		if (cfgparse_array_start(str, range.length))
		{
			cfgparse_array(vm, &val, arr, code, index);
		}
		else
		{
			vm->print(vm, L"L%u;C%u;", range.line, range.col);
			vm->error(vm, L"Expected '{'.", 0);
		}
	}
	else
	{
		vm->print(vm, L"L%u;C%u;", range.line, range.col);
		vm->error(vm, L"Expected '=' or '['.", 0);
		error = true;
	}
	if (error)
		return;
	str = code + identrange.start;
	node = config_create_node_value(str, identrange.length, val);
	config_push_node(root, node);
	node->parent = root;
}
//STRING = '"' anything '"' | '\'' anything '\'';
bool cfgparse_string_start(const wchar_t* code, unsigned int len) { return code[0] == L'"' || code[0] == L'\''; }
void cfgparse_string(PVM vm, VALUE *out, TR_ARR* arr, const wchar_t* code, unsigned int *index)
{
	TEXTRANGE range;
	const wchar_t* str;
	range = tr_arr_get(arr, *index);
	str = code + range.start;
	*out = value(STRING_TYPE(), base_voidptr(parse_string(vm, str, range.length)));
	(*index)++;
}
//NUMBER = ['-'] num ['.' num];
bool cfgparse_number_start(const wchar_t* code, unsigned int len) { return code[0] == L'-' || (code[0] >= L'0' && code[0] <= L'9'); }
void cfgparse_number(PVM vm, VALUE *out, TR_ARR* arr, const wchar_t* code, unsigned int *index)
{
	TEXTRANGE range;
	const wchar_t* str;
	wchar_t* endptr;
	float f;
	bool invert = false;
	range = tr_arr_get(arr, *index);
	str = code + range.start;
	if (str[0] == L'-' && range.length == 1)
	{
		invert = true;
		(*index)++;
		range = tr_arr_get(arr, *index);
		str = code + range.start;
	}
	f = wcstof(str, &endptr);
	if (invert)
	{
		f *= -1;
	}
	if (endptr != str)
	{
		*out = value(SCALAR_TYPE(), base_float(f));
		(*index)++;
	}
	else
	{
		vm->print(vm, L"L%u;C%u;", range.line, range.col);
		vm->error(vm, L"Expected NUMBER.", 0);
	}
}
//LOCALIZATION = '$' ident;
bool cfgparse_localization_start(const wchar_t* code, unsigned int len) { return code[0] == L'$'; }
void cfgparse_localization(PVM vm, VALUE *out, TR_ARR* arr, const wchar_t* code, unsigned int *index)
{
	TEXTRANGE range;
	const wchar_t* str;
	PSTRING pstring;
	range = tr_arr_get(arr, *index);
	str = code + range.start;
	if (str[0] == L'$' && range.length == 1)
	{
		(*index)++;
		range = tr_arr_get(arr, *index);
		str = code + range.start;
	}
	else
	{
		vm->print(vm, L"L%u;C%u;", range.line, range.col);
		vm->error(vm, L"Expected '$'.", 0);
	}
	pstring = string_create(range.length + 1);
	wcsncpy(pstring->val, str, range.length);
	pstring->val[range.length] = L'\0';
	*out = value(STRING_TYPE(), base_voidptr(pstring));
	(*index)++;
}
//ARRAY = '{' [ VALUE { ',' VALUE } ] '}'
bool cfgparse_array_start(const wchar_t* code, unsigned int len) { return code[0] == L'{'; }
void cfgparse_array(PVM vm, VALUE *out, TR_ARR* arr, const wchar_t* code, unsigned int *index)
{
	TEXTRANGE range;
	const wchar_t* str;
	PARRAY sqfarr = array_create();
	*out = value(ARRAY_TYPE(), base_voidptr(sqfarr));
	VALUE tmp;
	range = tr_arr_get(arr, *index);
	str = code + range.start;
	if (str[0] == L'{' && range.length == 1)
	{
		(*index)++;
		range = tr_arr_get(arr, *index);
		str = code + range.start;
	}
	else
	{
		vm->print(vm, L"L%u;C%u;", range.line, range.col);
		vm->error(vm, L"Expected '{'.", 0);
	}

	while (cfgparse_value_start(str, range.length))
	{
		cfgparse_value(vm, &tmp, arr, code, index);
		array_push(sqfarr, tmp);

		range = tr_arr_get(arr, *index);
		str = code + range.start;
		if (str[0] == ',' && range.length == 1)
		{
			(*index)++;
			range = tr_arr_get(arr, *index);
			str = code + range.start;
		}
		else if (str[0] == '}' && range.length == 1)
		{
			break;
		}
		else
		{
			vm->print(vm, L"L%u;C%u;", range.line, range.col);
			vm->error(vm, L"Expected ','.", 0);
		}
	}

	range = tr_arr_get(arr, *index);
	str = code + range.start;
	if (str[0] == L'}' && range.length == 1)
	{
		(*index)++;
		range = tr_arr_get(arr, *index);
		str = code + range.start;
	}
	else
	{
		vm->print(vm, L"L%u;C%u;", range.line, range.col);
		vm->error(vm, L"Expected '}'.", 0);
	}
}
//VALUE = STRING | NUMBER | LOCALIZATION | ARRAY;
bool cfgparse_value_start(const wchar_t* code, unsigned int len) { return cfgparse_string_start(code, len) || cfgparse_number_start(code, len) || cfgparse_localization_start(code, len) || cfgparse_array_start(code, len); }
void cfgparse_value(PVM vm, VALUE *out, TR_ARR* arr, const wchar_t* code, unsigned int *index)
{
	TEXTRANGE range;
	const wchar_t* str;
	range = tr_arr_get(arr, *index);
	str = code + range.start;
	if (cfgparse_string_start(str, range.length))
	{
		cfgparse_string(vm, out, arr, code, index);
	}
	else if (cfgparse_number_start(str, range.length))
	{
		cfgparse_number(vm, out, arr, code, index);
	}
	else if (cfgparse_localization_start(str, range.length))
	{
		cfgparse_localization(vm, out, arr, code, index);
	}
	else if (cfgparse_array_start(str, range.length))
	{
		cfgparse_array(vm, out, arr, code, index);
	}
	else
	{
		vm->print(vm, L"L%u;C%u;", range.line, range.col);
		vm->error(vm, L"Expected ''' or '\"' or '-' or NUMBER or '$' or '{'.", 0);
	}
}
PCONFIG cfgparse(PVM vm, const wchar_t* code)
{
	TR_ARR* arr = tr_arr_create();
	unsigned int index = 0;
	tokenize(arr, code);

	PCONFIG node = config_create_node(0, 0, 10);
	cfgparse_nodelist(vm, node, arr, code, &index);
	tr_arr_destroy(arr);
	return node;
}

PCONFIG cfgparse2(PVM vm, const wchar_t* code, unsigned int tr_arr_increase, unsigned int node_init_len)
{
	TR_ARR* arr = tr_arr_create();
	arr->increase = tr_arr_increase;
	unsigned int index = 0;
	tokenize(arr, code);

	PCONFIG node = config_create_node(0, 0, node_init_len);
	cfgparse_nodelist(vm, node, arr, code, &index);
	tr_arr_destroy(arr);
	return node;
}