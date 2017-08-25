#ifndef _STRING_MAP_H_
#define _STRING_MAP_H_

typedef struct sm_bucket
{
	char** names;
	void** values;
	unsigned int size;
	unsigned int top;

	unsigned int growth;
} sm_bucket;
typedef struct sm_list
{
	sm_bucket** buckets;
	unsigned int buckets_size;
} sm_list;

/*
Creates a new sm_bucket on HEAP and returns a pointer to it.

param 1: initial size of the bucket
param 2: the ammount of slots the bucket should grow when sm_grow_bucket is callen
*/
sm_bucket* sm_create_bucket(unsigned int initial_size, unsigned int growth);
/*
Destroys provided bucket and frees all memory associated with it (pointer passed in param 1 will get invalid).

param 1: valid pointer to a bucket allocated by sm_create_bucket
param 2: optional function to call when destroying values. If not required, pass NULL
*/
void sm_destroy_bucket(sm_bucket* bucket, void(*destroy_ptr)(void*));
/*
Increases amount of slots available in given bucket.

param 1: pointer to the bucket to increase
*/
int sm_grow_bucket(sm_bucket* bucket);

/*
Creates a new list containing given amount of buckets where each bucket will have provided size and growth.

param 1: the total bucket count of this list.
param 2: the initial size of each bucket.
param 3: the growth of each bucket.
*/
sm_list* sm_create_list(unsigned int bucketcount, unsigned int initial_bucket_size, unsigned int bucket_growth);
/*
Destroys provided list and all buckets it contains.
All memory associated with it will be freed (pointer passed in param 1 will get invalid).

param 1: valid pointer to a list allocated by sm_create_list
param 2: optional function to call when destroying values. If not required, pass NULL
*/
void sm_destroy_list(sm_list* list, void(*destroy_ptr)(void*));

/*
Received correct bucket for provided name.

param 1: valid pointer to a list allocated by sm_create_list
param 2: \0 terminated string as identifier
*/
sm_bucket* sm_get_bucket_for(sm_list* list, const char* name);

/*
Searches for the provided name in given bucket and returns the value.
If nothing is found, a NULL pointer will be returned.

param 1: valid pointer to a bucket allocated by sm_create_bucket
param 2: \0 terminated string as identifier
*/
void* sm_get_value_from_bucket(sm_bucket* bucket, const char* name);
/*
Searches for the provided name in given bucket and sets the value slot for the name to given value.
If there was already a value at that spot, it will be returned.
In any other case, NULL is returned.

param 1: valid pointer to a list allocated by sm_create_list
param 2: \0 terminated string as identifier
param 3: pointer to the value to set
*/
inline void* sm_get_value(sm_list* list, const char* name)
{
	return sm_get_value_from_bucket(sm_get_bucket_for(list, name), name);
}
void* sm_set_value_in_bucket(sm_bucket* bucket, const char* name, void* value);
/*
Searches for the provided name in given lists buckets and sets the value slot for the name to given value.
If there was already a value at that spot, it will be returned.
In any other case, NULL is returned.

param 1: valid pointer to a list allocated by sm_create_list
param 2: \0 terminated string as identifier
param 3: pointer to the value to set
*/
inline void* sm_set_value(sm_list* list, const char* name, void* value)
{
	return sm_set_value_in_bucket(sm_get_bucket_for(list, name), name, value);
}


#endif // !_STRING_MAP_H_
