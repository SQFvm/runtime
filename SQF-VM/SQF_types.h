#ifndef _SQF_TYPES_H_
#define _SQF_TYPES_H_

#ifndef __bool_true_false_are_defined
#error "SQF_types.h" requires stdbool header "stdbool.h"
#endif // !__bool_true_false_are_defined
#ifndef _SQF_H_
#error "SQF_types.h" has to be included after "SQF.h" 
#endif // !_SQF_H_
#ifndef _WSTRING_MAP_H_
#error "SQF_types.h" has to be included after "wstring_map.h" 
#endif // !_WSTRING_MAP_H_

//SQF.c --> sqfvm | Add TYPE command and if required deletion callback

#define STRING_MAX_LENGTH 9999999

struct ARRAY;
typedef struct ARRAY* PARRAY;


bool is_equal_to(PVALUE l, PVALUE r);

PCMD SCALAR_TYPE(void);
PCMD BOOL_TYPE(void);
PCMD IF_TYPE(void);
PCMD WHILE_TYPE(void);
PCMD NOTHING_TYPE(void);
PCMD ANY_TYPE(void);
PCMD NAN_TYPE(void);

typedef struct CODE
{
	wchar_t* val;
	unsigned int length;
	int refcount;
	PSTACK stack;
} CODE;
typedef CODE* PCODE;
PCMD CODE_TYPE(void);
PCODE code_create(const wchar_t* txt, int offset, int len);
void code_destroy(PCODE code);

typedef struct FOR
{
	wchar_t* variable;
	unsigned int variable_length;
	int start;
	int end;
	float step;
	float current;
	int refcount;
	bool started;
} FOR;
PCMD FOR_TYPE(void);
typedef FOR* PFOR;
PFOR for_create(const wchar_t* varname);
void for_destroy(PFOR f);

typedef struct NAMESPACE
{
	wsm_list* data;
	int refcount;
} NAMESPACE;
PCMD NAMESPACE_TYPE(void);
PCMD WITH_TYPE(void);
typedef NAMESPACE* PNAMESPACE;
PNAMESPACE namespace_create(void);
void namespace_destroy(PNAMESPACE namespace);
void namespace_set_var(PNAMESPACE namespace, const wchar_t* var, VALUE val);
PVALUE namespace_get_var(PNAMESPACE namespace, const wchar_t* var);

PNAMESPACE sqf_missionNamespace(void);
PNAMESPACE sqf_uiNamespace(void);
PNAMESPACE sqf_profileNamespace(void);
PNAMESPACE sqf_parsingNamespace(void);

typedef struct SWITCH
{
	int refcount;
	PVALUE default_code;
	PVALUE switch_value;
	PVALUE selected_code;
	bool was_executed;
} SWITCH;
typedef SWITCH* PSWITCH;
PCMD SWITCH_TYPE(void);
PSWITCH switch_create(VALUE val);
void switch_destroy(PSWITCH swtch);

typedef struct GROUP
{
	int refcount;
	PVALUE side;
	PVALUE members;
	wchar_t* ident;
	unsigned int ident_len;
} GROUP;
typedef GROUP* PGROUP;
PCMD GROUP_TYPE(void);
PGROUP group_create(int side);
void group_destroy(PGROUP group);
PVALUE group_get_leader(PGROUP group);
PGROUP group_from_ident(PVM vm, const wchar_t* ident);

//NON-SQF compliant types

typedef struct COUNT
{
	PVALUE code;
	PVALUE arr;
	unsigned int curtop;
	unsigned int count;
	int refcount;
} COUNT;
typedef COUNT* PCOUNT;
PCMD COUNT_TYPE(void);
PCOUNT count_create(PCODE code, PARRAY arr);
void count_destroy(PCOUNT count);

#endif // !_SQF_TYPES_H_
