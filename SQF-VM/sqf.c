#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <wchar.h>
#include <wctype.h>
#include <stdbool.h>
#include <stdint.h>

#include "wstring_op.h"
#include "wstring_map.h"
#include "sqffull.h"



PCMD create_command(const wchar_t* name, char type, CMD_CB fnc, char precedence, const wchar_t* usage, const wchar_t* examples, const wchar_t* desc)
{
	PCMD command = malloc(sizeof(CMD));
	int len;
	len = wcslen(name);
	command->name = malloc(sizeof(wchar_t) * (len + 1));
	command->name_len = len;
	wcscpy(command->name, name);
	command->precedence_level = precedence;
	command->type = type;
	command->type_code = type == 't' ? 1 : type == 'b' ? 2 : type == 'u' ? 4 : 8;
	command->callback = fnc;
	if (desc != 0)
	{
		len = wcslen(desc);
		command->description = malloc(sizeof(wchar_t) * (len + 1));
		command->description_len = len;
		wcscpy(command->description, desc);
	}
	else
	{
		command->description = 0;
		command->description_len = 0;
	}
	if (usage != 0)
	{
		len = wcslen(usage);
		command->usage = malloc(sizeof(wchar_t) * (len + 1));
		command->usage_len = len;
		wcscpy(command->usage, usage);
	}
	else
	{
		command->usage = 0;
		command->usage_len = 0;
	}
	if (examples != 0)
	{
		len = wcslen(examples);
		command->examples = malloc(sizeof(wchar_t) * (len + 1));
		command->examples_len = len;
		wcscpy(command->examples, examples);
	}
	else
	{
		command->examples = 0;
		command->examples_len = 0;
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
	if (command->usage != 0)
	{
		free(command->usage);
	}
	if (command->examples != 0)
	{
		free(command->examples);
	}
	free(command);
}

PVALUE find_var(PVM vm, const wchar_t* name)
{
	int i, j;
	PSCOPE scope;
	if (name == 0)
		return 0;
	for (i = vm->stack->top; i >= 0; i--)
	{
		if (vm->stack->data[i]->type == INST_SCOPE)
		{
			scope = get_scope(vm, vm->stack, vm->stack->data[i]);
			for (j = 0; j < scope->varstack_top; j++)
			{
				if (wstr_cmpi(scope->varstack_name[j], -1, name, -1) == 0)
				{
					return scope->varstack_value[j];
				}
			}
		}
	}
	return 0;
}
void set_var(PVM vm, const wchar_t* name, VALUE val)
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
				if (wstr_cmpi(scope->varstack_name[j], -1, name, -1) == 0)
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
void store_in_scope(PVM vm, PSCOPE scope, const wchar_t* name, VALUE val)
{
	int i;
	for (i = 0; i < scope->varstack_top; i++)
	{
		if (wstr_cmpi(scope->varstack_name[i], -1, name, -1) == 0)
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
void push_in_scope(PVM vm, PSCOPE scope, const wchar_t* name, VALUE val)
{
	int len;
	if (scope->varstack_top >= scope->varstack_size)
	{
		vm->error(vm, L"VARSTACK OVERFLOW", vm->stack);
	}
	else
	{
		len = wcslen(name);
		scope->varstack_name[scope->varstack_top] = malloc(sizeof(wchar_t) * (len + 1));
		wcscpy(scope->varstack_name[scope->varstack_top], name);

		scope->varstack_value[scope->varstack_top] = malloc(sizeof(VALUE));
		scope->varstack_value[scope->varstack_top]->type = val.type;
		scope->varstack_value[scope->varstack_top]->val = val.val;

		scope->varstack_top++;
	}
}

PCMD find_command(PVM vm, const wchar_t* name, char type)
{
	PCMD cmd;

	switch (type)
	{
		case 't':
			return wsm_get_value(vm->cmd_container->types, name);
		case 'n':
			return wsm_get_value(vm->cmd_container->nullar, name);
		case 'u':
			return wsm_get_value(vm->cmd_container->unary, name);
		case 'b':
			return wsm_get_value(vm->cmd_container->binary, name);
		case '*':
			cmd = wsm_get_value(vm->cmd_container->types, name);
			if (cmd != 0)
				return cmd;
			cmd = wsm_get_value(vm->cmd_container->nullar, name);
			if (cmd != 0)
				return cmd;
			cmd = wsm_get_value(vm->cmd_container->unary, name);
			if (cmd != 0)
				return cmd;
			cmd = wsm_get_value(vm->cmd_container->binary, name);
			return cmd;
		case 'c':
			cmd = wsm_get_value(vm->cmd_container->nullar, name);
			if (cmd != 0)
				return cmd;
			cmd = wsm_get_value(vm->cmd_container->unary, name);
			if (cmd != 0)
				return cmd;
			cmd = wsm_get_value(vm->cmd_container->binary, name);
			return cmd;
		case 'C':
			cmd = wsm_get_value(vm->cmd_container->binary, name);
			if (cmd != 0)
				return cmd;
			cmd = wsm_get_value(vm->cmd_container->unary, name);
			if (cmd != 0)
				return cmd;
			cmd = wsm_get_value(vm->cmd_container->nullar, name);
			return cmd;
		default:
			vm->error(vm, L"UNKOWN FILTER TYPE", vm->stack);
	}
	return 0;
}
PCMD find_type(PVM vm, const wchar_t* name)
{
	return find_command(vm, name, 't');
}

void runvm(PVM vm)
{
	PSTACK tmpstack = vm->stack;
	PSTACK tmpwork = vm->work;
	PSCRIPT script;
	int i;
	vm->is_suspending_environment = 0;
	execute(vm, -1);
	while (vm->scripts_top > 0 && !vm->die_flag)
	{
		for (i = 0; i < vm->scripts_top && !vm->die_flag; i++)
		{
			script = vm->scripts[i];
			vm->stack = script->stack;
			vm->work = script->work;
			vm->is_suspending_environment = 1;
			execute(vm, 100);
		}
		vm->stack = tmpstack;
		vm->work = tmpwork;
		for (i = 0; i < vm->scripts_top; i++)
		{
			script = vm->scripts[i];
			if (script->stack->top == 0)
			{
				sqfvm_dropscript(vm, script);
				i--;
			}
		}
	}
	while (vm->scripts_top > 0)
	{
		script = vm->scripts[0];
		sqfvm_dropscript(vm, script);
	}
}

void execute(PVM vm, int exitAfter)
{
	PINST inst;
	PINST inst2;
	PVALUE val;
	PVALUE val2;
	PSCOPE scope;
	int i;
	unsigned int ui;
	const wchar_t* str_const;
	while (vm->stack->top > 0)
	{
		if (exitAfter != -1 && exitAfter-- == 0)
		{
			return;
		}
		if (vm->enable_instruction_limit && vm->instcount >= vm->max_instructions
			&& !vm->die_flag)
		{
			vm->error(vm, L"MAX ALLOWED INSTRUCTION COUNT REACHED", vm->stack);
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
			get_command(vm, vm->stack, inst)->callback(vm,
				get_command(vm, vm->stack, inst));
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
					push_stack(vm, vm->work,
						inst_value(value(NOTHING_TYPE(), base_int(0))));
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
					push_stack(vm, vm->work,
						inst_value(value(NOTHING_TYPE(), base_int(0))));
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
			if (inst2 == 0)
				break;
			val = get_value(vm, vm->stack, inst2);
			if (get_var_name(vm, vm->stack, inst)[0] != '_')
			{
				vm->error(vm, L"CANNOT PRIVATE GLOBAL VARIABLES", vm->stack);
				push_stack(vm, vm->work, inst2);
			}
			else
			{
				store_in_scope(vm, scope, get_var_name(vm, vm->stack, inst),
					value(val->type, val->val));
				inst_destroy(inst2);
				inst_destroy(inst);
				break;
			}
		case INST_STORE_VAR:
			inst2 = pop_stack(vm, vm->work);
			if (inst2 == 0)
				break;
			if (get_var_name(vm, vm->stack, inst)[0] == '_')
			{
				val = find_var(vm, get_var_name(vm, vm->stack, inst));
				if (val == 0)
				{
					scope = top_scope(vm);
					val = get_value(vm, vm->stack, inst2);
					if (val == 0)
					{
						vm->error(vm, L"NO VALUE GIVEN", vm->stack);
					}
					else
					{
						store_in_scope(vm, scope, get_var_name(vm, vm->stack, inst),
							value(val->type, val->val));
					}
				}
				else
				{
					val2 = get_value(vm, vm->stack, inst2);
					set_var(vm, get_var_name(vm, vm->stack, inst),
						value(val2->type, val2->val));
				}
			}
			else
			{
				scope = top_scope(vm);
				val = get_value(vm, vm->stack, inst2);
				namespace_set_var(scope->ns, get_var_name(vm, vm->stack, inst),
					value(val->type, val->val));
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
			if (inst == 0 || inst2 == 0)
				break;
			val = get_value(vm, vm->stack, inst2);
			val2 = get_value(vm, vm->stack, inst);
			if (val == 0 || val2 == 0 || val2->type != ARRAY_TYPE())
			{
				vm->error(vm, L"INST_ARRAY_PUSH FAILED TO FIND ARRAY", vm->stack);
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

			i = -(int)(vm->work->top);
			while (vm->work->top != 0)
			{
				push_stack(vm, vm->stack, pop_stack(vm, vm->work));
			}
			if (i != 0)
			{
				push_stack(vm, vm->stack, inst_move(i));
			}
			copy_into_stack(vm, vm->stack, ((PCODE)val->val.ptr)->stack);
			inst_destroy(inst);
			break;
		case INST_POP:
			ui = inst->data.ui;
			for (; ui != 0; ui--)
			{
				inst_destroy(pop_stack(vm, vm->stack));
			}
			inst_destroy(inst);
			break;
		case INST_POP_EVAL:
			i = get_pop_eval(vm, vm->stack, inst)->popon ? 1 : 0;
			ui = get_pop_eval(vm, vm->stack, inst)->ammount;
			inst_destroy(inst);
			inst = pop_stack(vm, vm->work);
			if (inst == 0)
			{
				for (; ui != 0; ui--)
				{
					inst_destroy(pop_stack(vm, vm->stack));
				}
			}
			else
			{
				val = get_value(vm, vm->stack, inst);
				if (val == 0 || val->type == NOTHING_TYPE()
					|| (val->type == BOOL_TYPE()
						&& ((val->val.i > 0 && i) || (val->val.i == 0 && !i))))
				{
					for (; ui != 0; ui--)
					{
						inst_destroy(pop_stack(vm, vm->stack));
					}
				}
			}
			inst_destroy(inst);
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
		case INST_SCOPE_DROPOUT:
			str_const = get_var_name(vm, vm->stack, inst);
			inst_destroy(inst);
			while (vm->stack > 0)
			{
				while ((inst = pop_stack(vm, vm->stack))->type != INST_SCOPE
					&& vm->stack->top > 0)
				{
					inst_destroy(inst);
				}
				if (str_const == 0 || !wstr_cmpi(get_scope(vm, vm->stack, inst)->name, -1, str_const, -1))
				{
					break;
				}
			}
			break;
		}
		vm->instcount++;
	}
}


