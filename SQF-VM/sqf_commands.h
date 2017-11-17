#ifndef _SQF_COMMANDS_H_
#define _SQF_COMMANDS_H_

#ifndef INT64_C
#error "SQF_commands.h" requires stdint header "stdint.h"
#endif // !INT64_C


#ifndef _SQF_BASE_H_
#error "SQF_commands.h" has to be included after "SQF.h" 
#endif

int64_t system_time_ms(void);
void stringify_value(PVM vm, PSTRING str, PVALUE val);

void cmd_isequalto(void* input, CPCMD self);
void cmd_isequaltype(void* input, CPCMD self);
void cmd_plus(void* input, CPCMD self);
void cmd_plus_UNARY(void* input, CPCMD self);
void cmd_minus(void* input, CPCMD self);
void cmd_minus_UNARY(void* input, CPCMD self);
void cmd_multiply(void* input, CPCMD self);
void cmd_divide(void* input, CPCMD self);
void cmd_diag_LOG(void* input, CPCMD self);
void cmd_hint(void* input, CPCMD self);
void cmd_systemchat(void* input, CPCMD self);
void cmd_private(void* input, CPCMD self);

void cmd_if(void* input, CPCMD self);
void cmd_then(void* input, CPCMD self);
void cmd_else(void* input, CPCMD self);
void cmd_true(void* input, CPCMD self);
void cmd_false(void* input, CPCMD self);
void cmd_help(void* input, CPCMD self);
void cmd_help_UNARY(void* input, CPCMD self);

void cmd_str(void* input, CPCMD self);

void cmd_greaterthan(void* input, CPCMD self);
void cmd_lessthen(void* input, CPCMD self);
void cmd_largetthenorequal(void* input, CPCMD self);
void cmd_lessthenorequal(void* input, CPCMD self);
void cmd_equal(void* input, CPCMD self);
void cmd_notequal(void* input, CPCMD self);
void cmd_andand(void* input, CPCMD self);
void cmd_oror(void* input, CPCMD self);

void cmd_select(void* input, CPCMD self);

void cmd_while(void* input, CPCMD self);
void cmd_do(void* input, CPCMD self);
void cmd_typename(void* input, CPCMD self);
void cmd_count(void* input, CPCMD self);
void cmd_count_UNARY(void* input, CPCMD self);
void cmd_format(void* input, CPCMD self);
void cmd_call(void* input, CPCMD self);
void cmd_call_UNARY(void* input, CPCMD self);
void cmd_spawn(void* input, CPCMD self);
void cmd_scriptdone(void* input, CPCMD self);
void cmd_foreach(void* input, CPCMD self);

void cmd_for(void* input, CPCMD self);
void cmd_from(void* input, CPCMD self);
void cmd_to(void* input, CPCMD self);
void cmd_step(void* input, CPCMD self);
void cmd_parsingnamespace(void* input, CPCMD self);
void cmd_missionnamespace(void* input, CPCMD self);
void cmd_uinamespace(void* input, CPCMD self);
void cmd_profilenamespace(void* input, CPCMD self);
void cmd_diag_TICKTIME(void* input, CPCMD self);

//https://community.bistudio.com/wiki/Math_Commands
void cmd_abs(void* input, CPCMD self);
void cmd_deg(void* input, CPCMD self);
void cmd_log(void* input, CPCMD self);
void cmd_pi(void* input, CPCMD self);
void cmd_sin(void* input, CPCMD self);
void cmd_cos(void* input, CPCMD self);
void cmd_tan(void* input, CPCMD self);
void cmd_exp(void* input, CPCMD self);
void cmd_asin(void* input, CPCMD self);
void cmd_acos(void* input, CPCMD self);
void cmd_atan(void* input, CPCMD self);
void cmd_rad(void* input, CPCMD self);
void cmd_sqrt(void* input, CPCMD self);
void cmd_ceil(void* input, CPCMD self);
void cmd_random(void* input, CPCMD self);
void cmd_floor(void* input, CPCMD self);
void cmd_ln(void* input, CPCMD self);
void cmd_round(void* input, CPCMD self);

void cmd_atan2(void* input, CPCMD self);
void cmd_min(void* input, CPCMD self);
void cmd_max(void* input, CPCMD self);
void cmd_mod(void* input, CPCMD self);
void cmd_not(void* input, CPCMD self);
void cmd_powerof(void* input, CPCMD self);
void cmd_comment(void* input, CPCMD self);

void cmd_pushback(void* input, CPCMD self);
void cmd_set(void* input, CPCMD self);
void cmd_selectrandom(void* input, CPCMD self);
void cmd_resize(void* input, CPCMD self);
void cmd_deleteat(void* input, CPCMD self);
void cmd_append(void* input, CPCMD self);
void cmd_reverse(void* input, CPCMD self);
void cmd_find(void* input, CPCMD self);
void cmd_arrayintersect(void* input, CPCMD self);

void cmd_vectoradd(void* input, CPCMD self);
void cmd_vectordiff(void* input, CPCMD self);
void cmd_vectorcrossproduct(void* input, CPCMD self);
void cmd_vectordotproduct(void* input, CPCMD self);
void cmd_vectorcos(void* input, CPCMD self);
void cmd_vectormagnitude(void* input, CPCMD self);
void cmd_vectormagnitudesqr(void* input, CPCMD self);
void cmd_vectormultiply(void* input, CPCMD self);
void cmd_vectordistance(void* input, CPCMD self);
void cmd_vectordistancesqr(void* input, CPCMD self);
void cmd_vectornormalized(void* input, CPCMD self);

void cmd_getvariable(void* input, CPCMD self);
void cmd_setvariable(void* input, CPCMD self);

//Object manipulation
void cmd_createvehicle(void* input, CPCMD self);
void cmd_typeof(void* input, CPCMD self);
void cmd_getpos(void* input, CPCMD self);
void cmd_setpos(void* input, CPCMD self);
void cmd_velocity(void* input, CPCMD self);
void cmd_setvelocity(void* input, CPCMD self);
void cmd_domove(void* input, CPCMD self);
void cmd_objnull(void* input, CPCMD self);

#define SWITCH_SPECIAL_VAR L"__switch"
void cmd_switch(void* input, CPCMD self);
void cmd_case(void* input, CPCMD self);
void cmd_default(void* input, CPCMD self);
void cmd_caseoperator(void* input, CPCMD self);

void cmd_west(void* input, CPCMD self);
void cmd_blufor(void* input, CPCMD self);
void cmd_east(void* input, CPCMD self);
void cmd_opfor(void* input, CPCMD self);
void cmd_resistance(void* input, CPCMD self);
void cmd_independent(void* input, CPCMD self);
void cmd_sideunknown(void* input, CPCMD self);
void cmd_sidelogic(void* input, CPCMD self);
void cmd_sidefriendly(void* input, CPCMD self);
void cmd_sideenemy(void* input, CPCMD self);
void cmd_civilian(void* input, CPCMD self);
void cmd_sideempty(void* input, CPCMD self);

void cmd_nil(void* input, CPCMD self);
void cmd_allvariables(void* input, CPCMD self);
void cmd_with(void* input, CPCMD self);

void cmd_compile(void* input, CPCMD self);
void cmd_toarray(void* input, CPCMD self);
void cmd_tostring(void* input, CPCMD self);
void cmd_params(void* input, CPCMD self);
void cmd_params_UNARY(void* input, CPCMD self);

void cmd_isnil(void* input, CPCMD self);
void cmd_deletevehicle(void* input, CPCMD self);

void cmd_creategroup(void* input, CPCMD self);
void cmd_deletegroup(void* input, CPCMD self);
void cmd_groupid(void* input, CPCMD self);
void cmd_createunit(void* input, CPCMD self);
void cmd_units(void* input, CPCMD self);
void cmd_allgroups(void* input, CPCMD self);


void cmd_toupper(void* input, CPCMD self);
void cmd_tolower(void* input, CPCMD self);

#endif // !_SQF_COMMANDS_H_
