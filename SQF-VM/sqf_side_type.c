#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <stdint.h>
#include <stdbool.h>

#include "sqffull.h"



PCMD SIDE_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"SIDE", 't', 0, 0, 0, 0, 0);
	}
	return cmd;
}
const wchar_t* side_displayname(int i)
{
	switch (i)
	{
	case SIDE_VAL_EMPTY: return SIDE_WSTR_EMPTY;
	case SIDE_VAL_UNKNOWN: return SIDE_WSTR_UNKNOWN;
	case SIDE_VAL_CIV: return SIDE_WSTR_CIV;
	case SIDE_VAL_BLU: return SIDE_WSTR_BLU;
	case SIDE_VAL_OPF: return SIDE_WSTR_OPF;
	case SIDE_VAL_GUE: return SIDE_WSTR_GUE;
	case SIDE_VAL_LOGIC: return SIDE_WSTR_LOGIC;
	case SIDE_VAL_ENEMY: return SIDE_WSTR_ENEMY;
	case SIDE_VAL_FRIENDLY: return SIDE_WSTR_FRIENDLY;
	case SIDE_VAL_AMBIENT: return SIDE_WSTR_AMBIENT;
	default:
		return 0;
	}
}
const wchar_t* side_displayname2(const PVALUE val)
{
	return side_displayname(val->val.i);
}
unsigned char ** side_init_sidemap()
{
	int i;
	unsigned char ** map = malloc(sizeof(unsigned char*) * (SIDE_VAL_AMBIENT + 1));
	for (i = 0; i < SIDE_VAL_AMBIENT + 1; i++)
	{
		map[i] = malloc(sizeof(unsigned char) * (SIDE_VAL_AMBIENT + 1));
	}
	map[SIDE_VAL_EMPTY][SIDE_VAL_EMPTY] = 0;
	map[SIDE_VAL_EMPTY][SIDE_VAL_UNKNOWN] = 0;
	map[SIDE_VAL_EMPTY][SIDE_VAL_CIV] = 0;
	map[SIDE_VAL_EMPTY][SIDE_VAL_BLU] = 0;
	map[SIDE_VAL_EMPTY][SIDE_VAL_OPF] = 0;
	map[SIDE_VAL_EMPTY][SIDE_VAL_GUE] = 0;
	map[SIDE_VAL_EMPTY][SIDE_VAL_LOGIC] = 0;
	map[SIDE_VAL_EMPTY][SIDE_VAL_ENEMY] = 0;
	map[SIDE_VAL_EMPTY][SIDE_VAL_FRIENDLY] = 1;
	map[SIDE_VAL_EMPTY][SIDE_VAL_AMBIENT] = 1;
	map[SIDE_VAL_UNKNOWN][SIDE_VAL_EMPTY] = 0;
	map[SIDE_VAL_UNKNOWN][SIDE_VAL_UNKNOWN] = 0;
	map[SIDE_VAL_UNKNOWN][SIDE_VAL_CIV] = 0;
	map[SIDE_VAL_UNKNOWN][SIDE_VAL_BLU] = 0;
	map[SIDE_VAL_UNKNOWN][SIDE_VAL_OPF] = 0;
	map[SIDE_VAL_UNKNOWN][SIDE_VAL_GUE] = 0;
	map[SIDE_VAL_UNKNOWN][SIDE_VAL_LOGIC] = 0;
	map[SIDE_VAL_UNKNOWN][SIDE_VAL_ENEMY] = 0;
	map[SIDE_VAL_UNKNOWN][SIDE_VAL_FRIENDLY] = 1;
	map[SIDE_VAL_UNKNOWN][SIDE_VAL_AMBIENT] = 1;
	map[SIDE_VAL_CIV][SIDE_VAL_EMPTY] = 1;
	map[SIDE_VAL_CIV][SIDE_VAL_UNKNOWN] = 0;
	map[SIDE_VAL_CIV][SIDE_VAL_CIV] = 1;
	map[SIDE_VAL_CIV][SIDE_VAL_BLU] = 1;
	map[SIDE_VAL_CIV][SIDE_VAL_OPF] = 0;
	map[SIDE_VAL_CIV][SIDE_VAL_GUE] = 1;
	map[SIDE_VAL_CIV][SIDE_VAL_LOGIC] = 0;
	map[SIDE_VAL_CIV][SIDE_VAL_ENEMY] = 0;
	map[SIDE_VAL_CIV][SIDE_VAL_FRIENDLY] = 1;
	map[SIDE_VAL_CIV][SIDE_VAL_AMBIENT] = 1;
	map[SIDE_VAL_BLU][SIDE_VAL_EMPTY] = 1;
	map[SIDE_VAL_BLU][SIDE_VAL_UNKNOWN] = 0;
	map[SIDE_VAL_BLU][SIDE_VAL_CIV] = 1;
	map[SIDE_VAL_BLU][SIDE_VAL_BLU] = 1;
	map[SIDE_VAL_BLU][SIDE_VAL_OPF] = 0;
	map[SIDE_VAL_BLU][SIDE_VAL_GUE] = 1;
	map[SIDE_VAL_BLU][SIDE_VAL_LOGIC] = 0;
	map[SIDE_VAL_BLU][SIDE_VAL_ENEMY] = 0;
	map[SIDE_VAL_BLU][SIDE_VAL_FRIENDLY] = 1;
	map[SIDE_VAL_BLU][SIDE_VAL_AMBIENT] = 1;
	map[SIDE_VAL_OPF][SIDE_VAL_EMPTY] = 0;
	map[SIDE_VAL_OPF][SIDE_VAL_UNKNOWN] = 0;
	map[SIDE_VAL_OPF][SIDE_VAL_CIV] = 1;
	map[SIDE_VAL_OPF][SIDE_VAL_BLU] = 0;
	map[SIDE_VAL_OPF][SIDE_VAL_OPF] = 1;
	map[SIDE_VAL_OPF][SIDE_VAL_GUE] = 0;
	map[SIDE_VAL_OPF][SIDE_VAL_LOGIC] = 0;
	map[SIDE_VAL_OPF][SIDE_VAL_ENEMY] = 0;
	map[SIDE_VAL_OPF][SIDE_VAL_FRIENDLY] = 1;
	map[SIDE_VAL_OPF][SIDE_VAL_AMBIENT] = 1;
	map[SIDE_VAL_GUE][SIDE_VAL_EMPTY] = 0;
	map[SIDE_VAL_GUE][SIDE_VAL_UNKNOWN] = 0;
	map[SIDE_VAL_GUE][SIDE_VAL_CIV] = 1;
	map[SIDE_VAL_GUE][SIDE_VAL_BLU] = 1;
	map[SIDE_VAL_GUE][SIDE_VAL_OPF] = 0;
	map[SIDE_VAL_GUE][SIDE_VAL_GUE] = 1;
	map[SIDE_VAL_GUE][SIDE_VAL_LOGIC] = 0;
	map[SIDE_VAL_GUE][SIDE_VAL_ENEMY] = 0;
	map[SIDE_VAL_GUE][SIDE_VAL_FRIENDLY] = 1;
	map[SIDE_VAL_GUE][SIDE_VAL_AMBIENT] = 1;
	map[SIDE_VAL_LOGIC][SIDE_VAL_EMPTY] = 0;
	map[SIDE_VAL_LOGIC][SIDE_VAL_UNKNOWN] = 0;
	map[SIDE_VAL_LOGIC][SIDE_VAL_CIV] = 1;
	map[SIDE_VAL_LOGIC][SIDE_VAL_BLU] = 1;
	map[SIDE_VAL_LOGIC][SIDE_VAL_OPF] = 1;
	map[SIDE_VAL_LOGIC][SIDE_VAL_GUE] = 1;
	map[SIDE_VAL_LOGIC][SIDE_VAL_LOGIC] = 0;
	map[SIDE_VAL_LOGIC][SIDE_VAL_ENEMY] = 0;
	map[SIDE_VAL_LOGIC][SIDE_VAL_FRIENDLY] = 1;
	map[SIDE_VAL_LOGIC][SIDE_VAL_AMBIENT] = 1;
	map[SIDE_VAL_ENEMY][SIDE_VAL_EMPTY] = 0;
	map[SIDE_VAL_ENEMY][SIDE_VAL_UNKNOWN] = 0;
	map[SIDE_VAL_ENEMY][SIDE_VAL_CIV] = 0;
	map[SIDE_VAL_ENEMY][SIDE_VAL_BLU] = 0;
	map[SIDE_VAL_ENEMY][SIDE_VAL_OPF] = 0;
	map[SIDE_VAL_ENEMY][SIDE_VAL_GUE] = 0;
	map[SIDE_VAL_ENEMY][SIDE_VAL_LOGIC] = 0;
	map[SIDE_VAL_ENEMY][SIDE_VAL_ENEMY] = 0;
	map[SIDE_VAL_ENEMY][SIDE_VAL_FRIENDLY] = 1;
	map[SIDE_VAL_ENEMY][SIDE_VAL_AMBIENT] = 1;
	map[SIDE_VAL_FRIENDLY][SIDE_VAL_EMPTY] = 0;
	map[SIDE_VAL_FRIENDLY][SIDE_VAL_UNKNOWN] = 0;
	map[SIDE_VAL_FRIENDLY][SIDE_VAL_CIV] = 0;
	map[SIDE_VAL_FRIENDLY][SIDE_VAL_BLU] = 0;
	map[SIDE_VAL_FRIENDLY][SIDE_VAL_OPF] = 0;
	map[SIDE_VAL_FRIENDLY][SIDE_VAL_GUE] = 0;
	map[SIDE_VAL_FRIENDLY][SIDE_VAL_LOGIC] = 0;
	map[SIDE_VAL_FRIENDLY][SIDE_VAL_ENEMY] = 0;
	map[SIDE_VAL_FRIENDLY][SIDE_VAL_FRIENDLY] = 1;
	map[SIDE_VAL_FRIENDLY][SIDE_VAL_AMBIENT] = 1;
	map[SIDE_VAL_AMBIENT][SIDE_VAL_EMPTY] = 1;
	map[SIDE_VAL_AMBIENT][SIDE_VAL_UNKNOWN] = 1;
	map[SIDE_VAL_AMBIENT][SIDE_VAL_CIV] = 1;
	map[SIDE_VAL_AMBIENT][SIDE_VAL_BLU] = 1;
	map[SIDE_VAL_AMBIENT][SIDE_VAL_OPF] = 1;
	map[SIDE_VAL_AMBIENT][SIDE_VAL_GUE] = 1;
	map[SIDE_VAL_AMBIENT][SIDE_VAL_LOGIC] = 1;
	map[SIDE_VAL_AMBIENT][SIDE_VAL_ENEMY] = 1;
	map[SIDE_VAL_AMBIENT][SIDE_VAL_FRIENDLY] = 1;
	map[SIDE_VAL_AMBIENT][SIDE_VAL_AMBIENT] = 1;
	return map;
}
void side_destroy_sidemap(unsigned char** map)
{
	int i;
	for (i = 0; i < SIDE_VAL_AMBIENT + 1; i++)
	{
		free(map[i]);
	}
	free(map);
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