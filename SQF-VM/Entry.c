#include "textrange.h"
#include "SQF.h"
#include "SQF_types.h"
#include "SQF_parse.h"
#include <stdio.h>

void CMD_PLUS(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm, left);
	right_val = get_value(vm, right);
	push_stack(vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f + right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_MINUS(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm, left);
	right_val = get_value(vm, right);
	push_stack(vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f - right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_MULTIPLY(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm, left);
	right_val = get_value(vm, right);
	push_stack(vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f * right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_DIVIDE(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm, left);
	right_val = get_value(vm, right);
	push_stack(vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f / right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_DIAG_LOG(PVM vm)
{
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm->work);
	right_val = get_value(vm, right);
	if (right_val->type == STRING_TYPE())
	{
		printf("%s\n", ((PSTRING)right_val->val.ptr)->val);
	}
	else
	{
		printf("%lf\n", right_val->val.f);
	}

	inst_destroy(right);
}
void CMD_PRIVATE(PVM vm)
{
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm->work);
	right_val = get_value(vm, right);
	if (right_val->type == STRING_TYPE())
	{
		store_in_scope(vm, top_scope(vm), ((PSTRING)right_val)->val, value(find_type(vm, "any"), base_voidptr(0)));
	}
	else
	{
		error("Expected String", vm->stack);
	}

	inst_destroy(right);
}

//"stolen" from https://stackoverflow.com/a/314422/2684203
char * getline(void)
{
	char * line = malloc(100), *linep = line;
	size_t lenmax = 100, len = lenmax;
	int c;

	if (line == NULL)
		return NULL;

	for (;;)
	{
		c = fgetc(stdin);
		if (c == EOF)
			break;

		if (--len == 0)
		{
			len = lenmax;
			char * linen = realloc(linep, lenmax *= 2);

			if (linen == NULL)
			{
				free(linep);
				return NULL;
			}
			line = linen + (line - linep);
			linep = linen;
		}

		if ((*line++ = c) == '\n')
			break;
	}
	*line = '\0';
	return linep;
}

void main(int argc, char** argv)
{
	PVM vm = sqfvm(1000, 20, 100);
	//register_command(vm, create_command("SCALAR", 't', 0, 0));
	//register_command(vm, create_command("BOOL", 't', 0, 0));
	register_command(vm, create_command("ARRAY", 't', 0, 0));
	//register_command(vm, create_command("STRING", 't', 0, 0));
	register_command(vm, create_command("NOTHING", 't', 0, 0));
	register_command(vm, create_command("ANY", 't', 0, 0));
	register_command(vm, create_command("NAMESPACE", 't', 0, 0));
	register_command(vm, create_command("NaN", 't', 0, 0));
	register_command(vm, create_command("IF", 't', 0, 0));
	register_command(vm, create_command("WHILE", 't', 0, 0));
	register_command(vm, create_command("FOR", 't', 0, 0));
	register_command(vm, create_command("SWITCH", 't', 0, 0));
	register_command(vm, create_command("EXCEPTION", 't', 0, 0));
	register_command(vm, create_command("WITH", 't', 0, 0));
	//register_command(vm, create_command("CODE", 't', 0, 0));
	register_command(vm, create_command("OBJECT", 't', 0, 0));
	register_command(vm, create_command("VECTOR", 't', 0, 0));
	register_command(vm, create_command("TRANS", 't', 0, 0));
	register_command(vm, create_command("ORIENT", 't', 0, 0));
	register_command(vm, create_command("SIDE", 't', 0, 0));
	register_command(vm, create_command("GROUP", 't', 0, 0));
	register_command(vm, create_command("TEXT", 't', 0, 0));
	register_command(vm, create_command("SCRIPT", 't', 0, 0));
	register_command(vm, create_command("TARGET", 't', 0, 0));
	register_command(vm, create_command("JCLASS", 't', 0, 0));
	register_command(vm, create_command("CONFIG", 't', 0, 0));
	register_command(vm, create_command("DISPLAY", 't', 0, 0));
	register_command(vm, create_command("CONTROL", 't', 0, 0));
	register_command(vm, create_command("NetObject", 't', 0, 0));
	register_command(vm, create_command("SUBGROUP", 't', 0, 0));
	register_command(vm, create_command("TEAM_MEMBER", 't', 0, 0));
	register_command(vm, create_command("TASK", 't', 0, 0));
	register_command(vm, create_command("DIARY_RECORD", 't', 0, 0));
	register_command(vm, create_command("LOCATION", 't', 0, 0));

	register_command(vm, create_command("+", 'b', CMD_PLUS, 8));
	register_command(vm, create_command("-", 'b', CMD_MINUS, 8));
	register_command(vm, create_command("*", 'b', CMD_MULTIPLY, 9));
	register_command(vm, create_command("/", 'b', CMD_DIVIDE, 9));
	register_command(vm, create_command("diag_log", 'u', CMD_DIAG_LOG, 0));
	register_command(vm, create_command("private", 'u', CMD_PRIVATE, 0));


	/*
		Test 'file'
			1: private _test = 10 + 12.5;
			2: diag_log _test;
			3: _foo = "test";
			4: diag_log _foo;
			5: _foo = _test;
			6: diag_log _foo;
	*/
	
	////Create root scope
	//push_stack(vm->stack, inst_scope("all"));
	////diag_log _foo
	//push_stack(vm->stack, inst_command(find_command(vm, "diag_log", 'u')));
	//push_stack(vm->stack, inst_load_var("_foo"));
	////_foo = _test
	//push_stack(vm->stack, inst_store_var("_foo"));
	//push_stack(vm->stack, inst_load_var("_test"));
	////diag_log _foo
	//push_stack(vm->stack, inst_command(find_command(vm, "diag_log", 'u')));
	//push_stack(vm->stack, inst_load_var("_foo"));
	////_foo = "test"
	//push_stack(vm->stack, inst_store_var("_foo"));
	//push_stack(vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(string_create2("test")))));
	////diag_log _test
	//push_stack(vm->stack, inst_command(find_command(vm, "diag_log", 'u')));
	//push_stack(vm->stack, inst_load_var("_test"));
	////private _test = 10 + 12.5
	//push_stack(vm->stack, inst_store_var_local("_test"));
	//push_stack(vm->stack, inst_command(find_command(vm, "+", 'b')));
	//push_stack(vm->stack, inst_value(value(find_type(vm, "SCALAR"), base_float(10))));
	//push_stack(vm->stack, inst_value(value(find_type(vm, "SCALAR"), base_float(12.5))));
	//
	//execute(vm);
	//
	//parse(vm, "private _test = 10 + 12.5; diag_log _test; _foo = \"test\"; diag_log _foo; _foo = _test; diag_log _foo");
	//execute(vm);

	printf("Please enter a simple SQF expression:\n");
	parse(vm, getline());
	printf("-------------------------------------\n");
	execute(vm);
	printf("-------------------------------------\n");
	printf("To Quit, Press any key.");
	getchar();
	destroy_sqfvm(vm);
}
