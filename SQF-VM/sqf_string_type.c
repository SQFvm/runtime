#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "sqffull.h"

void TYPE_STRING_CALLBACK(void* input, CPCMD self)
{
	PVALUE val = input;
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
		cmd = create_command(L"STRING", 't', TYPE_STRING_CALLBACK, 0, NULL, NULL,
			NULL);
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
		string->val = malloc(sizeof(wchar_t) * (len + 1));
		if (string->val == 0)
		{
			string->length = 0;
		}
		else
		{
			memset(string->val, 0, sizeof(wchar_t) * (len + 1));
		}
	}
	return string;
}
PSTRING string_create2(const wchar_t* str)
{
	int len = wcslen(str);
	PSTRING string = string_create(len);
	if (string->length > 0)
	{
		wcscpy(string->val, str);
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
PSTRING string_concat(const PSTRING l, const PSTRING r)
{
	PSTRING string = string_create(l->length + r->length);
	if (string->length == 0)
		return string;
	if (l->length != 0)
	{
		wcscpy(string->val, l->val);
	}
	if (r->length != 0)
	{
		wcscpy(string->val + l->length, r->val);
	}
	return string;
}
PSTRING string_substring(const PSTRING string, unsigned int start, int length)
{
	if (length < 0)
	{
		length = string->length - start;
	}
	PSTRING str = string_create(length);
	if (str->length == 0)
		return str;
	wcsncpy(str->val, string->val + start, length);
	return str;
}
void string_modify_append(PSTRING string, const wchar_t* append)
{
	if (append == 0)
		return;
	unsigned int len = wcslen(append);
	wchar_t* ptr;
	if (len == 0)
		return;
	ptr = realloc(string->val, sizeof(wchar_t) * (string->length + len + 1));
	if (ptr == 0)
		return;
	string->val = ptr;
	wcscpy(string->val + string->length, append);
	string->length += len;
	string->val[string->length] = '\0';
}
void string_modify_nappend(PSTRING string, const wchar_t* append, unsigned int len)
{
	if (append == 0 || len == 0)
		return;
	wchar_t* ptr = realloc(string->val, sizeof(wchar_t) * (string->length + len + 1));
	if (ptr == 0)
		return;
	string->val = ptr;
	wcsncpy(string->val + string->length, append, len);
	string->length += len;
	string->val[string->length] = '\0';
}
void string_modify_append2(PSTRING string, int len)
{
	if (len == 0)
		return;
	wchar_t* ptr;
	ptr = realloc(string->val, sizeof(wchar_t) * (string->length + len + 1));
	if (ptr == 0)
		return;
	string->val = ptr;
	string->length += len;
	string->val[string->length] = '\0';
}