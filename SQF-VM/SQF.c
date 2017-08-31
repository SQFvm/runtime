#include "string_map.h"
#include "string_op.h"
#include "textrange.h"
#include "SQF.h"
#include "SQF_types.h"
#include "SQF_parse.h"
#include "errors.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



void push_stack(PVM vm, PSTACK stack, PINST inst)
{
	if (stack->top >= stack->size)
	{
		vm->error("STACK OVERFLOW", stack);
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
		vm->error("STACK UNDERFLOW", stack);
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
		vm->error("STACK OVERFLOW", stack);
	}
	else if (offset > 0)
	{
		vm->error("CANNOT PUSH USING INSERT_STACK", stack);
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
extern inline void register_command(PVM vm, PCMD cmd);

#define SQF_VM_INTERNAL_TYPE_COUNT 11
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
	vm->error = error;
	vm->die_flag = 0;
	vm->enable_instruction_limit = max_instructions == 0 ? 0 : 1;
	vm->max_instructions = max_instructions;


	register_command(vm, SCALAR_TYPE());
	register_command(vm, BOOL_TYPE());
	register_command(vm, ARRAY_TYPE());
	register_command(vm, CODE_TYPE());
	register_command(vm, STRING_TYPE());
	register_command(vm, NOTHING_TYPE());
	register_command(vm, ANY_TYPE());

	register_command(vm, NAMESPACE_TYPE());

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
	//6 being the "offset" of the pre-existing types (see sqfvm creation function, this number should correspond to the ammount of register_command calls in there)
	for (i = SQF_VM_INTERNAL_TYPE_COUNT; i < vm->cmds_top; i++)
	{
		destroy_command(vm->cmds[i]);
	}
	free(vm->cmds);

	free(vm);
}

PSTACK create_stack(unsigned int size, unsigned char allow_dbg)
{
	PSTACK stack = malloc(sizeof(STACK));
	stack->data = malloc(sizeof(PINST) * size);
	memset(stack->data, 0, sizeof(PINST) * size);
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
void resize_stack(PSTACK stack, unsigned int newsize)
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
		error("RESIZE OF STACK FAILED", stack);
	}
	else
	{
		stack->data = ptr;
		stack->size = newsize;
	}
}

PCMD create_command(const char* name, char type, CMD_CB fnc, char precedence, const char* desc)
{
	PCMD command = malloc(sizeof(CMD));
	int len = strlen(name);
	command->name = malloc(sizeof(char) * (len + 1));
	command->name_len = len;
	strcpy(command->name, name);
	command->precedence_level = precedence;
	command->type = type;
	command->type_code = type == 't' ? 1 : type == 'b' ? 2 : type == 'u' ? 4 : 8;
	command->callback = fnc;
	if (desc != 0)
	{
		len = strlen(desc);
		command->description = malloc(sizeof(char) * (len + 1));
		command->description_len = len;
		strcpy(command->description, desc);
	}
	else
	{
		command->description = 0;
		command->description_len = 0;
	}
	return command;
}
void destroy_command(PCMD command)
{
	free(command->name);
	if (command->description != 0)
	{
		free(command->description);
	}
	free(command);
}

PVALUE find_var(PVM vm, const char* name)
{
	int i, j;
	PSCOPE scope;
	for (i = vm->stack->top; i >= 0; i--)
	{
		if (vm->stack->data[i]->type == INST_SCOPE)
		{
			scope = get_scope(vm, vm->stack, vm->stack->data[i]);
			for (j = 0; j < scope->varstack_top; j++)
			{
				if (str_cmpi(scope->varstack_name[j], -1, name, -1) == 0)
				{
					return scope->varstack_value[j];
				}
			}
		}
	}
	return 0;
}
void set_var(PVM vm, const char* name, VALUE val)
{
	int i, j;
	PSCOPE first = 0;
	PSCOPE scope;
	PVALUE tmp;
	for (i = vm->stack->top; i >= 0; i--)
	{
		if (vm->stack->data[i]->type == INST_SCOPE)
		{
			scope = get_scope(vm, vm->stack, vm->stack->data[i]);
			if (first == 0)
			{
				first = scope;
			}
			for (j = 0; j < scope->varstack_top; j++)
			{
				if (str_cmpi(scope->varstack_name[j], -1, name, -1) == 0)
				{
					tmp = scope->varstack_value[j];
					scope->varstack_value[j] = malloc(sizeof(VALUE));
					scope->varstack_value[j]->type = val.type;
					scope->varstack_value[j]->val = val.val;
					inst_destroy_value(tmp);
					return;
				}
			}
		}
	}
	store_in_scope(vm, first, name, val);
}
PSCOPE top_scope(PVM vm)
{
	int i;
	for (i = vm->stack->top; i >= 0; i--)
	{
		if (vm->stack->data[i]->type == INST_SCOPE)
		{
			return get_scope(vm, vm->stack, vm->stack->data[i]);
		}
	}
	return 0;
}
void store_in_scope(PVM vm, PSCOPE scope, const char* name, VALUE val)
{
	int i;
	for (i = 0; i < scope->varstack_top; i++)
	{
		if (str_cmpi(scope->varstack_name[i], -1, name, -1) == 0)
		{
			inst_destroy_value(scope->varstack_value[i]);
			scope->varstack_value[i] = malloc(sizeof(VALUE));
			scope->varstack_value[i]->type = val.type;
			scope->varstack_value[i]->val = val.val;
			return;
		}
	}
	push_in_scope(vm, scope, name, val);
}
void push_in_scope(PVM vm, PSCOPE scope, const char* name, VALUE val)
{
	int len;
	if (scope->varstack_top >= scope->varstack_size)
	{
		error("VARSTACK OVERFLOW", vm->stack);
	}
	else
	{
		len = strlen(name);
		scope->varstack_name[scope->varstack_top] = malloc(sizeof(char) * (len + 1));
		strcpy(scope->varstack_name[scope->varstack_top], name);

		scope->varstack_value[scope->varstack_top] = malloc(sizeof(VALUE));
		scope->varstack_value[scope->varstack_top]->type = val.type;
		scope->varstack_value[scope->varstack_top]->val = val.val;

		scope->varstack_top++;
	}
}

PCMD find_command(PVM vm, const char* name, char type)
{
	int i;
	PCMD cmd;
	if (type != '*')
	{
		for (i = 0; i < vm->cmds_top; i++)
		{
			cmd = vm->cmds[i];
			if (cmd->type == type && str_cmpi(cmd->name, -1, name, -1) == 0)
			{
				return cmd;
			}
		}
	}
	else
	{
		for (i = 0; i < vm->cmds_top; i++)
		{
			cmd = vm->cmds[i];
			if (str_cmpi(cmd->name, -1, name, -1) == 0)
			{
				return cmd;
			}
		}
	}
	return 0;
}
PCMD find_type(PVM vm, const char* name)
{
	int i;
	PCMD cmd;
	for (i = 0; i < vm->cmds_top; i++)
	{
		cmd = vm->cmds[i];
		if (str_cmpi(cmd->name, -1, name, -1) == 0)
		{
			return cmd;
		}
		else if (cmd->type != 't')
		{
			break;
		}
	}
	return 0;
}

void error(const char* errMsg, PSTACK stack)
{
	printf("ERROR: %s\n", errMsg);
	getchar();
	exit(-1);
}

void execute(PVM vm)
{
	PINST inst;
	PINST inst2;
	PVALUE val;
	PVALUE val2;
	PSCOPE scope;
	int i, j;
	unsigned long inst_executed = 0;
	while (vm->stack->top > 0)
	{
		if (vm->enable_instruction_limit && inst_executed >= vm->max_instructions && !vm->die_flag)
		{
			vm->error("MAX ALLOWED INSTRUCTION COUNT REACHED (10000)", vm->stack);
			vm->die_flag = 1;
		}
		inst = pop_stack(vm, vm->stack);
		if (vm->die_flag)
		{
			inst_destroy(inst);
			continue;
		}
		switch (inst->type)
		{
		case INST_NOP:
			inst_destroy(inst);
			break;
		case INST_COMMAND:
			get_command(vm, vm->stack, inst)->callback(vm, get_command(vm, vm->stack, inst));
			inst_destroy(inst);
			break;
		case INST_SCOPE:
			inst_destroy(inst);
			break;
		case INST_LOAD_VAR:
			if (get_var_name(vm, vm->stack, inst)[0] == '_')
			{
				val = find_var(vm, get_var_name(vm, vm->stack, inst));
				if (val == 0)
				{
					push_stack(vm, vm->work, inst_value(value(NOTHING_TYPE(), base_int(0))));
				}
				else
				{
					push_stack(vm, vm->work, inst_value(value(val->type, val->val)));
				}
			}
			else
			{
				scope = top_scope(vm);
				val = namespace_get_var(scope->ns, get_var_name(vm, vm->stack, inst));
				if (val == 0)
				{
					push_stack(vm, vm->work, inst_value(value(NOTHING_TYPE(), base_int(0))));
				}
				else
				{
					push_stack(vm, vm->work, inst_value(value(val->type, val->val)));
				}
			}
			inst_destroy(inst);
			break;
		case INST_STORE_VAR_LOCAL:
			scope = top_scope(vm);
			inst2 = pop_stack(vm, vm->work);
			val = get_value(vm, vm->stack, inst2);
			if (get_var_name(vm, vm->stack, inst)[0] != '_')
			{
				vm->error("CANNOT PRIVATE GLOBAL VARIABLES", vm->stack);
				push_stack(vm, vm->work, inst2);
			}
			else
			{
				store_in_scope(vm, scope, get_var_name(vm, vm->stack, inst), value(val->type, val->val));
				inst_destroy(inst2);
				inst_destroy(inst);
				break;
			}
		case INST_STORE_VAR:
			inst2 = pop_stack(vm, vm->work);
			if (get_var_name(vm, vm->stack, inst)[0] == '_')
			{
				val = find_var(vm, get_var_name(vm, vm->stack, inst));
				if (val == 0)
				{
					scope = top_scope(vm);
					val = get_value(vm, vm->stack, inst2);
					store_in_scope(vm, scope, get_var_name(vm, vm->stack, inst), value(val->type, val->val));
				}
				else
				{
					val2 = get_value(vm, vm->stack, inst2);
					set_var(vm, get_var_name(vm, vm->stack, inst), value(val2->type, val2->val));
				}
			}
			else
			{
				scope = top_scope(vm);
				val = get_value(vm, vm->stack, inst2);
				namespace_set_var(scope->ns, get_var_name(vm, vm->stack, inst), value(val->type, val->val));
			}

			inst_destroy(inst2);
			inst_destroy(inst);
			break;
		case INST_VALUE:
			push_stack(vm, vm->work, inst);
			break;
		case INST_ARR_PUSH:
			inst_destroy(inst);
			inst2 = pop_stack(vm, vm->work);
			inst = pop_stack(vm, vm->work);
			val = get_value(vm, vm->stack, inst2);
			val2 = get_value(vm, vm->stack, inst);
			if (val == 0 || val2 == 0 || val2->type != ARRAY_TYPE())
			{
				vm->error("INST_ARRAY_PUSH FAILED TO FIND ARRAY", vm->stack);
				vm->die_flag = 1;
			}
			else
			{
				array_push(((PARRAY)val2->val.ptr), value(val->type, val->val));
			}
			push_stack(vm, vm->work, inst);
			inst_destroy(inst2);
			break;
		case INST_CODE_LOAD:
			i = inst->data.c ? 0 : 1;
			inst_destroy(inst);
			inst = pop_stack(vm, vm->work);
			val = get_value(vm, vm->stack, inst);

			i = -(vm->work->top);
			while (vm->work->top != 0)
			{
				push_stack(vm, vm->stack, pop_stack(vm, vm->work));
			}
			if (i != 0)
			{
				push_stack(vm, vm->stack, inst_move(i));
			}

			parse(vm, ((PCODE)val->val.ptr)->val, i);
			inst_destroy(inst);
			break;
		case INST_POP_EVAL:
			i = get_pop_eval(vm, vm->stack, inst)->popon ? 1 : 0;
			j = get_pop_eval(vm, vm->stack, inst)->ammount;
			inst_destroy(inst);
			inst = pop_stack(vm, vm->work);
			if (inst == 0)
			{
				for (; j >= 0; j--)
				{
					inst_destroy(pop_stack(vm, vm->stack));
				}
			}
			val = get_value(vm, vm->stack, inst);
			if (val == 0 || (val->val.i > 0 && i) || (val->val.i == 0 && !i))
			{
				for (; j >= 0; j--)
				{
					inst_destroy(pop_stack(vm, vm->stack));
				}
			}
			break;
		case INST_CLEAR_WORK:
			while (vm->work->top > 0)
			{
				inst_destroy(pop_stack(vm, vm->work));
			}
			inst_destroy(inst);
			break;
		case INST_DEBUG_INFO:
			inst_destroy(inst);
			break;
		case INST_MOVE:
			inst2 = pop_stack(vm, vm->work);
			insert_stack(vm, vm->stack, inst2, inst->data.i);
			inst_destroy(inst);
			break;
		}
		inst_executed++;
	}
}
