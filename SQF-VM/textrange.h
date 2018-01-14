#ifndef _TEXTRANGE_H_
#define _TEXTRANGE_H_


typedef struct TEXTRANGE
{
	unsigned int start;
	unsigned int length;
	unsigned int line;
	unsigned int col;
}TEXTRANGE;

typedef struct TR_ARR
{
	TEXTRANGE* data;
	unsigned int size;
	unsigned int top;
	unsigned int increase;
}TR_ARR;

TR_ARR* tr_arr_create(void);
void tr_arr_destroy(TR_ARR* arr);
void tr_arr_resize(TR_ARR* arr, unsigned int newsize);
TEXTRANGE tr_arr_get(TR_ARR* arr, unsigned int index);
void tr_arr_set(TR_ARR* arr, unsigned int index, TEXTRANGE range);
void tr_arr_push(TR_ARR* arr, TEXTRANGE range);

#endif // !_TEXTRANGE_H_
