#ifndef _SQF_COMMANDS_H_
#define _SQF_COMMANDS_H_
#ifndef _SQF_BASE_H_
#error "SQF_commands.h" has to be included after "SQF.h" 
#endif

int64_t system_time_ms(void);
void stringify_value(PVM vm, PSTRING str, PVALUE val);
unsigned char is_equal_to(PVM vm, PVALUE l, PVALUE r);

void CMD_ISEQUALTO(void* input, CPCMD self);
void CMD_ISEQUALTYPE(void* input, CPCMD self);
void CMD_PLUS(void* input, CPCMD self);
void CMD_PLUS_UNARY(void* input, CPCMD self);
void CMD_MINUS(void* input, CPCMD self);
void CMD_MINUS_UNARY(void* input, CPCMD self);
void CMD_MULTIPLY(void* input, CPCMD self);
void CMD_DIVIDE(void* input, CPCMD self);
void CMD_DIAG_LOG(void* input, CPCMD self);
void CMD_HINT(void* input, CPCMD self);
void CMD_SYSTEMCHAT(void* input, CPCMD self);
void CMD_PRIVATE(void* input, CPCMD self);

void CMD_IF(void* input, CPCMD self);
void CMD_THEN(void* input, CPCMD self);
void CMD_ELSE(void* input, CPCMD self);
void CMD_TRUE(void* input, CPCMD self);
void CMD_FALSE(void* input, CPCMD self);
void CMD_HELP(void* input, CPCMD self);
void CMD_HELP_UNARY(void* input, CPCMD self);

void CMD_STR(void* input, CPCMD self);

void CMD_LARGETTHEN(void* input, CPCMD self);
void CMD_LESSTHEN(void* input, CPCMD self);
void CMD_LARGETTHENOREQUAL(void* input, CPCMD self);
void CMD_LESSTHENOREQUAL(void* input, CPCMD self);
void CMD_EQUAL(void* input, CPCMD self);
void CMD_NOTEQUAL(void* input, CPCMD self);
void CMD_ANDAND(void* input, CPCMD self);
void CMD_OROR(void* input, CPCMD self);

void CMD_SELECT(void* input, CPCMD self);


void CMD_WHILE(void* input, CPCMD self);
void CMD_DO(void* input, CPCMD self);
void CMD_TYPENAME(void* input, CPCMD self);
void CMD_COUNT(void* input, CPCMD self);
void CMD_COUNT_UNARY(void* input, CPCMD self);
void CMD_FORMAT(void* input, CPCMD self);
void CMD_CALL(void* input, CPCMD self);
void CMD_CALL_UNARY(void* input, CPCMD self);
void CMD_FOREACH(void* input, CPCMD self);

void CMD_FOR(void* input, CPCMD self);
void CMD_FROM(void* input, CPCMD self);
void CMD_TO(void* input, CPCMD self);
void CMD_STEP(void* input, CPCMD self);
void CMD_PARSINGNAMESPACE(void* input, CPCMD self);
void CMD_MISSIONNAMESPACE(void* input, CPCMD self);
void CMD_UINAMESPACE(void* input, CPCMD self);
void CMD_PROFILENAMESPACE(void* input, CPCMD self);
void CMD_DIAG_TICKTIME(void* input, CPCMD self);

//https://community.bistudio.com/wiki/Math_Commands
void CMD_ABS(void* input, CPCMD self);
void CMD_DEG(void* input, CPCMD self);
void CMD_LOG(void* input, CPCMD self);
void CMD_PI(void* input, CPCMD self);
void CMD_SIN(void* input, CPCMD self);
void CMD_COS(void* input, CPCMD self);
void CMD_TAN(void* input, CPCMD self);
void CMD_EXP(void* input, CPCMD self);
void CMD_ASIN(void* input, CPCMD self);
void CMD_ACOS(void* input, CPCMD self);
void CMD_ATAN(void* input, CPCMD self);
void CMD_RAD(void* input, CPCMD self);
void CMD_SQRT(void* input, CPCMD self);
void CMD_CEIL(void* input, CPCMD self);
void CMD_RANDOM(void* input, CPCMD self);
void CMD_FLOOR(void* input, CPCMD self);
void CMD_LN(void* input, CPCMD self);
void CMD_ROUND(void* input, CPCMD self);

void CMD_ATAN2(void* input, CPCMD self);
void CMD_MIN(void* input, CPCMD self);
void CMD_MAX(void* input, CPCMD self);
void CMD_MOD(void* input, CPCMD self);
void CMD_NOT(void* input, CPCMD self);
void CMD_POWEROF(void* input, CPCMD self);
void CMD_COMMENT(void* input, CPCMD self);

void CMD_PUSHBACK(void* input, CPCMD self);
void CMD_SET(void* input, CPCMD self);

void CMD_GETVARIABLE(void* input, CPCMD self);
void CMD_SETVARIABLE(void* input, CPCMD self);

//Object manipulation
void CMD_CREATEVEHICLE(void* input, CPCMD self);
void CMD_TYPEOF(void* input, CPCMD self);
void CMD_GETPOS(void* input, CPCMD self);
void CMD_SETPOS(void* input, CPCMD self);
void CMD_VELOCITY(void* input, CPCMD self);
void CMD_SETVELOCITY(void* input, CPCMD self);
void CMD_DOMOVE(void* input, CPCMD self);
void CMD_OBJNULL(void* input, CPCMD self);

#define SWITCH_SPECIAL_VAR "__switch"
void CMD_SWITCH(void* input, CPCMD self);
void CMD_CASE(void* input, CPCMD self);
void CMD_DEFAULT(void* input, CPCMD self);
void CMD_CASEOPERATOR(void* input, CPCMD self);


void CMD_WEST(void* input, CPCMD self);
void CMD_BLUFOR(void* input, CPCMD self);
void CMD_EAST(void* input, CPCMD self);
void CMD_OPFOR(void* input, CPCMD self);
void CMD_RESISTANCE(void* input, CPCMD self);
void CMD_INDEPENDENT(void* input, CPCMD self);
void CMD_SIDEUNKNOWN(void* input, CPCMD self);
void CMD_SIDELOGIC(void* input, CPCMD self);
void CMD_SIDEFRIENDLY(void* input, CPCMD self);
void CMD_SIDEENEMY(void* input, CPCMD self);
void CMD_CIVILIAN(void* input, CPCMD self);
void CMD_SIDEEMPTY(void* input, CPCMD self);


void CMD_NIL(void* input, CPCMD self);
void CMD_ALLVARIABLES(void* input, CPCMD self);
void CMD_WITH(void* input, CPCMD self);

void CMD_COMPILE(void* input, CPCMD self);
void CMD_TOARRAY(void* input, CPCMD self);
void CMD_TOSTRING(void* input, CPCMD self);
void CMD_PARAMS(void* input, CPCMD self);
void CMD_PARAMS_UNARY(void* input, CPCMD self);

void CMD_ISNIL(void* input, CPCMD self);


#endif // !_SQF_COMMANDS_H_
