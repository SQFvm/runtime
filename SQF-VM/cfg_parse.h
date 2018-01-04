#ifndef _CFG_PARSE_H_
#define _CFG_PARSE_H_

#ifndef _CFG_BASE_H_
#error "cfg_parse.h" has to be included after "cfg_base.h" 
#endif // !_CFG_BASE_H_
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


bool cfgparse_isident(const wchar_t* str, unsigned int len);
void cfgparse_nodelist(PVM vm, PCONFIG root, TR_ARR* arr, const wchar_t* code, unsigned int *index);
void cfgparse_node(PVM vm, PCONFIG root, TR_ARR* arr, const wchar_t* code, unsigned int *index);
void cfgparse_confignode(PVM vm, PCONFIG root, TR_ARR* arr, const wchar_t* code, unsigned int *index);
void cfgparse_valuenode(PVM vm, PCONFIG root, TR_ARR* arr, const wchar_t* code, unsigned int *index);
bool cfgparse_string_start(const wchar_t* code, unsigned int len);
void cfgparse_string(PVM vm, VALUE *out, TR_ARR* arr, const wchar_t* code, unsigned int *index);
bool cfgparse_number_start(const wchar_t* code, unsigned int len);
void cfgparse_number(PVM vm, VALUE *out, TR_ARR* arr, const wchar_t* code, unsigned int *index);
bool cfgparse_localization_start(const wchar_t* code, unsigned int len);
void cfgparse_localization(PVM vm, VALUE *out, TR_ARR* arr, const wchar_t* code, unsigned int *index);
bool cfgparse_array_start(const wchar_t* code, unsigned int len);
void cfgparse_array(PVM vm, VALUE *out, TR_ARR* arr, const wchar_t* code, unsigned int *index);
bool cfgparse_value_start(const wchar_t* code, unsigned int len);
void cfgparse_value(PVM vm, VALUE *out, TR_ARR* arr, const wchar_t* code, unsigned int *index);
PCONFIG cfgparse(PVM vm, const wchar_t* code);
PCONFIG cfgparse2(PVM vm, const wchar_t* code, unsigned int tr_arr_increase, unsigned int node_init_len);
#endif // !_CFG_BASE_H_
