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
#include <string.h>
#include <stdio.h>

#include "basetype.h"
#include "string_op.h"
#include "string_map.h"
#include "textrange.h"
#include "SQF.h"
#include "SQF_types.h"
#include "SQF_object_type.h"
#include "SQF_side_type.h"
#include "SQF_commands.h"
#include "errors.h"

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
void stringify_value(PVM vm, PSTRING str, PVALUE val)
{
	PARRAY arr;
	int i;
	char* strptr;
	if (val->type == SCALAR_TYPE())
	{
		strptr = alloca(sizeof(char) * 64);
		sprintf(strptr, "%lf", val->val.f);
		string_modify_append(str, strptr);
	}
	else if (val->type == BOOL_TYPE() || val->type == IF_TYPE())
	{
		string_modify_append(str, val->val.i > 0 ? "true" : "false");
	}
	else if (val->type == ARRAY_TYPE())
	{
		arr = ((PARRAY)val->val.ptr);
		string_modify_append(str, "[");
		for (i = 0; i < arr->top; i++)
		{
			if (i > 0)
			{
				string_modify_append(str, ", ");
			}
			stringify_value(vm, str, arr->data[i]);
		}
		string_modify_append(str, "]");
	}
	else if (val->type == STRING_TYPE())
	{
		string_modify_append(str, "\"");
		string_modify_append(str, ((PSTRING)val->val.ptr)->val);
		string_modify_append(str, "\"");
	}
	else if (val->type == NAN_TYPE())
	{
		string_modify_append(str, "NaN");
	}
	else if (val->type == CODE_TYPE() || val->type == WHILE_TYPE())
	{
		string_modify_append(str, "{");
		string_modify_append(str, ((PCODE)val->val.ptr)->val);
		string_modify_append(str, "}");
	}
	else if (val->type == SIDE_TYPE())
	{
		string_modify_append(str, side_displayname(val));
	}
	else if (val->type == NOTHING_TYPE())
	{
		string_modify_append(str, "nil");
	}
	else if (val->type == ANY_TYPE())
	{
		string_modify_append(str, "any");
	}
	else
	{
		string_modify_append(str, "<");
		string_modify_append(str, val->type->name);
		string_modify_append(str, ">");
	}
}

unsigned char is_equal_to(PVM vm, PVALUE l, PVALUE r)
{
	PARRAY arrl;
	PARRAY arrr;
	PCODE codel;
	PCODE coder;
	PINST instl;
	PINST instr;
	int i;
	if (l->type != r->type)
		return 0;
	if (l->type == SCALAR_TYPE())
	{
		return l->val.f == r->val.f;
	}
	else if (l->type == BOOL_TYPE() || l->type == IF_TYPE())
	{
		return (l->val.i > 0 ? 1 : 0) == (r->val.i > 0 ? 1 : 0);
	}
	else if (l->type == ARRAY_TYPE())
	{
		arrl = l->val.ptr;
		arrr = r->val.ptr;
		if (arrl->top != arrr->top)
		{
			return 0;
		}
		for (i = 0; i < arrl->top; i++)
		{
			if (!is_equal_to(vm, arrl->data[i], arrr->data[i]))
				return 0;
		}
		return 1;
	}
	else if (l->type == STRING_TYPE())
	{
		return !strcmp(((PSTRING)l->val.ptr)->val, ((PSTRING)r->val.ptr)->val);
	}
	else if (l->type == NAMESPACE_TYPE())
	{
		return l->val.ptr == r->val.ptr;
	}
	else if (l->type == CODE_TYPE() || l->type == WHILE_TYPE())
	{
		codel = l->val.ptr;
		coder = r->val.ptr;

		if (codel->stack->top != coder->stack->top)
			return 0;
		for (i = 0; i < codel->stack->top; i++)
		{
			instl = codel->stack->data[i];
			instr = coder->stack->data[i];
			if (instl->type != instr->type)
				return 0;
			switch (instl->type)
			{
				case INST_VALUE:
					if (!is_equal_to(vm, get_value(vm, vm->stack, instl), get_value(vm, vm->stack, instr)))
						return 0;
					break;
				case INST_LOAD_VAR:
				case INST_STORE_VAR:
				case INST_STORE_VAR_LOCAL:
					if (str_cmpi(get_var_name(vm, vm->stack, instl), -1, get_var_name(vm, vm->stack, instr), -1))
						return 0;
					break;
				case INST_COMMAND:
					if (get_command(vm, vm->stack, instl) != get_command(vm, vm->stack, instr))
						return 0;
					break;
			}
		}
		return 1;
	}
	else if (l->type == SIDE_TYPE())
	{
		return side_equals(l, r);
	}
	else if (l->type == OBJECT_TYPE())
	{
		if (((POBJECT)l->val.ptr)->inner == 0 && ((POBJECT)r->val.ptr)->inner == 0)
		{
			return 1;
		}
		else
		{
			return l->val.ptr == r->val.ptr;
		}
	}
	//else if (l->type == WITH_TYPE())
	//{
	//	
	//}
	else
	{
		return 0;
	}
}


void CMD_ISEQUALTO(void* input, CPCMD self)
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
	push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(is_equal_to(vm, left_val, right_val)))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_PLUS(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY arr;
	PSTRING str;
	int i, j;
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
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		push_stack(vm, vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f + right_val->val.f))));
	}
	else if (left_val->type == STRING_TYPE())
	{
		if (right_val->type != STRING_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_STRING, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		str = string_concat(((PSTRING)left_val->val.ptr), ((PSTRING)right_val->val.ptr));
		push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(str))));
	}
	else if (left_val->type == ARRAY_TYPE())
	{
		j = 0;
		if (right_val->type == ARRAY_TYPE())
		{
			arr = ((PARRAY)right_val->val.ptr);
			for (i = arr->top - 1; i >= 0; i--, j++)
			{
				push_stack(vm, vm->stack, inst_arr_push());
				push_stack(vm, vm->stack, inst_value(value(arr->data[i]->type, arr->data[i]->val)));
			}
		}
		else
		{
			push_stack(vm, vm->stack, inst_arr_push());
			push_stack(vm, vm->stack, inst_value(value(right_val->type, right_val->val)));
		}
		arr = ((PARRAY)left_val->val.ptr);
		for (i = arr->top - 1; i >= 0; i--)
		{
			push_stack(vm, vm->stack, inst_arr_push());
			push_stack(vm, vm->stack, inst_value(value(arr->data[i]->type, arr->data[i]->val)));
		}
		push_stack(vm, vm->stack, inst_value(value(ARRAY_TYPE(), base_voidptr(array_create2(j)))));
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR ERR_OR ERR_STRING ERR_OR ERR_ARRAY, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_PLUS_UNARY(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(ARRAY_TYPE(), base_voidptr(array_copy(right_val->val.ptr)))));
		inst_destroy(right);
	}
	else if (right_val->type == SCALAR_TYPE())
	{
		push_stack(vm, vm->stack, inst_value(value(right_val->type, right_val->val)));
		inst_destroy(right);
	}
	else if (right_val->type == NAN_TYPE())
	{
		push_stack(vm, vm->stack, inst_value(value(right_val->type, right_val->val)));
		inst_destroy(right);
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_OR ERR_SCALAR ERR_OR ERR_NAN, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
}
void CMD_MINUS(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY larr;
	PARRAY rarr;
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
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		push_stack(vm, vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f - right_val->val.f))));
	}
	else if (left_val->type == ARRAY_TYPE())
	{
		if (right_val->type != ARRAY_TYPE())
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		k = 0;
		larr = left_val->val.ptr;
		rarr = right_val->val.ptr;
		for (i = larr->top - 1; i >= 0; i--)
		{
			for (j = 0; j < rarr->top; j++)
			{
				if (is_equal_to(vm, larr->data[i], rarr->data[j]))
				{
					break;
				}
			}
			if (j == rarr->top)
			{
				push_stack(vm, vm->stack, inst_arr_push());
				push_stack(vm, vm->stack, inst_value(value(larr->data[i]->type, larr->data[i]->val)));
				k++;
			}
		}
		push_stack(vm, vm->stack, inst_value(value(ARRAY_TYPE(), base_voidptr(array_create2(k)))));
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR ERR_OR ERR_ARRAY, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_MINUS_UNARY(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	push_stack(vm, vm->stack, inst_value(value(right_val->type, base_float(-right_val->val.f))));
	inst_destroy(right);
}
void CMD_MULTIPLY(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	push_stack(vm, vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f * right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_DIVIDE(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
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
	push_stack(vm, vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f / right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_DIAG_LOG(void* input, CPCMD self)
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
	vm->print(vm, "%s\n", str->val);
	string_destroy(str);
	inst_destroy(right);
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
}
void CMD_PRIVATE(void* input, CPCMD self)
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
			store_in_scope(vm, top_scope(vm), str->val, value(ANY_TYPE(), base_voidptr(0)));
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
					store_in_scope(vm, top_scope(vm), str->val, value(ANY_TYPE(), base_voidptr(0)));
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

void CMD_IF(void* input, CPCMD self)
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
void CMD_THEN(void* input, CPCMD self)
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
			vm->error(vm, ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_CODE, vm->stack);
		}
		else if (arr->top > 1 && arr->data[1]->type != CODE_TYPE())
		{
			vm->error(vm, ERR_ERR ERR_ARRAY_(1) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_CODE, vm->stack);
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
				push_stack(vm, vm->stack, inst_value(value(CODE_TYPE(), base_voidptr(code))));
			}
		}
	}
	else if (right_val->type == CODE_TYPE())
	{
		if (left_val->val.i)
		{
			push_stack(vm, vm->stack, inst_code_load(1));
			push_stack(vm, vm->stack, inst_value(value(CODE_TYPE(), base_voidptr(right_val->val.ptr))));
		}
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CODE ERR_OR ERR_ARRAY, vm->stack);
	}

	inst_destroy(left);
	inst_destroy(right);
}
void CMD_ELSE(void* input, CPCMD self)
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

	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(CODE_TYPE(), right_val->val)));
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(CODE_TYPE(), left_val->val)));
	push_stack(vm, vm->stack, inst_value(value(ARRAY_TYPE(), base_voidptr(array_create2(2)))));

	inst_destroy(left);
	inst_destroy(right);
}
void CMD_TRUE(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(1))));
}
void CMD_FALSE(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(0))));
}
void CMD_HELP(void* input, CPCMD self)
{
	PVM vm = input;
	int i;
	int count;
	CPCMD cmd;
	vm->print(vm, "ERRORS might result in crash\n\n");
	vm->print(vm, "NAME:TYPE:PRECEDENCE:USAGE\n");
	count = sm_count(vm->cmd_container->types);
	for (i = 0; i < count; i++)
	{
		cmd = sm_get_value_index(vm->cmd_container->types, i);
		vm->print(vm, "%s:%c:%d:%s\n", cmd->name, cmd->type, cmd->precedence_level, cmd->usage);
	}
	count = sm_count(vm->cmd_container->nullar);
	for (i = 0; i < count; i++)
	{
		cmd = sm_get_value_index(vm->cmd_container->nullar, i);
		vm->print(vm, "%s:%c:%d:%s\n", cmd->name, cmd->type, cmd->precedence_level, cmd->usage);
	}
	count = sm_count(vm->cmd_container->unary);
	for (i = 0; i < count; i++)
	{
		cmd = sm_get_value_index(vm->cmd_container->unary, i);
		vm->print(vm, "%s:%c:%d:%s\n", cmd->name, cmd->type, cmd->precedence_level, cmd->usage);
	}
	count = sm_count(vm->cmd_container->binary);
	for (i = 0; i < count; i++)
	{
		cmd = sm_get_value_index(vm->cmd_container->binary, i);
		vm->print(vm, "%s:%c:%d:%s\n", cmd->name, cmd->type, cmd->precedence_level, cmd->usage);
	}
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
}
void cmd_help_unary_helper(PVM vm, CPCMD cmd)
{
	char* ptr;
	char* ptr2;
	vm->print(vm, "<%s> %s\n", cmd->type == 'b' ? "BINARY" : cmd->type == 'u' ? "UNARY" : cmd->type == 'n' ? "NULLAR" : "TYPE", cmd->name);
	if (cmd->description != 0)
	{
		vm->print(vm, "\tDescription:\n\t\t%s\n", cmd->description);
	}
	if (cmd->type == 'b')
	{
		vm->print(vm, "\tPrecedence: %d", cmd->precedence_level);
	}
	if (cmd->usage)
	{
		vm->print(vm, "\tUsage:\n");
		ptr = cmd->usage;
		while ((ptr2 = strchr(ptr, '|')) != 0)
		{
			vm->print(vm, "\t\t%.*s\n", ptr2 - ptr, ptr);
			ptr = ptr2 + 1;
		}
	}
	if (cmd->examples)
	{
		vm->print(vm, "\tExamples:\n");
		ptr = cmd->examples;
		while ((ptr2 = strchr(ptr, '#')) != 0)
		{
			vm->print(vm, "\t\t%.*s\n", ptr2 - ptr, ptr);
			ptr = ptr2 + 1;
		}
	}
}
void CMD_HELP_UNARY(void* input, CPCMD self)
{
	PVM vm = input;
	CPCMD cmd;
	PINST right;
	PVALUE right_val;
	PSTRING str;
	unsigned char had_match = 0;
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
	cmd = sm_get_value(vm->cmd_container->types, str->val);
	if (cmd != 0)
	{
		if (!had_match)
		{
			had_match = 1;
		}
		else
		{
			vm->print(vm, "\n");
		}
		cmd_help_unary_helper(vm, cmd);
	}
	cmd = sm_get_value(vm->cmd_container->nullar, str->val);
	if (cmd != 0)
	{
		if (!had_match)
		{
			had_match = 1;
		}
		else
		{
			vm->print(vm, "\n");
		}
		cmd_help_unary_helper(vm, cmd);
	}
	cmd = sm_get_value(vm->cmd_container->unary, str->val);
	if (cmd != 0)
	{
		if (!had_match)
		{
			had_match = 1;
		}
		else
		{
			vm->print(vm, "\n");
		}
		cmd_help_unary_helper(vm, cmd);
	}
	cmd = sm_get_value(vm->cmd_container->binary, str->val);
	if (cmd != 0)
	{
		if (!had_match)
		{
			had_match = 1;
		}
		else
		{
			vm->print(vm, "\n");
		}
		cmd_help_unary_helper(vm, cmd);
	}
	if (!had_match)
	{
		vm->print(vm, "'%s' could not be located.\n", str->val);
	}
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
}


void CMD_STR(void* input, CPCMD self)
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
	push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(str))));
}

void CMD_LARGETTHEN(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(left_val->val.f > right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_LESSTHEN(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(left_val->val.f < right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_LARGETTHENOREQUAL(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(left_val->val.f >= right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_LESSTHENOREQUAL(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(left_val->val.f <= right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_EQUAL(void* input, CPCMD self)
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
	if (left_val->type != SCALAR_TYPE() && left_val->type != STRING_TYPE() && left_val->type != OBJECT_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR ERR_OR ERR_STRING ERR_OR ERR_OBJECT, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE() && right_val->type != STRING_TYPE() && right_val->type != OBJECT_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR ERR_OR ERR_STRING ERR_OR ERR_OBJECT, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != right_val->type)
	{
		vm->error(vm, ERR_ERR ERR_TYPES ERR_EQUAL_TYPE, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	else if (left_val->type == SCALAR_TYPE())
	{
		push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(left_val->val.f == right_val->val.f))));
	}
	else if (left_val->type == STRING_TYPE())
	{
		push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(!str_cmpi(((PSTRING)left_val->val.ptr)->val, -1, ((PSTRING)right_val->val.ptr)->val, -1)))));
	}
	else if (left_val->type == OBJECT_TYPE())
	{
		push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(left_val->val.ptr == right_val->val.ptr))));
	}
	else
	{
		#ifdef _WIN32
		__asm int 3;
		#endif
	}
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_NOTEQUAL(void* input, CPCMD self)
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
	if (left_val->type != SCALAR_TYPE() && left_val->type != STRING_TYPE() && left_val->type != OBJECT_TYPE())
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_SCALAR ERR_OR ERR_STRING ERR_OR ERR_OBJECT, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE() && right_val->type != STRING_TYPE() && right_val->type != OBJECT_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR ERR_OR ERR_STRING ERR_OR ERR_OBJECT, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (left_val->type != right_val->type)
	{
		vm->error(vm, ERR_ERR ERR_TYPES ERR_EQUAL_TYPE, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	else if (left_val->type == SCALAR_TYPE())
	{
		push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(left_val->val.f != right_val->val.f))));
	}
	else if (left_val->type == STRING_TYPE())
	{
		push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(str_cmpi(((PSTRING)left_val->val.ptr)->val, -1, ((PSTRING)right_val->val.ptr)->val, -1)))));
	}
	else if (left_val->type == OBJECT_TYPE())
	{
		push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(left_val->val.ptr != right_val->val.ptr))));
	}
	else
	{
		#ifdef _WIN32
		__asm int 3;
		#endif
	}
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_ANDAND(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type == BOOL_TYPE())
	{
		push_stack(vm, vm->stack, inst_value(value(left_val->type, base_int(left_val->val.i && right_val->val.i))));
		inst_destroy(left);
		inst_destroy(right);
	}
	else if (right_val->type == CODE_TYPE())
	{
		if (!left_val->val.i)
		{
			push_stack(vm, vm->stack, inst_value(value(left_val->type, base_int(0))));
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
}
void CMD_OROR(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type == BOOL_TYPE())
	{
		push_stack(vm, vm->stack, inst_value(value(left_val->type, base_int(left_val->val.i || right_val->val.i))));
		inst_destroy(left);
		inst_destroy(right);
	}
	else if (right_val->type == CODE_TYPE())
	{
		if (left_val->val.i)
		{
			push_stack(vm, vm->stack, inst_value(value(left_val->type, base_int(1))));
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
}

void CMD_SELECT(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY arr;
	PVALUE tmp;
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
			if (index < 0 || index > arr->top)
			{
				vm->error(vm, ERR_SPECIAL_SELECT_1, vm->stack);
				push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
			}
			else if (index == arr->top)
			{
				push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
			}
			else
			{
				tmp = arr->data[index];
				push_stack(vm, vm->stack, inst_value(value(tmp->type, tmp->val)));
			}
		}
		else
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		}
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_ARRAY, vm->stack);
	}


	inst_destroy(left);
	inst_destroy(right);
}


void CMD_WHILE(void* input, CPCMD self)
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
void CMD_DO(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PCODE code;
	PFOR pfor;
	PSWITCH swtch;
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
		push_stack(vm, vm->stack, inst_command(find_command(vm, "do", 'b')));
		push_stack(vm, vm->stack, left);
		push_stack(vm, vm->stack, right);
		push_stack(vm, vm->stack, inst_code_load(0));
		push_stack(vm, vm->stack, inst_value(value(CODE_TYPE(), base_voidptr(code))));
		push_stack(vm, vm->stack, inst_pop_eval(5, 0));
		push_stack(vm, vm->stack, inst_code_load(0));
		push_stack(vm, vm->stack, inst_value(value(CODE_TYPE(), left_val->val)));
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
		if (pfor->step > 0 ? pfor->current < pfor->end : pfor->current > pfor->end)
		{
			push_stack(vm, vm->stack, inst_command(find_command(vm, "do", 'b')));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, right);
			push_stack(vm, vm->stack, inst_scope("loop"));
			push_stack(vm, vm->stack, inst_code_load(0));
			push_stack(vm, vm->stack, inst_value(value(CODE_TYPE(), right_val->val)));
			push_stack(vm, vm->stack, inst_store_var_local(pfor->variable));
			push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(pfor->current))));
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
				push_stack(vm, vm->stack, inst_value(value(CODE_TYPE(), swtch->selected_code->val)));
			}
			else if (swtch->default_code != 0)
			{
				push_stack(vm, vm->stack, inst_code_load(0));
				push_stack(vm, vm->stack, inst_value(value(CODE_TYPE(), swtch->default_code->val)));
			}
			inst_destroy(left);
			inst_destroy(right);
		}
		else
		{
			swtch->was_executed = 1;
			push_stack(vm, vm->stack, inst_command(find_command(vm, "do", 'b')));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, right);
			push_stack(vm, vm->stack, inst_scope("switch"));
			push_stack(vm, vm->stack, inst_code_load(0));
			push_stack(vm, vm->stack, inst_value(value(CODE_TYPE(), right_val->val)));
			push_stack(vm, vm->stack, inst_store_var_local(SWITCH_SPECIAL_VAR));
			push_stack(vm, vm->stack, inst_value(value(SWITCH_TYPE(), base_voidptr(swtch))));
			push_stack(vm, vm->stack, inst_clear_work());
		}
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_WHILE ERR_OR ERR_FOR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
}
void CMD_TYPENAME(void* input, CPCMD self)
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
	push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(str))));
	inst_destroy(right);
}
void CMD_COUNT(void* input, CPCMD self)
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
			push_stack(vm, vm->stack, inst_value(value(COUNT_TYPE(), base_voidptr(count))));
			push_stack(vm, vm->stack, inst_scope(0));
			push_stack(vm, vm->stack, inst_code_load(0));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, inst_store_var_local("_x"));
			push_stack(vm, vm->stack, inst_value(value(arr->data[count->curtop]->type, arr->data[count->curtop]->val)));
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
			push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(count->count))));
			inst_destroy(left);
			inst_destroy(right);
		}
		else
		{
			push_stack(vm, vm->stack, inst_command(self));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, inst_scope(0));
			push_stack(vm, vm->stack, inst_code_load(0));
			push_stack(vm, vm->stack, inst_value(value(CODE_TYPE(), base_voidptr(count->code->val.ptr))));
			push_stack(vm, vm->stack, inst_store_var_local("_x"));
			push_stack(vm, vm->stack, inst_value(value(arr->data[count->curtop]->type, arr->data[count->curtop]->val)));
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
void CMD_COUNT_UNARY(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(((PSTRING)right_val->val.ptr)->length))));
	}
	else if (right_val->type == ARRAY_TYPE())
	{
		push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(((PARRAY)right_val->val.ptr)->top))));
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_STRING ERR_OR ERR_ARRAY, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	}
	inst_destroy(right);
}
void CMD_FORMAT(void* input, CPCMD self)
{
	PVM vm = input;
	PINST right;
	PVALUE right_val;
	PSTRING str;
	PSTRING str_out;
	PARRAY arr;
	char* ptr;
	char* ptr_last;
	char* endptr;
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
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		}
		else if (arr->data[0]->type != STRING_TYPE())
		{
			vm->error(vm, ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_STRING, vm->stack);
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		}
		else
		{
			str = arr->data[0]->val.ptr;
			str_out = string_create(0);
			ptr_last = str->val;
			while ((ptr = strchr(ptr_last, '%')) != 0)
			{
				string_modify_nappend(str_out, ptr_last, ptr - ptr_last);
				index = strtof(ptr + 1, &endptr);
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
						string_modify_append(str_out, ((PSTRING)arr->data[index]->val.ptr)->val);
					}
				}
				ptr_last = endptr;
			}
			string_modify_append(str_out, ptr_last);
			push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(str_out))));
		}
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	}
	inst_destroy(right);
}
void CMD_CALL(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_store_var_local("_this"));
		push_stack(vm, vm->stack, inst_value(value(left_val->type, left_val->val)));
		inst_destroy(left);
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CODE, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(right);
	}
}
void CMD_CALL_UNARY(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_store_var_local("_this"));
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CODE, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(right);
	}
}

void CMD_FOR(void* input, CPCMD self)
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
void CMD_FROM(void* input, CPCMD self)
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
void CMD_TO(void* input, CPCMD self)
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
void CMD_STEP(void* input, CPCMD self)
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
void CMD_PARSINGNAMESPACE(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(value(NAMESPACE_TYPE(), base_voidptr(sqf_parsingNamespace()))));
}
void CMD_MISSIONNAMESPACE(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(value(NAMESPACE_TYPE(), base_voidptr(sqf_missionNamespace()))));
}
void CMD_UINAMESPACE(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(value(NAMESPACE_TYPE(), base_voidptr(sqf_uiNamespace()))));
}
void CMD_PROFILENAMESPACE(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(value(NAMESPACE_TYPE(), base_voidptr(sqf_profileNamespace()))));
}
void CMD_DIAG_TICKTIME(void* input, CPCMD self)
{
	PVM vm = input;
	int64_t systime_cur = system_time_ms();
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(systime_cur - systime_start))));
}
void CMD_FOREACH(void* input, CPCMD self)
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
			push_stack(vm, vm->stack, inst_value(value(COUNT_TYPE(), base_voidptr(count))));
			push_stack(vm, vm->stack, right);
			push_stack(vm, vm->stack, inst_clear_work());
			push_stack(vm, vm->stack, inst_scope(0));
			push_stack(vm, vm->stack, inst_code_load(0));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, inst_store_var_local("_x"));
			push_stack(vm, vm->stack, inst_value(value(arr->data[count->curtop]->type, arr->data[count->curtop]->val)));
			push_stack(vm, vm->stack, inst_store_var_local("_forEachIndex"));
			push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(count->curtop))));
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
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
			inst_destroy(left);
			inst_destroy(right);
		}
		else
		{
			push_stack(vm, vm->stack, inst_command(self));
			push_stack(vm, vm->stack, left);
			push_stack(vm, vm->stack, right);
			push_stack(vm, vm->stack, inst_clear_work());
			push_stack(vm, vm->stack, inst_scope(0));
			push_stack(vm, vm->stack, inst_code_load(0));
			push_stack(vm, vm->stack, inst_value(value(CODE_TYPE(), base_voidptr(count->code->val.ptr))));
			push_stack(vm, vm->stack, inst_store_var_local("_x"));
			push_stack(vm, vm->stack, inst_value(value(arr->data[count->curtop]->type, arr->data[count->curtop]->val)));
			push_stack(vm, vm->stack, inst_store_var_local("_forEachIndex"));
			push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(count->curtop))));
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
void CMD_ABS(void* input, CPCMD self)
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
void CMD_DEG(void* input, CPCMD self)
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
void CMD_LOG(void* input, CPCMD self)
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
void CMD_PI(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(M_PI))));
}
void CMD_SIN(void* input, CPCMD self)
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
void CMD_COS(void* input, CPCMD self)
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
void CMD_TAN(void* input, CPCMD self)
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
void CMD_EXP(void* input, CPCMD self)
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
void CMD_ASIN(void* input, CPCMD self)
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
void CMD_ACOS(void* input, CPCMD self)
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
void CMD_ATAN(void* input, CPCMD self)
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
void CMD_RAD(void* input, CPCMD self)
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
void CMD_SQRT(void* input, CPCMD self)
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
void CMD_CEIL(void* input, CPCMD self)
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
void CMD_RANDOM(void* input, CPCMD self)
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
	f = ((double)rand() / (double)(RAND_MAX / right_val->val.f));
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(f))));
	inst_destroy(right);
}
void CMD_FLOOR(void* input, CPCMD self)
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
void CMD_LN(void* input, CPCMD self)
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
void CMD_ROUND(void* input, CPCMD self)
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

void CMD_ATAN2(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
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
void CMD_MIN(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
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
void CMD_MAX(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
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
void CMD_MOD(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
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
void CMD_NOT(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(right_val->val.i))));
	}
	else
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_BOOL, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(right);
		return;
	}
}
void CMD_POWEROF(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
		return;
	}
	if (right_val->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_SCALAR, vm->stack);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
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
void CMD_COMMENT(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(right);
		return;
	}
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(right);
}


void CMD_PUSHBACK(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = left_val->val.ptr;
	array_push(arr, value(right_val->type, right_val->val));
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(arr->top - 1))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_SET(void* input, CPCMD self)
{
	PVM vm = input;
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY arrl, arrr;
	int index, i;
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arrl = left_val->val.ptr;
	arrr = right_val->val.ptr;
	if (arrr->top < 1 || arrr->data[0]->type != SCALAR_TYPE())
	{
		vm->error(vm, ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_SCALAR, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (arrr->top < 2)
	{
		vm->error(vm, ERR_ERR ERR_ARRAY_(1) ERR_WAS_EXPECTED ERR_EXISTING, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	index = floor(arrr->data[0]->val.f);
	val = arrr->data[1];
	if (index > arrl->size)
	{
		array_resize(arrl, index + 1);
	}
	for (i = arrl->top; i < index; i++)
	{
		array_push(arrl, value(NOTHING_TYPE(), base_int(0)));
	}
	array_push(arrl, value(val->type, val->val));
	

	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(left);
	inst_destroy(right);
}


void CMD_GETVARIABLE(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type == ARRAY_TYPE())
	{
		arr = right_val->val.ptr;
		if (arr->top < 2)
		{
			vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_ARRAY_SIZE(2), vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
		if (arr->data[0]->type != STRING_TYPE())
		{
			vm->error(vm, ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_STRING, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
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
		val = namespace_get_var(ns, ((PSTRING)right_val->val.ptr)->val);
		if (val == 0)
		{
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_SETVARIABLE(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = right_val->val.ptr;
	if (arr->top < 2)
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_ARRAY_SIZE(2), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (arr->data[0]->type != STRING_TYPE())
	{
		vm->error(vm, ERR_ERR ERR_ARRAY_(0) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_STRING, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	namespace_set_var(ns, ((PSTRING)arr->data[0]->val.ptr)->val, value(arr->data[1]->type, arr->data[1]->val));
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(left);
	inst_destroy(right);
}

void CMD_CREATEVEHICLE(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = right_val->val.ptr;
	if (arr->top < 3)
	{
		vm->error(vm, ERR_RIGHT ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	for (i = 0; i < 3; i++)
	{
		if (arr->data[i]->type != SCALAR_TYPE())
		{
			vm->error(vm, ERR_ERR ERR_ARRAY_(i) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_SCALAR, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
	}
	obj = object_vehicle_create(((PSTRING)left_val->val.ptr)->val);
	obj->posX = arr->data[0]->val.f;
	obj->posY = arr->data[1]->val.f;
	obj->posZ = arr->data[2]->val.f;

	push_stack(vm, vm->stack, inst_value(value(OBJECT_TYPE(), base_voidptr(obj))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_TYPEOF(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	obj = right_val->val.ptr;
	push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(string_create2(obj->classname)))));
	inst_destroy(right);
}
void CMD_GETPOS(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	obj = right_val->val.ptr;
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(obj->posZ))));
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(obj->posY))));
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(obj->posX))));
	push_stack(vm, vm->stack, inst_value(value(ARRAY_TYPE(), base_voidptr(array_create2(3)))));
	inst_destroy(right);
}
void CMD_SETPOS(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = right_val->val.ptr;
	if (arr->top < 3)
	{
		vm->error(vm, ERR_RIGHT ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	for (i = 0; i < 3; i++)
	{
		if (arr->data[i]->type != SCALAR_TYPE())
		{
			vm->error(vm, ERR_ERR ERR_ARRAY_(i) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_SCALAR, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
	}
	obj = left_val->val.ptr;
	obj->posX = arr->data[0]->val.f;
	obj->posY = arr->data[1]->val.f;
	obj->posZ = arr->data[2]->val.f;

	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_VELOCITY(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	obj = right_val->val.ptr;
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(obj->velZ))));
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(obj->velY))));
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(obj->velX))));
	push_stack(vm, vm->stack, inst_value(value(ARRAY_TYPE(), base_voidptr(array_create2(3)))));
	inst_destroy(right);
}
void CMD_SETVELOCITY(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != ARRAY_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = right_val->val.ptr;
	if (arr->top < 3)
	{
		vm->error(vm, ERR_RIGHT ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	for (i = 0; i < 3; i++)
	{
		if (arr->data[i]->type != SCALAR_TYPE())
		{
			vm->error(vm, ERR_ERR ERR_ARRAY_(i) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_SCALAR, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
	}
	obj = left_val->val.ptr;
	obj->velX = arr->data[0]->val.f;
	obj->velY = arr->data[1]->val.f;
	obj->velZ = arr->data[2]->val.f;

	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_DOMOVE(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	arr = right_val->val.ptr;
	if (arr->top < 3)
	{
		vm->error(vm, ERR_RIGHT ERR_ARRAY ERR_ARRAY_SIZE(3), vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	for (i = 0; i < 3; i++)
	{
		if (arr->data[i]->type != SCALAR_TYPE())
		{
			vm->error(vm, ERR_ERR ERR_ARRAY_(i) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_SCALAR, vm->stack);
			inst_destroy(left);
			inst_destroy(right);
			push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
			return;
		}
	}

	if (left_val->type == OBJECT_TYPE())
	{
		obj = left_val->val.ptr;
		if (!obj->is_vehicle)
		{
			obj->posX = arr->data[0]->val.f;
			obj->posY = arr->data[1]->val.f;
			obj->posZ = arr->data[2]->val.f;
		}
	}
	else if (left_val->type == ARRAY_TYPE())
	{
		arr2 = left_val->val.ptr;

		for (i = 0; i < arr2->top; i++)
		{
			if (arr2->data[i]->type != OBJECT_TYPE())
			{
				vm->error(vm, ERR_ERR ERR_ARRAY_(i) ERR_WAS_EXPECTED ERR_OF_TYPE ERR_OBJECT, vm->stack);
				inst_destroy(left);
				inst_destroy(right);
				push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
				return;
			}
		}

		for (i = 0; i < arr2->top; i++)
		{
			obj = arr2->data[i]->val.ptr;
			if (!obj->is_vehicle)
			{
				obj->posX = arr->data[0]->val.f;
				obj->posY = arr->data[1]->val.f;
				obj->posZ = arr->data[2]->val.f;
			}
		}
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		inst_destroy(left);
		inst_destroy(right);
	}
	else
	{
		vm->error(vm, ERR_LEFT_TYPE ERR_OBJECT, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
}
void CMD_OBJNULL(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(value(OBJECT_TYPE(), base_voidptr(object_create("")))));
}



void CMD_SWITCH(void* input, CPCMD self)
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
	push_stack(vm, vm->stack, inst_value(value(SWITCH_TYPE(), base_voidptr(swtch))));
	inst_destroy(right);
}
void CMD_CASE(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	swtch = switch_create(value(right_val->type, right_val->val));
	push_stack(vm, vm->stack, inst_value(value(SWITCH_TYPE(), base_voidptr(swtch))));
	inst_destroy(right);
}
void CMD_DEFAULT(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	var = find_var(vm, SWITCH_SPECIAL_VAR);
	if (var == 0 || var->type != SWITCH_TYPE())
	{
		vm->error(vm, ERR_SPECIAL_SWITCH, vm->stack);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	swtch = var->val.ptr;
	swtch->default_code = value_copy(right_val);
	inst_destroy(right);
}
void CMD_CASEOPERATOR(void* input, CPCMD self)
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
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	if (right_val->type != CODE_TYPE())
	{
		vm->error(vm, ERR_RIGHT_TYPE ERR_CODE, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	var = find_var(vm, SWITCH_SPECIAL_VAR);
	if (var == 0 || var->type != SWITCH_TYPE())
	{
		vm->error(vm, ERR_SPECIAL_SWITCH, vm->stack);
		inst_destroy(left);
		inst_destroy(right);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		return;
	}
	swtch = var->val.ptr;
	left_swtch = left_val->val.ptr;

	if (is_equal_to(vm, swtch->switch_value, left_swtch->switch_value))
	{
		swtch->selected_code = value_copy(right_val);
		push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
		push_stack(vm, vm->stack, inst_scope_dropout(0));
	}
	push_stack(vm, vm->stack, inst_value(value(NOTHING_TYPE(), base_int(0))));
	inst_destroy(left);
	inst_destroy(right);
}

void CMD_WEST(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_blu()));
}
void CMD_BLUFOR(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_blu()));
}
void CMD_EAST(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_opf()));
}
void CMD_OPFOR(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_opf()));
}
void CMD_RESISTANCE(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_gue()));
}
void CMD_INDEPENDENT(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_gue()));
}
void CMD_SIDEUNKNOWN(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_unknown()));
}
void CMD_SIDELOGIC(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_logic()));
}
void CMD_SIDEFRIENDLY(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_friendly()));
}
void CMD_SIDEENEMY(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_enemy()));
}
void CMD_CIVILIAN(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_civ()));
}
void CMD_SIDEEMPTY(void* input, CPCMD self)
{
	PVM vm = input;
	push_stack(vm, vm->stack, inst_value(side_empty()));
}