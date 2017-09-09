#include "basetype.h"
#include "string_map.h"
#include "SQF_base.h"
#include "SQF.h"
#include "SQF_inst.h"
#include "SQF_types.h"
#include "SQF_object_type.h"

#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <stdio.h>

extern inline CPCMD get_command(PVM vm, PSTACK stack, PINST inst);
extern inline PVALUE get_value(PVM vm, PSTACK stack, PINST inst);
extern inline const char* get_var_name(PVM vm, PSTACK stack, PINST inst);
extern inline PSCOPE get_scope(PVM vm, PSTACK stack, PINST inst);
extern inline VALUE value(CPCMD type, BASE val);
extern inline PPOPEVAL get_pop_eval(PVM vm, PSTACK stack, PINST inst);
extern inline PDBGINF get_dbginf(PVM vm, PSTACK stack, PINST inst);


PVALUE value_copy(PVALUE in)
{
	PVALUE val = malloc(sizeof(VALUE));
	val->type = in->type;
	val->val = in->val;

	if (val->type->callback != 0)
	{
		val->type->callback(val, val->type);
	}
	return val;
}

static inline PINST inst(DATA_TYPE dt)
{
	PINST inst = malloc(sizeof(INST));
	inst->type = dt;
	inst->data.ptr = 0;
	return inst;
}
PINST inst_nop(void)
{
	return inst(INST_NOP);
}
PINST inst_command(CPCMD cmd)
{
	PINST p = inst(INST_COMMAND);
	p->data.cptr = cmd;
	return p;
}
PINST inst_value(VALUE val)
{
	PINST p = inst(INST_VALUE);
	p->data.ptr = malloc(sizeof(VALUE));
	((PVALUE)p->data.ptr)->type = val.type;
	((PVALUE)p->data.ptr)->val = val.val;
	return p;
}
PINST inst_load_var(const char* name)
{
	PINST p = inst(INST_LOAD_VAR);
	int len = strlen(name);
	p->data.ptr = malloc(sizeof(char) * (len + 1));
	strcpy(p->data.ptr, name);
	return p;
}
PINST inst_store_var(const char* name)
{
	PINST p = inst(INST_STORE_VAR);
	int len = strlen(name);
	p->data.ptr = malloc(sizeof(char) * (len + 1));
	strcpy(p->data.ptr, name);
	return p;
}
PINST inst_store_var_local(const char* name)
{
	PINST p = inst(INST_STORE_VAR_LOCAL);
	int len = strlen(name);
	p->data.ptr = malloc(sizeof(char) * (len + 1));
	strcpy(p->data.ptr, name);
	return p;
}
PINST inst_scope(const char* name)
{
	PINST p = inst(INST_SCOPE);
	PSCOPE s = malloc(sizeof(SCOPE));
	p->data.ptr = s;
	if (name != 0)
	{
		s->name_len = strlen(name);
		s->name = malloc(sizeof(char) * (s->name_len + 1));
		strcpy(s->name, name);
	}
	else
	{
		s->name = 0;
		s->name_len = 0;
	}
	s->varstack_size = 32;
	s->varstack_top = 0;
	s->varstack_name = malloc(sizeof(char*) * s->varstack_size);
	s->varstack_value = malloc(sizeof(VALUE*) * s->varstack_size);
	s->ns = sqf_missionNamespace();
	return p;
}
PINST inst_arr_push(void)
{
	return inst(INST_ARR_PUSH);
}
PINST inst_code_load(unsigned char createscope)
{
	PINST p = inst(INST_CODE_LOAD);
	p->data.c = createscope;
	return p;
}
PINST inst_pop_eval(unsigned int ammount, unsigned char popon)
{
	PINST p = inst(INST_POP_EVAL);
	PPOPEVAL popeval = malloc(sizeof(POPEVAL));
	p->data.ptr = popeval;
	popeval->ammount = ammount;
	popeval->popon = popon;
	return p;
}
PINST inst_clear_work(void)
{
	return inst(INST_CLEAR_WORK);
}
PINST inst_debug_info(unsigned int line, unsigned int col, unsigned long off, unsigned int length, const char* code)
{
	int i;
	int j;
	int len;
	int size;
	const char* str;
	PINST p = inst(INST_DEBUG_INFO);
	PDBGINF dbginf = malloc(sizeof(DBGINF));
	p->data.ptr = dbginf;
	dbginf->col = col;
	dbginf->line = line;
	dbginf->offset = off;
	dbginf->length = length;


	i = dbginf->offset - 15;
	len = 30;
	if (i < 0)
	{
		len += i;
		i = 0;
	}
	for (j = i; j < i + len; j++)
	{
		if (code[j] == '\0' || code[j] == '\n')
		{
			if (j < dbginf->offset)
			{
				i = j + 1;
			}
			else
			{
				len = j - i;
				break;
			}
		}
	}
	str = code + i;
	size = snprintf(0, 0, "%.*s\n%.*s%.*s\n", len, str, dbginf->offset - i, "                              ", dbginf->length > 30 ? 30 : dbginf->length, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	dbginf->hint = malloc(sizeof(char) * (size + 1));
	snprintf(dbginf->hint, size + 1, "%.*s\n%.*s%.*s\n", len, str, dbginf->offset - i, "                              ", dbginf->length > 30 ? 30 : dbginf->length, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");

	return p;
}
PINST inst_debug_info2(PDBGINF pdbginf)
{
	PINST p = inst(INST_DEBUG_INFO);
	PDBGINF dbginf = malloc(sizeof(DBGINF));
	dbginf->col = pdbginf->col;
	dbginf->line = pdbginf->line;
	dbginf->offset = pdbginf->offset;
	dbginf->length = pdbginf->length;
	dbginf->hint = malloc(sizeof(char) * (strlen(pdbginf->hint) + 1));
	strcpy(dbginf->hint, pdbginf->hint);
	p->data.ptr = dbginf;
	return p;
}
PINST inst_move(int off)
{
	PINST p = inst(INST_MOVE);
	p->data.i = off;
	return p;
}
PINST inst_scope_dropout(const char* scope)
{
	PINST p = inst(INST_SCOPE_DROPOUT);
	if (scope == 0)
	{
		p->data.ptr = 0;
	}
	else
	{
		int len = strlen(scope);
		p->data.ptr = malloc(sizeof(char) * (len + 1));
		strcpy(p->data.ptr, scope);
	}
	return p;
}


void inst_destroy(PINST inst)
{
	if (inst == 0)
		return;
	switch (inst->type)
	{
		case INST_NOP:
			break;
		case INST_COMMAND:
			break;
		case INST_VALUE:
			inst_destroy_value(get_value(0, 0, inst));
			break;
		case INST_LOAD_VAR:
			inst_destroy_var((char*)get_var_name(0, 0, inst));
			break;
		case INST_STORE_VAR:
			inst_destroy_var((char*)get_var_name(0, 0, inst));
			break;
		case INST_SCOPE:
			inst_destroy_scope(get_scope(0, 0, inst));
			break;
		case INST_STORE_VAR_LOCAL:
			inst_destroy_var((char*)get_var_name(0, 0, inst));
			break;
		case INST_ARR_PUSH:
			break;
		case INST_CODE_LOAD:
			break;
		case INST_POP_EVAL:
			inst_destroy_pop_eval(get_pop_eval(0, 0, inst));
			break;
		case INST_CLEAR_WORK:
			break;
		case INST_DEBUG_INFO:
			inst_destroy_dbginf(get_dbginf(0, 0, inst));
			break;
		case INST_MOVE:
			break;
		case INST_SCOPE_DROPOUT:
			inst_destroy_var((char*)get_var_name(0, 0, inst));
			break;
		default:
			#if _WIN32
			__asm int 3;
			#endif
			break;
	}
	free(inst);
}
void inst_destroy_scope(PSCOPE scope)
{
	int i;
	if (scope->name != 0)
	{
		free(scope->name);
	}
	for (i = 0; i < scope->varstack_top; i++)
	{
		inst_destroy_value(scope->varstack_value[i]);
		free(scope->varstack_name[i]);
	}
	free(scope->varstack_name);
	free(scope->varstack_value);
	free(scope);
}
void inst_destroy_value(PVALUE val)
{
	CPCMD cmd = val->type;
	val->type = 0;
	if (cmd->callback != 0)
	{
		cmd->callback(val, cmd);
	}
	free(val);
}
void inst_destroy_dbginf(PDBGINF dbginf)
{
	free(dbginf->hint);
	free(dbginf);
}
void inst_destroy_var(char* name)
{
	if (name == 0)
		return;
	free(name);
}

void inst_destroy_pop_eval(PPOPEVAL popeval)
{
	free(popeval);
}


