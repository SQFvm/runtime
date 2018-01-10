#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>
#include <stdbool.h>

#include "wstring_op.h"
#include "wstring_map.h"

extern inline void* wsm_get_value(wsm_list* list, const wchar_t* name);
extern inline void* wsm_set_value(wsm_list* list, const wchar_t* name, void* value);
extern inline void* wsm_drop_value(wsm_list* list, const wchar_t* name);



wsm_bucket* wsm_create_bucket(unsigned int initial_size, unsigned int growth)
{
	wsm_bucket* bucket = malloc(sizeof(wsm_bucket));
	bucket->growth = growth;
	bucket->names = malloc(sizeof(wchar_t*) * initial_size);
	bucket->values = malloc(sizeof(void*) * initial_size);
	bucket->size = initial_size;
	bucket->top = 0;
	return bucket;
}
void wsm_destroy_bucket(wsm_bucket* bucket, void(*destroy_ptr)(void*))
{
	int i;
	if (destroy_ptr == 0)
	{
		for (i = 0; i < (int)bucket->top; i++)
		{
			free(bucket->values[i]);
			free(bucket->names[i]);
		}
	}
	else
	{
		for (i = 0; i < (int)bucket->top; i++)
		{
			destroy_ptr(bucket->values[i]);
			free(bucket->names[i]);
		}
	}
	free(bucket->names);
	free(bucket->values);
	free(bucket);
}
int wsm_grow_bucket(wsm_bucket* bucket)
{
	wchar_t** names = realloc(bucket->names, sizeof(wchar_t*) * (bucket->size + bucket->growth));
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

wsm_list* wsm_create_list(unsigned int bucketcount, unsigned int initial_bucket_size, unsigned int bucket_growth)
{
	wsm_list* list = malloc(sizeof(wsm_list));
	int i;
	list->buckets = malloc(sizeof(wsm_bucket*) * bucketcount);
	list->buckets_size = bucketcount;
	for (i = 0; i < (int)bucketcount; i++)
	{
		list->buckets[i] = wsm_create_bucket(initial_bucket_size, bucket_growth);
	}
	return list;
}
void wsm_destroy_list(wsm_list* list, void(*destroy_ptr)(void*))
{
	int i;
	for (i = 0; i < (int)list->buckets_size; i++)
	{
		wsm_destroy_bucket(list->buckets[i], destroy_ptr);
	}
	free(list->buckets);
	free(list);
}

wsm_bucket* wsm_get_bucket_for(wsm_list* list, const wchar_t* name)
{
	unsigned int hash = 1;
	int i;
	for (i = 0; name[i] != '\0'; i++)
	{
		hash += (int)(tolower(name[i]) * 2.0 * (double)hash);
		hash >>= 1;
	}
	hash %= list->buckets_size;
	return list->buckets[hash];
}
void* wsm_get_value_from_bucket(wsm_bucket* bucket, const wchar_t* name)
{
	int i;
	for (i = 0; i < (int)bucket->top; i++)
	{
		if (!wstr_cmpi(bucket->names[i], -1, name, -1))
		{
			return bucket->values[i];
		}
	}
	return 0;
}
void* wsm_set_value_in_bucket(wsm_bucket* bucket, const wchar_t* name, void* value)
{
	int i;
	void* val;
	for (i = 0; i < (int)bucket->top; i++)
	{
		if (!wstr_cmpi(bucket->names[i], -1, name, -1))
		{
			val = bucket->values[i];
			bucket->values[i] = value;
			return val;
		}
	}
	if (bucket->top == bucket->size)
	{
		wsm_grow_bucket(bucket);
	}
	bucket->values[bucket->top] = value;
	i = wcslen(name);
	bucket->names[bucket->top] = malloc(sizeof(wchar_t) * (i + 1));
	wcscpy(bucket->names[bucket->top], name);
	bucket->names[bucket->top][i] = '\0';
	bucket->top++;
	return 0;
}

void* wsm_drop_value_from_bucket(wsm_bucket* bucket, const wchar_t* name)
{
	int i;
	bool value_found = false;
	void* val = 0;
	for (i = 0; i < (int)bucket->top; i++)
	{
		if (value_found)
		{
			bucket->values[i - 1] = bucket->values[i];
			bucket->names[i - 1] = bucket->names[i];
		}
		else
		{
			if (!wstr_cmpi(bucket->names[i], -1, name, -1))
			{
				val = bucket->values[i];
				free(bucket->names[i]);
				value_found = true;
			}
		}
	}
	if (val != 0)
	{
		bucket->top--;
	}
	return val;
}

unsigned int wsm_count(wsm_list* list)
{
	unsigned int count = 0;
	int i;
	wsm_bucket* cur;
	for (i = 0; i < (int)list->buckets_size; i++)
	{
		cur = list->buckets[i];
		count += cur->top;
	}
	return count;
}
void* wsm_get_value_index(wsm_list* list, unsigned int index)
{
	unsigned int count = 0;
	int i;
	wsm_bucket* cur;
	for (i = 0; i < (int)list->buckets_size; i++)
	{
		cur = list->buckets[i];
		count += cur->top;
		if (count > index)
		{
			count -= cur->top;
			index -= count;
			return cur->values[index];
		}
	}
	return 0;
}
wchar_t* wsm_get_name_index(wsm_list* list, unsigned int index)
{
	unsigned int count = 0;
	int i;
	wsm_bucket* cur;
	for (i = 0; i < (int)list->buckets_size; i++)
	{
		cur = list->buckets[i];
		count += cur->top;
		if (count > index)
		{
			count -= cur->top;
			index -= count;
			return cur->names[index];
		}
	}
	return 0;
}