#ifndef _WSTRING_MAP_H_
#define _WSTRING_MAP_H_

typedef struct wsm_bucket
{
	wchar_t** names;
	void** values;
	unsigned int size;
	unsigned int top;

	unsigned int growth;
} wsm_bucket;
typedef struct wsm_list
{
	wsm_bucket** buckets;
	unsigned int buckets_size;
} wsm_list;

/*
Creates a new wsm_bucket on HEAP and returns a pointer to it.

param 1: initial size of the bucket
param 2: the ammount of slots the bucket should grow when wsm_grow_bucket is called
*/
wsm_bucket* wsm_create_bucket(unsigned int initial_size, unsigned int growth);
/*
Destroys provided bucket and frees all memory associated with it (pointer passed in param 1 will get invalid).

param 1: valid pointer to a bucket allocated by wsm_create_bucket
param 2: optional function to call when destroying values. If not required, pass NULL
*/
void wsm_destroy_bucket(wsm_bucket* bucket, void(*destroy_ptr)(void*));
/*
Increases amount of slots available in given bucket.

param 1: pointer to the bucket to increase
*/
int wsm_grow_bucket(wsm_bucket* bucket);

/*
Creates a new list containing given amount of buckets where each bucket will have provided size and growth.

param 1: the total bucket count of this list.
param 2: the initial size of each bucket.
param 3: the growth of each bucket.
*/
wsm_list* wsm_create_list(unsigned int bucketcount, unsigned int initial_bucket_size, unsigned int bucket_growth);
/*
Destroys provided list and all buckets it contains.
All memory associated with it will be freed (pointer passed in param 1 will get invalid).

param 1: valid pointer to a list allocated by wsm_create_list
param 2: optional function to call when destroying values. If not required, pass NULL
*/
void wsm_destroy_list(wsm_list* list, void(*destroy_ptr)(void*));

/*
Received correct bucket for provided name.

param 1: valid pointer to a list allocated by wsm_create_list
param 2: \0 terminated string as identifier
*/
wsm_bucket* wsm_get_bucket_for(wsm_list* list, const wchar_t* name);

/*
Searches for the provided name in given bucket and returns the value.
If nothing is found, a NULL pointer will be returned.

param 1: valid pointer to a bucket allocated by wsm_create_bucket
param 2: \0 terminated string as identifier
*/
void* wsm_get_value_from_bucket(wsm_bucket* bucket, const wchar_t* name);

/*
Searches for the provided name in given bucket and drops it.
All indexes above it will be shifted by one down.
returns the value.
If nothing is found, a NULL pointer will be returned.

param 1: valid pointer to a list allocated by wsm_create_list
param 2: \0 terminated string as identifier
*/
void* wsm_drop_value_from_bucket(wsm_bucket* bucket, const wchar_t* name);
/*
Searches for the provided name in given bucket and sets the value slot for the name to given value.
If there was already a value at that spot, it will be returned.
In any other case, NULL is returned.

param 1: valid pointer to a list allocated by wsm_create_list
param 2: \0 terminated string as identifier
param 3: pointer to the value to set
*/
inline void* wsm_get_value(wsm_list* list, const wchar_t* name)
{
	return wsm_get_value_from_bucket(wsm_get_bucket_for(list, name), name);
}
/*
Searches for the provided name in given bucket and drops it.
All indexes above it will be shifted by one down.
returns the value.
If nothing is found, a NULL pointer will be returned.

param 1: valid pointer to a list allocated by wsm_create_list
param 2: \0 terminated string as identifier
*/
inline void* wsm_drop_value(wsm_list* list, const wchar_t* name)
{
	return wsm_drop_value_from_bucket(wsm_get_bucket_for(list, name), name);
}
/*
Searches for the provided name in given bucket and sets the value slot for the name to given value.
If there was already a value at that spot, it will be returned.
In any other case, NULL is returned.

param 1: valid pointer to a bucket allocated by wsm_create_bucket
param 2: \0 terminated string as identifier
param 3: pointer to the value to set
*/
void* wsm_set_value_in_bucket(wsm_bucket* bucket, const wchar_t* name, void* value);
/*
Searches for the provided name in given lists buckets and sets the value slot for the name to given value.
If there was already a value at that spot, it will be returned.
In any other case, NULL is returned.

param 1: valid pointer to a list allocated by wsm_create_list
param 2: \0 terminated string as identifier
param 3: pointer to the value to set
*/
inline void* wsm_set_value(wsm_list* list, const wchar_t* name, void* value)
{
	return wsm_set_value_in_bucket(wsm_get_bucket_for(list, name), name, value);
}

/*
Gets the total item count in provided list.

param 1: valid pointer to a list allocated by wsm_create_list
*/
unsigned int wsm_count(wsm_list* list);
/*
Receives an item via index from provided list.
Indexes might change when new items are added!
Only should be used for enumeration.
Returns 0 if index was too large;

param 1: valid pointer to a list allocated by wsm_create_list
param 2: Current index of enumeration
*/
void* wsm_get_value_index(wsm_list* list, unsigned int index);
/*
Receives an items name via index from provided list.
Indexes might change when new items are added!
Only should be used for enumeration.
Returns 0 if index was too large;

param 1: valid pointer to a list allocated by wsm_create_list
param 2: Current index of enumeration
*/
wchar_t* wsm_get_name_index(wsm_list* list, unsigned int index);


#endif // !_WSTRING_MAP_H_
