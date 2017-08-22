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
void parse(PVM vm, const char* code);


#endif // !_SQF_PARSE_H_
