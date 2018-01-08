#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>

#include "sqffull.h"

PVM sqfvm(unsigned int stack_size, unsigned int work_size, bool allow_dbg, unsigned long max_instructions)
{
	PVM vm = malloc(sizeof(VM));
	vm->stack = create_stack(stack_size, allow_dbg);
	vm->work = create_stack(work_size, 0);

	vm->cmd_container = GET_PCMDCNT();
	vm->error = orig_error;
	vm->warn = orig_warn;
	vm->die_flag = 0;
	vm->enable_instruction_limit = max_instructions == 0 ? 0 : 1;
	vm->max_instructions = max_instructions;
	vm->print = sqfvm_print;
	vm->print_custom_data = 0;
	vm->sidemap = side_init_sidemap();
	vm->instcount = 0;

	vm->scripts = malloc(sizeof(PSCRIPT) * 10);
	vm->scripts_size = 10;
	vm->scripts_top = 0;


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
	if (find_command(vm, WITH_TYPE()->name, 't') == 0) register_command(vm, WITH_TYPE());
	if (find_command(vm, CODE_TYPE()->name, 't') == 0) register_command(vm, CODE_TYPE());
	if (find_command(vm, OBJECT_TYPE()->name, 't') == 0) register_command(vm, OBJECT_TYPE());
	//register_command(vm, create_command("VECTOR", 't', 0, 0));
	//register_command(vm, create_command("TRANS", 't', 0, 0));
	//register_command(vm, create_command("ORIENT", 't', 0, 0));
	if (find_command(vm, SIDE_TYPE()->name, 't') == 0) register_command(vm, SIDE_TYPE());
	if (find_command(vm, GROUP_TYPE()->name, 't') == 0) register_command(vm, GROUP_TYPE());
	//register_command(vm, create_command("TEXT", 't', 0, 0));
	if (find_command(vm, SCRIPT_TYPE()->name, 't') == 0) register_command(vm, SCRIPT_TYPE());
	//register_command(vm, create_command("TARGET", 't', 0, 0));
	//register_command(vm, create_command("JCLASS", 't', 0, 0));
	if (find_command(vm, CONFIG_TYPE()->name, 't') == 0) register_command(vm, CONFIG_TYPE());
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
	int i;
	for (i = 0; i < (int)vm->scripts_top; i++)
	{
		if (vm->scripts[i] != 0)
		{
			script_destroy(vm->scripts[i]);
		}
	}
	free(vm->scripts);
	side_destroy_sidemap(vm->sidemap);
	destroy_stack(vm->stack);
	destroy_stack(vm->work);
	free(vm);
}

void sqfvm_pushscript(PVM vm, PSCRIPT script)
{
	if (vm->scripts_top == vm->scripts_size)
	{
		vm->scripts_size += SQFVM_SCRIPTS_GROWTH;
		vm->scripts = realloc(vm->scripts, sizeof(PSCRIPT) * vm->scripts_size);
	}
	vm->scripts[vm->scripts_top++] = script;
	value(SCRIPT_TYPE(), base_voidptr(script));
}
void sqfvm_dropscript(PVM vm, PSCRIPT script)
{
	int i;
	bool was_found = false;
	for (i = 0; i < (int)vm->scripts_top; i++)
	{
		if (was_found)
		{
			vm->scripts[i - 1] = vm->scripts[i];
		}
		else if(vm->scripts[i] == script)
		{
			inst_destroy_value(value_create_noref(SCRIPT_TYPE(), base_voidptr(script)));
			was_found = true;
			script = 0;
			vm->scripts[i] = 0;
		}
	}
	vm->scripts_top--;
}


PCMDCNT create_cmdcnt(void)
{
	PCMDCNT cmdcnt = malloc(sizeof(CMDCNT));
	cmdcnt->types = wsm_create_list(5, 10, 10);
	cmdcnt->nullar = wsm_create_list(25, 10, 10);
	cmdcnt->unary = wsm_create_list(25, 10, 10);
	cmdcnt->binary = wsm_create_list(25, 10, 10);
	return cmdcnt;
}

void destroy_command_void(void* d) { destroy_command(d); }
void destroy_cmdcnt(PCMDCNT cmdcnt)
{
	wsm_destroy_list(cmdcnt->types, destroy_command_void);
	wsm_destroy_list(cmdcnt->nullar, destroy_command_void);
	wsm_destroy_list(cmdcnt->unary, destroy_command_void);
	wsm_destroy_list(cmdcnt->binary, destroy_command_void);
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

void orig_error(PVM vm, const wchar_t* errMsg, PSTACK stack)
{
	wprintf(L"ERROR: %ls\n", errMsg);
	getchar();
	exit(-1);
}
void orig_warn(PVM vm, const wchar_t* errMsg, PSTACK stack)
{
	wprintf(L"WARNING: %ls\n", errMsg);
}
int sqfvm_print(PVM vm, const wchar_t* format, ...)
{
	va_list args;
	int res;
	va_start(args, format);

	res = vwprintf(format, args);

	va_end(args);
	return res;
}

PSTACK create_stack(unsigned int size, bool allow_dbg)
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
	for (i = 0; i < (int)stack->top; i++)
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
	for (i = newsize; i < (int)stack->top; i++)
	{
		inst_destroy(stack->data[i]);
	}
	ptr = realloc(stack->data, sizeof(PINST) * newsize);
	if (ptr == 0)
	{
		vm->error(vm, L"RESIZE OF STACK FAILED", vm->stack);
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
		vm->error(vm, L"STACK OVERFLOW", vm->stack);
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
		vm->error(vm, L"STACK UNDERFLOW", vm->stack);
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
		vm->error(vm, L"STACK OVERFLOW", vm->stack);
	}
	else if (offset > 0)
	{
		vm->error(vm, L"CANNOT PUSH USING INSERT_STACK", vm->stack);
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
#if defined(_WIN32) & !defined(__GNUC__)
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
	for (i = 0; i < (int)source->top; i++)
	{
		inst = source->data[i];
		push_stack(vm, target, copy_inst(vm, inst));
	}
}


void register_command(PVM vm, PCMD cmd)
{
	wsm_list* list = 0;
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
			vm->error(vm, L"UNKNOWN COMMAND TYPE", vm->stack);
			break;
	}
	if (list == 0)
		return;
	if (wsm_get_value(list, cmd->name) == 0)
	{
		cmd = wsm_set_value(list, cmd->name, cmd);
	}
	else
	{
		vm->error(vm, L"COMMAND ALREADY EXISTING", vm->stack);
	}
}
