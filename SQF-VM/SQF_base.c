#include "basetype.h"
#include "string_map.h"
#include "string_op.h"
#include "textrange.h"
#include "SQF_base.h"
#include "SQF.h"
#include "SQF_types.h"
#include "SQF_parse.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>


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
void copy_into_stack(PVM vm, PSTACK target, const PSTACK source)
{
	int i;
	PINST inst;
	PVALUE val;
	PPOPEVAL ppopeval;
	PDBGINF dbginf;
	for (i = 0; i < source->top; i++)
	{
		inst = source->data[i];
		switch (inst->type)
		{
			case INST_NOP:
				push_stack(vm, target, inst_nop());
				break;
			case INST_COMMAND:
				push_stack(vm, target, inst_command(get_command(vm, vm->stack, inst)));
				break;
			case INST_VALUE:
				val = get_value(vm, vm->stack, inst);
				push_stack(vm, target, inst_value(value(val->type, val->val)));
				break;
			case INST_LOAD_VAR:
				push_stack(vm, target, inst_load_var(get_var_name(vm, vm->stack, inst)));
				break;
			case INST_STORE_VAR:
				push_stack(vm, target, inst_store_var(get_var_name(vm, vm->stack, inst)));
				break;
			case INST_SCOPE:
				push_stack(vm, target, inst_scope(get_scope(vm, vm->stack, inst)->name));
				break;
			case INST_STORE_VAR_LOCAL:
				push_stack(vm, target, inst_store_var_local(get_var_name(vm, vm->stack, inst)));
				break;
			case INST_ARR_PUSH:
				push_stack(vm, target, inst_arr_push());
				break;
			case INST_CODE_LOAD:
				push_stack(vm, target, inst_code_load(inst->data.c));
				break;
			case INST_POP_EVAL:
				ppopeval = get_pop_eval(vm, vm->stack, inst);
				push_stack(vm, target, inst_pop_eval(ppopeval->ammount, ppopeval->popon));
				break;
			case INST_CLEAR_WORK:
				push_stack(vm, target, inst_clear_work());
				break;
			case INST_DEBUG_INFO:
				dbginf = get_dbginf(vm, vm->stack, inst);
				push_stack(vm, target, inst_debug_info(dbginf->line, dbginf->col, dbginf->offset, dbginf->length));
				break;
			case INST_MOVE:
				push_stack(vm, target, inst_move(inst->data.i));
				break;
			default:
				#if _WIN32
				__asm int 3;
				#endif
				break;
		}
	}
}

#define SQF_VM_INTERNAL_TYPE_COUNT 12
PVM sqfvm(unsigned int stack_size, unsigned int work_size, unsigned int cmds_size, unsigned char allow_dbg, unsigned long max_instructions)
{
	PVM vm = malloc(sizeof(VM));
	cmds_size += SQF_VM_INTERNAL_TYPE_COUNT;
	vm->stack = create_stack(stack_size, allow_dbg);
	vm->work = create_stack(work_size, 0);

	vm->cmds = malloc(sizeof(PCMD) * cmds_size);
	memset(vm->cmds, 0, sizeof(PINST) * cmds_size);
	vm->cmds_size = cmds_size;
	vm->cmds_top = 0;
	vm->error = orig_error;
	vm->die_flag = 0;
	vm->enable_instruction_limit = max_instructions == 0 ? 0 : 1;
	vm->max_instructions = max_instructions;
	vm->print = sqfvm_print;
	vm->print_custom_data = 0;


	register_command(vm, SCALAR_TYPE());
	register_command(vm, BOOL_TYPE());
	register_command(vm, ARRAY_TYPE());
	register_command(vm, CODE_TYPE());
	register_command(vm, STRING_TYPE());
	register_command(vm, NOTHING_TYPE());
	register_command(vm, ANY_TYPE());
	register_command(vm, NAMESPACE_TYPE());
	register_command(vm, NAN_TYPE());
	register_command(vm, IF_TYPE());
	register_command(vm, WHILE_TYPE());
	register_command(vm, FOR_TYPE());
	return vm;
}
void destroy_sqfvm(PVM vm)
{
	int i;
	destroy_stack(vm->stack);
	destroy_stack(vm->work);
	for (i = SQF_VM_INTERNAL_TYPE_COUNT; i < vm->cmds_top; i++)
	{
		destroy_command(vm->cmds[i]);
	}
	free(vm->cmds);

	free(vm);
}

void register_command(PVM vm, PCMD cmd)
{
	if (vm->cmds_top >= vm->cmds_size)
	{
		vm->error(vm, "COMMAND REGISTER OVERFLOW", vm->stack);
	}
	else
	{
		if (cmd->type == 't' && vm->cmds_top != 0 && vm->cmds[vm->cmds_top - 1]->type != 't')
		{
			vm->error(vm, "TYPE COMMAND REGISTERED AFTER NON-TYPE COMMANDS GOT REGISTERED", vm->stack);
		}
		vm->cmds[vm->cmds_top++] = cmd;
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
