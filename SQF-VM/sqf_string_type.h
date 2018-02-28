#ifndef _SQF_STRING_TYPE_H_
#define _SQF_STRING_TYPE_H_

#ifndef __bool_true_false_are_defined
#error "sqf_string_type.h" requires stdbool header "stdbool.h"
#endif // !__bool_true_false_are_defined

#ifndef _SQF_TYPES_H_
#error "sqf_string_type.h" has to be included after "SQF_types.h" 
#endif // !_SQF_TYPES_H_

typedef struct STRING
{
	wchar_t* val;
	unsigned int length;
	int refcount;
} STRING;
PCMD STRING_TYPE(void);
typedef STRING* PSTRING;

//Creates a new STRING object with given length. STRING will not be initialized!
PSTRING string_create(unsigned int len);
//Creates a new STRING object and initializes it with given c string
PSTRING string_create2(const wchar_t* str);
//Destroys an existing STRING object
void string_destroy(PSTRING string);
//Concatenates the two STRING objects and returns a new STRING object.
PSTRING string_concat(const PSTRING l, const PSTRING r);
//Will take given range from provided STRING object and create a new STRING object
PSTRING string_substring(const PSTRING string, unsigned int start, int length);
void string_resize(PSTRING string, int change);
//Appends provided cstring onto STRING object
void string_modify_append(PSTRING string, const wchar_t* append);
void string_modify_append2(PSTRING string, int len);
//Appends n characters of provided cstring onto STRING object
void string_modify_nappend(PSTRING string, const wchar_t* append,
	unsigned int len);

#endif // !_SQF_STRING_TYPE_H_
