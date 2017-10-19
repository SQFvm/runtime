#ifndef _SQF_SIDE_TYPE_H_
#define _SQF_SIDE_TYPE_H_


#ifndef _SQF_BASE_H_
#error "SQF_side_types.h" has to be included after "SQF_base.h" 
#endif // !_SQF_BASE_H_
#ifndef _SQF_TYPES_H_
#error "SQF_side_types.h" has to be included after "SQF_types.h" 
#endif // !_SQF_TYPES_H_


typedef int side;

#define SIDE_WSTR_CIV L"CIV"
#define SIDE_WSTR_BLU L"WEST"
#define SIDE_WSTR_OPF L"EAST"
#define SIDE_WSTR_GUE L"GUER"
#define SIDE_WSTR_LOGIC L"LOGIC"
#define SIDE_WSTR_ENEMY L"ENEMY"
#define SIDE_WSTR_FRIENDLY L"FRIENDLY"
#define SIDE_WSTR_AMBIENT L"AMBIENT LIFE"
#define SIDE_WSTR_EMPTY L"EMPTY"
#define SIDE_WSTR_UNKNOWN L"UNKNOWN"

#define SIDE_VAL_EMPTY ((side)0)
#define SIDE_VAL_UNKNOWN ((side)1)
#define SIDE_VAL_CIV ((side)2)
#define SIDE_VAL_BLU ((side)3)
#define SIDE_VAL_OPF ((side)4)
#define SIDE_VAL_GUE ((side)5)
#define SIDE_VAL_LOGIC ((side)6)
#define SIDE_VAL_ENEMY ((side)7)
#define SIDE_VAL_FRIENDLY ((side)8)
#define SIDE_VAL_AMBIENT ((side)9)


PCMD SIDE_TYPE(void);
const wchar_t* side_displayname(int i);
const wchar_t* side_displayname2(const PVALUE val);
unsigned char** side_init_sidemap(void);
void side_destroy_sidemap(unsigned char** map);

inline void side_set_firendly(PVM vm, const PVALUE self, const PVALUE other, bool flag) { vm->sidemap[self->val.i][other->val.i] = flag; }
inline bool side_equals(const PVALUE left, const PVALUE right) { return left->val.i == right->val.i; }
inline bool side_is_friendly(const PVM vm, const PVALUE self, const PVALUE other) { return vm->sidemap[self->val.i][other->val.i]; }
inline VALUE side_empty(void) { return value(SIDE_TYPE(), base_int(SIDE_VAL_EMPTY)); }
inline VALUE side_unknown(void) { return value(SIDE_TYPE(), base_int(SIDE_VAL_UNKNOWN)); }
inline VALUE side_civ(void) { return value(SIDE_TYPE(), base_int(SIDE_VAL_CIV)); }
inline VALUE side_blu(void) { return value(SIDE_TYPE(), base_int(SIDE_VAL_BLU)); }
inline VALUE side_opf(void) { return value(SIDE_TYPE(), base_int(SIDE_VAL_OPF)); }
inline VALUE side_gue(void) { return value(SIDE_TYPE(), base_int(SIDE_VAL_GUE)); }
inline VALUE side_logic(void) { return value(SIDE_TYPE(), base_int(SIDE_VAL_LOGIC)); }
inline VALUE side_enemy(void) { return value(SIDE_TYPE(), base_int(SIDE_VAL_ENEMY)); }
inline VALUE side_friendly(void) { return value(SIDE_TYPE(), base_int(SIDE_VAL_FRIENDLY)); }
inline VALUE side_ambient(void) { return value(SIDE_TYPE(), base_int(SIDE_VAL_AMBIENT)); }


#endif // !_SQF_SIDE_TYPE_H_
