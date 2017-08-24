#ifndef _SQF_H_
#define _SQF_H_

#include "SQF_base.h"
#include "SQF_inst.h"

PVALUE find_var(PVM vm, const char* name);
void set_var(PVM vm, const char* name, VALUE val);
PSCOPE top_scope(PVM vm);
void store_in_scope(PVM vm, PSCOPE scope, const char* name, VALUE val);

void execute(PVM vm);

PCMD create_command(const char* name, char type, CMD_CB fnc, char precedence, const char* desc);
void destroy_command(PCMD command);

PCMD find_command(PVM vm, const char* name, char type);
PCMD find_type(PVM vm, const char* name);

#endif