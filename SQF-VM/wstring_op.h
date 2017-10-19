#ifndef _WSTRING_OP_H_
#define _WSTRING_OP_H_

/*
Checks if provided string starts with other string.
Returns 0 if true or the index + 1 where the difference was found.

All parameters need to be a \0 terminated string!

param 1: String to check
param 2: String to find
*/
unsigned int wstr_sw(const wchar_t*, const wchar_t*);
/*
Checks if provided string ends with other string.
Returns 1 if lString ends with rString.
Will return 0 in any other case.

All parameters need to be a \0 terminated string!

param 1: String to check
param 2: String to find
*/
unsigned int wstr_ew(const wchar_t*, const wchar_t*);
/*
Checks if provided string starts with other string.
Returns 0 if true or the index + 1 where the difference was found.
Comparison is case insensitive.

All parameters need to be a \0 terminated string!

param 1: String to check
param 2: String to find
*/
unsigned int wstr_swi(const wchar_t*, const wchar_t*);
/*
Checks if provided string ends with other string.
Returns 1 if lString ends with rString.
Will return 0 in any other case.
Comparison is case insensitive.

All parameters need to be a \0 terminated string!

param 1: String to check
param 2: String to find
*/
unsigned int wstr_ewi(const wchar_t*, const wchar_t*);

/*
Checks if provided word is inside of source string.
Third parameter allows for providing word separators
Returns NULL ptr if string was not found, ptr to the string if it was

all parameters need to be a \0 terminated string!

param 1: String to check
param 2: String to find
param 3: Characters terminating or NULL for default: " ,-_\t"
*/
const wchar_t* wstr_strwrd(const wchar_t*, const wchar_t*, const wchar_t*);

/*
Returns NULL ptr if string was not found, ptr to the string if it was
Comparison is case insensitive.

all parameters need to be a \0 terminated string!

param 1: String to check
param 2: String to find
param 3: Characters terminating or NULL for default: " ,-_\t.?!+:;<>#"
*/
const wchar_t* wstr_strwrdi(const wchar_t*, const wchar_t*, const wchar_t*);

/*
Returns 1 if provided character is in character array, 0 in any other case.

param 1: Character to check
param 2: Characters which are valid, \0 Terminated
*/
int wchr_is(const wchar_t, const wchar_t*);


/*
Replaces provided char in given string with a different one.
Returns the count of replaced chars.

param 1: String to replace
param 2: Character to search
param 3: Character to replace
param 4: -1 or length of the string
*/
unsigned int wstr_repchr(wchar_t*, wchar_t, wchar_t, int);

/*
Checks whether provided strings are equal.
Returns 0 if strings are equal.

param 1: String 1 to check
param 2: -1 or length of string 1
param 3: String 2 to compare against
param 4: -1 or length of string 2
*/
unsigned int wstr_cmp(const wchar_t*, int, const wchar_t*, int);
/*
Checks whether provided strings are equal.
Comparison is case insensitive.
Returns 0 if strings are equal.

param 1: String 1 to check
param 2: -1 or length of string 1
param 3: String 2 to compare against
param 4: -1 or length of string 2
*/
unsigned int wstr_cmpi(const wchar_t*, int, const wchar_t*, int);


#endif // !_WSTRING_OP_H_
