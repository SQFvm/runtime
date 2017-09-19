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
	obj->inventory = array_create();
	if (classname != 0)
	{
		len = strlen(classname);
		obj->classname = malloc(sizeof(char) * (len + 1));
		strcpy(obj->classname, classname);
		obj->classname[len] = '\0';
	}
	obj->ns = namespace_create();
	obj->is_vehicle = 0;
	return obj;
}

POBJECT object_unit_create(const char* classname)
{
	POBJECT obj = object_create(classname);
	obj->is_vehicle = 0;
	PUNIT unit;
	obj->inner = malloc(sizeof(UNIT));
	unit = obj->inner;
	unit->displayname = string_create(0);
	return obj;
}
POBJECT object_vehicle_create(const char* classname)
{
	POBJECT obj = object_create(classname);
	obj->is_vehicle = 1;
	PVEHICLE veh;
	obj->inner = malloc(sizeof(VEHICLE));
	veh = obj->inner;
	veh->commander = 0;
	veh->gunner = 0;
	veh->driver = 0;
	veh->crew = array_create();
	return obj;
}

void object_destroy(POBJECT obj)
{
	if (obj->inner != 0)
	{
		object_destroy_inner(obj);
	}
	array_destroy(obj->inventory);
	namespace_destroy(obj->ns);
	free(obj->classname);
	free(obj);
}
void object_destroy_inner(POBJECT obj)
{
	PVEHICLE veh;
	PUNIT unit;
	if (obj->is_vehicle)
	{
		veh = obj->inner;
		inst_destroy_value(veh->commander);
		inst_destroy_value(veh->gunner);
		inst_destroy_value(veh->driver);
		array_destroy(veh->crew);
	}
	else
	{
		unit = obj->inner;
		string_destroy(unit->displayname);
	}
	obj->inner = 0;
}