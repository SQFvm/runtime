#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <setjmp.h>
#include <stdint.h>
#if _WIN32 & _DEBUG
	#include <crtdbg.h>
#endif
#ifdef __linux
	#include <alloca.h>
#endif // _GCC
#ifdef _WIN32
	#include <malloc.h>
#endif // _WIN32

#include "basetype.h"
#include "string_map.h"
#include "textrange.h"
#include "SQF.h"
#include "SQF_types.h"
#include "SQF_parse.h"
#include "SQF_commands.h"


#ifdef _WIN32
#define DLLEXPORT_PREFIX __declspec(dllexport)
#else
#define DLLEXPORT_PREFIX __attribute__((visibility("default")))
#endif


static jmp_buf program_exit;
int64_t systime_start = 0;



char* get_line(char* line, size_t lenmax)
{
	char* line_start = line;
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
			len++;
			line--;
		}

		if ((*line++ = c) == '\n')
			break;
	}
	*line = '\0';
	return line_start;
}

#define LINEBUFFER_SIZE 256

void custom_error(PVM vm, const char* errMsg, PSTACK stack)
{
	int len, i, j;
	const char* str;
	PDBGINF dbginf;
	if (stack->allow_dbg)
	{
		dbginf = 0;
		for (i = stack->top - 1; i >= 0; i--)
		{
			if (stack->data[i]->type == INST_DEBUG_INFO)
			{
				dbginf = get_dbginf(0, 0, stack->data[i]);
				break;
			}
		}
		if (dbginf != 0)
		{
			//i = dbginf->offset - 15;
			//len = 30;
			//if (i < 0)
			//{
			//	len += i;
			//	i = 0;
			//}
			//for (j = i; j < i + len; j++)
			//{
			//	if (current_code[j] == '\0' || current_code[j] == '\n')
			//	{
			//		if (j < dbginf->offset)
			//		{
			//			i = j + 1;
			//		}
			//		else
			//		{
			//			len = j - i;
			//			break;
			//		}
			//	}
			//}
			//str = current_code + i;
			//vm->print(vm, "%.*s\n%.*s%.*s\n", len, str, dbginf->offset - i, "                              ", dbginf->length > 30 ? 30 : dbginf->length, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
			vm->print(vm, "%s", dbginf->hint);
			vm->print(vm, "[ERR][L%d|C%d] %s\n", dbginf->line, dbginf->col, errMsg);
		}
		else
		{
			vm->print(vm, "[ERR] %s\n", errMsg);
		}
	}
	else
	{
		vm->print(vm, "[ERR] %s\n", errMsg);
	}
	vm->die_flag = 1;
	//longjmp(program_exit, 1);
}

void create_if_not_exist(PVM vm, const char* name, char type, CMD_CB fnc, char precedence, const char* desc)
{
	if (find_command(vm, name, type) == 0)
	{
		register_command(vm, create_command(name, type, fnc, precedence, desc));
	}
}
void register_commmands(PVM vm)
{
	/*
	//register_command(vm, create_command("SCALAR", 't', 0, 0));
	//register_command(vm, create_command("BOOL", 't', 0, 0));
	//register_command(vm, create_command("ARRAY", 't', 0, 0));
	//register_command(vm, create_command("STRING", 't', 0, 0));
	//register_command(vm, create_command("NOTHING", 't', 0, 0));
	//register_command(vm, create_command("ANY", 't', 0, 0));
	//register_command(vm, create_command("NAMESPACE", 't', 0, 0));
	//register_command(vm, create_command("NaN", 't', 0, 0));
	//register_command(vm, create_command("IF", 't', 0, 0));
	//register_command(vm, create_command("WHILE", 't', 0, 0));
	//register_command(vm, create_command("FOR", 't', 0, 0));
	register_command(vm, create_command("SWITCH", 't', 0, 0));
	register_command(vm, create_command("EXCEPTION", 't', 0, 0));
	register_command(vm, create_command("WITH", 't', 0, 0));
	//register_command(vm, create_command("CODE", 't', 0, 0));
	//register_command(vm, create_command("OBJECT", 't', 0, 0));
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

	create_if_not_exist(vm, "+", 'b', CMD_PLUS, 8, "<SCALAR> + <SCALAR> | <STRING> + <STRING> | <ARRAY> + <ANY>");
	create_if_not_exist(vm, "-", 'b', CMD_MINUS, 8, "<SCALAR> - <SCALAR> ");
	create_if_not_exist(vm, "*", 'b', CMD_MULTIPLY, 9, "<SCALAR> * <SCALAR>");
	create_if_not_exist(vm, "/", 'b', CMD_DIVIDE, 9, "<SCALAR> / <SCALAR>");
	create_if_not_exist(vm, ">", 'b', CMD_LARGETTHEN, 7, "<SCALAR> > <SCALAR>");
	create_if_not_exist(vm, "<", 'b', CMD_LESSTHEN, 7, "<SCALAR> < <SCALAR>");
	create_if_not_exist(vm, ">=", 'b', CMD_LARGETTHENOREQUAL, 7, "<SCALAR> >= <SCALAR>");
	create_if_not_exist(vm, "<=", 'b', CMD_LESSTHENOREQUAL, 7, "<SCALAR> <= <SCALAR>");
	create_if_not_exist(vm, "==", 'b', CMD_EQUAL, 7, "<SCALAR> > <SCALAR>");
	create_if_not_exist(vm, "||", 'b', CMD_OROR, 5, "<BOOL> || <BOOL> | <BOOL> || <CODE>");
	create_if_not_exist(vm, "&&", 'b', CMD_ANDAND, 6, "<BOOL> && <BOOL> | <BOOL> && <CODE>");
	create_if_not_exist(vm, "or", 'b', CMD_OROR, 5, "<BOOL> or <BOOL> | <BOOL> && <CODE>");
	create_if_not_exist(vm, "and", 'b', CMD_ANDAND, 6, "<BOOL> and <BOOL>");
	create_if_not_exist(vm, "select", 'b', CMD_SELECT, 10, "<ARRAY> select <SCALAR>");
	create_if_not_exist(vm, "then", 'b', CMD_THEN, 5, "<IF> then <ARRAY>");
	create_if_not_exist(vm, "else", 'b', CMD_ELSE, 6, "<CODE> else <CODE>");
	create_if_not_exist(vm, "do", 'b', CMD_DO, 0, "<WHILE> do <CODE> | <FOR> do <CODE>");
	create_if_not_exist(vm, "from", 'b', CMD_FROM, 0, "<FOR> from <SCALAR>");
	create_if_not_exist(vm, "to", 'b', CMD_TO, 0, "<FOR> to <SCALAR>");
	create_if_not_exist(vm, "step", 'b', CMD_STEP, 0, "<FOR> step <SCALAR>");
	create_if_not_exist(vm, "count", 'b', CMD_COUNT, 0, "<CODE> count <ARRAY> | <COUNT> count <BOOL>");
	create_if_not_exist(vm, "call", 'b', CMD_CALL, 0, "<ANY> call <CODE>");
	create_if_not_exist(vm, "atan2", 'b', CMD_ATAN2, 0, "<SCALAR> atan2 <SCALAR>");
	create_if_not_exist(vm, "min", 'b', CMD_MIN, 0, "<SCALAR> min <SCALAR>");
	create_if_not_exist(vm, "max", 'b', CMD_MAX, 0, "<SCALAR> max <SCALAR>");
	create_if_not_exist(vm, "mod", 'b', CMD_MOD, 0, "<SCALAR> mod <SCALAR>");
	create_if_not_exist(vm, "%", 'b', CMD_MOD, 0, "<SCALAR> % <SCALAR>");
	create_if_not_exist(vm, "^", 'b', CMD_POWEROF, 0, "<SCALAR> ^ <SCALAR>");
	create_if_not_exist(vm, "pushBack", 'b', CMD_PUSHBACK, 0, "<ARRAY> pushBack <ANY>");
	create_if_not_exist(vm, "set", 'b', CMD_SET, 0, "<ARRAY> set <ANY>");
	create_if_not_exist(vm, "isEqualTo", 'b', CMD_ISEQUALTO, 0, "<ANY> isEqualTo <ANY>");
	create_if_not_exist(vm, "createVehicle", 'b', CMD_CREATEVEHICLE, 0, "<STRING> createVehicle <ARRAY>");

	create_if_not_exist(vm, "diag_log", 'u', CMD_DIAG_LOG, 0, "diag_log <ANY>");
	create_if_not_exist(vm, "private", 'u', CMD_PRIVATE, 0, "private <STRING> | private <ARRAY>");
	create_if_not_exist(vm, "if", 'u', CMD_IF, 0, "if <BOOL>");
	create_if_not_exist(vm, "str", 'u', CMD_STR, 0, "str <ANY>");
	create_if_not_exist(vm, "while", 'u', CMD_WHILE, 0, "while <CODE>");
	create_if_not_exist(vm, "typeName", 'u', CMD_TYPENAME, 0, "typeName <ANY>");
	create_if_not_exist(vm, "for", 'u', CMD_FOR, 0, "for <STRING>");
	create_if_not_exist(vm, "-", 'u', CMD_MINUS_UNARY, 0, "- <SCALAR>");
	create_if_not_exist(vm, "count", 'u', CMD_COUNT_UNARY, 0, "count <STRING> | count <ARRAY>");
	create_if_not_exist(vm, "format", 'u', CMD_FORMAT, 0, "format <ARRAY>");
	create_if_not_exist(vm, "call", 'u', CMD_CALL_UNARY, 0, "call <CODE>");
	create_if_not_exist(vm, "abs", 'u', CMD_ABS, 0, "abs <SCALAR>");
	create_if_not_exist(vm, "deg", 'u', CMD_DEG, 0, "deg <SCALAR>");
	create_if_not_exist(vm, "log", 'u', CMD_LOG, 0, "log <SCALAR>");
	create_if_not_exist(vm, "sin", 'u', CMD_SIN, 0, "sin <SCALAR>");
	create_if_not_exist(vm, "cos", 'u', CMD_COS, 0, "cos <SCALAR>");
	create_if_not_exist(vm, "tan", 'u', CMD_TAN, 0, "tan <SCALAR>");
	create_if_not_exist(vm, "exp", 'u', CMD_EXP, 0, "exp <SCALAR>");
	create_if_not_exist(vm, "asin", 'u', CMD_ASIN, 0, "asin <SCALAR>");
	create_if_not_exist(vm, "acos", 'u', CMD_ACOS, 0, "acos <SCALAR>");
	create_if_not_exist(vm, "atan", 'u', CMD_ATAN, 0, "atan <SCALAR>");
	create_if_not_exist(vm, "atg", 'u', CMD_ATAN, 0, "atg <SCALAR>");
	create_if_not_exist(vm, "rad", 'u', CMD_RAD, 0, "rad <SCALAR>");
	create_if_not_exist(vm, "sqrt", 'u', CMD_SQRT, 0, "sqrt <SCALAR>");
	create_if_not_exist(vm, "ceil", 'u', CMD_CEIL, 0, "ceil <SCALAR>");
	create_if_not_exist(vm, "random", 'u', CMD_RANDOM, 0, "random <SCALAR>");
	create_if_not_exist(vm, "floor", 'u', CMD_FLOOR, 0, "floor <SCALAR>");
	create_if_not_exist(vm, "ln", 'u', CMD_LN, 0, "ln <SCALAR>");
	create_if_not_exist(vm, "round", 'u', CMD_ROUND, 0, "round <SCALAR>");
	create_if_not_exist(vm, "!", 'u', CMD_NOT, 0, "! <BOOL>");
	create_if_not_exist(vm, "comment", 'u', CMD_COMMENT, 0, "comment <BOOL>");


	create_if_not_exist(vm, "true", 'n', CMD_TRUE, 0, "true");
	create_if_not_exist(vm, "false", 'n', CMD_FALSE, 0, "false");
	create_if_not_exist(vm, "parsingNamespace", 'n', CMD_PARSINGNAMESPACE, 0, "parsingNamespace");
	create_if_not_exist(vm, "missionNamespace", 'n', CMD_MISSIONNAMESPACE, 0, "missionNamespace");
	create_if_not_exist(vm, "uiNamespace", 'n', CMD_UINAMESPACE, 0, "uiNamespace");
	create_if_not_exist(vm, "profileNamespace", 'n', CMD_PROFILENAMESPACE, 0, "profileNamespace");
	create_if_not_exist(vm, "diag_tickTime", 'n', CMD_DIAG_TICKTIME, 0, "diag_tickTime");
	create_if_not_exist(vm, "pi", 'n', CMD_PI, 0, "pi");



	create_if_not_exist(vm, "help__", 'n', CMD_HELP, 0, "Displays this help text.");
}

int vm_output_print(PVM vm, const char* format, ...)
{
	va_list args;
	int len;
	char* buff;
	va_start(args, format);

	len = vsnprintf(0, 0, format, args);
	buff = alloca(sizeof(char) * (len + 1));
	len = vsnprintf(buff, len + 1, format, args);

	string_modify_append(vm->print_custom_data, buff);

	va_end(args);
	return len;
}

DLLEXPORT_PREFIX unsigned char start_program(const char* input, unsigned long max_instructions, char* buffer, size_t buffer_size)
{
	int val;
	int i;
	unsigned char success;
	PVM vm = sqfvm(1000, 50, 1, max_instructions);
	vm->error = custom_error;
	if (systime_start == 0)
	{
		systime_start = system_time_ms();
	}


	register_commmands(vm);
	vm->print = vm_output_print;
	vm->print_custom_data = string_create(0);

	val = setjmp(program_exit);
	if (!val)
	{
		push_stack(vm, vm->stack, inst_scope("all"));
		parse(vm, input, 1);
		execute(vm);
		success = vm->die_flag;
	}
	if (vm->work->top != 0)
	{
		for (i = 0; i < vm->work->top; i++)
		{
			if (vm->work->data[i]->type == INST_VALUE)
			{
				if (get_value(vm, vm->stack, vm->work->data[i])->type == NOTHING_TYPE())
					continue;
				vm->print(vm, "[WORK]: ");
				stringify_value(vm, vm->print_custom_data, get_value(vm, vm->stack, vm->work->data[i]));
				vm->print(vm, "\n");
			}
		}
	}
	if (((PSTRING)vm->print_custom_data)->val != 0)
	{
		if (buffer == 0 || buffer_size == 0)
		{
			printf(((PSTRING)vm->print_custom_data)->val);
		}
		else
		{
			strncpy(buffer, ((PSTRING)vm->print_custom_data)->val, buffer_size);
		}
	}
	else
	{
		if (buffer == 0 || buffer_size == 0)
		{
			printf("<EMPTY>\n");
		}
		else
		{
			strncpy(buffer, "<EMPTY>\n", buffer_size);
		}
	}
	string_destroy(vm->print_custom_data);
	destroy_sqfvm(vm);
	return success;
}


#define RETCDE_OK 0
#define RETCDE_ERROR 1
#define RETCDE_RUNTIME_ERROR 2

int load_file(PSTRING buffer, const char* fpath)
{
	FILE* fptr = fopen(fpath, "r");
	size_t size;
	size_t curlen = buffer->length;
	int tailing = 0;
	int lcount = 1;
	if (fptr == 0)
	{
		printf("[ERR] Could not open file '%s'", fpath);
		return -1;
	}
	fseek(fptr, 0, SEEK_END);
	size = ftell(fptr);;
	rewind(fptr);
	string_modify_append2(buffer, size);
	memset(buffer->val + curlen, 0, sizeof(char) * size);
	fread(buffer->val + curlen, sizeof(char), size, fptr);
	for (; curlen < buffer->length; curlen++)
	{
		if (buffer->val[curlen] == '\n')
			lcount++;
		else if (buffer->val[curlen] == '\0')
			tailing++;
	}
	if (tailing > 0)
	{
		string_modify_append2(buffer, -tailing);
	}
	return lcount;
}

int main(int argc, char** argv)
{
	char linebuffer[LINEBUFFER_SIZE];
	const char* ptr;
	int i, j, k;
	unsigned char just_execute = 0;
	unsigned char prog_success = 0;
	PSTRING pstr;
	unsigned long max_inst = 10000;
	pstr = string_create(0);
	//_CrtSetBreakAlloc(593);
	j = 0;

	for (i = 0; i < argc; i++)
	{
		ptr = argv[i];
		if (ptr[0] == '-')
		{
			switch (ptr[1])
			{
				case '\0':
					printf("[ERR] empty argument passed.");
					return RETCDE_ERROR;
				case '?':
					printf("SQF-VM Help page\n");
					printf("./prog [-j] [-s 10000] [-i <FILE>] [-I <CODE>]\n");
					printf("\t-?\tOutputs this help\n");
					printf("\t-i\tLoads provided input file into the code-buffer.\n");
					printf("\t-I\tLoads provided input SQF code into the code-buffer.\n");
					printf("\t-a\tDisables user input and just executes the code-buffer.\n");
					printf("\t-s\tSets the maximum instruction count allowed before termination.\n");
					printf("\t  \tMaximum value is %lu.\n", LONG_MAX);
					printf("\t  \tIf `0` is passed, the limit will be disabled.\n");
					return RETCDE_OK;
					break;
				case 'i':
					if (i + 1 < argc)
					{
						k = load_file(pstr, argv[++i]);
						if (k < 0)
							return RETCDE_ERROR;
						j += k;
					}
					else
					{
						printf("[ERR] -i empty parameter");
						return RETCDE_ERROR;
					}
					break;
				case 'I':
					if (i + 1 < argc)
					{
						string_modify_append(pstr, argv[++i]);
						j++;
					}
					else
					{
						printf("[ERR] -I empty parameter");
						return RETCDE_ERROR;
					}
					break;
				case 'a':
					just_execute = 1;
					break;
				case 's':
					if (i + 1 < argc)
					{
						max_inst = strtoul(argv[i + 1], 0, 10);
						j++;
					}
					else
					{
						printf("[ERR] -I empty parameter");
						return RETCDE_ERROR;
					}
					break;
				default:
					break;
			}
		}
	}
	ptr = 0;
	i = j;

	if (!just_execute)
	{
		printf("Please enter your SQF code.\nTo get the capabilities, use the `help__` instruction.\nTo run the code, Press <ENTER> twice.\n");
		printf("%d:\t", i++);
		while (get_line(linebuffer, LINEBUFFER_SIZE)[0] != '\n')
		{
			string_modify_append(pstr, linebuffer);
			printf("%d:\t", i++);
		}
		//string_modify_append(pstr, "diag_log str [1, 2, \"test\", [1, 2, 3]]");
	}
	if (just_execute)
	{
		if (pstr->length > 0)
			prog_success = start_program(pstr->val, max_inst, 0, 0);
	}
	else
	{
		printf("-------------------------------------\n");
		if (pstr->length > 0)
			prog_success = start_program(pstr->val, max_inst, 0, 0);
		printf("-------------------------------------\n");
		printf("Press <ENTER> to finish.");
		get_line(linebuffer, LINEBUFFER_SIZE);
	}
	string_destroy(pstr);
	namespace_destroy(sqf_missionNamespace());
	namespace_destroy(sqf_parsingNamespace());
	namespace_destroy(sqf_profileNamespace());
	namespace_destroy(sqf_uiNamespace());

	destroy_cmdcnt(GET_PCMDCNT());

	#if _WIN32 & _DEBUG
	_CrtDumpMemoryLeaks();
	#endif
	return prog_success ? RETCDE_OK : RETCDE_RUNTIME_ERROR;
}