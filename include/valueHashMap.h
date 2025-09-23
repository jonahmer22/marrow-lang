#ifndef VALUEHASHMAP_H
#define VALUEHASHMAP_H

#include <stdbool.h>
#include <stddef.h>

// prototypes
typedef struct Value Value;

// value hash map struct for O(1) access of values in the vm
typedef struct ValueHashMap{
	Value *values;	// raw array of slots for values
	// unused values should be stored as a NULL pointer
	// gravestones will be a value, but a TYPE_NONE which shouldn't exist in normal operations

	// size and cap (used vs total)
	size_t size;
	size_t capacity;
} ValueHashMap;

typedef struct ValueHashMapIter{
	ValueHashMap *hashMap;
	size_t index;
} ValueHashMapIter;

// initialization
ValueHashMap *valueHashMapInit();
ValueHashMap *valueHashMapInitCapacity(size_t capacity);

// capacity management
bool valueHashMapEnsureCapacity(ValueHashMap *hashMap, size_t minCapacity);

// add an entry, entirely optional use of return value
Value *valueHashMapAdd(ValueHashMap *hashMap, const char *name, Value *value);
Value *valueHashMapPut(ValueHashMap *hashMap, const char *name, Value *value);

// fetch the value based off of it's name (key is a string) and return it
Value *valueHashMapGet(ValueHashMap *hashMap, const char *name);
Value *valueHashMapGetOrDefault(ValueHashMap *hashMap, const char *name, Value *defaultValue);

// check and count entries
bool valueHashMapHas(ValueHashMap *hashMap, const char *name);
size_t valueHashMapCount(ValueHashMap *hashMap);
bool valueHashMapIsEmpty(ValueHashMap *hashMap);

// remove a value from the hash map and return it's value
Value *valueHashMapRemove(ValueHashMap *hashMap, const char *name);

// maintenance utilities
void valueHashMapClear(ValueHashMap *hashMap);
ValueHashMap *valueHashMapClone(ValueHashMap *hashMap);
void valueHashMapFree(ValueHashMap *hashMap);
void valueHashMapRehash(ValueHashMap *hashMap, size_t newCapacity);

// iteration
ValueHashMapIter valueHashMapIterInit(ValueHashMap *hashMap);
bool valueHashMapIterNext(ValueHashMapIter *iter, const char **name, Value **value);

// direct indexed access (useful when iterating manually)
const char *valueHashMapKeyAt(ValueHashMap *hashMap, size_t index);
Value *valueHashMapValueAt(ValueHashMap *hashMap, size_t index);

#endif
