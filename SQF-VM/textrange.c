#include "textrange.h"
#include <stdlib.h>

TR_ARR* tr_arr_create(void)
{
	TR_ARR* arr = malloc(sizeof(TR_ARR));
	arr->data = malloc(sizeof(TEXTRANGE) * TR_ARR_INC);
	arr->size = TR_ARR_INC;
	arr->top = 0;
	return arr;
}
void tr_arr_destroy(TR_ARR* arr)
{
	free(arr->data);
	free(arr);
}
void tr_arr_resize(TR_ARR* arr, unsigned int newsize)
{
	TEXTRANGE* tr = realloc(arr->data, sizeof(TEXTRANGE) * newsize);
	if (tr != 0)
		arr->data = tr;
	arr->size = newsize;
}
TEXTRANGE tr_arr_get(TR_ARR* arr, unsigned int index)
{
	if (index >= arr->top)
	{
		return (TEXTRANGE)
		{
			.start = 0, .length = 0
		};
	}
	return arr->data[index];
}
void tr_arr_set(TR_ARR* arr, unsigned int index, TEXTRANGE range)
{
	if (index >= arr->size)
	{
		tr_arr_resize(arr, arr->size + TR_ARR_INC);
	}
	arr->data[index] = range;
	if (arr->top < index)
		arr->top = index;
}
void tr_arr_push(TR_ARR* arr, TEXTRANGE range)
{
	if (arr->top >= arr->size)
	{
		tr_arr_resize(arr, arr->size + TR_ARR_INC);
	}
	arr->data[arr->top++] = range;
}