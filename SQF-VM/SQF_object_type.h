#ifndef _SQF_OBJECT_TYPE_H_
#define _SQF_OBJECT_TYPE_H_


#ifndef _SQF_TYPES_H_
#error "SQF_object_types.h" has to be included after "SQF_types.h" 
#endif // !_SQF_TYPES_H_


typedef struct OBJECT
{
	PNAMESPACE ns;
	float posX;
	float posY;
	float posZ;
	float velX;
	float velY;
	float velZ;
	float healthpoints;
	unsigned char allow_damage;
	char* classname;
	int refcount;
	POBJECT in_vehicle;

}OBJECT;
typedef OBJECT* POBJECT;
PCMD OBJECT_TYPE(void);
POBJECT object_create(const char* classname);
void object_destroy(POBJECT obj);




#endif // !_SQF_OBJECT_TYPE_H_
