#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <stdint.h>
#include <stdbool.h>

#include "sqffull.h"


extern inline bool object_is_null(POBJECT obj);

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
		cmd = create_command(L"OBJECT", 't', TYPE_OBJECT_CALLBACK, 0, 0, 0, 0);
	}
	return cmd;
}
POBJECT object_create(const wchar_t* classname)
{
	POBJECT obj = malloc(sizeof(OBJECT));
	int len = 0;
	obj->refcount = 0;
	obj->position.x = 0;
	obj->position.y = 0;
	obj->position.z = 0;
	obj->velocity.x = 0;
	obj->velocity.y = 0;
	obj->velocity.z = 0;
	obj->healthpoints = 1;
	obj->allow_damage = true;
	obj->classname = 0;
	obj->inventory = array_create();
	if (classname != 0)
	{
		len = wcslen(classname);
		obj->classname = malloc(sizeof(wchar_t) * (len + 1));
		wcscpy(obj->classname, classname);
		obj->classname[len] = '\0';
	}
	obj->ns = namespace_create();
	obj->is_vehicle = false;
	obj->inner = 0;
	return obj;
}

POBJECT object_unit_create(const wchar_t* classname, PGROUP group)
{
	POBJECT obj = object_create(classname);
	obj->is_vehicle = 0;
	PUNIT unit;
	obj->inner = malloc(sizeof(UNIT));
	unit = obj->inner;
	unit->displayname = string_create(0);
	unit->groupident = string_create2(group->ident);
	array_push(group->members->val.ptr, value(OBJECT_TYPE(), base_voidptr(obj)));
	return obj;
}
POBJECT object_vehicle_create(const wchar_t* classname)
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
		string_destroy(unit->groupident);
	}
	free(obj->inner);
	obj->inner = 0;
}