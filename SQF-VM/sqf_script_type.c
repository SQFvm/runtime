#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <stdbool.h>
#include <stdint.h>
#include "sqffull.h"



void TYPE_SCRIPT_CALLBACK(void* input, CPCMD self)
{
	PVALUE val = input;
	PSCRIPT script = val->val.ptr;
	if (val->type == 0)
	{
		script->refcount--;
		if (script->refcount <= 0)
		{
			script_destroy(script);
		}
	}
	else
	{
		script->refcount++;
	}
}
PCMD SCRIPT_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"SCRIPT", 't', TYPE_SCRIPT_CALLBACK, 0, NULL, NULL, NULL);
	}
	return cmd;
}
PSCRIPT script_create(PVM vm)
{
	PSCRIPT script = malloc(sizeof(SCRIPT));
	script->refcount = 0;
	script->stack = create_stack(vm->stack->size, vm->stack->allow_dbg);
	script->work = create_stack(vm->work->size, vm->work->allow_dbg);
	return script;
}
void script_destroy(PSCRIPT script)
{
	destroy_stack(script->work);
	destroy_stack(script->stack);
	free(script);
}