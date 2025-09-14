#ifndef VALUELIST_H
#define VALUELIST_H

#include <stdint.h>
#include <stddef.h>

// forward declarations
typedef struct Value Value;

// basic list for the marrow vm, only items and len
typedef struct ValueList{
	Value *items;
	size_t len;
} ValueList;

#endif
