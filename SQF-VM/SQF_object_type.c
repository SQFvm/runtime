#include "basetype.h"
#include "string_map.h"
#include "SQF.h"
#include "SQF_types.h"
#include "SQF_object_type.h"

#include <stdlib.h>
#include <string.h>


void TYPE_OBJECT_CALLBACK(void* input, CPCMD self)
{
	PVALUE val = input;
	POBJECT obj = val->val.ptr;
	if (val->type == 0)
	{
		obj->refcount--;
		if (obj->refcount <= 0)
		{
			object_destroy(obj);
		}
	}
	else
	{
		obj->refcount++;
	}
}
PCMD OBJECT_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command("OBJECT", 't', TYPE_OBJECT_CALLBACK, 0, 0);
	}
	return cmd;
}
POBJECT object_create(const char* classname)
{
	POBJECT obj = malloc(sizeof(OBJECT));
	int len = 0;
	obj->refcount = 0;
	obj->posX = 0;
	obj->posY = 0;
	obj->posZ = 0;
	obj->velX = 0;
	obj->velY = 0;
	obj->velZ = 0;
	obj->healthpoints = 1;
	obj->allow_damage = 1;
	obj->classname = 0;
	if (classname != 0)
	{
		len = strlen(classname);
		obj->classname = malloc(sizeof(char) * (len + 1));
		strcpy(obj->classname, classname);
		obj->classname[len] = '\0';
	}
	obj->ns = namespace_create();
	return obj;
}
void object_destroy(POBJECT obj)
{
	namespace_destroy(obj->ns);
	free(obj->classname);
	free(obj);
}