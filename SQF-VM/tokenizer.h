#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#ifndef _TEXTRANGE_H_
#error "tokenizer.h" has to be included after "textrange.h" 
#endif // !_SQF_TYPES_H_

void tokenize(TR_ARR* arr, const wchar_t* code);
#endif