#ifndef _SQF_INST_H_
#define _SQF_INST_H_

#include "SQF_base.h"

inline VALUE value(CPCMD type, BASE val)
{
	VALUE v;
	v.type = type;
	v.val = val;
	if (type->callback != 0)
	{
		type->callback(&v, type);
	}
	return v;
}
PVALUE value_copy(PVALUE in);
PVALUE value_create(CPCMD type, BASE val);
PVALUE value_create_noref(CPCMD type, BASE val);

PINST inst_nop(void);
PINST inst_command(CPCMD cmd);
PINST inst_value(VALUE val);
PINST inst_load_var(const char* name);
PINST inst_store_var(const char* name);
PINST inst_store_var_local(const char* name);
PINST inst_scope(const char* name);
PINST inst_arr_push(void);
PINST inst_code_load(unsigned char createscope);
PINST inst_pop_eval(unsigned int ammount, unsigned char popon);
PINST inst_clear_work(void);
PINST inst_debug_info(unsigned int line, unsigned int col, unsigned long off, unsigned int length, const char* code);
PINST inst_debug_info2(PDBGINF pdbginf);
PINST inst_move(int off);
PINST inst_scope_dropout(const char* scope);



void inst_destroy(PINST inst);
void inst_destroy_dbginf(PDBGINF dbginf);
void inst_destroy_scope(PSCOPE scope);
void inst_destroy_value(PVALUE val);
void inst_destroy_var(char* name);
void inst_destroy_pop_eval(PPOPEVAL popeval);

//inline void get_nop(PINST) {}
inline CPCMD get_command(PVM vm, PSTACK stack, PINST inst)
{
	if (inst != 0 && inst->type == INST_COMMAND)
	{
		return (CPCMD)inst->data.ptr;
	}
	else
	{
		vm->error(vm, "TYPE MISSMATCH", stack);
		return 0;
	}
}
inline PVALUE get_value(PVM vm, PSTACK stack, PINST inst)
{
	if (inst != 0 && inst->type == INST_VALUE)
	{
		return (PVALUE)inst->data.ptr;
	}
	else
	{
		vm->error(vm, "TYPE MISSMATCH", stack);
		return 0;
	}
}
inline const char* get_var_name(PVM vm, PSTACK stack, PINST inst)
{
	if (inst != 0 && (inst->type == INST_LOAD_VAR || inst->type == INST_STORE_VAR || inst->type == INST_STORE_VAR_LOCAL || inst->type == INST_SCOPE_DROPOUT))
	{
		return (const char*)inst->data.ptr;
	}
	else
	{
		vm->error(vm, "TYPE MISSMATCH", stack);
		return 0;
	}
}
inline PSCOPE get_scope(PVM vm, PSTACK stack, PINST inst)
{
	if (inst != 0 && inst->type == INST_SCOPE)
	{
		return (PSCOPE)inst->data.ptr;
	}
	else
	{
		vm->error(vm, "TYPE MISSMATCH", stack);
		return 0;
	}
}
inline PPOPEVAL get_pop_eval(PVM vm, PSTACK stack, PINST inst)
{
	if (inst != 0 && inst->type == INST_POP_EVAL)
	{
		return (PPOPEVAL)inst->data.ptr;
	}
	else
	{
		vm->error(vm, "TYPE MISSMATCH", stack);
		return 0;
	}
}
inline PDBGINF get_dbginf(PVM vm, PSTACK stack, PINST inst)
{
	if (inst != 0 && inst->type == INST_DEBUG_INFO)
	{
		return (PDBGINF)inst->data.ptr;
	}
	else
	{
		vm->error(vm, "TYPE MISSMATCH", stack);
		return 0;
	}
}

#endif
