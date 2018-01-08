#ifdef __linux
#include <alloca.h>
#endif // _GCC
#ifdef _WIN32
#include <malloc.h>
#endif // _WIN32
#include <math.h>
#include <sys/timeb.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

#include "sqffull.h"
#include "errors.h"
#include "random.h"

#ifndef M_PI
#define M_PI 3.1415926535
#endif // !M_PI 3.1415926535

extern int64_t systime_start;

int64_t system_time_ms(void)
{
#ifdef _WIN32
	struct _timeb timebuffer;
	_ftime(&timebuffer);
	return (int64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
#else
	struct timeb timebuffer;
	ftime(&timebuffer);
	return (int64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
#endif
}

/**
 * Checks whether the given array is a 3D vector.
 * If not an error will be thrown
 */
char checkVector3(const PVM vm, const PARRAY array)
{
	// size check
	if (array->top != 3)
	{
		vm->error(vm, ERR_LEFT ERR_ARRAY ERR_ARRAY_SIZE_EXACT(3), vm->stack);

		return 0;
	}

	// content type check
	for (int i = 0; i < array->top; i++)
	{
		if (array->data[i]->type != SCALAR_TYPE())
		{
			vm->error(vm, ERR_CAN_ONLY_PROCESS ERR_SCALAR L" " ERR_ARRAYS, vm->stack);

			return 0;
		}
	}

	return 1;
}

void stringify_value(PVM vm, PSTRING str, PVALUE val)
{
	PARRAY arr;
	POBJECT obj;
	PGROUP grp;
	PCONFIG node;
	int i;
	int j;
	wchar_t* strptr;
	wchar_t* strptr2;
#ifdef __linux
#define LINUX_BUFFER_SIZE 8192
	static wchar_t linux_buffer[LINUX_BUFFER_SIZE];
#endif // __linux

	if (val->type == SCALAR_TYPE())
	{
		strptr = alloca(sizeof(wchar_t) * 64);
		swprintf(strptr, 64, L"%g", val->val.f);
		string_modify_append(str, strptr);
	}
	else if (val->type == BOOL_TYPE() || val->type == IF_TYPE())
	{
		string_modify_append(str, val->val.i > 0 ? L"true" : L"false");
	}
	else if (val->type == ARRAY_TYPE())
	{
		arr = ((PARRAY)val->val.ptr);
		string_modify_append(str, L"[");
		for (i = 0; i < arr->top; i++)
		{
			if (i > 0)
			{
				string_modify_append(str, L", ");
			}
			stringify_value(vm, str, arr->data[i]);
		}
		string_modify_append(str, L"]");
	}
	else if (val->type == STRING_TYPE())
	{
		string_modify_append(str, L"\"");
		strptr = ((PSTRING)val->val.ptr)->val;
		if (strptr != 0)
		{
			while ((strptr2 = wcschr(strptr, '"')) != 0)
			{
				string_modify_nappend(str, strptr, strptr2 - strptr);
				string_modify_append(str, L"\"\"");
				strptr = strptr2 + 1;
			}
			if (wcslen(strptr) > 0)
			{
				string_modify_append(str, strptr);
			}
		}
		string_modify_append(str, L"\"");
	}
	else if (val->type == NAN_TYPE())
	{
		string_modify_append(str, L"NaN");
	}
	else if (val->type == CODE_TYPE() || val->type == WHILE_TYPE())
	{
		string_modify_append(str, L"{");
		string_modify_append(str, ((PCODE)val->val.ptr)->val);
		string_modify_append(str, L"}");
	}
	else if (val->type == SIDE_TYPE())
	{
		string_modify_append(str, side_displayname2(val));
	}
	else if (val->type == NOTHING_TYPE())
	{
		string_modify_append(str, L"nil");
	}
	else if (val->type == GROUP_TYPE())
	{
		string_modify_append(str, ((PGROUP)val->val.ptr)->ident);
	}
	else if (val->type == CONFIG_TYPE())
	{
		node = val->val.ptr;
		if (node == 0)
		{
			string_modify_append(str, L"configNull");
		}
		else
		{
			i = (int)config_count_parents(node);
			for (; i >= 0; i--)
			{
				node = val->val.ptr;
				for (j = 0; j < i; j++)
				{
					node = node->parent;
				}
				string_modify_append(str, node->identifier);
				if (i != 0)
				{
					string_modify_append(str, L"/");
				}
			}
		}
	}
	else if (val->type == OBJECT_TYPE())
	{
		obj = val->val.ptr;
		if (obj->is_vehicle)
		{
			//1cd60264080# 1813620: heli_light_01_f.p3d
#ifdef __linux
			swprintf(linux_buffer, LINUX_BUFFER_SIZE, L"%p# %d: %ls", obj, 0, L"NOTIMPLEMENTED");
			string_modify_append(str, linux_buffer);
#else
			i = swprintf(0, 0, L"%p# %d: %ls", obj, 0, L"NOTIMPLEMENTED");
			strptr = alloca(sizeof(wchar_t) * (i + 1));
			swprintf(strptr, i + 1, L"%p# %d: %ls", obj, 0, L"NOTIMPLEMENTED");
			string_modify_append(str, strptr);
#endif
		}
		else
		{
			if (obj->inner == 0)
			{
				string_modify_append(str, L"objNull");
			}
			else
			{
				grp = group_from_ident(vm, ((PUNIT)obj->inner)->groupident->val);
				if (grp == 0)
				{
#ifdef __linux
					swprintf(linux_buffer, LINUX_BUFFER_SIZE, L"%ls:%d", L"NA", 0);
					string_modify_append(str, linux_buffer);
#else
					i = swprintf(0, 0, L"%ls:%d", L"NA", 0);
					strptr = alloca(sizeof(wchar_t) * (i + 1));
					swprintf(strptr, i + 1, L"%ls:%d", L"NA", 0);
					string_modify_append(str, strptr);
#endif
				}
				else
				{
					for (i = 0; i < ((PARRAY)grp->members->val.ptr)->top; i++)
					{
						if (((PARRAY)grp->members->val.ptr)->data[0]->val.ptr
							== obj)
							break;
					}
					j = i;
#ifdef __linux
					swprintf(linux_buffer, LINUX_BUFFER_SIZE, L"%ls:%d", grp->ident, j);
					string_modify_append(str, linux_buffer);
#else
					i = swprintf(0, 0, L"%ls:%d", grp->ident, j);
					strptr = alloca(sizeof(wchar_t) * (i + 1));
					swprintf(strptr, i + 1, L"%ls:%d", grp->ident, j);
					string_modify_append(str, strptr);
#endif
				}
			}
		}
	}
	else if (val->type == ANY_TYPE())
	{
		string_modify_append(str, L"any");
	}
	else
	{
		string_modify_append(str, L"<");
		string_modify_append(str, val->type->name);
		string_modify_append(str, L">");
	}
}

void cmd_isequalto(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	push_stack(vm, vm->stack,
		inst_value(
			value(BOOL_TYPE(),
				base_int(is_equal_to(left_val, right_val)))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_isequaltype(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	push_stack(vm, vm->stack,
		inst_value(
			value(BOOL_TYPE(),
				base_int(left_val->type == right_val->type))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_plus(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY arr;
	PARRAY outarr;
	PSTRING str;
	int i;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type == SCALAR_TYPE())
	{
		if (right_val->type != SCALAR_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		push_stack(vm, vm->stack,
			inst_value(
				value(left_val->type,
					base_float(
						left_val->val.f + right_val->val.f))));
	}
	else if (left_val->type == STRING_TYPE())
	{
		if (right_val->type != STRING_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		str = string_concat(((PSTRING)left_val->val.ptr),
			((PSTRING)right_val->val.ptr));
		push_stack(vm, vm->stack,
			inst_value(value(STRING_TYPE(), base_voidptr(str))));
	}
	else if (left_val->type == ARRAY_TYPE())
	{
		arr = ((PARRAY)left_val->val.ptr);
		outarr = array_create2(
			arr->top
				+ (right_val->type == ARRAY_TYPE() ?
					((PARRAY)right_val->val.ptr)->top : 1));
		for (i = 0; i < arr->top; i++)
		{
			array_push(outarr, value(arr->data[i]->type, arr->data[i]->val));
		}
		if (right_val->type == ARRAY_TYPE())
		{
			arr = ((PARRAY)right_val->val.ptr);
			for (i = 0; i < arr->top; i++)
			{
				array_push(outarr,
					value(arr->data[i]->type, arr->data[i]->val));
			}
		}
		else
		{
			array_push(outarr, value(right_val->type, right_val->val));
		}
		push_stack(vm, vm->stack,
			inst_value(value(ARRAY_TYPE(), base_voidptr(outarr))));
	}
	else
	{
		vm->error(vm,
		ERR_LEFT_TYPE ERR_SCALAR ERR_OR ERR_STRING ERR_OR ERR_ARRAY, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_plus_UNARY(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == ARRAY_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(ARRAY_TYPE(),
					base_voidptr(array_copy(right_val->val.ptr)))));
		inst_destroy(right);
	}
	else if (right_val->type == SCALAR_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(value(right_val->type, right_val->val)));
		inst_destroy(right);
	}
	else if (right_val->type == NAN_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(value(right_val->type, right_val->val)));
		inst_destroy(right);
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_OR ERR_SCALAR ERR_OR ERR_NAN,
			vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
}
void cmd_minus(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY larr;
	PARRAY rarr;
	PARRAY outarr;
	int i;
	int j;
	int k;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type == SCALAR_TYPE())
	{
		if (right_val->type != SCALAR_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		push_stack(vm, vm->stack,
			inst_value(
				value(left_val->type,
					base_float(
						left_val->val.f - right_val->val.f))));
	}
	else if (left_val->type == ARRAY_TYPE())
	{
		if (right_val->type != ARRAY_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		k = 0;
		larr = left_val->val.ptr;
		rarr = right_val->val.ptr;
		for (i = 0; i < larr->top; i++)
		{
			for (j = 0; j < rarr->top; j++)
			{
				if (is_equal_to(larr->data[i], rarr->data[j]))
				{
					break;
				}
			}
			if (j == rarr->top)
			{
				k++;
			}
		}

		outarr = array_create2(k);
		for (i = 0; i < larr->top; i++)
		{
			for (j = 0; j < rarr->top; j++)
			{
				if (is_equal_to(larr->data[i], rarr->data[j]))
				{
					break;
				}
			}
			if (j == rarr->top)
			{
				array_push(outarr,
					value(larr->data[i]->type, larr->data[i]->val));
			}
		}
		push_stack(vm, vm->stack,
			inst_value(value(ARRAY_TYPE(), base_voidptr(outarr))));
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR ERR_OR ERR_ARRAY, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_minus_UNARY(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	push_stack(vm, vm->stack,
		inst_value(value(right_val->type, base_float(-right_val->val.f))));
	inst_destroy(right);
}
void cmd_multiply(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	push_stack(vm, vm->stack,
		inst_value(
			value(left_val->type,
				base_float(left_val->val.f * right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_divide(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type == SCALAR_TYPE())
	{
		if (right_val->type != SCALAR_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		if (right_val->val.f == 0)
		{
			vm->error(vm, ERR_SPECIAL_DIVIDE_1, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack, inst_value(value(NAN_TYPE(), base_int(0))));
			return;
		}
		push_stack(vm, vm->stack,
			inst_value(
				value(left_val->type,
					base_float(left_val->val.f / right_val->val.f))));
		inst_destroy(left);
		inst_destroy(right);
	}
	else if (left_val->type == CONFIG_TYPE())
	{
		if (right_val->type != STRING_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		cmd_navigateconfighelper(vm, left_val->val.ptr, right_val->val.ptr);
		inst_destroy(left);
		inst_destroy(right);
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR ERR_OR ERR_CONFIG, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
}
void cmd_diag_LOG(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PSTRING str = string_create(0);
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		string_destroy(str);
		inst_destroy(right);
		return;
	}
	stringify_value(vm, str, right_val);
	vm->print(vm, L"[DIAG_LOG]: %ls\n", str->val);
	string_destroy(str);
	inst_destroy(right);
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
}
void cmd_hint(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != STRING_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (((PSTRING)right_val->val.ptr)->val == 0)
	{
		vm->print(vm, L"[HINT]: \n");
	}
	else
	{
		vm->print(vm, L"[HINT]: %ls\n", ((PSTRING)right_val->val.ptr)->val);
	}
	inst_destroy(right);
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
}
void cmd_systemchat(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != STRING_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (((PSTRING)right_val->val.ptr)->val == 0)
	{
		vm->print(vm, L"[SYSTEMCHAT]: \n");
	}
	else
	{
		vm->print(vm, L"[SYSTEMCHAT]: %ls\n",
			((PSTRING)right_val->val.ptr)->val);
	}
	inst_destroy(right);
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
}
void cmd_private(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PSTRING str;
	PARRAY arr;
	int i;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == STRING_TYPE())
	{
		str = right_val->val.ptr;
		if (str->length == 0)
		{
			vm->error(vm, ERR_RIGHT ERR_NOT_EMPTY, vm->stack);
		}
		else if (str->val[0] != '_')
		{
			vm->error(vm, ERR_SPECIAL_PRIVATE_1, vm->stack);
		}
		else
		{
			store_in_scope(vm, top_scope(vm), str->val,
				value(ANY_TYPE(), base_voidptr(0)));
		}
	}
	else if (right_val->type == ARRAY_TYPE())
	{
		arr = right_val->val.ptr;
		for (i = 0; i < arr->top; i++)
		{
			if (arr->data[i]->type != STRING_TYPE())
			{
				vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
			}
			else
			{
				str = arr->data[i]->val.ptr;
				if (str->length == 0)
				{
					vm->error(vm, ERR_RIGHT ERR_NOT_EMPTY, vm->stack);
				}
				else if (str->val[0] != '_')
				{
					vm->error(vm, ERR_SPECIAL_PRIVATE_1, vm->stack);
				}
				else
				{
					store_in_scope(vm, top_scope(vm), str->val,
						value(ANY_TYPE(), base_voidptr(0)));
				}
			}
		}
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING ERR_OR ERR_ARRAY, vm->stack);
	}

	inst_destroy(right);
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
}

void cmd_if(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	int flag;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == BOOL_TYPE())
	{
		flag = right_val->val.i > 0;
	}
	else if (right_val->type == SCALAR_TYPE())
	{
		flag = right_val->val.f > 0;
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_BOOL ERR_OR ERR_SCALAR, vm->stack);
	}
	push_stack(vm, vm->stack, inst_value(value(IF_TYPE(), base_int(flag))));

	inst_destroy(right);
}
void cmd_then(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PCODE code;
	PARRAY arr;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	if (left_val->type != IF_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_IF, vm->stack);
	}
	if (right_val->type == ARRAY_TYPE())
	{
		arr = right_val->val.ptr;
		if (arr->top == 0)
		{
			vm->error(vm, ERR_RIGHT ERR_NOT_EMPTY, vm->stack);
		}
		else if (arr->data[0]->type != CODE_TYPE())
		{
			vm->error(vm,
			ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_CODE,
				vm->stack);
		}
		else if (arr->top > 1 && arr->data[1]->type != CODE_TYPE())
		{
			vm->error(vm,
			ERR_ERR ERR_ARRAY_(1) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_CODE,
				vm->stack);
		}
		else
		{
			if (left_val->val.i)
			{
				code = arr->data[0]->val.ptr;
			}
			else if (arr->top > 1)
			{
				code = arr->data[1]->val.ptr;
			}
			if (code != 0)
			{
				push_stack(vm, vm->stack, inst_code_load(1));
				push_stack(vm, vm->stack,
					inst_value(value(CODE_TYPE(), base_voidptr(code))));
			}
		}
	}
	else if (right_val->type == CODE_TYPE())
	{
		if (left_val->val.i)
		{
			push_stack(vm, vm->stack, inst_code_load(1));
			push_stack(vm, vm->stack,
				inst_value(
					value(CODE_TYPE(),
						base_voidptr(right_val->val.ptr))));
		}
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CODE ERR_OR ERR_ARRAY, vm->stack);
	}

	inst_destroy(left);
	inst_destroy(right);
}
void cmd_else(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY arr;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	arr = array_create2(2);
	array_push(arr, value(CODE_TYPE(), left_val->val));
	array_push(arr, value(CODE_TYPE(), right_val->val));
	push_stack(vm, vm->stack,
		inst_value(value(ARRAY_TYPE(), base_voidptr(arr))));

	inst_destroy(left);
	inst_destroy(right);
}
void cmd_true(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(1))));
}
void cmd_false(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(0))));
}
void cmd_help(void* input, CPCMD self)
{
	PVM vm = input;
	int i;
	int count;
	CPCMD cmd;
	vm->print(vm, L"ERRORS might result in crash\n\n");
	vm->print(vm, L"NAME:TYPE:PRECEDENCE:USAGE\n");
	count = wsm_count(vm->cmd_container->types);
	for (i = 0; i < count; i++)
	{
		cmd = wsm_get_value_index(vm->cmd_container->types, i);
		vm->print(vm, L"%ls:%c:%d:%ls\n", cmd->name, cmd->type,
			cmd->precedence_level, cmd->usage);
	}
	count = wsm_count(vm->cmd_container->nullar);
	for (i = 0; i < count; i++)
	{
		cmd = wsm_get_value_index(vm->cmd_container->nullar, i);
		vm->print(vm, L"%ls:%c:%d:%ls\n", cmd->name, cmd->type,
			cmd->precedence_level, cmd->usage);
	}
	count = wsm_count(vm->cmd_container->unary);
	for (i = 0; i < count; i++)
	{
		cmd = wsm_get_value_index(vm->cmd_container->unary, i);
		vm->print(vm, L"%ls:%c:%d:%ls\n", cmd->name, cmd->type,
			cmd->precedence_level, cmd->usage);
	}
	count = wsm_count(vm->cmd_container->binary);
	for (i = 0; i < count; i++)
	{
		cmd = wsm_get_value_index(vm->cmd_container->binary, i);
		vm->print(vm, L"%ls:%c:%d:%ls\n", cmd->name, cmd->type,
			cmd->precedence_level, cmd->usage);
	}
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
}
void cmd_help_unary_helper(PVM vm, CPCMD cmd)
{
	wchar_t* ptr;
	wchar_t* ptr2;
	wchar_t* ptr3;
	vm->print(vm, L"<%ls> %ls\n",
		cmd->type == 'b' ? L"BINARY" : cmd->type == 'u' ? L"UNARY" :
		cmd->type == 'n' ? L"NULLAR" : L"TYPE", cmd->name);
	if (cmd->description != 0 && *cmd->description != '\0')
	{
		vm->print(vm, L"\tDescription:\n\t\t%ls\n", cmd->description);
	}
	if (cmd->type == 'b')
	{
		vm->print(vm, L"\tPrecedence: %d\n", cmd->precedence_level);
	}
	if (cmd->usage && *cmd->usage != '\0')
	{
		vm->print(vm, L"\tUsage:\n");
		ptr = cmd->usage;
		ptr3 = ptr;
		while ((ptr2 = wcschr(ptr3, '|')) != 0)
		{
			if (ptr2[1] == '|')
			{
				ptr3 = ptr2 + 2;
				continue;
			}
			while (*ptr == ' ')
				ptr++;
			vm->print(vm, L"\t\t%.*ls\n", ptr2 - ptr, ptr);
			ptr = ptr3 = ptr2 + 1;
		}
		if (wcslen(ptr) > 0)
		{
			while (*ptr == ' ')
				ptr++;
			if (wcslen(ptr) > 0)
			{
				vm->print(vm, L"\t\t%ls\n", ptr);
			}
		}
	}
	if (cmd->examples && *cmd->examples != '\0')
	{
		vm->print(vm, L"\tExamples:\n");
		ptr = cmd->examples;
		while ((ptr2 = wcschr(ptr, '#')) != 0)
		{
			while (*ptr == ' ')
				ptr++;
			vm->print(vm, L"\t\t%.*ls\n", ptr2 - ptr, ptr);
			ptr = ptr2 + 1;
		}
		if (wcslen(ptr) > 0)
		{
			while (*ptr == ' ')
				ptr++;
			if (wcslen(ptr) > 0)
			{
				vm->print(vm, L"\t\t%ls\n", ptr);
			}
		}
	}
}
void cmd_help_UNARY(void* input, CPCMD self)
{
	PVM vm = input;
	CPCMD cmd;
	PINST right;
	PVALUE right_val;
	PSTRING str;
	bool had_match = false;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != STRING_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
		inst_destroy(right);
		return;
	}
	if (((PSTRING)right_val->val.ptr)->length == 0)
	{
		vm->error(vm, ERR_RIGHT ERR_WAS_EXPECTED ERR_NON_EMPTY_STRING,
			vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	str = right_val->val.ptr;
	cmd = wsm_get_value(vm->cmd_container->types, str->val);
	if (cmd != 0)
	{
		if (!had_match)
		{
			had_match = true;
		}
		else
		{
			vm->print(vm, L"\n");
		}
		cmd_help_unary_helper(vm, cmd);
	}
	cmd = wsm_get_value(vm->cmd_container->nullar, str->val);
	if (cmd != 0)
	{
		if (!had_match)
		{
			had_match = true;
		}
		else
		{
			vm->print(vm, L"\n");
		}
		cmd_help_unary_helper(vm, cmd);
	}
	cmd = wsm_get_value(vm->cmd_container->unary, str->val);
	if (cmd != 0)
	{
		if (!had_match)
		{
			had_match = true;
		}
		else
		{
			vm->print(vm, L"\n");
		}
		cmd_help_unary_helper(vm, cmd);
	}
	cmd = wsm_get_value(vm->cmd_container->binary, str->val);
	if (cmd != 0)
	{
		if (!had_match)
		{
			had_match = true;
		}
		else
		{
			vm->print(vm, L"\n");
		}
		cmd_help_unary_helper(vm, cmd);
	}
	if (!had_match)
	{
		vm->print(vm, L"'%ls' could not be located.\n", str->val);
	}
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(right);
}
void cmd_parseconfig(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PSTRING str;
	PCONFIG node;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != STRING_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
		inst_destroy(right);
		return;
	}
	str = right_val->val.ptr;
	node = cfgparse(vm, str->val);
	inst_destroy(right);
	push_stack(vm, vm->stack, inst_value(value(CONFIG_TYPE(), base_voidptr(node))));
}

void cmd_mergefrom(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PCONFIG into;
	PCONFIG from;
	unsigned int i;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != CONFIG_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_CONFIG, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != CONFIG_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CONFIG, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	from = right_val->val.ptr;
	into = left_val->val.ptr;
	config_merge(into, from);
	push_stack(vm, vm->stack, left);
	inst_destroy(right);
}


void cmd_str(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PSTRING str = string_create(0);
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	stringify_value(vm, str, right_val);
	inst_destroy(right);
	push_stack(vm, vm->stack,
		inst_value(value(STRING_TYPE(), base_voidptr(str))));
}

void cmd_greaterthan(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	push_stack(vm, vm->stack,
		inst_value(
			value(BOOL_TYPE(),
				base_int(left_val->val.f > right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_lessthen(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	push_stack(vm, vm->stack,
		inst_value(
			value(BOOL_TYPE(),
				base_int(left_val->val.f < right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_largetthenorequal(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	push_stack(vm, vm->stack,
		inst_value(
			value(BOOL_TYPE(),
				base_int(left_val->val.f >= right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_lessthenorequal(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	push_stack(vm, vm->stack,
		inst_value(
			value(BOOL_TYPE(),
				base_int(left_val->val.f <= right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_equal(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SCALAR_TYPE() && left_val->type != STRING_TYPE()
		&& left_val->type != OBJECT_TYPE())
	{
		vm->error(vm,
		ERR_LEFT_TYPE ERR_SCALAR ERR_OR ERR_STRING ERR_OR ERR_OBJECT,
			vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE() && right_val->type != STRING_TYPE()
		&& right_val->type != OBJECT_TYPE())
	{
		vm->error(vm,
		ERR_RIGHT_TYPE ERR_SCALAR ERR_OR ERR_STRING ERR_OR ERR_OBJECT,
			vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != right_val->type)
	{
		vm->error(vm, ERR_ERR ERR_TYPES ERR_EQUAL_TYPE, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	else if (left_val->type == SCALAR_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(BOOL_TYPE(),
					base_int(
						left_val->val.f == right_val->val.f))));
	}
	else if (left_val->type == STRING_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(BOOL_TYPE(),
					base_int(
						!wstr_cmpi(
						((PSTRING)left_val->val.ptr)->val,
							-1,
							((PSTRING)right_val->val.ptr)->val,
							-1)))));
	}
	else if (left_val->type == OBJECT_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(BOOL_TYPE(),
					base_int(
						left_val->val.ptr
							== right_val->val.ptr))));
	}
	else
	{
		vm->error(vm, L"MISSING TYPE CHECK, PLEASE RAISE ISSUE IN GITHUB", vm->stack);
	}
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_notequal(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SCALAR_TYPE() && left_val->type != STRING_TYPE()
		&& left_val->type != OBJECT_TYPE())
	{
		vm->error(vm,
		ERR_LEFT_TYPE ERR_SCALAR ERR_OR ERR_STRING ERR_OR ERR_OBJECT,
			vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE() && right_val->type != STRING_TYPE()
		&& right_val->type != OBJECT_TYPE())
	{
		vm->error(vm,
		ERR_RIGHT_TYPE ERR_SCALAR ERR_OR ERR_STRING ERR_OR ERR_OBJECT,
			vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != right_val->type)
	{
		vm->error(vm, ERR_ERR ERR_TYPES ERR_EQUAL_TYPE, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	else if (left_val->type == SCALAR_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(BOOL_TYPE(),
					base_int(
						left_val->val.f != right_val->val.f))));
	}
	else if (left_val->type == STRING_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(BOOL_TYPE(),
					base_int(
						wstr_cmpi(
						((PSTRING)left_val->val.ptr)->val,
							-1,
							((PSTRING)right_val->val.ptr)->val,
							-1)))));
	}
	else if (left_val->type == OBJECT_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(BOOL_TYPE(),
					base_int(
						left_val->val.ptr
							!= right_val->val.ptr))));
	}
	else
	{
		vm->error(vm, L"MISSING TYPE CHECK, PLEASE RAISE ISSUE IN GITHUB", vm->stack);
	}
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_andand(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != BOOL_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_BOOL, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type == BOOL_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(left_val->type,
					base_int(
						left_val->val.i && right_val->val.i))));
		inst_destroy(left);
		inst_destroy(right);
	}
	else if (right_val->type == CODE_TYPE())
	{
		if (!left_val->val.i)
		{
			push_stack(vm, vm->stack,
				inst_value(value(left_val->type, base_int(0))));
		}
		else
		{
			push_stack(vm, vm->stack, inst_command(self));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, inst_code_load(1));
			push_stack(vm, vm->stack, right);
		}
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_BOOL, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
}
void cmd_oror(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != BOOL_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_BOOL, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type == BOOL_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(left_val->type,
					base_int(
						left_val->val.i || right_val->val.i))));
		inst_destroy(left);
		inst_destroy(right);
	}
	else if (right_val->type == CODE_TYPE())
	{
		if (left_val->val.i)
		{
			push_stack(vm, vm->stack,
				inst_value(value(left_val->type, base_int(1))));
		}
		else
		{
			push_stack(vm, vm->stack, inst_command(self));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, inst_code_load(1));
			push_stack(vm, vm->stack, right);
		}
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_BOOL, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
}

void cmd_select(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY arr;
	PVALUE tmp;
	PCONFIG node;
	int index;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	//ToDo: implement more select variants https://community.bistudio.com/wiki/select

	if (left_val->type == ARRAY_TYPE())
	{
		arr = left_val->val.ptr;
		if (right_val->type == SCALAR_TYPE())
		{
			index = roundf(right_val->val.f);
		}
		else if (right_val->type == BOOL_TYPE())
		{
			index = right_val->val.i > 0 ? 1 : 0;
		}
		else
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR ERR_OR ERR_BOOL, vm->stack);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			inst_destroy(left);
			inst_destroy(right);
			return;
		}

		if (index < 0 || index > arr->top)
		{
			vm->error(vm, ERR_SPECIAL_SELECT_1, vm->stack);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			inst_destroy(left);
			inst_destroy(right);
			return;
		}
	  else if (index == arr->top)
	  {
		  push_stack(vm, vm->stack,
		  	inst_value(value(NOTHING_TYPE(), base_int(0))));
		  vm->warn(vm, WARN_SELECT_NIL_ARRSIZE, vm->stack);
	  }
	  else
	  {
		  tmp = arr->data[index];
		  push_stack(vm, vm->stack, inst_value(value(tmp->type, tmp->val)));
	  }
	}
	else if (left_val->type == CONFIG_TYPE())
	{
		node = left_val->val.ptr;
		if (right_val->type == SCALAR_TYPE())
		{
			index = roundf(right_val->val.f);
		}
		else
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			inst_destroy(left);
			inst_destroy(right);
			return;
		}

		//ToDo: Check if `CONFIG select NUMBER (index == config childcount)` returns nil or errors
		if (index < 0 || index > node->children_top)
		{
			vm->error(vm, ERR_SPECIAL_SELECT_1, vm->stack);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			inst_destroy(left);
			inst_destroy(right);
			return;
		}
		else if (index == node->children_top)
		{
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
		}
		else
		{
			push_stack(vm, vm->stack, inst_value(value(CONFIG_TYPE(),
				base_voidptr(node->value.cfgnodes[index]))));
		}
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY ERR_OR ERR_CONFIG, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	inst_destroy(left);
	inst_destroy(right);
}

void cmd_selectrandom(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PARRAY arr;
	PVALUE tmp;
	int index;
#if _WIN32
	double rndres;
#endif
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}

	if (!(right_val->type == ARRAY_TYPE()))
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(right);
		return;
	}

	arr = right_val->val.ptr;

	if (arr->top == 0)
	{
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(right);
		return;
	}

#if _WIN32
	rndres = randlf(arr->top);
	index = (int)floor(rndres);
	if (index >= arr->top)
	{
		index = arr->top - 1;
	}
#else
	index = (int)floor(randlf(arr->top));
#endif

	tmp = arr->data[index];

	push_stack(vm, vm->stack, inst_value(value2(tmp)));

	inst_destroy(right);
}

void cmd_while(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != CODE_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CODE, vm->stack);
		inst_destroy(right);
		return;
	}
	right_val->type = WHILE_TYPE();
	push_stack(vm, vm->stack, right);
}
void cmd_do(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PINST tmpinst;
	PVALUE left_val;
	PVALUE right_val;
	PCODE code;
	PFOR pfor;
	PSWITCH swtch;
	PSCOPE scope;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != CODE_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CODE, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	else
	{
		code = right_val->val.ptr;
	}
	if (left_val->type == WHILE_TYPE())
	{
		push_stack(vm, vm->stack, inst_command(find_command(vm, L"do", 'b')));
		push_stack(vm, vm->stack, left);
		push_stack(vm, vm->stack, right);
		push_stack(vm, vm->stack, inst_code_load(0));
		push_stack(vm, vm->stack,
			inst_value(value(CODE_TYPE(), base_voidptr(code))));
		push_stack(vm, vm->stack, inst_pop_eval(5, 0));
		push_stack(vm, vm->stack, inst_code_load(0));
		push_stack(vm, vm->stack,
			inst_value(value(CODE_TYPE(), left_val->val)));
		push_stack(vm, vm->stack, inst_clear_work());
	}
	else if (left_val->type == FOR_TYPE())
	{
		pfor = left_val->val.ptr;
		if (pfor->started)
		{
			pfor->current += pfor->step;
		}
		else
		{
			pfor->current = pfor->start;
			pfor->started = 1;
		}
		if (pfor->step > 0 ?
								pfor->current < pfor->end :
								pfor->current > pfor->end)
		{
			push_stack(vm, vm->stack,
				inst_command(find_command(vm, L"do", 'b')));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, right);
			push_stack(vm, vm->stack, inst_scope(L"loop"));
			push_stack(vm, vm->stack, inst_code_load(0));
			push_stack(vm, vm->stack,
				inst_value(value(CODE_TYPE(), right_val->val)));
			push_stack(vm, vm->stack, inst_store_var_local(pfor->variable));
			push_stack(vm, vm->stack,
				inst_value(
					value(SCALAR_TYPE(), base_float(pfor->current))));
			push_stack(vm, vm->stack, inst_clear_work());
		}
		else
		{
			inst_destroy(left);
			inst_destroy(right);
		}
	}
	else if (left_val->type == SWITCH_TYPE())
	{
		swtch = left_val->val.ptr;
		if (swtch->was_executed)
		{
			if (swtch->selected_code != 0)
			{
				push_stack(vm, vm->stack, inst_code_load(0));
				push_stack(vm, vm->stack,
					inst_value(
						value(CODE_TYPE(), swtch->selected_code->val)));
			}
			else if (swtch->default_code != 0)
			{
				push_stack(vm, vm->stack, inst_code_load(0));
				push_stack(vm, vm->stack,
					inst_value(
						value(CODE_TYPE(), swtch->default_code->val)));
			}
			else
			{
				push_stack(vm, vm->stack,
					inst_value(value(BOOL_TYPE(), base_int(1))));
			}
			inst_destroy(left);
			inst_destroy(right);
		}
		else
		{
			swtch->was_executed = 1;
			push_stack(vm, vm->stack,
				inst_command(find_command(vm, L"do", 'b')));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, right);
			push_stack(vm, vm->stack, inst_scope(L"switch"));
			push_stack(vm, vm->stack, inst_code_load(0));
			push_stack(vm, vm->stack,
				inst_value(value(CODE_TYPE(), right_val->val)));
			push_stack(vm, vm->stack, inst_store_var_local(SWITCH_SPECIAL_VAR));
			push_stack(vm, vm->stack, inst_value(value(left->type, left->data)));
			push_stack(vm, vm->stack, inst_clear_work());
		}
	}
	else if (left_val->type == WITH_TYPE())
	{
		tmpinst = inst_scope(0);
		scope = get_scope(vm, vm->stack, tmpinst);
		scope->ns = left_val->val.ptr;
		push_stack(vm, vm->stack, tmpinst);
		push_stack(vm, vm->stack, inst_code_load(0));
		push_stack(vm, vm->stack,
			inst_value(value(CODE_TYPE(), right_val->val)));
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_WHILE ERR_OR ERR_FOR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
}
void cmd_typename(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PSTRING str;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	str = string_create2(right_val->type->name);
	push_stack(vm, vm->stack,
		inst_value(value(STRING_TYPE(), base_voidptr(str))));
	inst_destroy(right);
}
void cmd_count(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PCOUNT count;
	PARRAY arr;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type == CODE_TYPE())
	{
		if (right_val->type != ARRAY_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			return;
		}
		arr = right_val->val.ptr;
		if (arr->top == 0)
		{
			inst_destroy(left);
			inst_destroy(right);
		}
		else
		{
			count = count_create(left_val->val.ptr, right_val->val.ptr);
			push_stack(vm, vm->stack, inst_command(self));
			push_stack(vm, vm->stack,
				inst_value(value(COUNT_TYPE(), base_voidptr(count))));
			push_stack(vm, vm->stack, inst_scope(0));
			push_stack(vm, vm->stack, inst_code_load(0));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, inst_store_var_local(L"_x"));
			push_stack(vm, vm->stack,
				inst_value(
					value(arr->data[count->curtop]->type,
						arr->data[count->curtop]->val)));
			inst_destroy(right);
		}
	}
	else if (left_val->type == COUNT_TYPE())
	{
		if (right_val->type != BOOL_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_BOOL, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			return;
		}
		count = left_val->val.ptr;
		arr = count->arr->val.ptr;
		if (right_val->val.i)
		{
			count->count++;
		}
		if (++(count->curtop) == arr->top)
		{
			push_stack(vm, vm->stack,
				inst_value(value(SCALAR_TYPE(), base_float(count->count))));
			inst_destroy(left);
			inst_destroy(right);
		}
		else
		{
			push_stack(vm, vm->stack, inst_command(self));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, inst_scope(0));
			push_stack(vm, vm->stack, inst_code_load(0));
			push_stack(vm, vm->stack,
				inst_value(
					value(CODE_TYPE(),
						base_voidptr(count->code->val.ptr))));
			push_stack(vm, vm->stack, inst_store_var_local(L"_x"));
			push_stack(vm, vm->stack,
				inst_value(
					value(arr->data[count->curtop]->type,
						arr->data[count->curtop]->val)));
			inst_destroy(right);
		}
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_CODE ERR_OR ERR_COUNT, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
	}
}
void cmd_count_UNARY(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}

	if (right_val->type == STRING_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(SCALAR_TYPE(),
					base_float(
						((PSTRING)right_val->val.ptr)->length))));
	}
	else if (right_val->type == ARRAY_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(SCALAR_TYPE(),
					base_float(
					((PARRAY)right_val->val.ptr)->top))));
	}
	else if (right_val->type == CONFIG_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(SCALAR_TYPE(),
					base_float(
					((PCONFIG)right_val->val.ptr)->children_top))));
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING ERR_OR ERR_ARRAY ERR_OR ERR_CONFIG, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
	}
	inst_destroy(right);
}
void cmd_format(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PSTRING str;
	PSTRING str_out;
	PARRAY arr;
	wchar_t* ptr;
	wchar_t* ptr_last;
	wchar_t* endptr;
	int index;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}

	if (right_val->type == ARRAY_TYPE())
	{
		arr = right_val->val.ptr;
		if (arr->top == 0)
		{
			vm->error(vm, ERR_RIGHT ERR_NOT_EMPTY, vm->stack);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
		}
		else if (arr->data[0]->type != STRING_TYPE())
		{
			vm->error(vm,
			ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_STRING,
				vm->stack);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
		}
		else
		{
			str = arr->data[0]->val.ptr;
			str_out = string_create(0);
			ptr_last = str->val;
			while ((ptr = wcschr(ptr_last, '%')) != 0)
			{
				string_modify_nappend(str_out, ptr_last, ptr - ptr_last);
				index = wcstof(ptr + 1, &endptr);
				if (endptr == ptr)
				{
					vm->error(vm, ERR_ERR ERR_SPECIAL_FORMAT_1, vm->stack);
				}
				else if (index >= arr->top)
				{
					vm->error(vm, ERR_ERR ERR_SPECIAL_FORMAT_2, vm->stack);
				}
				else
				{
					if (arr->data[index]->type != STRING_TYPE())
					{
						stringify_value(vm, str_out, arr->data[index]);
					}
					else
					{
						string_modify_append(str_out,
							((PSTRING)arr->data[index]->val.ptr)->val);
					}
				}
				ptr_last = endptr;
			}
			string_modify_append(str_out, ptr_last);
			push_stack(vm, vm->stack,
				inst_value(value(STRING_TYPE(), base_voidptr(str_out))));
		}
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
	}
	inst_destroy(right);
}
void cmd_call(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	if (right_val->type == CODE_TYPE())
	{
		push_stack(vm, vm->stack, inst_scope(0));
		push_stack(vm, vm->stack, inst_code_load(0));
		push_stack(vm, vm->stack, right);
		push_stack(vm, vm->stack, inst_store_var_local(L"_this"));
		push_stack(vm, vm->stack,
			inst_value(value(left_val->type, left_val->val)));
		inst_destroy(left);
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CODE, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(right);
	}
}
void cmd_call_UNARY(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}

	if (right_val->type == CODE_TYPE())
	{
		push_stack(vm, vm->stack, inst_scope(0));
		push_stack(vm, vm->stack, inst_code_load(0));
		push_stack(vm, vm->stack, right);
		push_stack(vm, vm->stack, inst_store_var_local(L"_this"));
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CODE, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(right);
	}
}
void cmd_spawn(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PSCRIPT script;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	if (right_val->type == CODE_TYPE())
	{
		script = script_create(vm);
		sqfvm_pushscript(vm, script);
		push_stack(vm, script->stack, inst_scope(0));
		push_stack(vm, script->stack, inst_code_load(0));
		push_stack(vm, script->stack, right);
		push_stack(vm, script->stack, inst_store_var_local(L"_this"));
		push_stack(vm, script->stack,
			inst_value(value(left_val->type, left_val->val)));
		push_stack(vm, script->stack, inst_store_var_local(L"_thisScript"));
		push_stack(vm, script->stack,
			inst_value(value(SCRIPT_TYPE(), base_voidptr(script))));
		push_stack(vm, vm->stack,
			inst_value(value(SCRIPT_TYPE(), base_voidptr(script))));
		inst_destroy(left);
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CODE, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(right);
	}
}
void cmd_scriptdone(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}

	if (right_val->type == SCRIPT_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(BOOL_TYPE(),
					base_int(
						((PSCRIPT)right_val->val.ptr)->stack->top
							== 0 ? 1 : 0))));
		inst_destroy(right);
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCRIPT, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(right);
	}
}

void cmd_for(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PFOR f;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != STRING_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
		inst_destroy(right);
		return;
	}
	if (((PSTRING)right_val->val.ptr)->length == 0)
	{
		vm->error(vm, ERR_RIGHT ERR_NOT_EMPTY, vm->stack);
		inst_destroy(right);
		return;
	}
	if (((PSTRING)right_val->val.ptr)->val[0] != '_')
	{
		vm->error(vm, ERR_SPECIAL_FOR_1, vm->stack);
		inst_destroy(right);
		return;
	}
	f = for_create(((PSTRING)right_val->val.ptr)->val);
	push_stack(vm, vm->stack, inst_value(value(FOR_TYPE(), base_voidptr(f))));
	inst_destroy(right);
}
void cmd_from(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PFOR f;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != FOR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_FOR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	f = left_val->val.ptr;
	f->start = (int)roundf(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(FOR_TYPE(), base_voidptr(f))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_to(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PFOR f;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != FOR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_FOR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	f = left_val->val.ptr;
	f->end = (int)roundf(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(FOR_TYPE(), base_voidptr(f))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_step(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PFOR f;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != FOR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_FOR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	f = left_val->val.ptr;
	f->step = right_val->val.f;
	push_stack(vm, vm->stack, inst_value(value(FOR_TYPE(), base_voidptr(f))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_parsingnamespace(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack,
		inst_value(
			value(NAMESPACE_TYPE(),
				base_voidptr(sqf_parsingNamespace()))));
}
void cmd_missionnamespace(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack,
		inst_value(
			value(NAMESPACE_TYPE(),
				base_voidptr(sqf_missionNamespace()))));
}
void cmd_uinamespace(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack,
		inst_value(
			value(NAMESPACE_TYPE(), base_voidptr(sqf_uiNamespace()))));
}
void cmd_profilenamespace(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack,
		inst_value(
			value(NAMESPACE_TYPE(),
				base_voidptr(sqf_profileNamespace()))));
}
void cmd_diag_TICKTIME(void* input, CPCMD self)
{
	PVM vm = input;
	int64_t systime_cur = system_time_ms();
	push_stack(vm, vm->stack,
		inst_value(
			value(SCALAR_TYPE(),
				base_float(systime_cur - systime_start))));
}
void cmd_foreach(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PCOUNT count;
	PARRAY arr;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type == CODE_TYPE())
	{
		if (right_val->type != ARRAY_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			return;
		}
		arr = right_val->val.ptr;
		if (arr->top == 0)
		{
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			inst_destroy(left);
			inst_destroy(right);
		}
		else
		{
			count = count_create(left_val->val.ptr, right_val->val.ptr);
			push_stack(vm, vm->stack, inst_command(self));
			push_stack(vm, vm->stack,
				inst_value(value(COUNT_TYPE(), base_voidptr(count))));
			push_stack(vm, vm->stack, right);
			//push_stack(vm, vm->stack, inst_clear_work());
			push_stack(vm, vm->stack, inst_scope(0));
			push_stack(vm, vm->stack, inst_code_load(0));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, inst_store_var_local(L"_x"));
			push_stack(vm, vm->stack,
				inst_value(
					value(arr->data[count->curtop]->type,
						arr->data[count->curtop]->val)));
			push_stack(vm, vm->stack, inst_store_var_local(L"_forEachIndex"));
			push_stack(vm, vm->stack,
				inst_value(
					value(SCALAR_TYPE(), base_float(count->curtop))));
		}
	}
	else if (left_val->type == COUNT_TYPE())
	{
		if (right_val->type != ARRAY_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			return;
		}
		count = left_val->val.ptr;
		arr = right_val->val.ptr;
		if (++(count->curtop) == arr->top)
		{
			if (vm->work->top != 0)
			{
				push_stack(vm, vm->stack, pop_stack(vm, vm->work));
			}
			else
			{
				push_stack(vm, vm->stack,
					inst_value(value(NOTHING_TYPE(), base_int(0))));
			}
			inst_destroy(left);
			inst_destroy(right);
		}
		else
		{
			push_stack(vm, vm->stack, inst_command(self));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, right);
			//push_stack(vm, vm->stack, inst_clear_work());
			push_stack(vm, vm->stack, inst_scope(0));
			push_stack(vm, vm->stack, inst_code_load(0));
			push_stack(vm, vm->stack,
				inst_value(
					value(CODE_TYPE(),
						base_voidptr(count->code->val.ptr))));
			push_stack(vm, vm->stack, inst_store_var_local(L"_x"));
			push_stack(vm, vm->stack,
				inst_value(
					value(arr->data[count->curtop]->type,
						arr->data[count->curtop]->val)));
			push_stack(vm, vm->stack, inst_store_var_local(L"_forEachIndex"));
			push_stack(vm, vm->stack,
				inst_value(
					value(SCALAR_TYPE(), base_float(count->curtop))));
			if (vm->work->top != 0)
			{
				push_stack(vm, vm->stack, inst_clear_work());
			}
		}
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_CODE ERR_OR ERR_COUNT, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
	}
}

//https://community.bistudio.com/wiki/Math_Commands
void cmd_abs(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = abs((int)right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_deg(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = right_val->val.f * (180 / M_PI);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_log(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	if (right_val->val.f == -1)
	{
		push_stack(vm, vm->stack, inst_value(value(NAN_TYPE(), base_float(0))));
		inst_destroy(right);
	}
	f = log10(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_pi(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack,
		inst_value(value(SCALAR_TYPE(), base_float(M_PI))));
}
void cmd_sin(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = sin(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_cos(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = cos(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_tan(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = tan(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_exp(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = exp(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_asin(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = asin(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_acos(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = acos(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_atan(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = atan(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_rad(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = right_val->val.f / 360 * M_PI * 2;
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_sqrt(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = sqrt(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_ceil(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = ceil(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_random(void* input, CPCMD self)
{
	//ToDo: https://community.bistudio.com/wiki/random implement Alternative Syntax 1 & 2 & 3
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = randlf(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_floor(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = floor(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_ln(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = log(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void cmd_round(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	float f;
	if (right_val == 0)
	{
		inst_destroy(right);
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(right);
		return;
	}
	f = round(right_val->val.f);
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}

void cmd_atan2(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	float l, r;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	l = left_val->val.f;
	r = right_val->val.f;

	l = atan2(l, r);

	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(l))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_min(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	float l, r;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	l = left_val->val.f;
	r = right_val->val.f;

	l = l < r ? l : r;

	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(l))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_max(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	float l, r;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	l = left_val->val.f;
	r = right_val->val.f;

	l = l > r ? l : r;

	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(l))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_mod(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	float l, r;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	l = left_val->val.f;
	r = right_val->val.f;

	l = fmod(l, r);

	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(l))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_not(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == BOOL_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(value(BOOL_TYPE(), base_int(right_val->val.i))));
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_BOOL, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(right);
		return;
	}
}
void cmd_powerof(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	float l, r;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	l = left_val->val.f;
	r = right_val->val.f;

	l = pow(l, r);

	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(l))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_comment(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != STRING_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(right);
		return;
	}
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(right);
}

void cmd_pushback(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY arr;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = left_val->val.ptr;
	array_push(arr, value(right_val->type, right_val->val));
	push_stack(vm, vm->stack,
		inst_value(value(SCALAR_TYPE(), base_float(arr->top - 1))));
	inst_destroy(left);
	inst_destroy(right);
}

void cmd_set(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY arrl, arrr;
	int index;
	PVALUE val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arrl = left_val->val.ptr;
	arrr = right_val->val.ptr;
	if (arrr->top < 1 || arrr->data[0]->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_SCALAR,
			vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (arrr->top < 2)
	{
		vm->error(vm, ERR_ERR ERR_ARRAY_(1) ERR_WAS_EXPECTED ERR_EXISTING,
			vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	index = floor(arrr->data[0]->val.f);
	val = arrr->data[1];
	if (arrl->size < index + 1)
	{
		array_resize_top(arrl, index + 1);
	}
	if (arrl->data[index] != 0)
	{
		inst_destroy_value(arrl->data[index]);
	}
	if (arrl->top == index)
	{
		arrl->top++;
	}
	arrl->data[index] = value_copy(val);

	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(left);
	inst_destroy(right);
}

void cmd_resize(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY array;
	int newsize;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);

	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	if (left_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}

	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		inst_destroy(left);
		inst_destroy(right);

		return;
	}

	array = left_val->val.ptr;
	newsize = right_val->val.f;

	if (newsize >= 0)
	{
		array_resize_top(array, newsize);
	}
	else
	{
		vm->error(vm, ERR_RIGHT ERR_POSITIVE, vm->stack);
	}

	inst_destroy(left);
	inst_destroy(right);

	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

}

void cmd_deleteat(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY array;
	PVALUE val;
	int index;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);

	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	if (left_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}

	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		inst_destroy(left);
		inst_destroy(right);

		return;
	}

	array = left_val->val.ptr;
	index = (int)round(right_val->val.f);

	if (index < 0)
	{
		vm->error(vm, ERR_RIGHT ERR_POSITIVE, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
	}
	else
	{
		val = array_pop_at(array, index);
		push_stack(vm, vm->stack, inst_value(value(val->type, val->val)));
		inst_destroy_value(val);
	}

	inst_destroy(left);
	inst_destroy(right);
}

void cmd_reverse(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PARRAY array;

	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);

	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}

	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		inst_destroy(right);
		return;
	}

	array = right_val->val.ptr;

	array_reverse(array);

	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

	inst_destroy(right);
}

void cmd_find(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY array;
	PSTRING string;
	int index;
	wchar_t* ptr;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);

	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	if (left_val->type == NOTHING_TYPE() || right_val->type == NOTHING_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	if (left_val->type != ARRAY_TYPE() && left_val->type != STRING_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}

	if (left_val->type == ARRAY_TYPE())
	{
		array = left_val->val.ptr;

		index = array_index_of(array, right_val);
	}
	else
	{
		if (right_val->type != STRING_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);

			inst_destroy(left);
			inst_destroy(right);

			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		string = left_val->val.ptr;
		ptr = wcsstr(string->val, ((PSTRING)right_val->val.ptr)->val);
		if (ptr != NULL)
		{
			index = ptr - string->val;
		}
		else
		{
			index = -1;
		}

	}

	push_stack(vm, vm->stack,
		inst_value(value(SCALAR_TYPE(), base_float(index))));

	inst_destroy(left);
	inst_destroy(right);
}

void cmd_append(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY array1;
	PARRAY array2;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);

	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	if (left_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}

	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		inst_destroy(left);
		inst_destroy(right);

		return;
	}

	array1 = left_val->val.ptr;
	array2 = right_val->val.ptr;

	array_append(array1, array2);

	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

	inst_destroy(left);
	inst_destroy(right);
}

void cmd_arrayintersect(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;

	PARRAY result;
	PARRAY array1;
	PARRAY array2;
	PARRAY baseArray;
	PARRAY searchArray;
	int i;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);

	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	if (left_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}

	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		inst_destroy(left);
		inst_destroy(right);

		return;
	}

	array1 = left_val->val.ptr;
	array2 = right_val->val.ptr;

	baseArray = (array1->top > array2->top) ? array1 : array2;
	searchArray = (array1->top <= array2->top) ? array1 : array2;

	result = array_create();

	for (i = 0; i < baseArray->top; i++)
	{
		if (array_index_of(searchArray, baseArray->data[i]) != -1
			&& array_index_of(result, baseArray->data[i]) == -1)
		{
			array_push(result, value2(baseArray->data[i]));
		}
	}

	push_stack(vm, vm->stack,
		inst_value(value(ARRAY_TYPE(), base_voidptr(result))));

	inst_destroy(left);
	inst_destroy(right);
}

void cmd_vectoradd(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE leftVal;
	PVALUE rightVal;
	PARRAY leftArray;
	PARRAY rightArray;
	PARRAY result;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	leftVal = get_value(vm, vm->stack, left);
	rightVal = get_value(vm, vm->stack, right);

	// null check
	if (leftVal == 0 || rightVal == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	// type check
	if (leftVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	if (rightVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	rightArray = rightVal->val.ptr;
	leftArray = leftVal->val.ptr;

	if (!checkVector3(vm, rightArray) || !checkVector3(vm, leftArray))
	{
		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		return;
	}

	// calculate the result
	result = array_create2(4);

	array_push(result,
		addScalarPointer(leftArray->data[0], rightArray->data[0]));
	array_push(result,
		addScalarPointer(leftArray->data[1], rightArray->data[1]));
	array_push(result,
		addScalarPointer(leftArray->data[2], rightArray->data[2]));

	inst_destroy(left);
	inst_destroy(right);

	push_stack(vm, vm->stack,
		inst_value(value(ARRAY_TYPE(), base_voidptr(result))));
}

void cmd_vectordiff(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE leftVal;
	PVALUE rightVal;
	PARRAY leftArray;
	PARRAY rightArray;
	PARRAY result;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	leftVal = get_value(vm, vm->stack, left);
	rightVal = get_value(vm, vm->stack, right);

	// null check
	if (leftVal == 0 || rightVal == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	// type check
	if (leftVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	if (rightVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	rightArray = rightVal->val.ptr;
	leftArray = leftVal->val.ptr;

	if (!checkVector3(vm, rightArray) || !checkVector3(vm, leftArray))
	{
		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		return;
	}

	// calculate the result
	result = array_create2(4);

	array_push(result, substractScalarPointer(leftArray->data[0], rightArray->data[0]));
	array_push(result, substractScalarPointer(leftArray->data[1], rightArray->data[1]));
	array_push(result, substractScalarPointer(leftArray->data[2], rightArray->data[2]));

	inst_destroy(left);
	inst_destroy(right);

	push_stack(vm, vm->stack,
		inst_value(value(ARRAY_TYPE(), base_voidptr(result))));
}

void cmd_vectorcrossproduct(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE leftVal;
	PVALUE rightVal;
	PARRAY leftArray;
	PARRAY rightArray;
	PARRAY result;
	float x1;
	float x2;
	float x3;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	leftVal = get_value(vm, vm->stack, left);
	rightVal = get_value(vm, vm->stack, right);

	// null check
	if (leftVal == 0 || rightVal == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	// type check
	if (leftVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	if (rightVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	rightArray = rightVal->val.ptr;
	leftArray = leftVal->val.ptr;

	if (!checkVector3(vm, rightArray) || !checkVector3(vm, leftArray))
	{
		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		return;
	}

	// calculate the result
	result = array_create2(4);

	x1 = leftArray->data[1]->val.f * rightArray->data[2]->val.f
		- leftArray->data[2]->val.f * rightArray->data[1]->val.f;
	x2 = leftArray->data[2]->val.f * rightArray->data[0]->val.f
		- leftArray->data[0]->val.f * rightArray->data[2]->val.f;
	x3 = leftArray->data[0]->val.f * rightArray->data[1]->val.f
		- leftArray->data[1]->val.f * rightArray->data[0]->val.f;

	array_push(result, value(SCALAR_TYPE(), base_float(x1)));
	array_push(result, value(SCALAR_TYPE(), base_float(x2)));
	array_push(result, value(SCALAR_TYPE(), base_float(x3)));

	inst_destroy(left);
	inst_destroy(right);

	push_stack(vm, vm->stack,
		inst_value(value(ARRAY_TYPE(), base_voidptr(result))));
}

void cmd_vectordotproduct(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE leftVal;
	PVALUE rightVal;
	PARRAY leftArray;
	PARRAY rightArray;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	leftVal = get_value(vm, vm->stack, left);
	rightVal = get_value(vm, vm->stack, right);

	// null check
	if (leftVal == 0 || rightVal == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	// type check
	if (leftVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	if (rightVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	rightArray = rightVal->val.ptr;
	leftArray = leftVal->val.ptr;

	if (!checkVector3(vm, rightArray) || !checkVector3(vm, leftArray))
	{
		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		return;
	}

	// calculate the result
	push_stack(vm, vm->stack, inst_value(dotProductPointer_Value(leftArray, rightArray)));

	inst_destroy(left);
	inst_destroy(right);
}

void cmd_vectorcos(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE leftVal;
	PVALUE rightVal;
	PARRAY leftArray;
	PARRAY rightArray;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	leftVal = get_value(vm, vm->stack, left);
	rightVal = get_value(vm, vm->stack, right);

	// null check
	if (leftVal == 0 || rightVal == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	// type check
	if (leftVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	if (rightVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	rightArray = rightVal->val.ptr;
	leftArray = leftVal->val.ptr;

	if (!checkVector3(vm, rightArray) || !checkVector3(vm, leftArray))
	{
		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		return;
	}

	// calculate the result
	push_stack(vm, vm->stack,
		inst_value(
			value(SCALAR_TYPE(),
				base_float(
					dotProductPointer(leftArray, rightArray)
						/ (vectorMagnitudePointer(leftArray)
							* vectorMagnitudePointer(rightArray))))));

	inst_destroy(left);
	inst_destroy(right);
}

void cmd_vectormagnitude(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE rightVal;
	PARRAY rightArray;

	right = pop_stack(vm, vm->work);
	rightVal = get_value(vm, vm->stack, right);

	// null check
	if (rightVal == 0)
	{
		inst_destroy(right);
		return;
	}

	// type check
	if (rightVal->type != ARRAY_TYPE())
	{
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	rightArray = rightVal->val.ptr;

	if (!checkVector3(vm, rightArray))
	{
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		return;
	}

	// calculate the result
	push_stack(vm, vm->stack,
		inst_value(
			value(SCALAR_TYPE(),
				base_float(vectorMagnitudePointer(rightArray)))));

	inst_destroy(right);
}

void cmd_vectormagnitudesqr(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE rightVal;
	PARRAY rightArray;

	right = pop_stack(vm, vm->work);
	rightVal = get_value(vm, vm->stack, right);

	// null check
	if (rightVal == 0)
	{
		inst_destroy(right);
		return;
	}

	// type check
	if (rightVal->type != ARRAY_TYPE())
	{
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	rightArray = rightVal->val.ptr;

	if (!checkVector3(vm, rightArray))
	{
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		return;
	}

	// calculate the result
	push_stack(vm, vm->stack,
		inst_value(
			value(SCALAR_TYPE(),
				base_float(powf(vectorMagnitudePointer(rightArray), 2)))));

	inst_destroy(right);
}

void cmd_vectormultiply(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE leftVal;
	PVALUE rightVal;
	PARRAY array;
	PARRAY result;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	leftVal = get_value(vm, vm->stack, left);
	rightVal = get_value(vm, vm->stack, right);

	// null check
	if (leftVal == 0 || rightVal == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	// type check
	if (leftVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	if (rightVal->type != SCALAR_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);

		return;
	}

	array = leftVal->val.ptr;

	if (!checkVector3(vm, array))
	{
		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		return;
	}

	// calculate the result
	result = array_create2(4);

	array_push(result,
		value(SCALAR_TYPE(), base_float(array->data[0]->val.f * rightVal->val.f)));
	array_push(result,
		value(SCALAR_TYPE(), base_float(array->data[1]->val.f * rightVal->val.f)));
	array_push(result,
		value(SCALAR_TYPE(), base_float(array->data[2]->val.f * rightVal->val.f)));

	push_stack(vm, vm->stack, inst_value(value(ARRAY_TYPE(), base_voidptr(result))));

	inst_destroy(left);
	inst_destroy(right);
}

void cmd_vectordistance(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE leftVal;
	PVALUE rightVal;
	PARRAY leftArray;
	PARRAY rightArray;
	float result;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	leftVal = get_value(vm, vm->stack, left);
	rightVal = get_value(vm, vm->stack, right);

	// null check
	if (leftVal == 0 || rightVal == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	// type check
	if (leftVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	if (rightVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	rightArray = rightVal->val.ptr;
	leftArray = leftVal->val.ptr;

	if (!checkVector3(vm, rightArray) || !checkVector3(vm, leftArray))
	{
		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		return;
	}

	// calculate the diffArray
	result = powf(rightArray->data[0]->val.f - leftArray->data[0]->val.f, 2);
	result += powf(rightArray->data[1]->val.f - leftArray->data[1]->val.f, 2);
	result += powf(rightArray->data[2]->val.f - leftArray->data[2]->val.f, 2);

	push_stack(vm, vm->stack,
		inst_value(value(SCALAR_TYPE(), base_float(powf(result, 1.0 / 2.0)))));

	inst_destroy(left);
	inst_destroy(right);
}

void cmd_vectordistancesqr(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE leftVal;
	PVALUE rightVal;
	PARRAY leftArray;
	PARRAY rightArray;
	float result;

	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	leftVal = get_value(vm, vm->stack, left);
	rightVal = get_value(vm, vm->stack, right);

	// null check
	if (leftVal == 0 || rightVal == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}

	// type check
	if (leftVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	if (rightVal->type != ARRAY_TYPE())
	{
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	rightArray = rightVal->val.ptr;
	leftArray = leftVal->val.ptr;

	if (!checkVector3(vm, rightArray) || !checkVector3(vm, leftArray))
	{
		inst_destroy(left);
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		return;
	}

	// calculate the diffArray
	result = powf(rightArray->data[0]->val.f - leftArray->data[0]->val.f, 2);
	result += powf(rightArray->data[1]->val.f - leftArray->data[1]->val.f, 2);
	result += powf(rightArray->data[2]->val.f - leftArray->data[2]->val.f, 2);

	push_stack(vm, vm->stack,
		inst_value(value(SCALAR_TYPE(), base_float(result))));

	inst_destroy(left);
	inst_destroy(right);
}

void cmd_vectornormalized(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE rightVal;
	PARRAY array;
	float magnitude;
	PARRAY result;

	right = pop_stack(vm, vm->work);
	rightVal = get_value(vm, vm->stack, right);

	// null check
	if (rightVal == 0)
	{
		inst_destroy(right);
		return;
	}

	// type check
	if (rightVal->type != ARRAY_TYPE())
	{
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));

		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);

		return;
	}

	array = rightVal->val.ptr;

	if (!checkVector3(vm, array))
	{
		inst_destroy(right);

		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));

		return;
	}

	// calculate the result
	result = array_create2(4);

	magnitude = vectorMagnitudePointer(array);

	if (magnitude > 0)
	{
		array_push(result,
			value(SCALAR_TYPE(), base_float(array->data[0]->val.f / magnitude)));
		array_push(result,
			value(SCALAR_TYPE(), base_float(array->data[1]->val.f / magnitude)));
		array_push(result,
			value(SCALAR_TYPE(), base_float(array->data[2]->val.f / magnitude)));
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			array_push(result, value(SCALAR_TYPE(), base_float(0)));
		}
	}

	push_stack(vm, vm->stack, inst_value(value(ARRAY_TYPE(), base_voidptr(result))));

	inst_destroy(right);
}

void cmd_getvariable(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PNAMESPACE ns;
	PARRAY arr;
	PVALUE val;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type == NAMESPACE_TYPE())
	{
		ns = left_val->val.ptr;
	}
	else if (left_val->type == OBJECT_TYPE())
	{
		ns = ((POBJECT)left_val->val.ptr)->ns;
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_NAMESPACE ERR_OR ERR_OBJECT, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type == ARRAY_TYPE())
	{
		arr = right_val->val.ptr;
		if (arr->top < 2)
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_ARRAY_SIZE(2),
				vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		if (arr->data[0]->type != STRING_TYPE())
		{
			vm->error(vm,
			ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_STRING,
				vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		if (((PSTRING)arr->data[0]->val.ptr)->length == 0)
		{
			vm->error(vm,
			ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_NON_EMPTY_STRING,
				vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		val = namespace_get_var(ns, ((PSTRING)arr->data[0]->val.ptr)->val);
		if (val == 0)
		{
			val = arr->data[1];
		}
		push_stack(vm, vm->stack, inst_value(value(val->type, val->val)));
	}
	else if (right_val->type == STRING_TYPE())
	{
		if (((PSTRING)right_val->val.ptr)->length == 0)
		{
			vm->error(vm, ERR_RIGHT ERR_WAS_EXPECTED ERR_NON_EMPTY_STRING,
				vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		val = namespace_get_var(ns, ((PSTRING)right_val->val.ptr)->val);
		if (val == 0)
		{
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
		}
		else
		{
			push_stack(vm, vm->stack, inst_value(value(val->type, val->val)));
		}
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	inst_destroy(left);
	inst_destroy(right);
}

void cmd_setvariable(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PNAMESPACE ns;
	PARRAY arr;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type == NAMESPACE_TYPE())
	{
		ns = left_val->val.ptr;
	}
	else if (left_val->type == OBJECT_TYPE())
	{
		ns = ((POBJECT)left_val->val.ptr)->ns;
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_NAMESPACE ERR_OR ERR_OBJECT, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = right_val->val.ptr;
	if (arr->top < 2)
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_ARRAY_SIZE(2), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (arr->data[0]->type != STRING_TYPE())
	{
		vm->error(vm,
		ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_STRING,
			vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (((PSTRING)arr->data[0]->val.ptr)->length == 0)
	{
		vm->error(vm,
		ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_NON_EMPTY_STRING, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}

	namespace_set_var(ns, ((PSTRING)arr->data[0]->val.ptr)->val,
		value(arr->data[1]->type, arr->data[1]->val));
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(left);
	inst_destroy(right);
}

void cmd_createvehicle(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY arr;
	POBJECT obj;
	int i;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != STRING_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_STRING, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = right_val->val.ptr;
	if (arr->top < 3)
	{
		vm->error(vm, ERR_RIGHT ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	for (i = 0; i < 3; i++)
	{
		if (arr->data[i]->type != SCALAR_TYPE())
		{
			vm->error(vm,
			ERR_ERR ERR_ARRAY_(i) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_SCALAR,
				vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
	}
	obj = object_vehicle_create(((PSTRING)left_val->val.ptr)->val);
	obj->position = (vec3 ) {
			.x = arr->data[0]->val.f,
			.y = arr->data[0]->val.f, .z = arr->data[0]->val.f
		};

	push_stack(vm, vm->stack,
		inst_value(value(OBJECT_TYPE(), base_voidptr(obj))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_typeof(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	POBJECT obj;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != OBJECT_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_OBJECT, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	obj = right_val->val.ptr;
	push_stack(vm, vm->stack,
		inst_value(
			value(STRING_TYPE(),
				base_voidptr(string_create2(obj->classname)))));
	inst_destroy(right);
}
void cmd_getpos(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	POBJECT obj;
	PARRAY arr;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != OBJECT_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_OBJECT, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	obj = right_val->val.ptr;
	arr = array_create2(3);
	array_push(arr, value(SCALAR_TYPE(), base_float(obj->position.x)));
	array_push(arr, value(SCALAR_TYPE(), base_float(obj->position.y)));
	array_push(arr, value(SCALAR_TYPE(), base_float(obj->position.z)));
	push_stack(vm, vm->stack,
		inst_value(value(ARRAY_TYPE(), base_voidptr(arr))));
	inst_destroy(right);
}
void cmd_setpos(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	POBJECT obj;
	PARRAY arr;
	int i;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != OBJECT_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_OBJECT, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = right_val->val.ptr;
	if (arr->top < 3)
	{
		vm->error(vm, ERR_RIGHT ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	for (i = 0; i < 3; i++)
	{
		if (arr->data[i]->type != SCALAR_TYPE())
		{
			vm->error(vm,
			ERR_ERR ERR_ARRAY_(i) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_SCALAR,
				vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
	}
	obj = left_val->val.ptr;
	obj->position = (vec3 ) {
			.x = arr->data[0]->val.f,
			.y = arr->data[0]->val.f, .z = arr->data[0]->val.f
		};

	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_velocity(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	POBJECT obj;
	PARRAY arr;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != OBJECT_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_OBJECT, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	obj = right_val->val.ptr;
	arr = array_create2(3);
	array_push(arr, value(SCALAR_TYPE(), base_float(obj->velocity.x)));
	array_push(arr, value(SCALAR_TYPE(), base_float(obj->velocity.y)));
	array_push(arr, value(SCALAR_TYPE(), base_float(obj->velocity.z)));
	push_stack(vm, vm->stack,
		inst_value(value(ARRAY_TYPE(), base_voidptr(arr))));
	inst_destroy(right);
}
void cmd_setvelocity(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	POBJECT obj;
	PARRAY arr;
	int i;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != OBJECT_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_OBJECT, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = right_val->val.ptr;
	if (arr->top < 3)
	{
		vm->error(vm, ERR_RIGHT ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	for (i = 0; i < 3; i++)
	{
		if (arr->data[i]->type != SCALAR_TYPE())
		{
			vm->error(vm,
			ERR_ERR ERR_ARRAY_(i) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_SCALAR,
				vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
	}
	obj = left_val->val.ptr;
	obj->velocity = (vec3 ) {
			.x = arr->data[0]->val.f,
			.y = arr->data[0]->val.f, .z = arr->data[0]->val.f
		};

	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_domove(void* input, CPCMD self)
{
	//ToDo: Add Simulation and make the movement smooth
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	POBJECT obj;
	PARRAY arr;
	PARRAY arr2;
	int i;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = right_val->val.ptr;
	if (arr->top < 3)
	{
		vm->error(vm, ERR_RIGHT ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	for (i = 0; i < 3; i++)
	{
		if (arr->data[i]->type != SCALAR_TYPE())
		{
			vm->error(vm,
			ERR_ERR ERR_ARRAY_(i) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_SCALAR,
				vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
	}

	if (left_val->type == OBJECT_TYPE())
	{
		obj = left_val->val.ptr;
		if (!obj->is_vehicle)
		{
			obj->position = (vec3 ) {
					.x = arr->data[0]->val.f, .y =
						arr->data[0]->val.f, .z = arr->data[0]->val.f
				};
		}
	}
	else if (left_val->type == ARRAY_TYPE())
	{
		arr2 = left_val->val.ptr;

		for (i = 0; i < arr2->top; i++)
		{
			if (arr2->data[i]->type != OBJECT_TYPE())
			{
				vm->error(vm,
				ERR_ERR ERR_ARRAY_(i) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_OBJECT,
					vm->stack);
				inst_destroy(left);
				inst_destroy(right);
				push_stack(vm, vm->stack,
					inst_value(value(NOTHING_TYPE(), base_int(0))));
				return;
			}
		}

		for (i = 0; i < arr2->top; i++)
		{
			obj = arr2->data[i]->val.ptr;
			if (!obj->is_vehicle)
			{
				obj->position = (vec3 ) {
						.x = arr->data[0]->val.f, .y =
							arr->data[0]->val.f, .z = arr->data[0]->val.f
					};
			}
		}
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_OBJECT, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
}
void cmd_objnull(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack,
		inst_value(value(OBJECT_TYPE(), base_voidptr(object_create(L"")))));
}

void cmd_switch(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PSWITCH swtch;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}

	swtch = switch_create(value(right_val->type, right_val->val));
	push_stack(vm, vm->stack,
		inst_value(value(SWITCH_TYPE(), base_voidptr(swtch))));
	inst_destroy(right);
}
void cmd_case(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PSWITCH swtch;
	PVALUE var;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	var = find_var(vm, SWITCH_SPECIAL_VAR);
	if (var == 0 || var->type != SWITCH_TYPE())
	{
		vm->error(vm, ERR_SPECIAL_SWITCH, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	swtch = switch_create(value(right_val->type, right_val->val));
	push_stack(vm, vm->stack,
		inst_value(value(SWITCH_TYPE(), base_voidptr(swtch))));
	inst_destroy(right);
}
void cmd_default(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PVALUE var;
	PSWITCH swtch;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != CODE_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CODE, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	var = find_var(vm, SWITCH_SPECIAL_VAR);
	if (var == 0 || var->type != SWITCH_TYPE())
	{
		vm->error(vm, ERR_SPECIAL_SWITCH, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	swtch = var->val.ptr;
	swtch->default_code = value_copy(right_val);
	inst_destroy(right);
}
void cmd_caseoperator(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PVALUE var;
	PSWITCH swtch;
	PSWITCH left_swtch;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != SWITCH_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SWITCH, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != CODE_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CODE, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	var = find_var(vm, SWITCH_SPECIAL_VAR);
	if (var == 0 || var->type != SWITCH_TYPE())
	{
		vm->error(vm, ERR_SPECIAL_SWITCH, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	swtch = var->val.ptr;
	left_swtch = left_val->val.ptr;

	if (is_equal_to(swtch->switch_value, left_swtch->switch_value))
	{
		swtch->selected_code = value_copy(right_val);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		push_stack(vm, vm->stack, inst_scope_dropout(0));
	}
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(left);
	inst_destroy(right);
}

void cmd_west(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_blu()));
}
void cmd_blufor(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_blu()));
}
void cmd_east(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_opf()));
}
void cmd_opfor(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_opf()));
}
void cmd_resistance(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_gue()));
}
void cmd_independent(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_gue()));
}
void cmd_sideunknown(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_unknown()));
}
void cmd_sidelogic(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_logic()));
}
void cmd_sidefriendly(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_friendly()));
}
void cmd_sideenemy(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_enemy()));
}
void cmd_civilian(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_civ()));
}
void cmd_sideempty(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_empty()));
}
void cmd_nil(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
}
void cmd_allvariables(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PARRAY arr;
	int i;
	wsm_list* list;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == NAMESPACE_TYPE())
	{
		list = ((PNAMESPACE)right_val->val.ptr)->data;
	}
	else if (right_val->type == OBJECT_TYPE())
	{
		list = ((POBJECT)right_val->val.ptr)->ns->data;
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_NAMESPACE ERR_OR ERR_OBJECT,
			vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = array_create2(wsm_count(list));
	for (i = 0; i < arr->size; i++)
	{
		array_push(arr,
			value(STRING_TYPE(),
				base_voidptr(
					string_create2(
						wsm_get_name_index(list,
						(unsigned int)i)))));
	}
	push_stack(vm, vm->stack,
		inst_value(value(ARRAY_TYPE(), base_voidptr(arr))));
	inst_destroy(right);
}
void cmd_with(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == NAMESPACE_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(value(WITH_TYPE(), right_val->val)));
		inst_destroy(right);
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_NAMESPACE, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
}
void cmd_compile(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == STRING_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(
				value(CODE_TYPE(),
					base_voidptr(
						parse_into_code(vm,
							((PSTRING)right_val->val.ptr)->val)))));
		inst_destroy(right);
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
}

void cmd_toarray(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PARRAY arr;
	PSTRING str;
	int i;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == STRING_TYPE())
	{
		str = right_val->val.ptr;
		arr = array_create2(str->length);
		for (i = 0; i < str->length; i++)
		{
			array_push(arr,
				value(SCALAR_TYPE(),
					base_float((float)(int)str->val[i])));
		}
		push_stack(vm, vm->stack,
			inst_value(value(ARRAY_TYPE(), base_voidptr(arr))));
		inst_destroy(right);
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
}
void cmd_tostring(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PARRAY arr;
	PSTRING str;
	int i;
	bool err_flag = 0;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == ARRAY_TYPE())
	{
		arr = right_val->val.ptr;
		for (i = 0; i < arr->top; i++)
		{
			if (arr->data[i]->type != SCALAR_TYPE())
			{
				vm->error(vm,
				ERR_ERR ERR_ARRAY_(i) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_SCALAR,
					vm->stack);
				err_flag = true;
			}
		}
		if (err_flag)
		{
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		str = string_create(arr->top + 1);
		for (i = 0; i < arr->top; i++)
		{
			str->val[i] = (wchar_t)(wint_t)arr->data[i]->val.f;
		}
		push_stack(vm, vm->stack,
			inst_value(value(STRING_TYPE(), base_voidptr(str))));
		inst_destroy(right);
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
}

void params_helper(PVM vm, PVALUE input, PARRAY format)
{
	PVALUE tmp;
	PSTRING varname;
	PVALUE defaultval;
	PARRAY expecteddatatypes;
	PVALUE expectedarrcount;
	PSCOPE topscope = top_scope(vm);
	PARRAY content = 0;
	int i, j;
	bool success_flag = true;

	if (input->type == ARRAY_TYPE())
	{
		content = input->val.ptr;
	}
	for (i = 0; i < format->top; i++)
	{
		tmp = format->data[i];
		varname = 0;
		defaultval = 0;
		expectedarrcount = 0;
		expecteddatatypes = 0;
		if (tmp->type == STRING_TYPE())
		{
			varname = tmp->val.ptr;
			if (varname->length == 0)
				continue;
		}
		else if (tmp->type == ARRAY_TYPE())
		{
			//Varname validation
			if (((PARRAY)tmp->val.ptr)->top == 0)
			{
				vm->error(vm, ERR_SPECIAL_PARAMS_FORMAT_ERROR, vm->stack);
				push_stack(vm, vm->stack,
					inst_value(value(NOTHING_TYPE(), base_int(0))));
				return;
			}
			varname = ((PARRAY)tmp->val.ptr)->data[0]->val.ptr;
			if (varname->length == 0 || varname->val == 0
				|| varname->val[0] != '_')
			{
				vm->error(vm, ERR_SPECIAL_PARAMS_VARNAME, vm->stack);
				push_stack(vm, vm->stack,
					inst_value(value(NOTHING_TYPE(), base_int(0))));
				return;
			}

			//Receive default-value
			if (((PARRAY)tmp->val.ptr)->top > 1)
			{
				defaultval = ((PARRAY)tmp->val.ptr)->data[1];
			}

			//Receive allowed datatypes
			if (((PARRAY)tmp->val.ptr)->top > 2)
			{
				if (((PARRAY)tmp->val.ptr)->data[2]->type == ARRAY_TYPE())
				{
					expecteddatatypes =
						((PARRAY)tmp->val.ptr)->data[2]->val.ptr;
				}
				else if (((PARRAY)tmp->val.ptr)->data[3]->type
					!= NOTHING_TYPE())
				{
					vm->error(vm, ERR_SPECIAL_PARAMS_FORMAT_ERROR, vm->stack);
					push_stack(vm, vm->stack,
						inst_value(value(NOTHING_TYPE(), base_int(0))));
					return;
				}
			}

			//Receive expected array count
			if (((PARRAY)tmp->val.ptr)->top > 3)
			{
				if (((PARRAY)tmp->val.ptr)->data[3]->type == ARRAY_TYPE())
				{
					expectedarrcount = ((PARRAY)tmp->val.ptr)->data[3];
					//ensure all content of expected array count array is of type scalar
					for (j = 0; j < ((PARRAY)expectedarrcount->val.ptr)->top;
						j++)
					{
						if (((PARRAY)expectedarrcount->val.ptr)->data[j]->type
							!= SCALAR_TYPE())
						{
							vm->error(vm, ERR_SPECIAL_PARAMS_FORMAT_ERROR,
								vm->stack);
							push_stack(vm, vm->stack,
								inst_value(
									value(NOTHING_TYPE(),
										base_int(0))));
							return;
						}
					}
				}
				else if (((PARRAY)tmp->val.ptr)->data[3]->type
					== SCALAR_TYPE())
				{
					expectedarrcount = ((PARRAY)tmp->val.ptr)->data[3];
				}
				else if (((PARRAY)tmp->val.ptr)->data[3]->type
					!= NOTHING_TYPE())
				{
					vm->error(vm, ERR_SPECIAL_PARAMS_FORMAT_ERROR, vm->stack);
					push_stack(vm, vm->stack,
						inst_value(value(NOTHING_TYPE(), base_int(0))));
					return;
				}
			}
		}
		else
		{
			vm->error(vm, ERR_SPECIAL_PARAMS_FORMAT_ERROR, vm->stack);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}

		//if not exists, use default
		if (content == 0)
		{
			tmp = input;
		}
		else if (content->top <= i)
		{
			tmp = defaultval;
			success_flag = false;
		}
		else
		{
			tmp = content->data[i];
			if (tmp->type == NOTHING_TYPE())
			{
				tmp = defaultval;
				success_flag = false;
			}
		}
		//Check datatypes matches
		if (tmp != 0 && expecteddatatypes != 0 && expecteddatatypes->top != 0)
		{
			for (j = 0; j < expecteddatatypes->top; j++)
			{
				if (expecteddatatypes->data[j]->type == tmp->type)
				{
					break;
				}
			}
			if (j == expecteddatatypes->top)
			{
				tmp = defaultval;
				success_flag = false;
				vm->warn(vm, ERR_SPECIAL_PARAMS_FORMAT_MISSMATCH, vm->stack);
			}
		}

		//check array count matches (if val is array)
		if (tmp != 0 && tmp->type == ARRAY_TYPE() && expectedarrcount != 0)
		{
			if (expectedarrcount->type == ARRAY_TYPE())
			{
				for (j = 0; j < ((PARRAY)expectedarrcount->val.ptr)->top; j++)
				{
					if (((PARRAY)expectedarrcount->val.ptr)->data[j]->val.f
						== ((PARRAY)tmp->val.ptr)->top)
					{
						break;
					}
				}
				if (j == ((PARRAY)expectedarrcount->val.ptr)->top)
				{
					tmp = defaultval;
					success_flag = false;
					vm->warn(vm, ERR_SPECIAL_PARAMS_FORMAT_MISSMATCH,
						vm->stack);
				}
			}
			else if (expectedarrcount->type == SCALAR_TYPE())
			{
				if (((PARRAY)tmp->val.ptr)->top
					!= (int)expectedarrcount->val.f)
				{
					tmp = defaultval;
					success_flag = false;
					vm->warn(vm, ERR_SPECIAL_PARAMS_FORMAT_MISSMATCH,
						vm->stack);
				}
			}
		}
		if (tmp == 0)
		{
			store_in_scope(vm, topscope, varname->val,
				value(NOTHING_TYPE(), base_int(0)));
		}
		else
		{
			store_in_scope(vm, topscope, varname->val,
				value(tmp->type, tmp->val));
		}
		if (content == 0)
			break;
	}
	push_stack(vm, vm->stack,
		inst_value(value(BOOL_TYPE(), base_int(success_flag))));
}
void cmd_params(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY arrformat;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arrformat = right_val->val.ptr;
	params_helper(vm, left_val, arrformat);
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_params_UNARY(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY arrformat;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	left_val = find_var(vm, L"_this");
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arrformat = right_val->val.ptr;
	params_helper(vm, left_val, arrformat);
	inst_destroy(right);
}
void cmd_isnil(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PVALUE val;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == STRING_TYPE())
	{
		if (((PSTRING)right_val->val.ptr)->length == 0
			|| ((PSTRING)right_val->val.ptr)->val[0] != '_')
		{
			vm->error(vm, ERR_SPECIAL_ISNIL_1, vm->stack);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		val = find_var(vm, ((PSTRING)right_val->val.ptr)->val);
		push_stack(vm, vm->stack,
			inst_value(
				value(BOOL_TYPE(),
					base_int(
						val == 0
							|| val->type == NOTHING_TYPE() ?
																1 :
																0))));
		inst_destroy(right);
	}
	else if (right_val->type == CODE_TYPE())
	{
		push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(1))));
		push_stack(vm, vm->stack, inst_pop(1));
		push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(0))));
		push_stack(vm, vm->stack, inst_pop_eval(2, 1));
		push_stack(vm, vm->stack, inst_code_load(1));
		push_stack(vm, vm->stack, right);
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING ERR_OR ERR_CODE, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
}
void cmd_deletevehicle(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	POBJECT obj;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == OBJECT_TYPE())
	{
		obj = right_val->val.ptr;
		object_destroy_inner(obj);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_OBJECT, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
}
void cmd_creategroup(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PGROUP group;
	PVALUE tmp;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == SIDE_TYPE())
	{
		do
		{
			group = group_create(right_val->val.i);
			tmp = wsm_set_value(sqf_group_map(), group->ident,
				value_create(GROUP_TYPE(), base_voidptr(group)));
			if (tmp != 0)
			{
				inst_destroy_value(
					wsm_set_value(sqf_group_map(), group->ident, tmp));
			}
		} while (tmp != 0);
		push_stack(vm, vm->stack,
			inst_value(value(GROUP_TYPE(), base_voidptr(group))));
		inst_destroy(right);
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SIDE, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
}
void cmd_deletegroup(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PGROUP group;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == GROUP_TYPE())
	{
		group = right_val->val.ptr;
		if (((PARRAY)group->members->val.ptr)->top == 0)
		{
			inst_destroy_value(wsm_drop_value(sqf_group_map(), group->ident));
		}
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_GROUP, vm->stack);
	}
	inst_destroy(right);
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
}
void cmd_groupid(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PGROUP group;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == GROUP_TYPE())
	{
		group = right_val->val.ptr;
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(
				value(STRING_TYPE(),
					base_voidptr(string_create2(group->ident)))));
		;
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_GROUP, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
	}
}
void cmd_createunit(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PVALUE tmp;
	PGROUP group;
	POBJECT obj;
	PARRAY arr;
	vec3 pos;
	int i;
	float f;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != GROUP_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_GROUP, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_GROUP, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = right_val->val.ptr;
	if (arr->top < 5)
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_ARRAY_SIZE(5), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (arr->data[0]->type != STRING_TYPE())
	{
		vm->error(vm,
		ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_STRING,
			vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (arr->data[1]->type == ARRAY_TYPE())
	{
		if (((PARRAY)arr->data[1]->val.ptr)->top < 2)
		{
			vm->error(vm,
				ERR_ERR ERR_ARRAY_(1) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_ARRAY ERR_ARRAY_SIZE(2),
				vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		for (i = 0; i < ((PARRAY)arr->data[1]->val.ptr)->top && i < 3; i++)
		{
			if (((PARRAY)arr->data[1]->val.ptr)->data[i]->type
				!= SCALAR_TYPE())
			{
				vm->error(vm,
					ERR_ERR ERR_ARRAY_(1) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_ARRAY ERR_ARRAY_SIZE(2),
					vm->stack);
				inst_destroy(left);
				inst_destroy(right);
				push_stack(vm, vm->stack,
					inst_value(value(NOTHING_TYPE(), base_int(0))));
				return;
			}
		}
		pos.x = ((PARRAY)arr->data[1]->val.ptr)->data[0]->val.f;
		pos.y = ((PARRAY)arr->data[1]->val.ptr)->data[1]->val.f;
		if (((PARRAY)arr->data[1]->val.ptr)->top >= 3)
		{
			pos.z = ((PARRAY)arr->data[1]->val.ptr)->data[2]->val.f;
		}
	}
	else if (arr->data[1]->type == OBJECT_TYPE())
	{
		pos = ((POBJECT)arr->data[1]->val.ptr)->position;
	}
	else if (arr->data[1]->type == GROUP_TYPE())
	{
		tmp = group_get_leader(((PGROUP)arr->data[1]->val.ptr));
		if (tmp == 0 || object_is_null(tmp->val.ptr))
		{
			vm->error(vm, ERR_SPECIAL_CREATEUNIT_1, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		pos = ((POBJECT)tmp->val.ptr)->position;
	}
	else
	{
		vm->error(vm,
		ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_STRING,
			vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (arr->data[2]->type != ARRAY_TYPE())
	{
		vm->error(vm,
		ERR_ERR ERR_ARRAY_(2) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_ARRAY,
			vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (arr->data[3]->type == SCALAR_TYPE())
	{
		f = arr->data[3]->val.f;
		if (f > 0)
		{
			pos.x += randlf(f) - (f / 2);
			pos.y += randlf(f) - (f / 2);
		}
	}
	else
	{
		vm->error(vm,
		ERR_ERR ERR_ARRAY_(3) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_SCALAR,
			vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (arr->data[4]->type != STRING_TYPE())
	{
		vm->error(vm,
		ERR_ERR ERR_ARRAY_(4) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_STRING,
			vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	//ToDo: (2) check for markers
	//ToDo: (4) Take care of special
	group = left_val->val.ptr;
	obj = object_unit_create(((PSTRING)arr->data[0]->val.ptr)->val, group);
	obj->position = pos;

	inst_destroy(left);
	inst_destroy(right);
	push_stack(vm, vm->stack,
		inst_value(value(OBJECT_TYPE(), base_voidptr(obj))));
}

void cmd_units(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PGROUP group;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type == GROUP_TYPE())
	{
		group = right_val->val.ptr;
	}
	else if (right_val->type == OBJECT_TYPE())
	{
		if (((POBJECT)right_val->val.ptr)->inner == 0)
		{
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(
					value(ARRAY_TYPE(),
						base_voidptr(array_create2(0)))));
			return;
		}
		else if (((POBJECT)right_val->val.ptr)->is_vehicle)
		{
			//ToDo: get group from leader
			inst_destroy(right);
			push_stack(vm, vm->stack,
				inst_value(
					value(ARRAY_TYPE(),
						base_voidptr(array_create2(0)))));
			return;
		}
		else
		{
			group = group_from_ident(vm,
				((PUNIT)((POBJECT)right_val->val.ptr)->inner)->groupident->val);
		}
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_GROUP, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (group == 0)
	{
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(
				value(ARRAY_TYPE(), base_voidptr(array_create2(0)))));
		return;
	}
	push_stack(vm, vm->stack,
		inst_value(
			value(ARRAY_TYPE(),
				base_voidptr(
					array_copy(group->members->val.ptr)))));
	inst_destroy(right);
}
void cmd_allgroups(void* input, CPCMD self)
{
	PVM vm = input;
	PARRAY arr;
	unsigned int i;
	unsigned int j;
	j = wsm_count(sqf_group_map());
	arr = array_create2(j);
	for (i = 0; i < j; i++)
	{
		array_push(arr,
			value(GROUP_TYPE(),
				base_voidptr(
				((PVALUE)wsm_get_value_index(sqf_group_map(), i))->val.ptr)));
	}

	push_stack(vm, vm->stack,
		inst_value(value(ARRAY_TYPE(), base_voidptr(arr))));
}


void cmd_configfile(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(value(CONFIG_TYPE(), base_voidptr(sqf_configFile()))));
}

void cmd_navigateconfighelper(PVM vm, PCONFIG config, PSTRING string)
{
	PCONFIG node = config;
	bool found = false;
	int i;
	if (node == 0)
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_NOT_NULL, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	do
	{
		for (i = 0; i < node->children_top; i++)
		{
			if (wstr_cmpi(node->value.cfgnodes[i]->identifier, -1, string->val, -1) == 0)
			{
				node = node->value.cfgnodes[i];
				found = true;
				break;
			}
		}
	} while (!found && (node = config_find_inheriting_node(node)) != 0);

	if (node == config || node == 0)
	{
		push_stack(vm, vm->stack,
			inst_value(value(CONFIG_TYPE(), base_voidptr(0))));
	}
	else
	{
		push_stack(vm, vm->stack,
			inst_value(value(CONFIG_TYPE(), base_voidptr(node))));
	}
}
void cmd_navigateconfig(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PCONFIG node;
	PSTRING string;
	left = pop_stack(vm, vm->work);
	right = pop_stack(vm, vm->work);
	left_val = get_value(vm, vm->stack, left);
	right_val = get_value(vm, vm->stack, right);
	if (left_val == 0 || right_val == 0)
	{
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != CONFIG_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_CONFIG, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != STRING_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	node = left_val->val.ptr;
	string = right_val->val.ptr;
	cmd_navigateconfighelper(vm, node, string);
	inst_destroy(left);
	inst_destroy(right);
}
void cmd_configname(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PCONFIG node;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != CONFIG_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CONFIG, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	node = right_val->val.ptr;
	if (node == 0)
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_NOT_NULL, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	push_stack(vm, vm->stack,
		inst_value(value(STRING_TYPE(), base_voidptr(string_create2(node->identifier)))));
	inst_destroy(right);
}
void cmd_inheritsfrom(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PCONFIG node;
	PCONFIG config;
	int i;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != CONFIG_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CONFIG, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	config = node = right_val->val.ptr;
	if (node == 0)
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_NOT_NULL, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	node = config_find_inheriting_node(node);
	push_stack(vm, vm->stack,
		inst_value(value(CONFIG_TYPE(), base_voidptr(0))));
	inst_destroy(right);
}
void cmd_getnumber(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PCONFIG node;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != CONFIG_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CONFIG, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	node = right_val->val.ptr;
	if (node == 0)
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_NOT_NULL, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (node->children_size == 0 && node->value.value->type == SCALAR_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(value(SCALAR_TYPE(), node->value.value->val)));
	}
	else
	{
		vm->warn(vm, ERR_RIGHT_TYPE ERR_CONFIGWITH ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(SCALAR_TYPE(), base_float(0))));
	}
	inst_destroy(right);
}
void cmd_getarray(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PCONFIG node;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != CONFIG_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CONFIG, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	node = right_val->val.ptr;
	if (node == 0)
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_NOT_NULL, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (node->children_size == 0 && node->value.value->type == ARRAY_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(value(ARRAY_TYPE(), node->value.value->val)));
	}
	else
	{
		vm->warn(vm, ERR_RIGHT_TYPE ERR_CONFIGWITH ERR_ARRAY, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(ARRAY_TYPE(), base_voidptr(array_create2(0)))));
	}
	inst_destroy(right);
}
void cmd_gettext(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PCONFIG node;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != CONFIG_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CONFIG, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	node = right_val->val.ptr;
	if (node == 0)
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_NOT_NULL, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (node->children_size == 0 && node->value.value->type == STRING_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(value(STRING_TYPE(), node->value.value->val)));
	}
	else
	{
		vm->warn(vm, ERR_RIGHT_TYPE ERR_CONFIGWITH ERR_STRING, vm->stack);
		push_stack(vm, vm->stack,
			inst_value(value(STRING_TYPE(), base_voidptr(string_create(0)))));
	}
	inst_destroy(right);
}
void cmd_isnumber(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PCONFIG node;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != CONFIG_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CONFIG, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	node = right_val->val.ptr;
	if (node == 0)
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_NOT_NULL, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (node->children_size == 0 && node->value.value->type == SCALAR_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(value(BOOL_TYPE(), base_int(1))));
	}
	else
	{
		push_stack(vm, vm->stack,
			inst_value(value(BOOL_TYPE(), base_int(0))));
	}
	inst_destroy(right);
}
void cmd_isarray(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PCONFIG node;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != CONFIG_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CONFIG, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	node = right_val->val.ptr;
	if (node == 0)
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_NOT_NULL, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (node->children_size == 0 && node->value.value->type == ARRAY_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(value(BOOL_TYPE(), base_int(1))));
	}
	else
	{
		push_stack(vm, vm->stack,
			inst_value(value(BOOL_TYPE(), base_int(0))));
	}
	inst_destroy(right);
}
void cmd_istext(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PCONFIG node;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != CONFIG_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CONFIG, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	node = right_val->val.ptr;
	if (node == 0)
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_NOT_NULL, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (node->children_size == 0 && node->value.value->type == STRING_TYPE())
	{
		push_stack(vm, vm->stack,
			inst_value(value(BOOL_TYPE(), base_int(1))));
	}
	else
	{
		push_stack(vm, vm->stack,
			inst_value(value(BOOL_TYPE(), base_int(0))));
	}
	inst_destroy(right);
}
void cmd_isclass(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PCONFIG node;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != CONFIG_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CONFIG, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	node = right_val->val.ptr;
	if (node == 0)
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_NOT_NULL, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (node->children_size != 0)
	{
		push_stack(vm, vm->stack,
			inst_value(value(BOOL_TYPE(), base_int(1))));
	}
	else
	{
		push_stack(vm, vm->stack,
			inst_value(value(BOOL_TYPE(), base_int(0))));
	}
	inst_destroy(right);
}

void cmd_toupper(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PSTRING outstring;
	unsigned int i;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != STRING_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	outstring = string_create2(((PSTRING)right_val->val.ptr)->val);
	for (i = 0; i < outstring->length; i++)
	{
		outstring->val[i] = towupper(outstring->val[i]);
	}
	push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(outstring))));
	inst_destroy(right);
}
void cmd_tolower(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PSTRING outstring;
	unsigned int i;
	right = pop_stack(vm, vm->work);
	right_val = get_value(vm, vm->stack, right);
	if (right_val == 0)
	{
		inst_destroy(right);
		return;
	}
	if (right_val->type != STRING_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack,
			inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	outstring = string_create2(((PSTRING)right_val->val.ptr)->val);
	for (i = 0; i < outstring->length; i++)
	{
		outstring->val[i] = towlower(outstring->val[i]);
	}
	push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(outstring))));
	inst_destroy(right);
}