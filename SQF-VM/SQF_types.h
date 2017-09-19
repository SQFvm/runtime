#ifndef _SQF_TYPES_H_
#define _SQF_TYPES_H_


#ifndef _SQF_H_
#error "SQF_types.h" has to be included after "SQF.h" 
#endif // !_SQF_H_
#ifndef _STRING_MAP_H_
#error "SQF_types.h" has to be included after "string_map.h" 
#endif // !_STRING_MAP_H_

//SQF.c --> sqfvm | Add TYPE command and if required deletion callback

PCMD SCALAR_TYPE(void);
PCMD BOOL_TYPE(void);
PCMD IF_TYPE(void);
PCMD WHILE_TYPE(void);
PCMD NOTHING_TYPE(void);
PCMD ANY_TYPE(void);
PCMD NAN_TYPE(void);



typedef struct CODE
{
	char* val;
	unsigned int length;
	int refcount;
	PSTACK stack;
}CODE;
typedef CODE* PCODE;
PCMD CODE_TYPE(void);
PCODE code_create(const char* txt, int offset, int len);
void code_destroy(PCODE code);

typedef struct STRING
{
	char* val;
	unsigned int length;
	int refcount;
}STRING;
PCMD STRING_TYPE(void);
typedef STRING* PSTRING;

///Creates a new STRING object with given length. STRING will not be initialized!
PSTRING string_create(unsigned int len);
///Creates a new STRING object and initializes it with given c string
PSTRING string_create2(const char* str);
///Destroys an existing STRING object
void string_destroy(PSTRING string);
///Concatenates the two STRING objects and returns a new STRING object.
PSTRING string_concat(const PSTRING l, const PSTRING r);
///Will take given range from provided STRING object and create a new STRING object
PSTRING string_substring(const PSTRING string, unsigned int start, int length);
///Appends provided cstring onto STRING object
void string_modify_append(PSTRING string, const char* append);
void string_modify_append2(PSTRING string, int len);
///Appends n characters of provided cstring onto STRING object
void string_modify_nappend(PSTRING string, const char* append, unsigned int len);

#define ARRAY_DEFAULT_INC 10
typedef struct ARRAY
{
	PVALUE* data;
	unsigned int size;
	unsigned int top;
	int refcount;
}ARRAY;
PCMD ARRAY_TYPE(void);
typedef ARRAY* PARRAY;

PARRAY array_create(void);
PARRAY array_create2(unsigned int initialsize);
void array_destroy(PARRAY arr);
void array_resize(PARRAY arr, unsigned int newsize);
void array_push(PARRAY arr, VALUE val);
PARRAY array_copy(const PARRAY arrIn);


typedef struct FOR
{
	char* variable;
	unsigned int variable_length;
	int start;
	int end;
	float step;
	float current;
	int refcount;
	unsigned char started;
}FOR;
PCMD FOR_TYPE(void);
typedef FOR* PFOR;
PFOR for_create(const char* varname);
void for_destroy(PFOR f);


typedef struct NAMESPACE
{
	sm_list* data;
	int refcount;
}NAMESPACE;
PCMD NAMESPACE_TYPE(void);
typedef NAMESPACE* PNAMESPACE;
PNAMESPACE namespace_create(void);
void namespace_destroy(PNAMESPACE namespace);
void namespace_set_var(PNAMESPACE namespace, const char* var, VALUE val);
PVALUE namespace_get_var(PNAMESPACE namespace, const char* var);

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
	unsigned char was_executed;
}SWITCH;
typedef SWITCH* PSWITCH;
PCMD SWITCH_TYPE(void);
PSWITCH switch_create(VALUE val);
void switch_destroy(PSWITCH swtch);


typedef struct GROUP
{
	int refcount;
	PVALUE side;
	PVALUE members;
}GROUP;
typedef GROUP* PGROUP;
PCMD GROUP_TYPE(void);
PGROUP group_create(int side);
void group_destroy(PGROUP code);



//NON-SQF compliant types


typedef struct COUNT
{
	PVALUE code;
	PVALUE arr;
	unsigned int curtop;
	unsigned int count;
	int refcount;
}COUNT;
typedef COUNT* PCOUNT;
PCMD COUNT_TYPE(void);
PCOUNT count_create(PCODE code, PARRAY arr);
void count_destroy(PCOUNT count);




#endif // !_SQF_TYPES_H_
