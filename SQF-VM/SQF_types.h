#ifndef _SQF_TYPES_H_
#define _SQF_TYPES_H_


#ifndef _SQF_H_
#error "SQF_parse.h" has to be included after "SQF.h" 
#endif // !_SQF_H_

//SQF.c --> sqfvm | Add TYPE command and if required deletion callback


typedef struct CODE
{
	PSTACK stack;
	int refcount;
}CODE;
typedef CODE* PCODE;
PCMD CODE_TYPE(void);
PCODE code_create(int initial_size);
void code_destroy(PCODE code);

typedef struct STRING
{
	char* val;
	unsigned int length;
	int refcount;
}STRING;
PCMD STRING_TYPE(void);
typedef STRING* PSTRING;
typedef const STRING* CPSTRING;

///Creates a new STRING object with given length. STRING will not be initialized!
PSTRING string_create(unsigned int len);
///Creates a new STRING object and initializes it with given c string
PSTRING string_create2(const char* str);
///Destroys an existing STRING object
void string_destroy(PSTRING string);
///Concatenates the two STRING objects and returns a new STRING object.
PSTRING string_concat(CPSTRING l, CPSTRING r);
///Will take given range from provided STRING object and create a new STRING object
PSTRING string_substring(CPSTRING string, unsigned int start, int length);




#endif // !_SQF_TYPES_H_
