#include "basetype.h"
#include "string_map.h"
#include "string_op.h"
#include "textrange.h"
#include "SQF_base.h"
#include "SQF.h"
#include "SQF_types.h"
#include "SQF_object_type.h"
#include "SQF_side_type.h"
#include "SQF_parse.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

void cb_cmdcnt_destroy(void* data)
{
	PCMD cmd = data;
	destroy_command(cmd);
}
PCMDCNT create_cmdcnt(void)
{
	PCMDCNT cmdcnt = malloc(sizeof(CMDCNT));
	cmdcnt->types = sm_create_list(10, 10, 10);
	cmdcnt->nullar = sm_create_list(10, 10, 10);
	cmdcnt->unary = sm_create_list(10, 10, 10);
	cmdcnt->binary = sm_create_list(10, 10, 10);
	return cmdcnt;
}
void destroy_cmdcnt(PCMDCNT cmdcnt)
{
	sm_destroy_list(cmdcnt->types, cb_cmdcnt_destroy);
	sm_destroy_list(cmdcnt->nullar, cb_cmdcnt_destroy);
	sm_destroy_list(cmdcnt->unary, cb_cmdcnt_destroy);
	sm_destroy_list(cmdcnt->binary, cb_cmdcnt_destroy);
	free(cmdcnt);
}

PCMDCNT GET_PCMDCNT(void)
{
	static PCMDCNT pcmdcnt = 0;
	if (pcmdcnt == 0)
	{
		pcmdcnt = create_cmdcnt();
	}
	return pcmdcnt;
}

void orig_error(PVM vm, const char* errMsg, PSTACK stack)
{
	printf("ERROR: %s\n", errMsg);
	getchar();
	exit(-1);
}
PSTACK create_stack(unsigned int size, unsigned char allow_dbg)
{
	PSTACK stack = malloc(sizeof(STACK));
	if (size == 0)
	{
		stack->data = 0;
	}
	else
	{
		stack->data = malloc(sizeof(PINST) * size);
		memset(stack->data, 0, sizeof(PINST) * size);
	}
	stack->size = size;
	stack->top = 0;
	stack->allow_dbg = allow_dbg;
	return stack;
}
void destroy_stack(PSTACK stack)
{
	int i;
	for (i = 0; i < stack->top; i++)
	{
		inst_destroy(stack->data[i]);
	}
	free(stack->data);
	free(stack);
}
void resize_stack(PVM vm, PSTACK stack, unsigned int newsize)
{
	if (stack->size == newsize)
		return;
	int i;
	PINST* ptr;
	for (i = newsize; i < stack->top; i++)
	{
		inst_destroy(stack->data[i]);
	}
	ptr = realloc(stack->data, sizeof(PINST) * newsize);
	if (ptr == 0)
	{
		vm->error(vm, "RESIZE OF STACK FAILED", stack);
	}
	else
	{
		stack->data = ptr;
		stack->size = newsize;
	}
}
void push_stack(PVM vm, PSTACK stack, PINST inst)
{
	if (stack->top >= stack->size)
	{
		vm->error(vm, "STACK OVERFLOW", stack);
	}
	else if (inst != 0 && inst->type == INST_DEBUG_INFO && !stack->allow_dbg)
	{
		inst_destroy(inst);
	}
	else
	{
		stack->data[stack->top++] = inst;
	}
}
PINST pop_stack(PVM vm, PSTACK stack)
{
	if (stack->top == 0)
	{
		vm->error(vm, "STACK UNDERFLOW", stack);
		return 0;
	}
	else
	{
		return stack->data[--stack->top];
	}
}
void insert_stack(PVM vm, PSTACK stack, PINST inst, int offset)
{
	PINST tmp;
	if (stack->top >= stack->size)
	{
		vm->error(vm, "STACK OVERFLOW", stack);
	}
	else if (offset > 0)
	{
		vm->error(vm, "CANNOT PUSH USING INSERT_STACK", stack);
	}
	else if (inst != 0 && inst->type == INST_DEBUG_INFO && !stack->allow_dbg)
	{
		inst_destroy(inst);
	}
	else
	{
		for (; offset < 0; offset++)
		{
			tmp = stack->data[stack->top + offset];
			stack->data[stack->top + offset] = inst;
			inst = tmp;
		}
		stack->data[stack->top++] = inst;
	}
}
PINST copy_inst(PVM vm, const PINST instIn)
{
	PVALUE val;
	PPOPEVAL ppopeval;
	switch (instIn->type)
	{
	case INST_NOP:
		return inst_nop();
	case INST_COMMAND:
		return inst_command(get_command(vm, vm->stack, instIn));
	case INST_VALUE:
		val = get_value(vm, vm->stack, instIn);
		if (val->type == ARRAY_TYPE())
		{
			return inst_value(value(val->type, base_voidptr(array_create())));
		}
		else
		{
			return inst_value(value(val->type, val->val));
		}
	case INST_LOAD_VAR:
		return inst_load_var(get_var_name(vm, vm->stack, instIn));
	case INST_STORE_VAR:
		return inst_store_var(get_var_name(vm, vm->stack, instIn));
	case INST_SCOPE:
		return inst_scope(get_scope(vm, vm->stack, instIn)->name);
	case INST_STORE_VAR_LOCAL:
		return inst_store_var_local(get_var_name(vm, vm->stack, instIn));
	case INST_ARR_PUSH:
		return inst_arr_push();
	case INST_CODE_LOAD:
		return inst_code_load(instIn->data.c);
	case INST_POP_EVAL:
		ppopeval = get_pop_eval(vm, vm->stack, instIn);
		return inst_pop_eval(ppopeval->ammount, ppopeval->popon);
	case INST_CLEAR_WORK:
		return inst_clear_work();
	case INST_DEBUG_INFO:
		return inst_debug_info2(get_dbginf(vm, vm->stack, instIn));
	case INST_MOVE:
		return inst_move(instIn->data.i);
	default:
#if _WIN32
		__asm int 3;
#else
		return 0;
#endif
		break;
	}
}
void copy_into_stack(PVM vm, PSTACK target, const PSTACK source)
{
	int i;
	PINST inst;
	for (i = 0; i < source->top; i++)
	{
		inst = source->data[i];
		push_stack(vm, target, copy_inst(vm, inst));
	}
}

PVM sqfvm(unsigned int stack_size, unsigned int work_size, unsigned char allow_dbg, unsigned long max_instructions)
{
	PVM vm = malloc(sizeof(VM));
	vm->stack = create_stack(stack_size, allow_dbg);
	vm->work = create_stack(work_size, 0);

	vm->cmd_container = GET_PCMDCNT();
	vm->error = orig_error;
	vm->die_flag = 0;
	vm->enable_instruction_limit = max_instructions == 0 ? 0 : 1;
	vm->max_instructions = max_instructions;
	vm->print = sqfvm_print;
	vm->print_custom_data = 0;
	vm->sidemap = side_init_sidemap();


	if (find_command(vm, COUNT_TYPE()->name, 't') == 0) register_command(vm, COUNT_TYPE());

	if (find_command(vm, SCALAR_TYPE()->name, 't') == 0) register_command(vm, SCALAR_TYPE());
	if (find_command(vm, BOOL_TYPE()->name, 't') == 0) register_command(vm, BOOL_TYPE());
	if (find_command(vm, ARRAY_TYPE()->name, 't') == 0) register_command(vm, ARRAY_TYPE());
	if (find_command(vm, STRING_TYPE()->name, 't') == 0) register_command(vm, STRING_TYPE());
	if (find_command(vm, NOTHING_TYPE()->name, 't') == 0) register_command(vm, NOTHING_TYPE());
	if (find_command(vm, ANY_TYPE()->name, 't') == 0) register_command(vm, ANY_TYPE());
	if (find_command(vm, NAMESPACE_TYPE()->name, 't') == 0) register_command(vm, NAMESPACE_TYPE());
	if (find_command(vm, NAN_TYPE()->name, 't') == 0) register_command(vm, NAN_TYPE());
	if (find_command(vm, IF_TYPE()->name, 't') == 0) register_command(vm, IF_TYPE());
	if (find_command(vm, WHILE_TYPE()->name, 't') == 0) register_command(vm, WHILE_TYPE());
	if (find_command(vm, FOR_TYPE()->name, 't') == 0) register_command(vm, FOR_TYPE());
	if (find_command(vm, SWITCH_TYPE()->name, 't') == 0) register_command(vm, SWITCH_TYPE());
	//register_command(vm, create_command("EXCEPTION", 't', 0, 0));
	//register_command(vm, create_command("WITH", 't', 0, 0));
	if (find_command(vm, CODE_TYPE()->name, 't') == 0) register_command(vm, CODE_TYPE());
	if (find_command(vm, OBJECT_TYPE()->name, 't') == 0) register_command(vm, OBJECT_TYPE());
	//register_command(vm, create_command("VECTOR", 't', 0, 0));
	//register_command(vm, create_command("TRANS", 't', 0, 0));
	//register_command(vm, create_command("ORIENT", 't', 0, 0));
	if (find_command(vm, SIDE_TYPE()->name, 't') == 0) register_command(vm, SIDE_TYPE());
	//register_command(vm, create_command("GROUP", 't', 0, 0));
	//register_command(vm, create_command("TEXT", 't', 0, 0));
	//register_command(vm, create_command("SCRIPT", 't', 0, 0));
	//register_command(vm, create_command("TARGET", 't', 0, 0));
	//register_command(vm, create_command("JCLASS", 't', 0, 0));
	//register_command(vm, create_command("CONFIG", 't', 0, 0));
	//register_command(vm, create_command("DISPLAY", 't', 0, 0));
	//register_command(vm, create_command("CONTROL", 't', 0, 0));
	//register_command(vm, create_command("NetObject", 't', 0, 0));
	//register_command(vm, create_command("SUBGROUP", 't', 0, 0));
	//register_command(vm, create_command("TEAM_MEMBER", 't', 0, 0));
	//register_command(vm, create_command("TASK", 't', 0, 0));
	//register_command(vm, create_command("DIARY_RECORD", 't', 0, 0));
	//register_command(vm, create_command("LOCATION", 't', 0, 0));
	return vm;
}
void destroy_sqfvm(PVM vm)
{
	side_destroy_sidemap(vm->sidemap);
	destroy_stack(vm->stack);
	destroy_stack(vm->work);
	free(vm);
}

void register_command(PVM vm, PCMD cmd)
{
	sm_list* list = 0;
	switch (cmd->type)
	{
		case 't':
			list = vm->cmd_container->types;
			break;
		case 'n':
			list = vm->cmd_container->nullar;
			break;
		case 'u':
			list = vm->cmd_container->unary;
			break;
		case 'b':
			list = vm->cmd_container->binary;
			break;
		default:
			vm->error(vm, "UNKNOWN COMMAND TYPE", vm->stack);
			break;
	}
	if (list == 0)
		return;
	if (sm_get_value(list, cmd->name) == 0)
	{
		cmd = sm_set_value(list, cmd->name, cmd);
	}
	else
	{
		vm->error(vm, "COMMAND ALREADY EXISTING", vm->stack);
	}
}
int sqfvm_print(PVM vm, const char* format, ...)
{
	va_list args;
	int res;
	va_start(args, format);

	res = vprintf(format, args);

	va_end(args);
	return res;
}
