#include "SQF.h"
#include "SQF_types.h"

#include <malloc.h>
#include <string.h>

void TYPE_CODE_CALLBACK(PVALUE val)
{
	PCODE code = val->val.ptr;
	if (val->type == 0)
	{
		code->refcount--;
		if (code->refcount <= 0)
		{
			code_destroy(code);
		}
	}
	else
	{
		code->refcount++;
	}
}
PCMD CODE_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command("CODE", 't', TYPE_CODE_CALLBACK, 0);
	}
	return cmd;
}
PCODE code_create(int initial_size)
{
	PCODE code = malloc(sizeof(CODE));
	code->stack = create_stack(initial_size);
	code->refcount = 0;
	return code;
}
void code_destroy(PCODE code)
{
	destroy_stack(code->stack);
	free(code);
}


void TYPE_STRING_CALLBACK(PVALUE val)
{
	PSTRING string = val->val.ptr;
	if (val->type == 0)
	{
		string->refcount--;
		if (string->refcount <= 0)
		{
			string_destroy(string);
		}
	}
	else
	{
		string->refcount++;
	}
}
PCMD STRING_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command("STRING", 't', TYPE_STRING_CALLBACK, 0);
	}
	return cmd;
}
PSTRING string_create(unsigned int len)
{
	PSTRING string = malloc(sizeof(STRING));
	string->length = len;
	string->val = 0;
	string->refcount = 0;
	if (len > 0)
	{
		string->val = malloc(sizeof(char) * len + 1);
	}
	return string;
}
PSTRING string_create2(const char* str)
{
	int len = strlen(str);
	PSTRING string = string_create(len);
	if (len > 0)
	{
		strcpy(string->val, str);
	}
	return string;
}
void string_destroy(PSTRING string)
{
	if (string->val != 0)
	{
		free(string->val);
	}
	free(string);
}
PSTRING string_concat(CPSTRING l, CPSTRING r)
{
	PSTRING string = string_create(l->length + r->length);
	strcpy(string->val, l->val);
	strcpy(string->val + l->length, r->val);
	return string;
}
PSTRING string_substring(CPSTRING string, unsigned int start, int length)
{
	if (length < 0)
	{
		length = string->length - start;
	}
	PSTRING str = string_create(length);
	strncpy(str->val, string->val + start, length);
	return str;
}