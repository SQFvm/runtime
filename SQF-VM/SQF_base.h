#ifndef _SQF_BASE_H_
#define _SQF_BASE_H_

#include "basetype.h"

//Structure containing VM instructions
typedef unsigned char DATA_TYPE;
#define INST_NOP ((DATA_TYPE)0)
#define INST_COMMAND ((DATA_TYPE)1)
#define INST_VALUE ((DATA_TYPE)2)
#define INST_LOAD_VAR ((DATA_TYPE)3)
#define INST_STORE_VAR ((DATA_TYPE)4)
#define INST_SCOPE ((DATA_TYPE)5)
#define INST_STORE_VAR_LOCAL ((DATA_TYPE)6)
#define INST_ARR_PUSH ((DATA_TYPE)7)
#define INST_CODE_LOAD ((DATA_TYPE)8)
#define INST_POP_EVAL ((DATA_TYPE)9)
#define INST_CLEAR_WORK ((DATA_TYPE)10)
typedef struct INST
{
	DATA_TYPE type;
	BASE data;
} INST;
typedef INST* PINST;

//Structure containing VM Commands (eg. +, -, *, /)
struct CMD;
typedef void(*CMD_CB)(void*, const struct CMD* self);
typedef struct CMD
{
	char type;
	unsigned char type_code;
	CMD_CB callback;
	char precedence_level;
	char* name;
	unsigned int name_len;
	char* description;
	unsigned int description_len;
} CMD;
typedef CMD* PCMD;
typedef const CMD* CPCMD;

//Basic instruction stack
typedef struct STACK
{
	PINST* data;
	unsigned int size;
	unsigned int top;
} STACK;
typedef STACK* PSTACK;

//Structure containing all VM related informations
typedef struct VM
{
	PSTACK stack;
	PSTACK work;

	PCMD* cmds;
	unsigned int cmds_size;
	unsigned int cmds_top;
	void(*error)(const char*, PSTACK);
} VM;
typedef VM* PVM;

//Simple value structure containing a type command
typedef struct VALUE
{
	CPCMD type;
	BASE val;
} VALUE;
typedef VALUE* PVALUE;

//Scope structure containing required scope informations
struct NAMESPACE;
typedef struct SCOPE
{
	char* name;
	unsigned int name_len;

	char** varstack_name;
	VALUE** varstack_value;
	unsigned int varstack_size;
	unsigned int varstack_top;
	struct NAMESPACE* ns;
}SCOPE;
typedef SCOPE* PSCOPE;

typedef struct POPEVAL
{
	unsigned int ammount;
	unsigned char popon;
}POPEVAL;
typedef POPEVAL* PPOPEVAL;





void error(const char* errMsg, PSTACK stack);
PSTACK create_stack(unsigned int size);
void destroy_stack(PSTACK stack);
void resize_stack(PSTACK stack, unsigned int newsize);
inline void push_stack(PVM vm, PSTACK stack, PINST inst)
{
	if (stack->top >= stack->size)
	{
		vm->error("STACK OVERFLOW", stack);
	}
	else
	{
		stack->data[stack->top++] = inst;
	}
}
inline PINST pop_stack(PVM vm, PSTACK stack)
{
	if (stack->top == 0)
	{
		vm->error("STACK UNDERFLOW", stack);
		return 0;
	}
	else
	{
		return stack->data[--stack->top];
	}
}

PVM sqfvm(unsigned int stack_size, unsigned int work_size, unsigned int cmds_size);
void destroy_sqfvm(PVM vm);

inline void register_command(PVM vm, PCMD cmd)
{
	if (vm->cmds_top >= vm->cmds_size)
	{
		vm->error("COMMAND REGISTER OVERFLOW", vm->stack);
	}
	else
	{
		if (cmd->type == 't' && vm->cmds_top != 0 && vm->cmds[vm->cmds_top - 1]->type != 't')
		{
			vm->error("TYPE COMMAND REGISTERED AFTER NON-TYPE COMMANDS GOT REGISTERED", vm->stack);
		}
		vm->cmds[vm->cmds_top++] = cmd;
	}
}
#endif
