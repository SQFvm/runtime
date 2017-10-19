#include <stdlib.h>
#include <wchar.h>
#include <stdbool.h>
#include <stdint.h>

#include "sqffull.h"

void TYPE_ARRAY_CALLBACK(void* input, CPCMD self)
{
	PVALUE val = input;
	PARRAY arr = val->val.ptr;
	if (val->type == 0)
	{
		arr->refcount--;
		if (arr->refcount <= 0)
		{
			array_destroy(arr);
		}
	}
	else
	{
		arr->refcount++;
	}
}
PCMD ARRAY_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"ARRAY", 't', TYPE_ARRAY_CALLBACK, 0, NULL, NULL,
			NULL);
	}
	return cmd;
}
PARRAY array_create2(unsigned int initialsize)
{
	PARRAY arr = malloc(sizeof(ARRAY));
	arr->data = malloc(sizeof(PVALUE) * initialsize);
	arr->size = initialsize;
	arr->top = 0;
	arr->refcount = 0;
	return arr;
}
PARRAY array_create(void)
{
	return array_create2(ARRAY_DEFAULT_INC);
}
void array_destroy(PARRAY arr)
{
	int i;
	for (i = 0; i < arr->top; i++)
	{
		inst_destroy_value(arr->data[i]);
	}
	free(arr->data);
	free(arr);
}
void array_resize(PARRAY arr, unsigned int newsize)
{
	PVALUE* data = realloc(arr->data, sizeof(PVALUE) * newsize);
	if (data != 0)
	{
		arr->data = data;
		arr->size = newsize;
	}
}
void array_resize_top(PARRAY arr, unsigned int newsize)
{
	int i;
	if (arr->top == newsize)
	{
		return;
	}
	else if (arr->top > newsize)
	{
		// delete array elements until respective size is reached
		for (i = arr->top - 1; i >= newsize; i--)
		{
			inst_destroy_value(arr->data[i]);
			arr->data[i] = NULL;
		}
	}
	else
	{
		if (newsize < arr->size)
		{
			array_resize(arr, newsize);
		}
		// create respective elements with nil value
		for (i = arr->top; i < newsize; i++)
		{
			array_push(arr, value(NOTHING_TYPE(), base_int(0)));
		}
	}

	arr->top = newsize;
}

void array_push(PARRAY arr, VALUE val)
{
	if (arr->top >= arr->size)
	{
		array_resize(arr, arr->size + ARRAY_DEFAULT_INC);
	}
	arr->data[arr->top] = malloc(sizeof(VALUE));
	arr->data[arr->top]->type = val.type;
	arr->data[arr->top]->val = val.val;
	arr->top++;
}
int array_index_of(const PARRAY array, const PVALUE value)
{
	int i;
	for (i = 0; i < array->top; i++)
	{
		if (is_equal_to(array->data[i], value))
		{
			return i;
		}
	}
	return -1;
}

PVALUE array_pop(PARRAY array)
{
	PVALUE val = array->data[--array->top];
	array->data[array->top] = NULL;
	return val;
}

PVALUE array_pop_at(PARRAY array, int index)
{
	PVALUE val;
	int i;
	if (index < 0 || index >= array->top)
	{
		return value_create(NOTHING_TYPE(), base_int(0));
	}

	val = array->data[index];

	for (i = index; i < array->top - 1; i++)
	{
		array->data[i] = array->data[i + 1];
	}
	array->data[--array->top] = NULL;

	return val;
}

void array_append(PARRAY arr1, PARRAY arr2)
{
	int i;
	if (arr1->size < arr1->top + arr2->top)
	{
		array_resize(arr1, arr1->top + arr2->top);
	}
	for (i = 0; i < arr2->top; i++)
	{
		array_push(arr1, value2(arr2->data[i]));
	}
}

void array_reverse(PARRAY array)
{
	int i, j;
	PVALUE tmp;
	for (i = 0, j = array->top - 1; i < j; i++, j--)
	{
		tmp = array->data[i];
		array->data[i] = array->data[j];
		array->data[j] = tmp;
	}
}

PARRAY array_copy(const PARRAY arrIn)
{
	PARRAY arrOut = array_create2(arrIn->top);
	int i;
	PVALUE val;
	PARRAY tmp;
	for (i = 0; i < arrIn->top; i++)
	{
		val = arrIn->data[i];
		if (val->type == ARRAY_TYPE())
		{
			tmp = array_copy(val->val.ptr);
			array_push(arrOut, value(ARRAY_TYPE(), base_voidptr(tmp)));
		}
		else
		{
			array_push(arrOut, value2(val));
		}
	}
	return arrOut;
}