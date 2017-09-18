#include "basetype.h"
#include "bool.h"
#include "string_map.h"
#include "SQF.h"
#include "SQF_base.h"
#include "SQF_types.h"
#include "SQF_side_type.h"



PCMD SIDE_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command("SIDE", 't', 0, 0, 0);
	}
	return cmd;
}
const char* side_displayname(const PVALUE val)
{
	switch (val->val.i)
	{
	case SIDE_VAL_EMPTY: return SIDE_STR_EMPTY;
	case SIDE_VAL_UNKNOWN: return SIDE_STR_UNKNOWN;
	case SIDE_VAL_CIV: return SIDE_STR_CIV;
	case SIDE_VAL_BLU: return SIDE_STR_BLU;
	case SIDE_VAL_OPF: return SIDE_STR_OPF;
	case SIDE_VAL_GUE: return SIDE_STR_GUE;
	case SIDE_VAL_LOGIC: return SIDE_STR_LOGIC;
	case SIDE_VAL_ENEMY: return SIDE_STR_ENEMY;
	case SIDE_VAL_FRIENDLY: return SIDE_STR_FRIENDLY;
	case SIDE_VAL_AMBIENT: return SIDE_STR_AMBIENT;
	default:
		return 0;
	}
}
void side_init_sidemap(struct sidemap *map)
{
	map->map[SIDE_VAL_EMPTY][SIDE_VAL_EMPTY] = false;
	map->map[SIDE_VAL_EMPTY][SIDE_VAL_UNKNOWN] = false;
	map->map[SIDE_VAL_EMPTY][SIDE_VAL_CIV] = false;
	map->map[SIDE_VAL_EMPTY][SIDE_VAL_BLU] = false;
	map->map[SIDE_VAL_EMPTY][SIDE_VAL_OPF] = false;
	map->map[SIDE_VAL_EMPTY][SIDE_VAL_GUE] = false;
	map->map[SIDE_VAL_EMPTY][SIDE_VAL_LOGIC] = false;
	map->map[SIDE_VAL_EMPTY][SIDE_VAL_ENEMY] = false;
	map->map[SIDE_VAL_EMPTY][SIDE_VAL_FRIENDLY] = true;
	map->map[SIDE_VAL_EMPTY][SIDE_VAL_AMBIENT] = true;
	map->map[SIDE_VAL_UNKNOWN][SIDE_VAL_EMPTY] = false;
	map->map[SIDE_VAL_UNKNOWN][SIDE_VAL_UNKNOWN] = false;
	map->map[SIDE_VAL_UNKNOWN][SIDE_VAL_CIV] = false;
	map->map[SIDE_VAL_UNKNOWN][SIDE_VAL_BLU] = false;
	map->map[SIDE_VAL_UNKNOWN][SIDE_VAL_OPF] = false;
	map->map[SIDE_VAL_UNKNOWN][SIDE_VAL_GUE] = false;
	map->map[SIDE_VAL_UNKNOWN][SIDE_VAL_LOGIC] = false;
	map->map[SIDE_VAL_UNKNOWN][SIDE_VAL_ENEMY] = false;
	map->map[SIDE_VAL_UNKNOWN][SIDE_VAL_FRIENDLY] = true;
	map->map[SIDE_VAL_UNKNOWN][SIDE_VAL_AMBIENT] = true;
	map->map[SIDE_VAL_CIV][SIDE_VAL_EMPTY] = true;
	map->map[SIDE_VAL_CIV][SIDE_VAL_UNKNOWN] = false;
	map->map[SIDE_VAL_CIV][SIDE_VAL_CIV] = true;
	map->map[SIDE_VAL_CIV][SIDE_VAL_BLU] = true;
	map->map[SIDE_VAL_CIV][SIDE_VAL_OPF] = false;
	map->map[SIDE_VAL_CIV][SIDE_VAL_GUE] = true;
	map->map[SIDE_VAL_CIV][SIDE_VAL_LOGIC] = false;
	map->map[SIDE_VAL_CIV][SIDE_VAL_ENEMY] = false;
	map->map[SIDE_VAL_CIV][SIDE_VAL_FRIENDLY] = true;
	map->map[SIDE_VAL_CIV][SIDE_VAL_AMBIENT] = true;
	map->map[SIDE_VAL_BLU][SIDE_VAL_EMPTY] = true;
	map->map[SIDE_VAL_BLU][SIDE_VAL_UNKNOWN] = false;
	map->map[SIDE_VAL_BLU][SIDE_VAL_CIV] = true;
	map->map[SIDE_VAL_BLU][SIDE_VAL_BLU] = true;
	map->map[SIDE_VAL_BLU][SIDE_VAL_OPF] = false;
	map->map[SIDE_VAL_BLU][SIDE_VAL_GUE] = true;
	map->map[SIDE_VAL_BLU][SIDE_VAL_LOGIC] = false;
	map->map[SIDE_VAL_BLU][SIDE_VAL_ENEMY] = false;
	map->map[SIDE_VAL_BLU][SIDE_VAL_FRIENDLY] = true;
	map->map[SIDE_VAL_BLU][SIDE_VAL_AMBIENT] = true;
	map->map[SIDE_VAL_OPF][SIDE_VAL_EMPTY] = false;
	map->map[SIDE_VAL_OPF][SIDE_VAL_UNKNOWN] = false;
	map->map[SIDE_VAL_OPF][SIDE_VAL_CIV] = true;
	map->map[SIDE_VAL_OPF][SIDE_VAL_BLU] = false;
	map->map[SIDE_VAL_OPF][SIDE_VAL_OPF] = true;
	map->map[SIDE_VAL_OPF][SIDE_VAL_GUE] = false;
	map->map[SIDE_VAL_OPF][SIDE_VAL_LOGIC] = false;
	map->map[SIDE_VAL_OPF][SIDE_VAL_ENEMY] = false;
	map->map[SIDE_VAL_OPF][SIDE_VAL_FRIENDLY] = true;
	map->map[SIDE_VAL_OPF][SIDE_VAL_AMBIENT] = true;
	map->map[SIDE_VAL_GUE][SIDE_VAL_EMPTY] = false;
	map->map[SIDE_VAL_GUE][SIDE_VAL_UNKNOWN] = false;
	map->map[SIDE_VAL_GUE][SIDE_VAL_CIV] = true;
	map->map[SIDE_VAL_GUE][SIDE_VAL_BLU] = true;
	map->map[SIDE_VAL_GUE][SIDE_VAL_OPF] = false;
	map->map[SIDE_VAL_GUE][SIDE_VAL_GUE] = true;
	map->map[SIDE_VAL_GUE][SIDE_VAL_LOGIC] = false;
	map->map[SIDE_VAL_GUE][SIDE_VAL_ENEMY] = false;
	map->map[SIDE_VAL_GUE][SIDE_VAL_FRIENDLY] = true;
	map->map[SIDE_VAL_GUE][SIDE_VAL_AMBIENT] = true;
	map->map[SIDE_VAL_LOGIC][SIDE_VAL_EMPTY] = false;
	map->map[SIDE_VAL_LOGIC][SIDE_VAL_UNKNOWN] = false;
	map->map[SIDE_VAL_LOGIC][SIDE_VAL_CIV] = true;
	map->map[SIDE_VAL_LOGIC][SIDE_VAL_BLU] = true;
	map->map[SIDE_VAL_LOGIC][SIDE_VAL_OPF] = true;
	map->map[SIDE_VAL_LOGIC][SIDE_VAL_GUE] = true;
	map->map[SIDE_VAL_LOGIC][SIDE_VAL_LOGIC] = false;
	map->map[SIDE_VAL_LOGIC][SIDE_VAL_ENEMY] = false;
	map->map[SIDE_VAL_LOGIC][SIDE_VAL_FRIENDLY] = true;
	map->map[SIDE_VAL_LOGIC][SIDE_VAL_AMBIENT] = true;
	map->map[SIDE_VAL_ENEMY][SIDE_VAL_EMPTY] = false;
	map->map[SIDE_VAL_ENEMY][SIDE_VAL_UNKNOWN] = false;
	map->map[SIDE_VAL_ENEMY][SIDE_VAL_CIV] = false;
	map->map[SIDE_VAL_ENEMY][SIDE_VAL_BLU] = false;
	map->map[SIDE_VAL_ENEMY][SIDE_VAL_OPF] = false;
	map->map[SIDE_VAL_ENEMY][SIDE_VAL_GUE] = false;
	map->map[SIDE_VAL_ENEMY][SIDE_VAL_LOGIC] = false;
	map->map[SIDE_VAL_ENEMY][SIDE_VAL_ENEMY] = false;
	map->map[SIDE_VAL_ENEMY][SIDE_VAL_FRIENDLY] = true;
	map->map[SIDE_VAL_ENEMY][SIDE_VAL_AMBIENT] = true;
	map->map[SIDE_VAL_FRIENDLY][SIDE_VAL_EMPTY] = false;
	map->map[SIDE_VAL_FRIENDLY][SIDE_VAL_UNKNOWN] = false;
	map->map[SIDE_VAL_FRIENDLY][SIDE_VAL_CIV] = false;
	map->map[SIDE_VAL_FRIENDLY][SIDE_VAL_BLU] = false;
	map->map[SIDE_VAL_FRIENDLY][SIDE_VAL_OPF] = false;
	map->map[SIDE_VAL_FRIENDLY][SIDE_VAL_GUE] = false;
	map->map[SIDE_VAL_FRIENDLY][SIDE_VAL_LOGIC] = false;
	map->map[SIDE_VAL_FRIENDLY][SIDE_VAL_ENEMY] = false;
	map->map[SIDE_VAL_FRIENDLY][SIDE_VAL_FRIENDLY] = true;
	map->map[SIDE_VAL_FRIENDLY][SIDE_VAL_AMBIENT] = true;
	map->map[SIDE_VAL_AMBIENT][SIDE_VAL_EMPTY] = true;
	map->map[SIDE_VAL_AMBIENT][SIDE_VAL_UNKNOWN] = true;
	map->map[SIDE_VAL_AMBIENT][SIDE_VAL_CIV] = true;
	map->map[SIDE_VAL_AMBIENT][SIDE_VAL_BLU] = true;
	map->map[SIDE_VAL_AMBIENT][SIDE_VAL_OPF] = true;
	map->map[SIDE_VAL_AMBIENT][SIDE_VAL_GUE] = true;
	map->map[SIDE_VAL_AMBIENT][SIDE_VAL_LOGIC] = true;
	map->map[SIDE_VAL_AMBIENT][SIDE_VAL_ENEMY] = true;
	map->map[SIDE_VAL_AMBIENT][SIDE_VAL_FRIENDLY] = true;
	map->map[SIDE_VAL_AMBIENT][SIDE_VAL_AMBIENT] = true;

}
extern inline void side_set_firendly(PVM vm, const PVALUE self, const PVALUE other, bool flag);
extern inline bool side_equals(const PVALUE left, const PVALUE right);
extern inline bool side_is_friendly(const PVM vm, PVALUE self, const PVALUE other);
extern inline VALUE side_empty(void);
extern inline VALUE side_unknown(void);
extern inline VALUE side_civ(void);
extern inline VALUE side_blu(void);
extern inline VALUE side_opf(void);
extern inline VALUE side_gue(void);
extern inline VALUE side_logic(void);
extern inline VALUE side_enemy(void);
extern inline VALUE side_friendly(void);
extern inline VALUE side_ambient(void);