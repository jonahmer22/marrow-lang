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

#endif
