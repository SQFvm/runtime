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
		type->callback(&v);
	}
	return v;
}

PINST inst_nop(void);
PINST inst_command(CPCMD cmd);
PINST inst_value(VALUE val);
PINST inst_load_var(const char* name);
PINST inst_store_var(const char* name);
PINST inst_store_var_local(const char* name);
PINST inst_scope(const char* name);
PINST inst_arr_push(void);
PINST inst_code_load(void);

void inst_destroy(PINST inst);
void inst_destroy_scope(PSCOPE scope);
void inst_destroy_value(PVALUE val);
void inst_destroy_var(char* name);

//inline void get_nop(PINST) {}
inline CPCMD get_command(PSTACK stack, PINST inst)
{
	if (inst->type == INST_COMMAND)
	{
		return (CPCMD)inst->data.ptr;
	}
	else
	{
		error("TYPE MISSMATCH", stack);
		return 0;
	}
}
inline PVALUE get_value(PSTACK stack, PINST inst)
{
	if (inst->type == INST_VALUE)
	{
		return (PVALUE)inst->data.ptr;
	}
	else
	{
		error("TYPE MISSMATCH", stack);
		return 0;
	}
}
inline const char* get_var_name(PSTACK stack, PINST inst)
{
	if (inst->type == INST_LOAD_VAR || inst->type == INST_STORE_VAR || inst->type == INST_STORE_VAR_LOCAL)
	{
		return (const char*)inst->data.ptr;
	}
	else
	{
		error("TYPE MISSMATCH", stack);
		return 0;
	}
}
inline PSCOPE get_scope(PSTACK stack, PINST inst)
{
	if (inst->type == INST_SCOPE)
	{
		return (PSCOPE)inst->data.ptr;
	}
	else
	{
		error("TYPE MISSMATCH", stack);
		return 0;
	}
}


#endif