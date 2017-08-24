#include "string_map.h"
#include "string_op.h"
#include <malloc.h>
#include <string.h>

extern inline void* sm_get_value(sm_list* list, const char* name);
extern inline void* sm_set_value(sm_list* list, const char* name, void* value);

sm_bucket* sm_create_bucket(unsigned int initial_size, unsigned int growth)
{
	sm_bucket* bucket = malloc(sizeof(sm_bucket));
	bucket->growth = growth;
	bucket->names = malloc(sizeof(char*) * initial_size);
	bucket->values = malloc(sizeof(void*) * initial_size);
	bucket->size = initial_size;
	bucket->top = 0;
	return bucket;
}
void sm_destroy_bucket(sm_bucket* bucket, void(*destroy_ptr)(void*))
{
	int i;
	if (destroy_ptr == 0)
	{
		for (i = 0; i < bucket->top; i++)
		{
			free(bucket->names[i]);
		}
	}
	else
	{
		for (i = 0; i < bucket->top; i++)
		{
			destroy_ptr(bucket->values[i]);
			free(bucket->names[i]);
		}
	}
	free(bucket->names);
	free(bucket->values);
	free(bucket);
}
int sm_grow_bucket(sm_bucket* bucket)
{
	char** names = realloc(bucket->names, sizeof(char*) * (bucket->size + bucket->growth));
	void** values = realloc(bucket->values, sizeof(void*) * (bucket->size + bucket->growth));

	if (names != 0)
	{
		bucket->names = names;
	}
	if (values != 0)
	{
		bucket->values = values;
	}
	if (names != 0 && values != 0)
	{
		bucket->size += bucket->growth;
		return 0;
	}
	else
	{
		return -1;
	}
}

sm_list* sm_create_list(unsigned int bucketcount, unsigned int initial_bucket_size, unsigned int bucket_growth)
{
	sm_list* list = malloc(sizeof(sm_list));
	int i;
	list->buckets = malloc(sizeof(sm_bucket*) * bucketcount);
	list->buckets_size = bucketcount;
	for (i = 0; i < bucketcount; i++)
	{
		list->buckets[i] = sm_create_bucket(initial_bucket_size, bucket_growth);
	}
	return list;
}
void sm_destroy_list(sm_list* list, void(*destroy_ptr)(void*))
{
	int i;
	for (i = 0; i < list->buckets_size; i++)
	{
		sm_destroy_bucket(list->buckets[i], destroy_ptr);
	}
	free(list);
}

sm_bucket* sm_get_bucket_for(sm_list* list, const char* name)
{
	unsigned int hash = 1;
	int i;
	for (i = 0; name[i] != '\0'; i++)
	{
		hash += name[i] * 2.0 * (double)hash;
		hash >>= 1;
	}
	hash %= list->buckets_size;
	return list->buckets[hash];
}
void* sm_get_value_from_bucket(sm_bucket* bucket, const char* name)
{
	int i;
	for (i = 0; i < bucket->top; i++)
	{
		if (!str_cmpi(bucket->names[i], -1, name, -1))
		{
			return bucket->values[i];
		}
	}
	return 0;
}
void* sm_set_value_in_bucket(sm_bucket* bucket, const char* name, void* value)
{
	int i;
	void* val;
	for (i = 0; i < bucket->top; i++)
	{
		if (!str_cmpi(bucket->names[i], -1, name, -1))
		{
			val = bucket->values[i];
			bucket->values[i] = value;
			return val;
		}
	}
	if (bucket->top == bucket->size)
	{
		sm_grow_bucket(bucket);
	}
	bucket->values[bucket->top] = value;
	i = strlen(name);
	bucket->names[bucket->top] = malloc(sizeof(char) * (i + 1));
	strcpy(bucket->names[bucket->top], name);
	bucket->names[bucket->top][i] = '\0';
	bucket->top++;
	return 0;
}