#ifndef _SQF_PARSE_H_
#define _SQF_PARSE_H_

#ifndef _SQF_H_
#error "SQF_parse.h" has to be included after "SQF.h" 
#endif // !_SQF_H_
#ifndef _SQF_TYPES_H_
#error "SQF_parse.h" has to be included after "SQF_parse.h" 
#endif // !_SQF_TYPES_H_
#ifndef _TEXTRANGE_H_
#error "SQF_parse.h" has to be included after "textrange.h" 
#endif // !_SQF_TYPES_H_



void tokenize(TR_ARR* arr, const char* code);
CPCMD fndcmd(PVM vm, const char* name, unsigned int len);
CPCMD fndcmd2(PVM vm, const char* name, unsigned int len, unsigned char filter);
void parse_form_code(PVM vm, PSTACK stack, const char* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter);
void parse_form_array(PVM vm, PSTACK stack, const char* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter);
void parse_partial(PVM vm, PSTACK stack, const char* code, TR_ARR* arr, unsigned int arr_start, unsigned int arr_end, unsigned int* stack_counter);
void parse(PVM vm, const char* code, unsigned char createscope);
PCODE parse_into_code(PVM vm, const char* code);


#endif // !_SQF_PARSE_H_
