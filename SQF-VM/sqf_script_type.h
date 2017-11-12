#ifndef _SQF_SCRIPT_TYPE_H_
#define _SQF_SCRIPT_TYPE_H_

#ifndef _SQF_BASE_H_
#error "SQF_script_type.h" has to be included after "string_map.h"
#endif // !_SQF_BASE_H_

typedef struct SCRIPT
{
	PSTACK stack;
	PSTACK work;
	int refcount;
}SCRIPT;
typedef SCRIPT* PSCRIPT;
PCMD SCRIPT_TYPE(void);
PSCRIPT script_create(PVM vm);
void script_destroy(PSCRIPT script);

#endif // !_SQF_SCRIPT_TYPE_H_
