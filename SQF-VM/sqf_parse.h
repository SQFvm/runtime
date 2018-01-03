#ifndef _SQF_PARSE_H_
#define _SQF_PARSE_H_

#ifndef __bool_true_false_are_defined
#error "SQF_types.h" requires stdbool header "stdbool.h"
#endif // !__bool_true_false_are_defined
#ifndef _SQF_H_
#error "SQF_parse.h" has to be included after "SQF.h" 
#endif // !_SQF_H_
#ifndef _SQF_TYPES_H_
#error "SQF_parse.h" has to be included after "SQF_parse.h" 
#endif // !_SQF_TYPES_H_
#ifndef _TEXTRANGE_H_
#error "SQF_parse.h" has to be included after "textrange.h" 
#endif // !_SQF_TYPES_H_


PSTRING parse_string(PVM vm, const wchar_t* str, unsigned int len);
CPCMD fndcmd(PVM vm, const wchar_t* name, unsigned int len);
CPCMD fndcmd2(PVM vm, const wchar_t* name, unsigned int len, unsigned char filter);
void parse_form_code(PVM vm, PSTACK stack, const wchar_t* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter);
void parse_form_array(PVM vm, PSTACK stack, const wchar_t* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter);
void parse_partial(PVM vm, PSTACK stack, const wchar_t* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter);
void parse(PVM vm, const wchar_t* code, bool createscope);
PCODE parse_into_code(PVM vm, const wchar_t* code);
#endif // !_SQF_PARSE_H_
