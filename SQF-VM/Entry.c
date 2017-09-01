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
static const char* current_code = 0;
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
	if (current_code != 0 && stack->allow_dbg)
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
			i = dbginf->offset - 15;
			len = 30;
			if (i < 0)
			{
				len += i;
				i = 0;
			}
			for (j = i; j < i + len; j++)
			{
				if (current_code[j] == '\0' || current_code[j] == '\n')
				{
					if (j < dbginf->offset)
					{
						i = j + 1;
					}
					else
					{
						len = j - i;
						break;
					}
				}
			}
			str = current_code + i;
			vm->print(vm, "%.*s\n%.*s%.*s\n", len, str, dbginf->offset - i, "                              ", dbginf->length > 30 ? 30 : dbginf->length, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
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
	/register_command(vm, create_command("FOR", 't', 0, 0));
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

	register_command(vm, create_command("+", 'b', CMD_PLUS, 8, "<SCALAR> + <SCALAR> | <STRING> + <STRING> | <ARRAY> + <ANY>"));
	register_command(vm, create_command("-", 'b', CMD_MINUS, 8, "<SCALAR> - <SCALAR> "));
	register_command(vm, create_command("*", 'b', CMD_MULTIPLY, 9, "<SCALAR> * <SCALAR>"));
	register_command(vm, create_command("/", 'b', CMD_DIVIDE, 9, "<SCALAR> / <SCALAR>"));
	register_command(vm, create_command(">", 'b', CMD_LARGETTHEN, 7, "<SCALAR> > <SCALAR>"));
	register_command(vm, create_command("<", 'b', CMD_LESSTHEN, 7, "<SCALAR> < <SCALAR>"));
	register_command(vm, create_command(">=", 'b', CMD_LARGETTHENOREQUAL, 7, "<SCALAR> >= <SCALAR>"));
	register_command(vm, create_command("<=", 'b', CMD_LESSTHENOREQUAL, 7, "<SCALAR> <= <SCALAR>"));
	register_command(vm, create_command("==", 'b', CMD_EQUAL, 7, "<SCALAR> > <SCALAR>"));
	register_command(vm, create_command("||", 'b', CMD_OROR, 5, "<BOOL> || <BOOL> | <BOOL> || <CODE>"));
	register_command(vm, create_command("&&", 'b', CMD_ANDAND, 6, "<BOOL> && <BOOL> | <BOOL> && <CODE>"));
	register_command(vm, create_command("or", 'b', CMD_OROR, 5, "<BOOL> or <BOOL> | <BOOL> && <CODE>"));
	register_command(vm, create_command("and", 'b', CMD_ANDAND, 6, "<BOOL> and <BOOL>"));
	register_command(vm, create_command("select", 'b', CMD_SELECT, 10, "<ARRAY> select <SCALAR>"));
	register_command(vm, create_command("then", 'b', CMD_THEN, 5, "<IF> then <ARRAY>"));
	register_command(vm, create_command("else", 'b', CMD_ELSE, 6, "<CODE> else <CODE>"));
	register_command(vm, create_command("do", 'b', CMD_DO, 0, "<WHILE> do <CODE> | <FOR> do <CODE>"));
	register_command(vm, create_command("from", 'b', CMD_FROM, 0, "<FOR> from <SCALAR>"));
	register_command(vm, create_command("to", 'b', CMD_TO, 0, "<FOR> to <SCALAR>"));
	register_command(vm, create_command("step", 'b', CMD_STEP, 0, "<FOR> step <SCALAR>"));
	register_command(vm, create_command("count", 'b', CMD_COUNT, 0, "<CODE> count <ARRAY> | <COUNT> count <BOOL>"));
	register_command(vm, create_command("call", 'b', CMD_CALL, 0, "<ANY> call <CODE>"));
	register_command(vm, create_command("atan2", 'b', CMD_ATAN2, 0, "<SCALAR> atan2 <SCALAR>"));
	register_command(vm, create_command("min", 'b', CMD_MIN, 0, "<SCALAR> min <SCALAR>"));
	register_command(vm, create_command("max", 'b', CMD_MAX, 0, "<SCALAR> max <SCALAR>"));
	register_command(vm, create_command("mod", 'b', CMD_MOD, 0, "<SCALAR> mod <SCALAR>"));
	register_command(vm, create_command("%", 'b', CMD_MOD, 0, "<SCALAR> % <SCALAR>"));
	register_command(vm, create_command("^", 'b', CMD_POWEROF, 0, "<SCALAR> ^ <SCALAR>"));
	register_command(vm, create_command("pushBack", 'b', CMD_PUSHBACK, 0, "<ARRAY> pushBack <ANY>"));

	register_command(vm, create_command("diag_log", 'u', CMD_DIAG_LOG, 0, "diag_log <ANY>"));
	register_command(vm, create_command("private", 'u', CMD_PRIVATE, 0, "private <STRING> | private <ARRAY>"));
	register_command(vm, create_command("if", 'u', CMD_IF, 0, "if <BOOL>"));
	register_command(vm, create_command("str", 'u', CMD_STR, 0, "str <ANY>"));
	register_command(vm, create_command("while", 'u', CMD_WHILE, 0, "while <CODE>"));
	register_command(vm, create_command("typeName", 'u', CMD_TYPENAME, 0, "typeName <ANY>"));
	register_command(vm, create_command("for", 'u', CMD_FOR, 0, "for <STRING>"));
	register_command(vm, create_command("-", 'u', CMD_MINUS_UNARY, 0, "- <SCALAR>"));
	register_command(vm, create_command("count", 'u', CMD_COUNT_UNARY, 0, "count <STRING> | count <ARRAY>"));
	register_command(vm, create_command("format", 'u', CMD_FORMAT, 0, "format <ARRAY>"));
	register_command(vm, create_command("call", 'u', CMD_CALL_UNARY, 0, "call <CODE>"));
	register_command(vm, create_command("abs", 'u', CMD_ABS, 0, "abs <SCALAR>"));
	register_command(vm, create_command("deg", 'u', CMD_DEG, 0, "deg <SCALAR>"));
	register_command(vm, create_command("log", 'u', CMD_LOG, 0, "log <SCALAR>"));
	register_command(vm, create_command("sin", 'u', CMD_SIN, 0, "sin <SCALAR>"));
	register_command(vm, create_command("cos", 'u', CMD_COS, 0, "cos <SCALAR>"));
	register_command(vm, create_command("tan", 'u', CMD_TAN, 0, "tan <SCALAR>"));
	register_command(vm, create_command("exp", 'u', CMD_EXP, 0, "exp <SCALAR>"));
	register_command(vm, create_command("asin", 'u', CMD_ASIN, 0, "asin <SCALAR>"));
	register_command(vm, create_command("acos", 'u', CMD_ACOS, 0, "acos <SCALAR>"));
	register_command(vm, create_command("atan", 'u', CMD_ATAN, 0, "atan <SCALAR>"));
	register_command(vm, create_command("atg", 'u', CMD_ATAN, 0, "atg <SCALAR>"));
	register_command(vm, create_command("rad", 'u', CMD_RAD, 0, "rad <SCALAR>"));
	register_command(vm, create_command("sqrt", 'u', CMD_SQRT, 0, "sqrt <SCALAR>"));
	register_command(vm, create_command("ceil", 'u', CMD_CEIL, 0, "ceil <SCALAR>"));
	register_command(vm, create_command("random", 'u', CMD_RANDOM, 0, "random <SCALAR>"));
	register_command(vm, create_command("floor", 'u', CMD_FLOOR, 0, "floor <SCALAR>"));
	register_command(vm, create_command("ln", 'u', CMD_LN, 0, "ln <SCALAR>"));
	register_command(vm, create_command("round", 'u', CMD_ROUND, 0, "round <SCALAR>"));
	register_command(vm, create_command("!", 'u', CMD_NOT, 0, "! <BOOL>"));
	register_command(vm, create_command("comment", 'u', CMD_COMMENT, 0, "comment <BOOL>"));


	register_command(vm, create_command("true", 'n', CMD_TRUE, 0, "true"));
	register_command(vm, create_command("false", 'n', CMD_FALSE, 0, "false"));
	register_command(vm, create_command("parsingNamespace", 'n', CMD_PARSINGNAMESPACE, 0, "parsingNamespace"));
	register_command(vm, create_command("missionNamespace", 'n', CMD_MISSIONNAMESPACE, 0, "missionNamespace"));
	register_command(vm, create_command("uiNamespace", 'n', CMD_UINAMESPACE, 0, "uiNamespace"));
	register_command(vm, create_command("profileNamespace", 'n', CMD_PROFILENAMESPACE, 0, "profileNamespace"));
	register_command(vm, create_command("diag_tickTime", 'n', CMD_DIAG_TICKTIME, 0, "diag_tickTime"));
	register_command(vm, create_command("pi", 'n', CMD_PI, 0, "pi"));
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
	PVM vm = sqfvm(1000, 50, 100, 1, max_instructions);
	vm->error = custom_error;
	if (systime_start == 0)
	{
		systime_start = system_time_ms();
	}


	register_commmands(vm);
	vm->print = vm_output_print;
	vm->print_custom_data = string_create(0);

	current_code = input;
	register_command(vm, create_command("help", 'n', CMD_HELP, 0, "Displays this help text."));
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
	current_code = 0;
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
	PVM vm;
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
		printf("Please enter your SQF code.\nTo get the capabilities, use the `help` instruction.\nTo run the code, Press <ENTER> twice.\n");
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
	vm = sqfvm(0, 0, 0, 0, 0);
	for (i = 0; i < vm->cmds_top; i++)
	{
		destroy_command(vm->cmds[i]);
	}
	destroy_sqfvm(vm);

	#if _WIN32 & _DEBUG
	_CrtDumpMemoryLeaks();
	#endif
	return prog_success ? RETCDE_OK : RETCDE_RUNTIME_ERROR;
}