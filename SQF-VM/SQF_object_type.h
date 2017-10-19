#ifndef _SQF_OBJECT_TYPE_H_
#define _SQF_OBJECT_TYPE_H_

#ifndef __bool_true_false_are_defined
#error "SQF_object_type.h" requires stdbool header "stdbool.h"
#endif // !__bool_true_false_are_defined

#ifndef _VECTOR_H_
#error "SQF_object_types.h" has to be included after "vector.h" 
#endif // !_SQF_TYPES_H_


#ifndef _SQF_TYPES_H_
#error "SQF_object_types.h" has to be included after "SQF_types.h" 
#endif // !_SQF_TYPES_H_

#ifndef _SQF_ARRAY_TYPE_H_
#error "SQF_object_types.h" has to be included after "sqf_array_type.h" 
#endif // !_SQF_ARRAY_TYPE_H_



typedef struct UNIT
{
	PSTRING displayname;
	PSTRING groupident;
}UNIT;
typedef UNIT* PUNIT;

typedef struct VEHICLE
{
	PVALUE gunner;
	PVALUE commander;
	PVALUE driver;
	PARRAY crew;
}VEHICLE;
typedef VEHICLE* PVEHICLE;

typedef struct OBJECT
{
	PNAMESPACE ns;
	PARRAY inventory;
	vec3 position;
	vec3 velocity;
	float healthpoints;
	bool allow_damage;
	wchar_t* classname;
	int refcount;
	void* inner;
	bool is_vehicle;
}OBJECT;
typedef OBJECT* POBJECT;




inline bool object_is_null(POBJECT obj)
{
	return obj == 0 || obj->inner == 0;
}

PCMD OBJECT_TYPE(void);
POBJECT object_create(const wchar_t* classname);
POBJECT object_unit_create(const wchar_t* classname, PGROUP group);
POBJECT object_vehicle_create(const wchar_t* classname);
void object_destroy(POBJECT obj);
void object_destroy_inner(POBJECT obj);




#endif // !_SQF_OBJECT_TYPE_H_
