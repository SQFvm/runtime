#ifndef _SQF_OBJECT_TYPE_H_
#define _SQF_OBJECT_TYPE_H_


#ifndef _VECTOR_H_
#error "SQF_object_types.h" has to be included after "vector.h" 
#endif // !_SQF_TYPES_H_
#ifndef _SQF_TYPES_H_
#error "SQF_object_types.h" has to be included after "SQF_types.h" 
#endif // !_SQF_TYPES_H_



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
	unsigned char allow_damage;
	char* classname;
	int refcount;
	void* inner;
	unsigned char is_vehicle;
}OBJECT;
typedef OBJECT* POBJECT;




inline unsigned char object_is_null(POBJECT obj)
{
	return obj == 0 || obj->inner == 0;
}

PCMD OBJECT_TYPE(void);
POBJECT object_create(const char* classname);
POBJECT object_unit_create(const char* classname, PGROUP group);
POBJECT object_vehicle_create(const char* classname);
void object_destroy(POBJECT obj);
void object_destroy_inner(POBJECT obj);




#endif // !_SQF_OBJECT_TYPE_H_
