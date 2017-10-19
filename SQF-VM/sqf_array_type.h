#ifndef _SQF_ARRAY_TYPE_H_
#define _SQF_ARRAY_TYPE_H_

#ifndef __bool_true_false_are_defined
#error "SQF_types.h" requires stdbool header "stdbool.h"
#endif // !__bool_true_false_are_defined
#ifndef _SQF_H_
#error "SQF_types.h" has to be included after "SQF.h" 
#endif // !_SQF_H_
#ifndef _WSTRING_MAP_H_
#error "SQF_types.h" has to be included after "wstring_map.h" 
#endif // !_WSTRING_MAP_H_

#define ARRAY_DEFAULT_INC 10
typedef struct ARRAY
{
	PVALUE* data;
	unsigned int size;
	unsigned int top;
	int refcount;
} ARRAY;
PCMD ARRAY_TYPE(void);
typedef ARRAY* PARRAY;

PARRAY array_create(void);
PARRAY array_create2(unsigned int initialsize);
void array_destroy(PARRAY arr);
void array_resize(PARRAY arr, unsigned int newsize);
//Adjusts theoretical size of array (ARRAY.top) to desired size.
//Will take care that provided array will have required practical size
//to contain theoretical size.
//Empty slots will be filled with NOTHING values.
void array_resize_top(PARRAY arr, unsigned int newsize);
void array_push(PARRAY arr, VALUE val);
int array_index_of(const PARRAY array, const PVALUE value);
PVALUE array_pop(PARRAY array);
PVALUE array_pop_at(PARRAY array, int index);
void array_reverse(PARRAY array);
//Appends content of arr2 into arr1
void array_append(PARRAY arr1, PARRAY arr2);
PARRAY array_copy(const PARRAY arrIn);


/**
* Calculates the dot product of the given arrays that are interpreted as 3D vectors.
* There is no checking performed whether the given values are
* actually arrays or that they have the proper size so make sure of that before
* calling this function!
*/
inline float dotProductPointer(const PARRAY leftArray, const PARRAY rightArray)
{
	return leftArray->data[0]->val.f * rightArray->data[0]->val.f
		+ leftArray->data[1]->val.f * rightArray->data[1]->val.f
		+ leftArray->data[2]->val.f * rightArray->data[2]->val.f;
}

/**
* Calculates the dot product of the given arrays that are interpreted as 3D vectors.
* There is no checking performed whether the given values are
* actually arrays or that they have the proper size so make sure of that before
* calling this function!
*/
inline float dotProduct(const ARRAY leftArray, const ARRAY rightArray) { return dotProductPointer((PARRAY)&leftArray, (PARRAY)&rightArray); }

/**
* Calculates the dot product of the given arrays that are interpreted as 3D vectors.
* There is no checking performed whether the given values are
* actually arrays or that they have the proper size so make sure of that before
* calling this function!
*/
inline VALUE dotProductPointer_Value(const PARRAY leftArray, const PARRAY rightArray) { return value(SCALAR_TYPE(), base_float(dotProductPointer(leftArray, rightArray))); }

/**
* Calculates the dot product of the given arrays that are interpreted as 3D vectors.
* There is no checking performed whether the given values are
* actually arrays or that they have the proper size so make sure of that before
* calling this function!
*/
inline VALUE dotProduct_Value(const ARRAY leftArray, const ARRAY rightArray) { return value(SCALAR_TYPE(), base_float(dotProduct(leftArray, rightArray))); }

/**
* Calculates the magnitude of a 3D vector
* There is no checking performed whether the given values are
* actually arrays or that they have the proper size so make sure of that before
* calling this function!
*/
inline float vectorMagnitudePointer(const PARRAY array) { return powf(powf(array->data[0]->val.f, 2) + powf(array->data[1]->val.f, 2) + powf(array->data[2]->val.f, 2), (1.0 / 2.0)); }

/**
* Calculates the magnitude of a 3D vector
* There is no checking performed whether the given values are
* actually arrays or that they have the proper size so make sure of that before
* calling this function!
*/
inline float vectorMagnitude(const ARRAY array) { return vectorMagnitudePointer((PARRAY)&array); }

#endif // !_SQF_ARRAY_TYPE_H_
