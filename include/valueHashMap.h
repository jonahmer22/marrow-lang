#ifndef VALUEHASHMAP_H
#define VALUEHASHMAP_H

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

// initialization
ValueHashMap *valueHashMapInit();

// add an entry, entirely optional use of return value
Value *valueHashMapAdd(ValueHashMap *hashMap, const char *name, Value *value);

// fetch the value based off of it's name (key is a string) and return it
Value *valueHashMapGet(ValueHashMap *hashMap, const char *name);

// remove a value from the hash map and return it's value
Value *valueHashMapRemove(ValueHashMap* hashMap, const char *name);

#endif
