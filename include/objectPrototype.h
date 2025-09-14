#ifndef OBJECTPROTOTYPE_H
#define OBJECTPROTOTYPE_H

// forward declarations
typedef struct MethodTable MethodTable;
typedef struct ValueHashMap ValueHashMap;

// prototype storage for objects, so that they can be created from a stored basic state in the vm
typedef struct ObjectPrototype{
	const char *name;
	MethodTable *methods;
	ValueHashMap *values;
	size_t creatorFunctionIndex;
} ObjectPrototype;

#endif
