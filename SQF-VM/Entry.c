#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>
#include <wchar.h>
#include <wctype.h>
#include <stdbool.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#if _WIN32 & _DEBUG
#include <crtdbg.h>
#endif
#ifdef __linux
#include <alloca.h>
#endif // _GCC
#ifdef _WIN32
#include <malloc.h>
#endif // _WIN32

#include "sqffull.h"


static jmp_buf program_exit;
int64_t systime_start = 0;


wchar_t* gen_wchar_string(const char *input)
{
	const size_t size = strlen(input) + 1;
	wchar_t* output = malloc(sizeof(wchar_t) * size);
	mbstowcs(output, input, size);

	return output;
}

void cmd_productversion(void* input, CPCMD self);

char* get_line(char* line, size_t lenmax)
{
	char* line_start = line;
	size_t len = lenmax;
	int c;

	if (line == NULL)
		return NULL;

	while (1)
	{
		c = fgetc(stdin);
		if (c == EOF)
			break;

		if (--len == 0)
		{
			len++;
			line--;
		}

		if ((*line++ = c) == '\n')
			break;
	}
	*line = '\0';
	return line_start;
}

#define LINEBUFFER_SIZE 2048

void custom_error(PVM vm, const wchar_t* errMsg, PSTACK stack)
{
	int i;
	PDBGINF dbginf;
	if (stack != 0 && stack->allow_dbg)
	{
		dbginf = 0;
		for (i = stack->top - 1; i >= 0; i--)
		{
			if (stack->data[i]->type == INST_DEBUG_INFO)
			{
				dbginf = get_dbginf(0, 0, stack->data[i]);
				break;
			}
		}
		if (dbginf != 0)
		{
			vm->print(vm, L"%ls", dbginf->hint);
			vm->print(vm, L"[ERR][L%d|C%d] %ls\n", dbginf->line, dbginf->col, errMsg);
		}
		else
		{
			vm->print(vm, L"[ERR] %ls\n", errMsg);
		}
	}
	else
	{
		vm->print(vm, L"[ERR] %ls\n", errMsg);
	}
	vm->die_flag = 1;
	//longjmp(program_exit, 1);
}
void custom_warn(PVM vm, const wchar_t* errMsg, PSTACK stack)
{
	int i;
	PDBGINF dbginf;
	if (stack->allow_dbg)
	{
		dbginf = 0;
		for (i = stack->top - 1; i >= 0; i--)
		{
			if (stack->data[i]->type == INST_DEBUG_INFO)
			{
				dbginf = get_dbginf(0, 0, stack->data[i]);
				break;
			}
		}
		if (dbginf != 0)
		{
			vm->print(vm, L"%ls", dbginf->hint);
			vm->print(vm, L"[WRN][L%d|C%d] %ls\n", dbginf->line, dbginf->col, errMsg);
		}
		else
		{
			vm->print(vm, L"[WRN] %ls\n", errMsg);
		}
	}
	else
	{
		vm->print(vm, L"[WRN] %ls\n", errMsg);
	}
}

void create_cmd(PVM vm, const wchar_t* name, char type, CMD_CB fnc, char precedence, const wchar_t* usage, const wchar_t* examples_cs, const wchar_t* desc)
{
	register_command(vm,
		create_command(name, type, fnc, precedence, usage, examples_cs, desc));
}
void register_commmands(PVM vm)
{
	if (find_command(vm, L"+", 'b') != 0)
		return;
	create_cmd(vm, L"+", 'b', cmd_plus, 6, L"<SCALAR> + <SCALAR> | <STRING> + <STRING> | <ARRAY> + <ANY>", L"1 + 1 //2#" L"\"foo\" + \"bar\" //\"foobar\"#" L"[] + 1 //[1]#", L"b added to a.");
	create_cmd(vm, L"-", 'b', cmd_minus, 6, L"<SCALAR> - <SCALAR> | <ARRAY> - <ARRAY>", L"1 - 1 //0#" L"[0,[0],[[0]]] - [0] //[[0], [[0]]]#" L"[0,[0],[[0]]] - [[0]] //[0, [[0]]]#" L"[0,[0],[[0]]] - [[[0]]] //[0, [0]]#", L"Subtracts b from a. a and b need to be of the same type, both Numbers or both Arrays. In Arma 3 it is possible to subtract nested arrays.");
	create_cmd(vm, L"*", 'b', cmd_multiply, 7, L"<SCALAR> * <SCALAR>", L"0.5 * 100 //50#", L"Returns the value of a multiplied by b.");
	create_cmd(vm, L"/", 'b', cmd_divide, 7, L"<SCALAR> / <SCALAR> | <CONFIG> / <STRING>", L"15 / 3 //5#", L"a divided by b. Division by 0 throws \"Division by zero\" error, however script doesn't stop and the result of such division is assumed to be 0. If lval is CONFIG and rval is STRING then this acts as alias to >>a");
	create_cmd(vm, L">", 'b', cmd_greaterthan, 3, L"<SCALAR> > <SCALAR>", L"10 > 1 //true#1 > 10 //false#", L"Returns true if a is greater than b, else returns false.");
	create_cmd(vm, L"<", 'b', cmd_lessthen, 3, L"<SCALAR> < <SCALAR>", L"1 < 2 //true#2 < 1 //false#", L"Returns true if a is less than b, else returns false.");
	create_cmd(vm, L">=", 'b', cmd_largetthenorequal, 3, L"<SCALAR> >= <SCALAR>", L"10 > 1 //true#1 > 10 //false#10 >= 10 //true#", L"Returns true if a is greater than or equal to b, else returns false.");
	create_cmd(vm, L"<=", 'b', cmd_lessthenorequal, 3, L"<SCALAR> <= <SCALAR>", L"1 < 2 //true#2 <= 1 //false#2 <= 2 //true#", L"Returns true if a is less than or equal to b, else returns false.");
	create_cmd(vm, L"==", 'b', cmd_equal, 3, L"<SCALAR> == <SCALAR> | <OBJECT> == <OBJECT> | <STRING> == <STRING>", L"1 == 1 //true#" L"1 == 2 //false#" L"_obj1 = \"B_Soldier_F\" createVehicle [0, 0, 0]; _obj1 == _obj1 //true#" L"_obj1 = \"B_Soldier_F\" createVehicle [0, 0, 0]; _obj2 = \"B_Soldier_F\" createVehicle [0, 0, 0]; _obj1 == _obj2 //false#" L"\"foo\" == \"foo\" //true#" L"\"foo\" == \"bar\" //false#", L"Check if one value is equal to another. Both values need to be of the same type.");
	create_cmd(vm, L"!=", 'b', cmd_notequal, 3, L"<SCALAR> != <SCALAR> | <OBJECT> != <OBJECT> | <STRING> != <STRING>", L"1 != 1 //false#" L"1 != 2 //true#" L"_obj1 = \"B_Soldier_F\" createVehicle [0, 0, 0]; _obj1 != _obj1 //false#" L"_obj1 = \"B_Soldier_F\" createVehicle [0, 0, 0]; _obj2 = \"B_Soldier_F\" createVehicle [0, 0, 0]; _obj1 != _obj2 //true#" L"\"foo\" != \"foo\" //false#" L"\"foo\" != \"bar\" //true#", L"Returns whether one value is not equal to another.");
	create_cmd(vm, L"||", 'b', cmd_oror, 1, L"<BOOL> || <BOOL> | <BOOL> || <CODE>", L"true || true //true#" L"true || false //true#" L"false || false //false#" L"true || {false} //true#" L"false || {true} //true#" L"false || {false} //false#"  L"true or {true} //true#", L"Returns true only if one or both conditions are true.In case of the alternative syntax, lazy evaluation is used(if left operand is true, evaluation of the right side is skipped completely).Identical to : a or b");
	create_cmd(vm, L"&&", 'b', cmd_andand, 2, L"<BOOL> && <BOOL> | <BOOL> && <CODE>", L"true && true //true#" L"true && false //false#" L"false && false //false#" L"true && {false} //false#" L"false && {true} //false#" L"false && {false} //false#" L"true && {true} //true#", L"Returns true only if both conditions are true. In case of the alternative syntax, lazy evaluation is used (if left operand is false, evaluation of the right side is skipped completely). Identical to: a and b");
	create_cmd(vm, L"or", 'b', cmd_oror, 1, L"<BOOL> or <BOOL> | <BOOL> or <CODE>", L"true or true //true#" L"true or false //true#" L"false or false //false#" L"true or {false} //true#" L"false or {true} //true#" L"false or {false} //false#"  L"true or {true} //true#", L"Returns true only if one or both conditions are true. In case of the alternative syntax, lazy evaluation is used (if left operand is true, evaluation of the right side is skipped completely). Identical to: a || b");
	create_cmd(vm, L"and", 'b', cmd_andand, 2, L"<BOOL> and <BOOL>", L"true and true //true#" L"true and false //false#" L"false and false //false#" L"true and {false} //false#" L"false and {true} //false#" L"false and {false} //false#" L"true and {true} //true#", L"Returns true only if both conditions are true. In case of the alternative syntax, lazy evaluation is used (if left operand is false, evaluation of the right side is skipped completely). Identical to: a && b");
	create_cmd(vm, L"select", 'b', cmd_select, 10, L"<ARRAY> select <SCALAR> | <ARRAY> select <BOOL>", L"[0] select 0 //0#" L"[] select 0 //nil#" L"[10, 0] select false //10#" L"[10, 0] select true //0#", L"Selects an element from an array, config entry from Config or substring from a string or a range from an array.");
	create_cmd(vm, L"then", 'b', cmd_then, 4, L"<IF> then <ARRAY>", L"if true then [0, 1] //0#" L"if true then [0, 1] //0# if false then [0, 1] //1#", L"First or second element of array is executed depending on result of if condition. Result of the expression executed is returned as a result (result may be Nothing).");
	create_cmd(vm, L"else", 'b', cmd_else, 5, L"<CODE> else <CODE>", L"{} else {} //[{}, {}]#", L"Executes else code when if condition returns false");
	create_cmd(vm, L"do", 'b', cmd_do, 4, L"<WHILE> do <CODE> | <FOR> do <CODE> | <WITH> do <CODE>", L"b = 10; a = 0; while {b > a} do {a = a + 1}; a //10#", L"Executes code. Used as part of construct, such as while, with, for and switch. The code is always executed in missionNamespace unless do is used with with as an argument.");
	create_cmd(vm, L"from", 'b', cmd_from, 4, L"<FOR> from <SCALAR>", L"", L"Continue sequence of for var construct. It sets the start value of the index variable.");
	create_cmd(vm, L"to", 'b', cmd_to, 4, L"<FOR> to <SCALAR>", L"", L"Continue sequence of for var construct. It sets the end value of the index variable");
	create_cmd(vm, L"step", 'b', cmd_step, 4, L"<FOR> step <SCALAR>", L"", L"Optionally can set step. If you want to count down, step must be specified, and set negative. Default value is 1.");
	create_cmd(vm, L"count", 'b', cmd_count, 4, L"<CODE> count <ARRAY> | <COUNT> count <STRING>", L"{_x > 2} count [1, 2, 3, 4, 5] //3#", L"Can be used to count the number of elements in array with a condition.");
	create_cmd(vm, L"call", 'b', cmd_call, 4, L"<ANY> call <CODE>", L"[1] call {_this} //[1]#", L"Executes given set of compiled instructions with an argument passed to the executed code in magic variable _this.");
	create_cmd(vm, L"atan2", 'b', cmd_atan2, 7, L"<SCALAR> atan2 <SCALAR>", L"5 atan2 3 //59.0362#", L"ArcTangent of x/y. Used to determine the angle of a vector [x,y]. Result in Degrees between -180 and 180. Note that this command can handle y being 0, unlike when using atan, and will return 90");
	create_cmd(vm, L"min", 'b', cmd_min, 6, L"<SCALAR> min <SCALAR>", L"1 min 2 //1#" L"10 min 5 //5#", L"The smaller of a, b");
	create_cmd(vm, L"max", 'b', cmd_max, 6, L"<SCALAR> max <SCALAR>", L"1 max 2 //2#" L"10 max 5 //10#", L"The greater of a, b");
	create_cmd(vm, L"mod", 'b', cmd_mod, 7, L"<SCALAR> mod <SCALAR>", L"1 max 2 //1#" L"10 max 5 //0#" L"3 mod 2 //1#", L"Remainder of a divided by b. Identical to: a % b");
	create_cmd(vm, L"%", 'b', cmd_mod, 7, L"<SCALAR> % <SCALAR>", L"1 % 2 //1#" L"10 % 5 //0#" L"3 % 2 //1#", L"Remainder of a divided by b. Identical to: a mod b");
	create_cmd(vm, L"^", 'b', cmd_powerof, 9, L"<SCALAR> ^ <SCALAR>", L"5 ^ 2 //25#", L"a raised to the power of b");
	create_cmd(vm, L"pushBack", 'b', cmd_pushback, 4, L"<ARRAY> pushBack <ANY>", L"[] pushBack 1 //0#" L"[1, 2, 3] pushBack 4 // 3#", L"Insert an element to the back of the given array. This command modifies the original array.");
	create_cmd(vm, L"set", 'b', cmd_set, 4, L"<ARRAY> set <ANY>", L"_arr = [10]; _arr set [0, 9]; _arr //[9]#" L"_arr = []; _arr set [0, 1]; _arr //[1]#", L"Changes the element at the given(zero - based) index of the array.If the element does not exist, resize index + 1 is called to create it.");
	create_cmd(vm, L"isEqualTo", 'b', cmd_isequalto, 4, L"<ANY> isEqualTo <ANY>", L"1 isEqualTo 2 //false#", L"Performs strict comparison between var1 and var2 and returns true if equal, otherwise false. Strict means that it would check that both arguments are of the same data type and then compare the values.");
	create_cmd(vm, L"createVehicle", 'b', cmd_createvehicle, 4, L"<STRING> createVehicle <ARRAY>", L"", L"Creates an empty object of given classname type");
	create_cmd(vm, L"getVariable", 'b', cmd_getvariable, 4, L"<NAMESPACE> getVariable <STRING> | <NAMESPACE> getVariable <ARRAY> | <OBJECT> getVariable <STRING> | <OBJECT> getVariable <ARRAY>", L"", L"Return the value of variable in the variable space assigned to various data types. Variable name can be anything, including special characters.");
	create_cmd(vm, L"setVariable", 'b', cmd_setvariable, 4, L"<NAMESPACE> setVariable <ARRAY> | <OBJECT> setVariable <ARRAY>", L"", L"Set variable to given value in the variable space of given element. Variable name can be anything, including special characters.");
	create_cmd(vm, L"setPos", 'b', cmd_setpos, 4, L" <OBJECT> setPos <ARRAY>", L"_obj = \"B_Soldier_F\" createVehicle [0, 0, 0]; _obj setPos [0, 0, 10]; position _obj //[0, 0, 10]#", L"Sets object position.");
	create_cmd(vm, L"setVelocity", 'b', cmd_setvelocity, 4, L" <OBJECT> setVelocity <ARRAY>", L"_obj = \"B_Soldier_F\" createVehicle [0, 0, 0]; _obj setVelocity [0, 0, 10]; velocity _obj //[0, 0, 10]#", L"Set velocity (speed vector) of a vehicle. Units are in metres per second.");
	create_cmd(vm, L"forEach", 'b', cmd_foreach, 4, L"<CODE> forEach <ARRAY> | <COUNT> forEach <ARRAY>", L"", L"Executes the given command(s) on every item of an array. The array items are represented by the magic variable _x.The array indices are represented by _forEachIndex.");
	create_cmd(vm, L":", 'b', cmd_caseoperator, 4, L"<SWITCH> : <CODE>", L"", L"See switch");
	create_cmd(vm, L"doMove", 'b', cmd_domove, 4, L"<OBJECT> doMove <ARRAY> | <ARRAY> doMove <ARRAY>", L"", L"Order the given unit(s) to move to the given position (without radio messages).");
	create_cmd(vm, L"isEqualType", 'b', cmd_isequaltype, 4, L"<ANY> isEqualType <ANY>", L"[1, 2] isEqualType 1 //false#" L"[1, 2] isEqualType [] //true#" L"false isEqualType 1 //false#" L"false isEqualType true //true#", L"Compares 2 values by their type. A much faster alternative to typeName a == typeName b.");
	create_cmd(vm, L"params", 'b', cmd_params, 4, L"<ANY> params <ARRAY>", L"position (\"B_Soldier_F\" createVehicle [10, 20, 30]) params [\"\", \"\", \"_z\"]; _z" L"[1, nil, 2] params [\"_var1\", [\"_var2\", 23], \"_var3\"]; _var2 //23#" L"private _paramsres = [1, 2] params [\"_var1\", \"_var2\", [\"_var3\", true, [true]]]; [_var3, _paramsres] //[true, false]#" L"private _paramsres = [1, 2, 3] params [\"_var1\", \"_var2\", [\"_var3\", true, [true]]]; [_var3, _paramsres] //[true, false]#" L"private _paramsres = [1, \"ok\", [1, 2, 3]] params [[\"_var1\", 0, [0]], [\"_var2\", "L", ["L"]], [\"_var3\", [0,0,0], [[], objNull, 0], [2,3]]]; _paramsres //true#", L"Parses input argument into array of private variables. When used without argument, as shown in main syntax, internal variable _this, which is usually available inside functions and event handlers, is used as argument.In addition to simple parsing directly into variables, input can be tested in case it is undefined, of the wrong type or of the wrong size(if array) and substituted if necessary with default values.");
	create_cmd(vm, L"createUnit", 'b', cmd_createunit, 4, L"<GROUP> createUnit <ARRAY>", L"_group = createGroup west; _unit = _group createUnit [\"B_Soldier_F\", [0, 0, 0], [], 0, \"NONE\"]; typeName _unit; //\"OBJECT\"", L"Create unit of a class that's defined in CfgVehicles. The Group parameter MUST be an existing group or the unit won't be created.");
	create_cmd(vm, L"spawn", 'b', cmd_spawn, 4, L"<ANY> spawn <CODE>", L"", L"Adds given code to the scheduler. Exactly when the code will be executed is unknown, it depends on how busy is the engine and how filled up is the scheduler. Therefore spawn does not wait for the supplied code to finish, instead, spawn returns a Script handle to the scheduler task. scriptDone command can be used to check the code completion. Additional arguments are passed to the code in local variable _this. Since Arma 3 v1.55 the script handle also exists inside the code in _thisScript variable.");
	create_cmd(vm, L"resize", 'b', cmd_resize, 4, L"<ARRAY> resize <SCALAR>", L"_arrayNum = [0,1,2,3,4]; _arrayNum resize 2; // _arrayNum is now [0,1]", L"Changes the size of the given array. The command does not return new array, it resizes the source array to the desired number of elements. If the new size is bigger than the current size, the new places are filled with nils.");
	create_cmd(vm, L"deleteAt", 'b', cmd_deleteat, 4, L"<ARRAY> deleteAt <SCALAR>", L"", L"");
	create_cmd(vm, L"append", 'b', cmd_append, 4, L"<ARRAY> append <ARRAY>", L"_arr = [1,2,3]; _arr append [4,5,6]; hint str _arr; //[1,2,3,4,5,6]", L"Appends array2 to the back of array1 modifying array1. NOTE: append does not return array, it modifies existing array. If you need to return a copy, use \"+\"");
	create_cmd(vm, L"find", 'b', cmd_find, 4, L"<ARRAY> find <ANY>#" L"<STRING> find <STRING>", L"[\"Apples\",\"Oranges\",\"Pears\"] find \"Oranges\"; //result is 1#" L"[1,[2],[[3]]] find [[3]]; //result is 2#" L"if (magazines player find \"Strela\" >= 0) then {hint \"You've got Strela!\"};#" L"hint str (\"japa is the man!\" find \"the man!\"); //8", L"Searches for an array element within array or a string within a string. Returns the 0 based index on success or -1 if not found. Test is cASe-seNsItiVE ");
	create_cmd(vm, L"arrayIntersect", 'b', cmd_arrayintersect, 4, L"<ARRAY> arrayIntersect <ARRAY>", L"_arr1 = [1,2,3,4,5,2,3,4]; _arr2 = [4,5,6,1,2,3,5,6]; hint str (_arr1 arrayIntersect _arr2); // [4,5,1,2,3]", L"Intersects array1 with array2 returning array of unique common elements. Additionally, using the same array for array1 and array2 will simply return array of unique elements. Intersects only 1st dimension of an array.");
	create_cmd(vm, L"vectorAdd", 'b', cmd_vectoradd, 4, L"<VECTOR3D> vectorAdd <VECTOR3D>", L"[5,10,5] vectorAdd [5,5,10]; //returns [10,15,15]", L"Adds two 3D vectors.");
	create_cmd(vm, L"vectorDiff", 'b', cmd_vectordiff, 4, L"<VECTOR3D> vectorDiff <VECTOR3D>", L"[5,10,5] vectorDiff [5,5,10]; //returns [0,5,-5]", L"Subtracts one 3D vector from another. (vector1 - vector2)");
	create_cmd(vm, L"vectorCrossProduct", 'b', cmd_vectorcrossproduct, 4, L"<VECTOR3D> vectorCrossProduct <VECTOR3D>", L"[0,1,0] vectorCrossProduct [-1,0,0]; //[0,-0,1]", L"Cross product of two 3D vectors. In layman's terms, if you have a polygon (surface) defined by 3 points, you can find a normal to it (just like terrain surfaceNormal). To invert direction of the normal, swap arguments around. ");
	create_cmd(vm, L"vectorDotProduct", 'b', cmd_vectordotproduct, 4, L"<VECTOR3D> vectorDotProduct <VECTOR3D>", L"[1,0,1] vectorDotProduct [0,0,2]; //returns 2", L"Dot product of two 3D vectors.");
	create_cmd(vm, L"vectorCos", 'b', cmd_vectorcos, 4, L"<VECTOR3D> vectorCos <VECTOR3D>", L"[1,0,0] vectorCos [0,0,2]; //returns 0", L"Cosine of angle between two 3D vectors.");
	create_cmd(vm, L"vectorMultiply", 'b', cmd_vectormultiply, 4, L"<VECTOR3D> ectorMultiply <SCALAR>", L"[1,2,3] vectorMultiply 3; //[3,6,9]", L"Multiplies 3D vector by a scalar.");
	create_cmd(vm, L"vectorDistance", 'b', cmd_vectordistance, 4, L"<VECTOR3D> vectorDistance <VECTOR3D>", L"_euclideanDist = getPosASL player vectorDistance [0,0,0];", L"Distance between two 3D vectors.");
	create_cmd(vm, L"vectorDistanceSqr", 'b', cmd_vectordistancesqr, 4, L"<VECTOR3D> vectorDistanceSqr <VECTOR3D>", L"_distSqr = getPos player vectorDistanceSqr [0,0,2];", L"Squared distance between two 3D vectors.");
	create_cmd(vm, L">>", 'b', cmd_navigateconfig, 4, L"<CONFIG> >> <STRING>", L"", L"Returns subentry of config entry with given name.");

	create_cmd(vm, L"diag_log", 'u', cmd_diag_LOG, 4, L"diag_log <ANY>", L"", L"Dumps the argument's value. Each call creates a new line.");
	create_cmd(vm, L"systemChat", 'u', cmd_systemchat, 4, L"systemChat <STRING>", L"", L"Writes the argument's value plaintext. Each call creates a new line.");
	create_cmd(vm, L"hint", 'u', cmd_hint, 4, L"hint <STRING>", L"", L"Writes the argument's value plaintext. Each call creates a new line.");
	create_cmd(vm, L"private", 'u', cmd_private, 4, L"private <STRING> | private <ARRAY>", L"_a = 10; [] call { private \"_a\"; _a = 20; }; _a //10#" L"_a = 10; _b = 20; [] call { private [\"_a\", \"_b\"]; _a = 50; _b = 50; }; [_a, _b] //[10, 20]#", L"Sets a variable to the innermost scope");
	create_cmd(vm, L"if", 'u', cmd_if, 4, L"if <BOOL>", L"", L"See then");
	create_cmd(vm, L"str", 'u', cmd_str, 4, L"str <ANY>", L"", L"Returns the string representation of any value passed.");
	create_cmd(vm, L"while", 'u', cmd_while, 4, L"while <CODE>", L"while {b > a} //{b > a}#", L"Repeats Code while condition is true. A part of while do construct.");
	create_cmd(vm, L"typeName", 'u', cmd_typename, 4, L"typeName <ANY>", L"", L"Returns the data type of an expression. The type is returned as all-uppercase string.");
	create_cmd(vm, L"for", 'u', cmd_for, 4, L"for <STRING>", L"_a = 0; for \"_i\" from 1 to 10 do { _a = _a + _i; }; _a //45#", L"This operator creates a For Type which is used in the for-constructs.");
	create_cmd(vm, L"+", 'u', cmd_plus_UNARY, 4, L"+ <ARRAY> | + <SCALAR> | + <NaN>", L"+1 //1#" L"+[10] //[10]#", L"Unary plus - returns a copy of valuea.");
	create_cmd(vm, L"-", 'u', cmd_minus_UNARY, 4, L"- <SCALAR>", L"-1 //-1#" L"_a = 1; -_a //-1#", L"Unary minus : zero minus a");
	create_cmd(vm, L"count", 'u', cmd_count_UNARY, 4, L"count <STRING> | count <ARRAY>", L"count [0, 1, 2] //3#" L"count \"foobar\" //6#", L"Can be used to count the number of elements in array or the number of characters in a string");
	create_cmd(vm, L"format", 'u', cmd_format, 4, L"format <ARRAY>", L"format [\"%1 - %2 = %3\", 3, 2, 1] //\"3 - 2 = 1\"", L"Composes a string containing other variables or other variable types. Converts any variable type to a string.");
	create_cmd(vm, L"call", 'u', cmd_call_UNARY, 4, L"call <CODE>", L"call {1} //1#", L"Executes given set of compiled instructions.");
	create_cmd(vm, L"abs", 'u', cmd_abs, 4, L"abs <SCALAR>", L"abs -3 //3#", L"Absolute value of a real number");
	create_cmd(vm, L"deg", 'u', cmd_deg, 4, L"deg <SCALAR>", L"deg 1 //57.295#", L"Convert a number from Radians to Degrees.");
	create_cmd(vm, L"log", 'u', cmd_log, 4, L"log <SCALAR>", L"log 10 //1#", L"Base-10 logarithm of x.");
	create_cmd(vm, L"sin", 'u', cmd_sin, 4, L"sin <SCALAR>", L"sin 30 //0.5#", L"Sine of x, argument in Degrees.");
	create_cmd(vm, L"cos", 'u', cmd_cos, 4, L"cos <SCALAR>", L"cos 60 //0.5#", L"Cosine of a number, argument in degrees.");
	create_cmd(vm, L"tan", 'u', cmd_tan, 4, L"tan <SCALAR>", L"tan 45 //1#", L"Tangent of x, argument in Degrees.");
	create_cmd(vm, L"exp", 'u', cmd_exp, 4, L"exp <SCALAR>", L"1 //2.7182#", L"Let x be a number, then exp (x) is equal to e to the power of x (or e^x)");
	create_cmd(vm, L"asin", 'u', cmd_asin, 4, L"asin <SCALAR>", L"asin 0.5 //30#", L"Arcsine of a number, result in Degrees.");
	create_cmd(vm, L"acos", 'u', cmd_acos, 4, L"acos <SCALAR>", L"acos 0.5 //60#", L"ArcCosine of a number, result in Degrees.");
	create_cmd(vm, L"atan", 'u', cmd_atan, 4, L"atan <SCALAR>", L"atan 1 //45#", L"ArcTangent of a number, result in Degrees. Identical to atg.");
	create_cmd(vm, L"atg", 'u', cmd_atan, 4, L"atg <SCALAR>", L"atan 1 //45#", L"ArcTangent of a number, result in Degrees. Identical to atan.");
	create_cmd(vm, L"rad", 'u', cmd_rad, 4, L"rad <SCALAR>", L"rad 180 //3.1415#", L"Convert x from Degrees to Radians. 360 degrees is equal to 2 multiplied with pi.");
	create_cmd(vm, L"sqrt", 'u', cmd_sqrt, 4, L"sqrt <SCALAR>", L"sqrt 9 //3#", L"Returns square root of x.");
	create_cmd(vm, L"ceil", 'u', cmd_ceil, 4, L"ceil <SCALAR>", L"ceil 5.25 //6#" L"ceil -5.25 //-5#", L"The ceil value of x.");
	create_cmd(vm, L"random", 'u', cmd_random, 4, L"random <SCALAR>", L"", L"Random real (floating point) value from 0 (inclusive) to x (not inclusive).");
	create_cmd(vm, L"floor", 'u', cmd_floor, 4, L"floor <SCALAR>", L"ceil 5.25 //5#" L"ceil -5.25 //-6#", L"Returns the next lowest integer in relation to x.");
	create_cmd(vm, L"ln", 'u', cmd_ln, 4, L"ln <SCALAR>", L"ln 10 //2.302#", L"Natural logarithm of x.");
	create_cmd(vm, L"round", 'u', cmd_round, 4, L"round <SCALAR>", L"round 5.25 //5#" L"round 5.55 //6#", L"Rounds up or down to the closest integer.");
	create_cmd(vm, L"!", 'u', cmd_not, 4, L"! <BOOL>", L"!true //false#" L"!false //true#", L"Returns a negation of Boolean expression. That means true becomes false and vice versa.");
	create_cmd(vm, L"comment", 'u', cmd_comment, 4, L"comment <BOOL>", L"", L"Define a comment. Mainly used in SQF Syntax, as you're able to introduce comment lines with semicolons in a SQS script.");
	create_cmd(vm, L"typeOf", 'u', cmd_typeof, 4, L"typeOf <OBJECT>", L"typeOf (\"B_Soldier_F\" createVehicle [0, 0, 0]) //\"B_Soldier_F\"#", L"Returns the config class name of given object or "L" if object doesn't have config class.");
	create_cmd(vm, L"getPos", 'u', cmd_getpos, 4, L"getPos <OBJECT>", L"_obj = \"B_Soldier_F\" createVehicle [0, 0, 0]; getPos _obj //[0, 0, 0]#", L"Returns the object position in format PositionAGLS. Z value is height over the surface underneath. Identical to position");
	create_cmd(vm, L"position", 'u', cmd_getpos, 4, L"position <OBJECT>", L"_obj = \"B_Soldier_F\" createVehicle [0, 0, 0]; position _obj //[0, 0, 0]#", L"Returns the object position in format PositionAGLS. Z value is height over the surface underneath. Identical to getPos");
	create_cmd(vm, L"velocity", 'u', cmd_velocity, 4, L"velocity <OBJECT>", L"_obj = \"B_Soldier_F\" createVehicle [0, 0, 0]; _obj setVelocity [0, 0, 10]; velocity _obj //[0, 0, 10]#", L"Return velocity (speed vector) of Unit as an array with format [x, y, z]. Units are in metres per second.");
	create_cmd(vm, L"switch", 'u', cmd_switch, 4, L"switch <ANY>", L"switch (1) do { case 1: { 10 }; case 2: { 22 }; default { false }; //10#" L"switch (0) do { case 1: { 10 }; case 2: { 22 }; default { false }; //false#", L"Creates a switch type that can be used inside a switch do -block");
	create_cmd(vm, L"case", 'u', cmd_case, 4, L"case <ANY>", L"", L"See switch");
	create_cmd(vm, L"default", 'u', cmd_default, 4, L"default <CODE>", L"", L"See switch");
	create_cmd(vm, L"allVariables", 'u', cmd_allvariables, 4, L"allVariables <OBJECT> | allVariables <NAMESPACE> | allVariables <GROUP>", L"", L"Returns a list of all variables from desired namespace.");
	create_cmd(vm, L"with", 'u', cmd_with, 4, L"with <NAMESPACE>", L"", L"Creates a With Type that is used inside a do construct in order to execute code inside a given namespace.");
	create_cmd(vm, L"compile", 'u', cmd_compile, 4, L"compile <STRING>", L"a = 10; call compile \"a = 20;\"; a //20#" L"compile \"diag_log 10;\" //{diag_log 10;}#", L"Compile expression into executable code.");
	create_cmd(vm, L"toArray", 'u', cmd_toarray, 4, L"toArray <STRING>", L"toArray \"test\" //[116, 101, 115, 116]#", L"Converts the supplied String into an Array of Numbers.");
	create_cmd(vm, L"toString", 'u', cmd_tostring, 4, L"toString <ARRAY>", L"toString [116, 101, 115, 116] //\"test\"#", L"Converts the supplied Array of Numbers into a String.");
	create_cmd(vm, L"params", 'u', cmd_params_UNARY, 4, L"params <ARRAY>", L"[1, 2, 3] call { params [\"_one\", \"_two\", \"_three\"]; _two} //2#" L"123 call { params [\"_myvar\"]; _myvar } //123#" L"[1, 2] call { private _paramsres = params [\"_var1\", \"_var2\", [\"_var3\", true, [true]]]; [_var3, _paramsres] }; //[true, false]#" L"[1, 2, 3] call { private _paramsres = params [\"_var1\", \"_var2\", [\"_var3\", true, [true]]]; [_var3, _paramsres] }; //[true, false]#" L"[1, \"ok\", [1, 2, 3]] call { private _paramsres = params [[\"_var1\", 0, [0]], [\"_var2\", "L", ["L"]], [\"_var3\", [0,0,0], [[], objNull, 0], [2,3]]]; _paramsres}; //true#", L"Parses input argument into array of private variables. When used without argument, as shown in main syntax, internal variable _this, which is usually available inside functions and event handlers, is used as argument.In addition to simple parsing directly into variables, input can be tested in case it is undefined, of the wrong type or of the wrong size(if array) and substituted if necessary with default values.");
	create_cmd(vm, L"isNil", 'u', cmd_isnil, 4, L"isNil <STRING> | isNil <CODE>", L"isNil \"_foo\" //true#" L"_foo = 10; isNil \"_foo\" //false#" L"isNil { nil } //true#" L"isNil { 10 } //false#", L"Tests whether the variable defined by the String argument is undefined, or whether an expression result passed as Code is undefined. The command returns true if the variable or the expression result is undefined(i.e.the expression result is Void), and false in all other cases.");
	create_cmd(vm, L"deleteVehicle", 'u', cmd_deletevehicle, 4, L"deleteVehicle <OBJECT>", L"", L"Deletes an object.");
	create_cmd(vm, L"createGroup", 'u', cmd_creategroup, 4, L"createGroup <SIDE>", L"", L"Creates a new Group for the given Side.");
	create_cmd(vm, L"deleteGroup", 'u', cmd_deletegroup, 4, L"deleteGroup <GROUP>", L"", L"Destroys the given group. Group must be empty.");
	create_cmd(vm, L"groupId", 'u', cmd_groupid, 4, L"groupId <GROUP>", L"", L"Returns group name.");
	create_cmd(vm, L"units", 'u', cmd_units, 4, L"units <GROUP>", L"", L"Returns an array with all the units in the group or group of the unit.");
	create_cmd(vm, L"scriptDone", 'u', cmd_scriptdone, 4, L"scriptDone <SCRIPT>", L"", L"Check if a script is finished running using the Script_(Handle) returned by execVM or spawn.");
	create_cmd(vm, L"selectRandom", 'u', cmd_selectrandom, 4, L"selectRandom <ARRAY>", L"_randomElement = selectRandom [1,2,3,4,5];", L"Returns a random element from the given array. Engine solution to BIS_fnc_selectRandom");
	create_cmd(vm, L"reverse", 'u', cmd_reverse, 4, L"reverse <ARRAY>", L"_arr = [1,2,3]; reverse _arr; hint str _arr; //[3,2,1]", L"Reverses given array by reference (modifies the original array, just like resize). ");
	create_cmd(vm, L"vectorMagnitude", 'u', cmd_vectormagnitude, 4, L"vectorMagnitude <VECTOR3D>", L"_size = vectorMagnitude [0,3,4]; //5", L"Magnitude of a 3D vector.");
	create_cmd(vm, L"vectorMagnitudeSqr", 'u', cmd_vectormagnitudesqr, 4, L"vectorMagnitudeSqr <VECTOR3D>", L"_size = vectorMagnitude [0,3,4]; //25", L"Squared magnitude of a 3D vector.");
	create_cmd(vm, L"vectorNormalized", 'u', cmd_vectornormalized, 4, L"vectorNormalized <VECTOR3D>", L"vectorNormalized [12345,7890,38383]; //[0.300481,0.192045,0.934254]", L"Returns normalized vector (unit vector, vectorMagnitude = 1) of given vector. If given vector is 0 result is a 0 vector as well.");
	create_cmd(vm, L"inheritsFrom", 'u', cmd_inheritsfrom, 4, L"inheritsFrom <CONFIG>", L"", L"Returns base entry of config entry.");
	create_cmd(vm, L"getNumber", 'u', cmd_getnumber, 4, L"getNumber <CONFIG>", L"", L"Extract number from config entry.");
	create_cmd(vm, L"getText", 'u', cmd_gettext, 4, L"getText <CONFIG>", L"", L"Extract text from config entry.");
	create_cmd(vm, L"getArray", 'u', cmd_getarray, 4, L"getArray <CONFIG>", L"", L"Extract array from config entry.");
	create_cmd(vm, L"isArray", 'u', cmd_isarray, 4, L"isArray <CONFIG>", L"", L"Check if config entry represents array.");
	create_cmd(vm, L"isClass", 'u', cmd_isclass, 4, L"isClass <CONFIG>", L"", L"Check if config entry represents config class.");
	create_cmd(vm, L"isNumber", 'u', cmd_isnumber, 4, L"isNumber <CONFIG>", L"", L"Check if config entry represents number.");
	create_cmd(vm, L"isText", 'u', cmd_istext, 4, L"isText <CONFIG>", L"", L"Check if config entry represents text.");
	create_cmd(vm, L"tolower", 'u', cmd_tolower, 4, L"tolower <STRING>", L"tolower \"ABC\"; //\"abc\"", L"Returns a string with every character lowered.");
	create_cmd(vm, L"toupper", 'u', cmd_toupper, 4, L"toupper <STRING>", L"toupper \"abc\"; //\"ABC\"", L"Returns a string with every character upped.");


	create_cmd(vm, L"true", 'n', cmd_true, 4, L"true", L"", L"");
	create_cmd(vm, L"false", 'n', cmd_false, 4, L"false", L"", L"");
	create_cmd(vm, L"parsingNamespace", 'n', cmd_parsingnamespace, 4, L"parsingNamespace", L"", L"");
	create_cmd(vm, L"missionNamespace", 'n', cmd_missionnamespace, 4, L"missionNamespace", L"", L"");
	create_cmd(vm, L"uiNamespace", 'n', cmd_uinamespace, 4, L"uiNamespace", L"", L"");
	create_cmd(vm, L"profileNamespace", 'n', cmd_profilenamespace, 4, L"profileNamespace", L"", L"");
	create_cmd(vm, L"diag_tickTime", 'n', cmd_diag_TICKTIME, 4, L"diag_tickTime", L"", L"");
	create_cmd(vm, L"pi", 'n', cmd_pi, 4, L"pi", L"", L"");
	create_cmd(vm, L"productVersion", 'n', cmd_productversion, 4, L"productVersion", L"", L"");
	create_cmd(vm, L"west", 'n', cmd_west, 4, L"west", L"", L"");
	create_cmd(vm, L"blufor", 'n', cmd_blufor, 4, L"blufor", L"", L"");
	create_cmd(vm, L"east", 'n', cmd_east, 4, L"east", L"", L"");
	create_cmd(vm, L"opfor", 'n', cmd_opfor, 4, L"opfor", L"", L"");
	create_cmd(vm, L"resistance", 'n', cmd_resistance, 4, L"resistance", L"", L"");
	create_cmd(vm, L"independent", 'n', cmd_independent, 4, L"independent", L"", L"");
	create_cmd(vm, L"sideUnknown", 'n', cmd_sideunknown, 4, L"sideUnknown", L"", L"");
	create_cmd(vm, L"sideLogic", 'n', cmd_sidelogic, 4, L"sideLogic", L"", L"");
	create_cmd(vm, L"sideFriendly", 'n', cmd_sidefriendly, 4, L"sideFriendly", L"", L"");
	create_cmd(vm, L"sideEnemy", 'n', cmd_sideenemy, 4, L"sideEnemy", L"", L"");
	create_cmd(vm, L"civilian", 'n', cmd_civilian, 4, L"civilian", L"", L"");
	create_cmd(vm, L"sideEmpty", 'n', cmd_sideempty, 4, L"sideEmpty", L"", L"");
	create_cmd(vm, L"objNull", 'n', cmd_objnull, 4, L"objNull", L"", L"");
	create_cmd(vm, L"allGroups", 'n', cmd_allgroups, 4, L"allGroups", L"", L"Return a list of all groups created. Unlike ArmA, contains groups of all sides.");
	create_cmd(vm, L"nil", 'n', cmd_nil, 4, L"nil", L"", L"Nil value. This value can be used to undefine existing variables.");
	create_cmd(vm, L"configFile", 'n', cmd_configfile, 4, L"nil", L"", L"Return root of config entries hierarchy.");



	create_cmd(vm, L"help__", 'n', cmd_help, 4, L"help__", L"help__ //nil#", L"Displays all commands available with usage, precedence and type.");
	create_cmd(vm, L"help__", 'u', cmd_help_UNARY, 4, L"help__ <STRING>", L"help__ \"typeOf\"//nil#", L"Outputs information to given command. Expects a <STRING> on righthand. Usage: help__ \"command\"");
	create_cmd(vm, L"parseconfig__", 'u', cmd_parseconfig, 4, L"parseconfig__ <STRING>", L"", L"Parses provided string into an unnamed parent confignode. Returns the unnamed node.");
	create_cmd(vm, L"mergefrom__", 'b', cmd_mergefrom, 4, L"<CONFIG> mergefrom__ <CONFIG>", L"", L"Merges the content from the righthand node into the lefthand node. Cannot merge valuenodes.");
}

int vm_output_print(PVM vm, const wchar_t* format, ...)
{
	va_list args;
	va_list args_bullshittery;
	int len;
	wchar_t* buff;
	va_start(args, format);
	va_copy(args_bullshittery, args);
#if defined(__GNUC__)
	len = 2024;
	buff = alloca(sizeof(wchar_t) * (len + 1));
	vswprintf(buff, len + 1, format, args_bullshittery);
	buff[len] = 0;
	string_modify_append(vm->print_custom_data, buff);
#else
	len = vswprintf(0, 0, format, args);
	if (len < 500)
	{
		buff = alloca(sizeof(wchar_t) * (len + 1));
		vswprintf(buff, len + 1, format, args_bullshittery);
		string_modify_append(vm->print_custom_data, buff);
	}
	else
	{
		buff = malloc(sizeof(wchar_t) * (len + 1));
		vswprintf(buff, len + 1, format, args_bullshittery);
		string_modify_append(vm->print_custom_data, buff);
		free(buff);
	}
#endif
	va_end(args);
	va_end(args_bullshittery);
	return len;
}

int load_file(PSTRING buffer, const char* fpath);
DLLEXPORT_PREFIX void load_file_into_sqf_configFile(const char* path)
{
	PVM vm = sqfvm(10000, 50, 1, 0);
	PSTRING str = string_create(0);
	PCONFIG node;
	load_file(str, path);
	node = cfgparse2(vm, str->val, 1000000, 2000);
	string_destroy(str);
	destroy_sqfvm(vm);

	config_merge(sqf_configFile(), node);
	config_destroy_node(node);
}

DLLEXPORT_PREFIX unsigned char start_program(const wchar_t* input, unsigned long max_instructions, wchar_t* buffer, size_t buffer_size, PVM vm)
{
	int val;
	int i;
	unsigned char success;
	if (vm == 0)
	{
		vm = sqfvm(10000, 50, 1, max_instructions);
		register_commmands(vm);
		vm->error = custom_error;
		vm->warn = custom_warn;
		vm->print = vm_output_print;
		vm->print_custom_data = string_create(0);
	}
	if (systime_start == 0)
	{
		systime_start = system_time_ms();
	}


	val = setjmp(program_exit);
	if (!val)
	{
		push_stack(vm, vm->stack, inst_scope(L"all"));
		parse(vm, input, 1);
		runvm(vm);
		success = vm->die_flag;
	}
	if (vm->work->top != 0)
	{
		for (i = 0; i < vm->work->top; i++)
		{
			if (vm->work->data[i]->type == INST_VALUE)
			{
				if (get_value(vm, vm->stack, vm->work->data[i])->type == NOTHING_TYPE())
					continue;
				vm->print(vm, L"[WORK]: ");
				stringify_value(vm, vm->print_custom_data, get_value(vm, vm->stack, vm->work->data[i]));
				vm->print(vm, L"\n");
			}
		}
	}
	vm->print(vm, L"Finished in %d instructions\n", vm->instcount);
	if (((PSTRING)vm->print_custom_data)->val != 0)
	{
		if (buffer == 0 || buffer_size == 0)
		{
			wprintf(((PSTRING)vm->print_custom_data)->val);
		}
		else
		{
			wcsncpy(buffer, ((PSTRING)vm->print_custom_data)->val, buffer_size);
			buffer[((PSTRING)vm->print_custom_data)->length >= buffer_size ? buffer_size - 1 : ((PSTRING)vm->print_custom_data)->length] = 0;
		}
	}
	else
	{
		if (buffer == 0 || buffer_size == 0)
		{
			wprintf(L"<EMPTY>\n");
		}
		else
		{
			wcsncpy(buffer, L"<EMPTY>\n", buffer_size);
		}
	}
	if (vm == 0)
	{
		destroy_sqfvm(vm);
		string_destroy(vm->print_custom_data);
	}
	return success;
}



#define RETCDE_OK 0
#define RETCDE_ERROR 1
#define RETCDE_RUNTIME_ERROR 2

//#define MAIN_BUFFER_SIZE 1990

int get_bom_skip(const char* buff)
{
	if (buff[0] == (char)0xEF && buff[1] == (char)0xBB && buff[2] == (char)0xBF)
	{
		//UTF-8
		return 3;
	}
	else if (buff[0] == (char)0xFE && buff[1] == (char)0xFF)
	{
		//UTF-16 (BE)
		return 2;
	}
	else if (buff[0] == (char)0xFE && buff[1] == (char)0xFE)
	{
		//UTF-16 (LE)
		return 2;
	}
	else if (buff[0] == (char)0x00 && buff[1] == (char)0x00 && buff[2] == (char)0xFF && buff[3] == (char)0xFF)
	{
		//UTF-32 (BE)
		return 2;
	}
	else if (buff[0] == (char)0xFF && buff[1] == (char)0xFF && buff[2] == (char)0x00 && buff[3] == (char)0x00)
	{
		//UTF-32 (LE)
		return 2;
	}
	else if (buff[0] == (char)0x2B && buff[1] == (char)0x2F && buff[2] == (char)0x76 &&
		(buff[3] == (char)0x38 || buff[3] == (char)0x39 || buff[3] == (char)0x2B || buff[3] == (char)0x2F))
	{
		//UTF-7
		return 4;
	}
	else if (buff[0] == (char)0xF7 && buff[1] == (char)0x64 && buff[2] == (char)0x4C)
	{
		//UTF-1
		return 3;
	}
	else if (buff[0] == (char)0xDD && buff[1] == (char)0x73 && buff[2] == (char)0x66 && buff[3] == (char)0x73)
	{
		//UTF-EBCDIC
		return 3;
	}
	else if (buff[0] == (char)0x0E && buff[1] == (char)0xFE && buff[2] == (char)0xFF)
	{
		//SCSU
		return 3;
	}
	else if (buff[0] == (char)0xFB && buff[1] == (char)0xEE && buff[2] == (char)0x28)
	{
		//BOCU-1
		if (buff[3] == (char)0xFF)
			return 4;
		return 3;
	}
	else if (buff[0] == (char)0x84 && buff[1] == (char)0x31 && buff[2] == (char)0x95 && buff[3] == (char)0x33)
	{
		//GB 18030
		return 3;
	}
	return 0;
}

int load_file(PSTRING buffer, const char* fpath)
{
	FILE* fptr = fopen(fpath, "rb");
	size_t size;
	wchar_t* buff2;
	char* filebuff;
	int tailing = 0;
	int lcount = 1;
	int i;
	unsigned int bomskip = 0;
	if (fptr == 0)
	{
		printf("[ERR] Could not open file '%s'", fpath);
		return -1;
	}
	fseek(fptr, 0, SEEK_END);
	size = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);
	filebuff = malloc(sizeof(char) * (size + 1));
	memset(filebuff, 0, sizeof(char) * (size + 1));
	fread(filebuff, sizeof(char), size, fptr);
	bomskip = get_bom_skip(filebuff);
	for (i = 0 + bomskip; i < size; i++)
	{
		if (filebuff[i] == '\n')
			lcount++;
		else if (filebuff[i] == '\0')
			tailing++;
	}

	buff2 = gen_wchar_string(filebuff + bomskip);
	free(filebuff);
	string_modify_append(buffer, buff2);
	free(buff2);
	return lcount;
}

int main(int argc, char** argv)
{
	char linebuffer[LINEBUFFER_SIZE];
	const char* ptr;
	int i, j, k;
	unsigned char just_execute = 0;
	unsigned char prog_success = 0;
	PSTRING pstr;
	PSTRING cfgbuff;
	PCONFIG confignode;
	unsigned long max_inst = 10000;
	wchar_t* tmpconverted;
	PVM vm;
#ifdef MAIN_BUFFER_SIZE
	wchar_t outbuffer[MAIN_BUFFER_SIZE];
#endif // MAIN_BUFFER_SIZE

	pstr = string_create(0);
	cfgbuff = string_create(0);
#if _WIN32 & _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(832);
#endif
	j = 0;

	for (i = 0; i < argc; i++)
	{
		ptr = argv[i];
		if (ptr[0] == '-')
		{
			switch (ptr[1])
			{
			case '\0':
				wprintf(L"[ERR] empty argument passed.");
				return RETCDE_ERROR;
			case '?':
				wprintf(L"SQF-VM Help page\n");
				wprintf(L"./prog [-j] [-m 10000] [-s <FILE>] [-S <CODE>] [-c <FILE>] [-C <CONFIG>]\n");
				wprintf(L"\t-?\tOutputs this help\n");
				wprintf(L"\t-s\tLooks up the path, and loads the SQF code inside into the code-buffer.\n");
				wprintf(L"\t-S\tLoads provided input SQF code into the code-buffer.\n");
				wprintf(L"\t-a\tDisables user input and just executes the code-buffer.\n");
				wprintf(L"\t-m\tSets the maximum instruction count allowed before termination.\n");
				wprintf(L"\t  \tMaximum value is %lu.\n", LONG_MAX);
				wprintf(L"\t  \tIf `0` is passed, the limit is disabled.\n");
				wprintf(L"\t-c\tLooks up the path, and merges the config inside into the configFile.\n");
				wprintf(L"\t-C\tMerges provided input config into the configFile.\n");
				return RETCDE_OK;
				break;
			case 'c':
				if (i + 1 < argc)
				{
					k = load_file(cfgbuff, argv[++i]);
					if (k < 0)
						return RETCDE_ERROR;
					j += k;
				}
				else
				{
					wprintf(L"[ERR] -s empty parameter");
					return RETCDE_ERROR;
				}
				break;
			case 'C':
				if (i + 1 < argc)
				{
					tmpconverted = gen_wchar_string(argv[++i]);
					string_modify_append(cfgbuff, tmpconverted);
					free(tmpconverted);
				}
				else
				{
					wprintf(L"[ERR] -I empty parameter");
					return RETCDE_ERROR;
				}
				break;
			case 's':
				if (i + 1 < argc)
				{
					k = load_file(pstr, argv[++i]);
					if (k < 0)
						return RETCDE_ERROR;
					j += k;
				}
				else
				{
					wprintf(L"[ERR] -s empty parameter");
					return RETCDE_ERROR;
				}
				break;
			case 'S':
				if (i + 1 < argc)
				{
					tmpconverted = gen_wchar_string(argv[++i]);
					string_modify_append(pstr, tmpconverted);
					free(tmpconverted);
					j++;
				}
				else
				{
					wprintf(L"[ERR] -I empty parameter");
					return RETCDE_ERROR;
				}
				break;
			case 'a':
				just_execute = 1;
				break;
			case 'm':
				if (i + 1 < argc)
				{
					max_inst = strtoul(argv[i + 1], 0, 10);
					j++;
				}
				else
				{
					wprintf(L"[ERR] -m empty parameter");
					return RETCDE_ERROR;
				}
				break;
			default:
				break;
			}
		}
	}
	ptr = 0;
	i = j;
	vm = sqfvm(10000, 50, 1, max_inst);
	register_commmands(vm);
	vm->error = custom_error;
	vm->warn = custom_warn;
	vm->print = vm_output_print;
	vm->print_custom_data = string_create(0);
	//load_file_into_sqf_configFile("C:\\Users\\marco.silipo\\Downloads\\AiO.1.62.137494\\test.cpp");
	//load_file_into_sqf_configFile("C:\\Users\\marco.silipo\\Downloads\\AiO.1.62.137494\\AiO.1.62.137494.cpp");
	if (cfgbuff->length != 0)
	{
		confignode = cfgparse(vm, cfgbuff->val);
		config_merge(sqf_configFile(), confignode);
		config_destroy_node(confignode);
	}

	if (!just_execute)
	{
		wprintf(L"Please enter your SQF code.\nTo get the capabilities, use the `help__` instruction.\nTo run the code, Press <ENTER> twice.\n");
		wprintf(L"%d:\t", i++);
		while (get_line(linebuffer, LINEBUFFER_SIZE)[0] != '\n')
		{
			tmpconverted = gen_wchar_string(linebuffer);
			string_modify_append(pstr, tmpconverted);
			free(tmpconverted);
			wprintf(L"%d:\t", i++);
		}
		//string_modify_append(pstr, L"diag_log str [1, 2, \"test\", [1, 2, 3]]");
	}
#ifdef MAIN_BUFFER_SIZE
	if (just_execute)
	{
		if (pstr->length > 0)
		{
			prog_success = start_program(pstr->val, max_inst, outbuffer, MAIN_BUFFER_SIZE, vm);
			wprintf(L"%ls\n", outbuffer);
		}
	}
	else
	{
		wprintf(L"-------------------------------------\n");
		if (pstr->length > 0)
		{
			prog_success = start_program(pstr->val, max_inst, outbuffer, MAIN_BUFFER_SIZE, vm);
			wprintf(L"%ls\n", outbuffer);
		}
		wprintf(L"-------------------------------------\n");
		wprintf(L"Press <ENTER> to finish.");
		get_line(linebuffer, LINEBUFFER_SIZE);
	}
#else
	if (just_execute)
	{
		if (pstr->length > 0)
			prog_success = start_program(pstr->val, max_inst, 0, 0, vm);
	}
	else
	{
		wprintf(L"-------------------------------------\n");
		if (pstr->length > 0)
			prog_success = start_program(pstr->val, max_inst, 0, 0, vm);
		wprintf(L"-------------------------------------\n");
		wprintf(L"Press <ENTER> to finish.");
		get_line(linebuffer, LINEBUFFER_SIZE);
	}
#endif // MAIN_BUFFER_SIZE
	string_destroy(vm->print_custom_data);
	config_destroy_node(sqf_configFile());
	destroy_sqfvm(vm);
	string_destroy(pstr);
	string_destroy(cfgbuff);
	namespace_destroy(sqf_missionNamespace());
	namespace_destroy(sqf_parsingNamespace());
	namespace_destroy(sqf_profileNamespace());
	namespace_destroy(sqf_uiNamespace());

	wsm_destroy_list(sqf_group_map(), inst_destroy_value);

	destroy_cmdcnt(GET_PCMDCNT());

#if _WIN32 & _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return prog_success ? RETCDE_OK : RETCDE_RUNTIME_ERROR;
}
void cmd_productversion(void* input, CPCMD self)
{
	PVM vm = input;

	//String(Arma 3) - Architecture - x64 for 64bit or x86 for 32bit Since version 1.67
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(string_create2(L"x86")))));
	//String(Arma 3) - Platform(L"Windows", "Linux" or "OSX")
	push_stack(vm, vm->stack, inst_arr_push());
#if !_WIN32 & !__linux
	push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(string_create2(L"UNKNOWN")))));
#else
#ifdef _WIN32
	push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(string_create2(L"WINDOWS")))));
#endif // _WIN32
#ifdef __linux
	push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(string_create2(L"UNIX")))));
#endif
#endif
	//Boolean(Arma 3) - Product is launched using mods true or vanilla false
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(BOOL_TYPE(), base_int(0))));
	//String(Arma 3) - Product Branch
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(string_create2(L"COMMUNITY")))));
	//Number - Product Build Number
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(3))));
	//Number - Product Version Number
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(SCALAR_TYPE(), base_float(01))));
	//String - Product Name Short
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(string_create2(L"SQF-VM")))));
	//String - Product Name-
	push_stack(vm, vm->stack, inst_arr_push());
	push_stack(vm, vm->stack, inst_value(value(STRING_TYPE(), base_voidptr(string_create2(L"SQF-VM (0.1.4 ALPHA)")))));


	push_stack(vm, vm->stack,
		inst_value(value(ARRAY_TYPE(), base_voidptr(array_create2(8)))));
}
