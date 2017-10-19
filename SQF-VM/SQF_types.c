#include <stdlib.h>
#include <wchar.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "sqffull.h"

bool is_equal_to(PVALUE l, PVALUE r)
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
		for (i = 0; i < (int)arrl->top; i++)
		{
			if (!is_equal_to(arrl->data[i], arrr->data[i]))
				return 0;
		}
		return 1;
	}
	else if (l->type == STRING_TYPE())
	{
		return !wcscmp(((PSTRING)l->val.ptr)->val, ((PSTRING)r->val.ptr)->val);
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
		for (i = 0; i < (int)codel->stack->top; i++)
		{
			instl = codel->stack->data[i];
			instr = coder->stack->data[i];
			if (instl->type != instr->type)
				return 0;
			switch (instl->type)
			{
			case INST_VALUE:
				if (!is_equal_to(get_value(0, 0, instl),
					get_value(0, 0, instr)))
					return 0;
				break;
			case INST_LOAD_VAR:
			case INST_STORE_VAR:
			case INST_STORE_VAR_LOCAL:
				if (wstr_cmpi(get_var_name(0, 0, instl), -1,
					get_var_name(0, 0, instr), -1))
					return 0;
				break;
			case INST_COMMAND:
				if (get_command(0, 0, instl)
					!= get_command(0, 0, instr))
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
		if (((POBJECT)l->val.ptr)->inner == 0
			&& ((POBJECT)r->val.ptr)->inner == 0)
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


void TYPE_CODE_CALLBACK(void* input, CPCMD self);
PCMD SCALAR_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"SCALAR", 't', 0, 0, NULL, NULL, NULL);
	}
	return cmd;
}
PCMD BOOL_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"BOOL", 't', 0, 0, NULL, NULL, NULL);
	}
	return cmd;
}
PCMD IF_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"IF", 't', 0, 0, NULL, NULL, NULL);
	}
	return cmd;
}
PCMD WHILE_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"WHILE", 't', TYPE_CODE_CALLBACK, 0, NULL, NULL,
		NULL);
	}
	return cmd;
}
PCMD NOTHING_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"NOTHING", 't', 0, 0, NULL, NULL, NULL);
	}
	return cmd;
}
PCMD ANY_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"ANY", 't', 0, 0, NULL, NULL, NULL);
	}
	return cmd;
}
PCMD NAN_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"NaN", 't', 0, 0, NULL, NULL, NULL);
	}
	return cmd;
}

void TYPE_CODE_CALLBACK(void* input, CPCMD self)
{
	PVALUE val = input;
	PCODE code = val->val.ptr;
	if (val->type == 0)
	{
		code->refcount--;
		if (code->refcount <= 0)
		{
			code_destroy(code);
		}
	}
	else
	{
		code->refcount++;
	}
}
PCMD CODE_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"CODE", 't', TYPE_CODE_CALLBACK, 0, NULL, NULL,
		NULL);
	}
	return cmd;
}
PCODE code_create(const wchar_t* txt, int offset, int len)
{
	PCODE code = malloc(sizeof(CODE));
	code->length = len;
	code->val = malloc(sizeof(wchar_t) * (len + 1));
	wcsncpy(code->val, txt + offset, len);
	code->val[len] = '\0';
	code->refcount = 0;
	code->stack = create_stack(0, 1);
	return code;
}
void code_destroy(PCODE code)
{
	free(code->val);
	destroy_stack(code->stack);
	free(code);
}



void TYPE_FOR_CALLBACK(void* input, CPCMD self)
{
	PVALUE val = input;
	PFOR f = val->val.ptr;
	if (val->type == 0)
	{
		f->refcount--;
		if (f->refcount <= 0)
		{
			for_destroy(f);
		}
	}
	else
	{
		f->refcount++;
	}
}
PCMD FOR_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"FOR", 't', TYPE_FOR_CALLBACK, 0, NULL, NULL,
		NULL);
	}
	return cmd;
}
PFOR for_create(const wchar_t* varname)
{
	PFOR f = malloc(sizeof(FOR));
	int len = wcslen(varname);
	f->variable = malloc(sizeof(wchar_t) * (len + 1));
	wcscpy(f->variable, varname);
	f->variable[len] = '\0';
	f->variable_length = len;
	f->start = 0;
	f->end = 0;
	f->step = 1;
	f->started = 0;
	f->refcount = 0;

	return f;
}
void for_destroy(PFOR f)
{
	if (f->variable != 0)
	{
		free(f->variable);
	}
	free(f);
}

void TYPE_NAMESPACE_CALLBACK(void* input, CPCMD self)
{
	PVALUE val = input;
	PNAMESPACE namespace = val->val.ptr;
	if (val->type == 0)
	{
		namespace->refcount--;
		if (namespace->refcount <= 0)
		{
			namespace_destroy(namespace);
		}
	}
	else
	{
		namespace->refcount++;
	}
}
PCMD NAMESPACE_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"NAMESPACE", 't', TYPE_NAMESPACE_CALLBACK, 0, NULL,
		NULL, NULL);
	}
	return cmd;
}
PCMD WITH_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"WITH", 't', TYPE_NAMESPACE_CALLBACK, 0, NULL,
		NULL, NULL);
	}
	return cmd;
}
PNAMESPACE namespace_create(void)
{
	PNAMESPACE namespace = malloc(sizeof(NAMESPACE));
	namespace->refcount = 0;
	namespace->data = wsm_create_list(74, 10, 10);
	return namespace;
}
void NAMESPACE_SM_LIST_DESTROY(void* ptr)
{
	PVALUE val = ptr;
	if (ptr == 0)
		return;
	inst_destroy_value(val);
}
void namespace_destroy(PNAMESPACE namespace)
{
	wsm_destroy_list(namespace->data, NAMESPACE_SM_LIST_DESTROY);
	free(namespace);
}
void namespace_set_var(PNAMESPACE namespace, const wchar_t* var, VALUE val)
{
	if (val.type == NOTHING_TYPE())
	{
		NAMESPACE_SM_LIST_DESTROY(wsm_drop_value(namespace->data, var));
	}
	else
	{
		NAMESPACE_SM_LIST_DESTROY(
				wsm_set_value(namespace->data, var,
						value_create_noref(val.type, val.val)));
	}
}
PVALUE namespace_get_var(PNAMESPACE namespace, const wchar_t* var)
{
	return wsm_get_value(namespace->data, var);
}

PNAMESPACE sqf_missionNamespace(void)
{
	static PNAMESPACE ns = 0;
	if (ns == 0)
	{
		ns = namespace_create();
		ns->refcount++;
	}
	return ns;
}
PNAMESPACE sqf_uiNamespace(void)
{
	static PNAMESPACE ns = 0;
	if (ns == 0)
	{
		ns = namespace_create();
		ns->refcount++;
	}
	return ns;
}
PNAMESPACE sqf_profileNamespace(void)
{
	static PNAMESPACE ns = 0;
	if (ns == 0)
	{
		ns = namespace_create();
		ns->refcount++;
	}
	return ns;
}
PNAMESPACE sqf_parsingNamespace(void)
{
	static PNAMESPACE ns = 0;
	if (ns == 0)
	{
		ns = namespace_create();
		ns->refcount++;
	}
	return ns;
}

void TYPE_SWITCH_CALLBACK(void* input, CPCMD self)
{
	PVALUE val = input;
	PSWITCH swtch = val->val.ptr;
	if (val->type == 0)
	{
		swtch->refcount--;
		if (swtch->refcount <= 0)
		{
			switch_destroy(swtch);
		}
	}
	else
	{
		swtch->refcount++;
	}
}
PCMD SWITCH_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"SWITCH", 't', TYPE_SWITCH_CALLBACK, 0, NULL, NULL,
		NULL);
	}
	return cmd;
}
PSWITCH switch_create(VALUE val)
{
	PSWITCH swtch = malloc(sizeof(SWITCH));
	swtch->default_code = 0;
	swtch->selected_code = 0;
	swtch->refcount = 0;
	swtch->was_executed = 0;
	swtch->switch_value = malloc(sizeof(VALUE));
	swtch->switch_value->type = val.type;
	swtch->switch_value->val = val.val;
	return swtch;
}
void switch_destroy(PSWITCH swtch)
{
	if (swtch->default_code != 0)
	{
		inst_destroy_value(swtch->default_code);
	}
	if (swtch->selected_code != 0)
	{
		inst_destroy_value(swtch->selected_code);
	}
	inst_destroy_value(swtch->switch_value);
	free(swtch);
}

void TYPE_GROUP_CALLBACK(void* input, CPCMD self)
{
	PVALUE val = input;
	PGROUP group = val->val.ptr;
	if (val->type == 0)
	{
		group->refcount--;
		if (group->refcount <= 0)
		{
			group_destroy(group);
		}
	}
	else
	{
		group->refcount++;
	}
}
PCMD GROUP_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"GROUP", 't', TYPE_GROUP_CALLBACK, 0, NULL, NULL,
		NULL);
	}
	return cmd;
}
PGROUP group_create(int side)
{
	static int count = 1;
	PGROUP group = malloc(sizeof(GROUP));
	group->refcount = 0;
	group->members = value_create(ARRAY_TYPE(), base_voidptr(array_create()));
	group->side = value_create(SIDE_TYPE(), base_int(side));
	group->ident_len = swprintf(0, 0, L"%c ALPHA %d", side_displayname(side)[0],
			count);
	group->ident = malloc(sizeof(wchar_t) * (group->ident_len + 1));
	swprintf(group->ident, group->ident_len + 1, L"%c ALPHA %d",
			side_displayname(side)[0], count);
	count++;
	return group;
}
void group_destroy(PGROUP group)
{
	inst_destroy_value(group->members);
	inst_destroy_value(group->side);
	free(group->ident);
	free(group);
}

PVALUE group_get_leader(PGROUP group)
{
	if (((PARRAY) group->members->val.ptr)->top == 0)
		return 0;
	else
		return ((PARRAY) group->members->val.ptr)->data[0];
}
PGROUP group_from_ident(PVM vm, const wchar_t* ident)
{
	PGROUP grp;
	int i;
	int j = wsm_count(vm->groupmap);
	for (i = 0; i < j; i++)
	{
		grp = ((PVALUE) wsm_get_value_index(vm->groupmap, i))->val.ptr;
		if (wstr_cmpi(grp->ident, -1, ident, -1))
		{
			return grp;
		}
	}
	return 0;
}

//NON-SQF TYPES

void TYPE_COUNT_CALLBACK(void* input, CPCMD self)
{
	PVALUE val = input;
	PCOUNT count = val->val.ptr;
	if (val->type == 0)
	{
		count->refcount--;
		if (count->refcount <= 0)
		{
			count_destroy(count);
		}
	}
	else
	{
		count->refcount++;
	}
}
PCMD COUNT_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"COUNT__", 't', TYPE_COUNT_CALLBACK, 0, NULL, NULL,
				L"non - sqf compliant helper type");
	}
	return cmd;
}
PCOUNT count_create(PCODE code, PARRAY arr)
{
	PCOUNT count = malloc(sizeof(COUNT));

	PVALUE pvalue = malloc(sizeof(VALUE));
	VALUE val = value(CODE_TYPE(), base_voidptr(code));
	pvalue->type = val.type;
	pvalue->val = val.val;
	count->code = pvalue;

	pvalue = malloc(sizeof(VALUE));
	val = value(ARRAY_TYPE(), base_voidptr(arr));
	pvalue->type = val.type;
	pvalue->val = val.val;
	count->arr = pvalue;

	count->curtop = 0;
	count->refcount = 0;
	count->count = 0;
	return count;
}
void count_destroy(PCOUNT count)
{
	inst_destroy_value(count->code);
	inst_destroy_value(count->arr);
	free(count);
}
