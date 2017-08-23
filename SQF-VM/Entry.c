#include "textrange.h"
#include "SQF.h"
#include "SQF_types.h"
#include "SQF_parse.h"
#include <stdio.h>
#include <malloc.h>
#ifdef __linux
#include <alloca.h>
#endif // _GCC
#include <math.h>
#include <string.h>
#include <setjmp.h>

static PSTRING outputbuffer = 0;
static jmp_buf program_exit;

void stringify_value(PVM vm, PSTRING str, PVALUE val)
{
	PARRAY arr;
	int i;
	char* strptr;
	if (val->type == STRING_TYPE())
	{
		string_modify_append(str, "\"");
		string_modify_append(str, ((PSTRING)val->val.ptr)->val);
		string_modify_append(str, "\"");
	}
	else if (val->type == SCALAR_TYPE())
	{
		strptr = alloca(sizeof(char) * 64);
		sprintf(strptr, "%lf\0", val->val.f);
		string_modify_append(str, strptr);
	}
	else if (val->type == BOOL_TYPE())
	{
		string_modify_append(str, val->val.i > 0 ? "true" : "false");
	}
	else if (val->type == CODE_TYPE())
	{
		string_modify_append(str, "{");
		string_modify_append(str, ((PCODE)val->val.ptr)->val);
		string_modify_append(str, "}");
	}
	else if (val->type == ARRAY_TYPE())
	{
		arr = ((PARRAY)val->val.ptr);
		string_modify_append(str, "[");
		for (i = 0; i < arr->top; i++)
		{
			if (i > 0)
			{
				string_modify_append(str, ", ");
			}
			stringify_value(vm, str, arr->data[i]);
		}
		string_modify_append(str, "]");
	}
	else
	{
		vm->error("STR TYPE MISSMATCH", vm->stack);
	}
}

void CMD_PLUS(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
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
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
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
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
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
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
	push_stack(vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f / right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_DIAG_LOG(PVM vm)
{
	PINST right;
	PVALUE right_val;
	char* ptr;
	int len;
	right = pop_stack(vm->work);
	right_val = get_value(vm->stack, right);
	if (right_val->type == STRING_TYPE())
	{
		len = ((PSTRING)right_val->val.ptr)->length;
		if (len > 1024)
			len = 1024;
		ptr = alloca(sizeof(char) * (len + 2));
		sprintf(ptr, "%.*s\n", 1024, ((PSTRING)right_val->val.ptr)->val);
		ptr[len + 1] = '\0';
		string_modify_append(outputbuffer, ptr);
	}
	else
	{
		vm->error("expected string", vm->stack);
	}

	inst_destroy(right);
}
void CMD_PRIVATE(PVM vm)
{
	PINST right;
	PVALUE right_val;
	right = pop_stack(vm->work);
	right_val = get_value(vm->stack, right);
	if (right_val->type == STRING_TYPE())
	{
		store_in_scope(vm, top_scope(vm), ((PSTRING)right_val)->val, value(find_type(vm, "any"), base_voidptr(0)));
	}
	else
	{
		vm->error("Expected String", vm->stack);
	}

	inst_destroy(right);
}

void CMD_IF(PVM vm)
{
	PINST right;
	PVALUE right_val;
	int flag;
	right = pop_stack(vm->work);
	right_val = get_value(vm->stack, right);
	if (right_val->type == BOOL_TYPE())
	{
		flag = right_val->val.i > 0;
	}
	else if (right_val->type == SCALAR_TYPE())
	{
		flag = right_val->val.f > 0;
	}
	else
	{
		vm->error("expected bool", vm->stack);
	}
	push_stack(vm->stack, inst_value(value(IF_TYPE(), base_int(flag))));

	inst_destroy(right);
}
void CMD_THEN(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
	PCODE code;
	PARRAY arr;

	if (left_val->type != IF_TYPE())
	{
		vm->error("expected left type to be IF", vm->stack);
	}
	if (right_val->type == ARRAY_TYPE())
	{
		arr = right_val->val.ptr;
		if (arr->top == 0)
		{
			vm->error("Array is empty", vm->stack);
		}
		if (arr->data[0]->type != CODE_TYPE())
		{
			vm->error("Array[0] was expected to be CODE", vm->stack);
		}
		if (arr->top > 1 && arr->data[1]->type != CODE_TYPE())
		{
			vm->error("Array[1] was expected to be CODE", vm->stack);
		}
		if (left_val->val.i)
		{
			code = arr->data[0]->val.ptr;
		}
		else if(arr->top > 1)
		{
			code = arr->data[1]->val.ptr;
		}
		if (code != 0)
		{
			push_stack(vm->stack, inst_code_load());
			push_stack(vm->stack, inst_value(value(CODE_TYPE(), base_voidptr(code))));
		}
	}
	else if(right_val->type == CODE_TYPE())
	{
		if (left_val->val.i)
		{
			push_stack(vm->stack, inst_code_load());
			push_stack(vm->stack, inst_value(value(CODE_TYPE(), base_voidptr(right_val->val.ptr))));
		}
	}
	else
	{
		vm->error("expected right type to be ARRAY or CODE", vm->stack);
	}

	inst_destroy(left);
	inst_destroy(right);
}
void CMD_ELSE(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);

	push_stack(vm->stack, inst_arr_push());
	push_stack(vm->stack, inst_value(value(CODE_TYPE(), right_val->val)));
	push_stack(vm->stack, inst_arr_push());
	push_stack(vm->stack, inst_value(value(CODE_TYPE(), left_val->val)));
	push_stack(vm->stack, inst_value(value(ARRAY_TYPE(), base_voidptr(array_create2(2)))));

	inst_destroy(left);
	inst_destroy(right);
}
void CMD_TRUE(PVM vm)
{
	push_stack(vm->stack, inst_value(value(BOOL_TYPE(), base_int(1))));
}
void CMD_FALSE(PVM vm)
{
	push_stack(vm->stack, inst_value(value(BOOL_TYPE(), base_int(0))));
}
void CMD_HELP(PVM vm)
{
	int i;
	CPCMD cmd;
	printf("GLOBAL variables are treated as LOCAL too!\n");
	printf("ERRORS will probably result in crash\n");
	printf("NAME:TYPE:PRECEDENCE\n");
	for (i = 0; i < vm->cmds_top; i++)
	{
		cmd = vm->cmds[i];
		printf("%s:%c:%d\n", cmd->name, cmd->type, cmd->precedence_level);
	}
	
}

void CMD_STR(PVM vm)
{
	PINST right;
	PVALUE right_val;
	PSTRING str = string_create(0);
	right = pop_stack(vm->work);
	right_val = get_value(vm->stack, right);
	stringify_value(vm, str, right_val);
	inst_destroy(right);
	push_stack(vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(str))));
}

void CMD_LARGETTHEN(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
	push_stack(vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f > right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_LESSTHEN(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
	push_stack(vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f < right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_LARGETTHENOREQUAL(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
	push_stack(vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f >= right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_LESSTHENOREQUAL(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
	push_stack(vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f <= right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_EQUAL(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
	push_stack(vm->stack, inst_value(value(left_val->type, base_float(left_val->val.f == right_val->val.f))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_ANDAND(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
	if (left_val->type != BOOL_TYPE() || right_val->type != BOOL_TYPE())
	{
		vm->error("Expected left and right to be of type BOOL", vm->stack);
	}
	push_stack(vm->stack, inst_value(value(left_val->type, base_float(left_val->val.i && right_val->val.i))));
	inst_destroy(left);
	inst_destroy(right);
}
void CMD_OROR(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
	if (left_val->type != BOOL_TYPE() || right_val->type != BOOL_TYPE())
	{
		vm->error("Expected left and right to be of type BOOL", vm->stack);
	}
	push_stack(vm->stack, inst_value(value(left_val->type, base_float(left_val->val.i || right_val->val.i))));
	inst_destroy(left);
	inst_destroy(right);
}

void CMD_SELECT(PVM vm)
{
	PINST left;
	PINST right;
	PVALUE left_val;
	PVALUE right_val;
	PARRAY arr;
	PVALUE tmp;
	left = pop_stack(vm->work);
	right = pop_stack(vm->work);
	left_val = get_value(vm->stack, left);
	right_val = get_value(vm->stack, right);
	//ToDo: implement more select variants https://community.bistudio.com/wiki/select

	if (left_val->type == ARRAY_TYPE())
	{
		arr = left_val->val.ptr;
		if (right_val->type == SCALAR_TYPE())
		{
			tmp = arr->data[(int)roundf(right_val->val.f)];
			push_stack(vm->stack, inst_value(value(tmp->type, tmp->val)));
		}
		else
		{
			vm->error("Expected SCALAR right", vm->stack);
		}
	}
	else
	{
		vm->error("Expected ARRAY left", vm->stack);
	}


	inst_destroy(left);
	inst_destroy(right);
}

//"stolen" from https://stackoverflow.com/a/314422/2684203
char * getline(char* line, size_t lenmax)
{
	char * *linep = line;
	size_t len = lenmax;
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

#define LINEBUFFER_SIZE 256
__declspec(dllexport) char* start_program(char* input);
void main(int argc, char** argv)
{
	char linebuffer[LINEBUFFER_SIZE];
	char* ptr;
	int i = 1;

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
	
	//parse(vm, "private _test = 10 + 12.5; diag_log _test; _foo = \"test\"; diag_log _foo; _foo = _test; diag_log _foo");
	//execute(vm);
	PSTRING pstr = string_create(0);
	printf("Please enter your SQF code.\nTo get the capabilities, use the `help` instruction.\nTo run the code, Press <ENTER> twice.\n");
	printf("%d:\t", i++);
	while (getline(linebuffer, LINEBUFFER_SIZE)[0] != '\n')
	{
		string_modify_append(pstr, linebuffer);
		printf("%d:\t", i++);
	}
	ptr = start_program(pstr->val);
	printf("-------------------------------------\n");
	if (ptr == 0)
	{
		printf("<EMPTY>\n");
	}
	else
	{
		printf("%s", ptr);
	}
	printf("-------------------------------------\n");
	printf("Press <ENTER> to finish.");
	getline(linebuffer, LINEBUFFER_SIZE);
	string_destroy(pstr);
}

void custom_error(const char* errMsg, PSTACK stack)
{
	int len = strlen(errMsg) + 8;
	char* str = alloca(sizeof(char) * (len + 1));
	sprintf(str, "ERROR: %s\n", errMsg);
	str[len] = '\0';
	string_modify_append(outputbuffer, str);
	longjmp(program_exit, 1);
}

__declspec(dllexport) char* start_program(char* input)
{
	PVM vm = sqfvm(1000, 50, 100);
	vm->error = custom_error;
	int val;
	if (outputbuffer == 0)
	{
		outputbuffer = string_create(0);
	}
	else
	{
		free(outputbuffer->val);
		outputbuffer->val = 0;
		outputbuffer->length = 0;
	}
	/*
	//register_command(vm, create_command("SCALAR", 't', 0, 0));
	//register_command(vm, create_command("BOOL", 't', 0, 0));
	//register_command(vm, create_command("ARRAY", 't', 0, 0));
	//register_command(vm, create_command("STRING", 't', 0, 0));
	register_command(vm, create_command("NOTHING", 't', 0, 0));
	register_command(vm, create_command("ANY", 't', 0, 0));
	register_command(vm, create_command("NAMESPACE", 't', 0, 0));
	register_command(vm, create_command("NaN", 't', 0, 0));
	//register_command(vm, create_command("IF", 't', 0, 0));
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
	*/

	register_command(vm, create_command("+", 'b', CMD_PLUS, 8));
	register_command(vm, create_command("-", 'b', CMD_MINUS, 8));
	register_command(vm, create_command("*", 'b', CMD_MULTIPLY, 9));
	register_command(vm, create_command("/", 'b', CMD_DIVIDE, 9));
	register_command(vm, create_command(">", 'b', CMD_LARGETTHEN, 7));
	register_command(vm, create_command("<", 'b', CMD_LESSTHEN, 7));
	register_command(vm, create_command(">=", 'b', CMD_LARGETTHENOREQUAL, 7));
	register_command(vm, create_command("<=", 'b', CMD_LESSTHENOREQUAL, 7));
	register_command(vm, create_command("==", 'b', CMD_EQUAL, 7));
	register_command(vm, create_command("||", 'b', CMD_OROR, 5));
	register_command(vm, create_command("&&", 'b', CMD_ANDAND, 6));
	register_command(vm, create_command("diag_log", 'u', CMD_DIAG_LOG, 0));
	register_command(vm, create_command("private", 'u', CMD_PRIVATE, 0));
	register_command(vm, create_command("if", 'u', CMD_IF, 5));
	register_command(vm, create_command("then", 'b', CMD_THEN, 5));
	register_command(vm, create_command("else", 'b', CMD_ELSE, 6));
	register_command(vm, create_command("true", 'n', CMD_TRUE, 10));
	register_command(vm, create_command("false", 'n', CMD_FALSE, 10));
	register_command(vm, create_command("help", 'n', CMD_HELP, 10));
	register_command(vm, create_command("str", 'u', CMD_STR, 0));
	register_command(vm, create_command("select", 'b', CMD_SELECT, 10));
	val = setjmp(program_exit);
	if (!val)
	{
		parse(vm, input);
		execute(vm);
	}
	destroy_sqfvm(vm);
	return outputbuffer->val;
}