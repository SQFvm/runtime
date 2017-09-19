#ifndef _SQF_SIDE_TYPE_H_
#define _SQF_SIDE_TYPE_H_


#ifndef _SQF_BASE_H_
#error "SQF_side_types.h" has to be included after "SQF_base.h" 
#endif // !_SQF_BASE_H_
#ifndef _SQF_TYPES_H_
#error "SQF_side_types.h" has to be included after "SQF_types.h" 
#endif // !_SQF_TYPES_H_


typedef int side;

#define SIDE_STR_CIV "CIV"
#define SIDE_STR_BLU "WEST"
#define SIDE_STR_OPF "EAST"
#define SIDE_STR_GUE "GUER"
#define SIDE_STR_LOGIC "LOGIC"
#define SIDE_STR_ENEMY "ENEMY"
#define SIDE_STR_FRIENDLY "FRIENDLY"
#define SIDE_STR_AMBIENT "AMBIENT LIFE"
#define SIDE_STR_EMPTY "EMPTY"
#define SIDE_STR_UNKNOWN "UNKNOWN"

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
const char* side_displayname(const PVALUE val);
unsigned char** side_init_sidemap(void);
void side_destroy_sidemap(unsigned char** map);

inline void side_set_firendly(PVM vm, const PVALUE self, const PVALUE other, unsigned char flag) { vm->sidemap[self->val.i][other->val.i] = flag; }
inline unsigned char side_equals(const PVALUE left, const PVALUE right) { return left->val.i == right->val.i; }
inline unsigned char side_is_friendly(const PVM vm, const PVALUE self, const PVALUE other) { return vm->sidemap[self->val.i][other->val.i]; }
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
