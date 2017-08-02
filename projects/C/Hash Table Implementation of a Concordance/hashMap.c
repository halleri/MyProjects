/* CS261- Assignment 6 - hashMap.c/
/* Name: Eric Hall
* Date: Monday, August 03, 2015
 * Solution description: Implementation of Hash Map */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hashMap.h"



/*the first hashing function you can use*/
int stringHash1(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += str[i];
	return r;
}

/*the second hashing function you can use*/
int stringHash2(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += (i+1) * str[i]; /*the difference between stringHash1 and stringHash2 is on this line*/
	return r;
}

/* initialize the supplied hashMap struct*/
void _initMap (struct hashMap * ht, int tableSize)
{
	int index;
	if(ht == NULL)
		return;
	ht->table = (hashLink**)malloc(sizeof(hashLink*) * tableSize);
	ht->tableSize = tableSize;
	ht->count = 0;
	for(index = 0; index < tableSize; index++)
		ht->table[index] = NULL;
}

/* allocate memory and initialize a hash map*/
hashMap *createMap(int tableSize) {
	assert(tableSize > 0);
	hashMap *ht;
	ht = malloc(sizeof(hashMap));
	assert(ht != 0);
	_initMap(ht, tableSize);
	return ht;
}

/*
 Free all memory used by the buckets.
 Note: Before freeing up a hashLink, free the memory occupied by key and value
 */
void _freeMap (struct hashMap * ht)
{
	/*write this*/
	for (int index = 0; index < ht->tableSize; index++){

		struct hashLink* current = ht->table[index];
		while(current != NULL){
			struct hashLink * next = current->next;
			free(current);
			current = next;
		}
		ht->table[index] = 0;
	}
	free(ht->table);
	ht->tableSize = 0;
	ht->count = 0;

}

/* Deallocate buckets and the hash map.*/
void deleteMap(hashMap *ht) {
	assert(ht!= 0);
	/* Free all memory used by the buckets */
	_freeMap(ht);
	/* free the hashMap struct */
	free(ht);
}

/*
Resizes the hash table to be the size newTableSize
*/
void _setTableSize(struct hashMap * ht, int newTableSize)
{
	/*write this*/
	hashMap* tmp = createMap(newTableSize);
	for (int index = 0; index < ht->tableSize; index++){
		hashLink* current = ht->table[index];

		while(current != NULL){
			insertMap(tmp, current->key, current->value);
			current = current->next;
		}
	}
	_freeMap(ht);
	ht->count = tmp->count;
	ht->table = tmp->table;
	ht->tableSize = tmp->tableSize;

	tmp->table = NULL;
	free(tmp);
}

/*
 insert the following values into a hashLink, you must create this hashLink but
 only after you confirm that this key does not already exist in the table. For example, you
 cannot have two hashLinks for the word "taco".

 if a hashLink already exists in the table for the key provided you should
 replace that hashLink--this requires freeing up the old memory pointed by hashLink->value
 and then pointing hashLink->value to value v.

 also, you must monitor the load factor and resize when the load factor is greater than
 or equal LOAD_FACTOR_THRESHOLD (defined in hashMap.h).
 */
void insertMap (struct hashMap * ht, KeyType k, ValueType v)
{
	/*write this*/
	struct hashLink * newMap = malloc(sizeof(struct hashLink));
	newMap->next = 0;
	newMap->key = k;
	newMap->value = v;

	if(containsKey(ht, k)){
		removeKey(ht, k);
	}
	int index = stringHash1(k) % ht->tableSize;

	if(index < 0){
		index = index + ht->tableSize;
	}
	if(!(ht->table[index])){
		ht->table[index] = newMap;
	}
	else{
		struct hashLink * current = ht->table[index];
		while(current->next){
			current = current->next;
		}
		current->next = newMap;
	}
	ht->count++;
}

/*
 this returns the value (which is void*) stored in a hashLink specified by the key k.

 if the user supplies the key "taco" you should find taco in the hashTable, then
 return the value member of the hashLink that represents taco.

 if the supplied key is not in the hashtable return NULL.
 */
ValueType* atMap (struct hashMap * ht, KeyType k)
{
	/*write this*/
	int index;
	#if(HASHING_FUNCTION == 1)
		index = stringHash1(k) % ht->tableSize;

	#elif(HASHING_FUNCTION == 2)
		index = stringHash2(k) % ht->tableSize;

	#else
	#error HASHING_FUNCTION not valid.
	#endif

		hashLink* current = ht->table[index];

		while(current != NULL) {

			if(strcmp(current->key, k) == 0){
				return &current->value;
			}
			current = current->next;
		}
		return NULL;
}

/*
 a simple yes/no if the key is in the hashtable.
 0 is no, all other values are yes.
 */
int containsKey (struct hashMap * ht, KeyType k)
{
	/*write this*/
	int index;

	#if HASHING_FUNCTION == 1
		index = stringHash1(k) % ht->tableSize;

	#elif HASHING_FUNCTION == 2
		index = stringHash1(k) % ht->tableSize;

	#else
	#error HASHING_FUNCTION not valid.
	#endif

	hashLink* current = ht->table[index];

	while(current != NULL){
		if(strcmp(current->key, k) == 0){
			return 1;
		}
		current = current->next;
	}
	return 0;
}

/*
 find the hashlink for the supplied key and remove it, also freeing the memory
 for that hashlink. it is not an error to be unable to find the hashlink, if it
 cannot be found do nothing (or print a message) but do not use an assert which
 will end your program.
 */
void removeKey (struct hashMap * ht, KeyType k)
{
	int index;
	/*write this*/
	#if HASHING_FUNCTION == 1
		index = stringHash1(k) % ht->tableSize;

	#elif HASHING_FUNCTION == 2
		index = stringHash2(k) % ht->tableSize;

	#else
	#error HASHING_FUNCTION not valid.

	#endif

		hashLink* current = ht->table[index];

		if(current == NULL)
			return;

		hashLink* next = ht->table[index]->next;

		if(strcmp(current->key, k) == 0){
			free(current->key);
			ht->table[index] = next;
			free(current);
			return;
		}

		while(next != NULL){

			if(strcmp(next->key, k) == 0) {

				free(next->key);
				current->next = next->next;
				free(next);
				return;
			}
			current = current->next;
			next = next->next;
		}
}

/*
 returns the number of hashLinks in the table
 */
int size (struct hashMap *ht)
{
	/*write this*/
	return ht->count;

}

/*
 returns the number of buckets in the table
 */
int capacity(struct hashMap *ht)
{
	/*write this*/
	return ht->tableSize;
}

/*
 returns the number of empty buckets in the table, these are buckets which have
 no hashlinks hanging off of them.
 */
int emptyBuckets(struct hashMap *ht)
{
	/*write this*/
	int emptyBucket = 0;
	for (int index = 0; index < ht->tableSize; index++){
		if(ht->table[index] == NULL){
			emptyBucket++;
		}
	}
	return emptyBucket;
}

/*
 returns the ratio of: (number of hashlinks) / (number of buckets)

 this value can range anywhere from zero (an empty table) to more then 1, which
 would mean that there are more hashlinks then buckets (but remember hashlinks
 are like linked list nodes so they can hang from each other)
 */
float tableLoad(struct hashMap *ht)
{
	/*write this*/
	return ht->count/ht->tableSize;
}
void printMap (struct hashMap * ht)
{
	int i;
	struct hashLink *temp;
	for(i = 0;i < capacity(ht); i++){
		temp = ht->table[i];
		if(temp != 0) {
			printf("\nBucket Index %d -> ", i);
		}
		while(temp != 0){
			printf("Key:%s|", temp->key);
			printValue(temp->value);
			printf(" -> ");
			temp=temp->next;
		}
	}
}


