#ifndef OBJECTPROTOTYPE_H
#define OBJECTPROTOTYPE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// forward declarations
typedef struct Value Value;
typedef struct Function Function;
typedef struct MethodTable MethodTable;
typedef struct ValueHashMap ValueHashMap;

// prototype storage for objects, so that they can be created from a stored basic state in the vm
typedef struct ObjectPrototype{
	const char *name;
	MethodTable *methods;
	ValueHashMap *values;
	size_t creatorFunctionIndex;
} ObjectPrototype;

typedef struct ObjectPrototypeValueIter{
	ObjectPrototype *prototype;
	size_t index;
} ObjectPrototypeValueIter;

typedef struct ObjectPrototypeMethodIter{
	ObjectPrototype *prototype;
	size_t index;
} ObjectPrototypeMethodIter;

// initialization and cleanup
ObjectPrototype *objectPrototypeInit(const char *name);
ObjectPrototype *objectPrototypeClone(ObjectPrototype *prototype);
void objectPrototypeFree(ObjectPrototype *prototype);

// creator function management
void objectPrototypeSetCreatorFunction(ObjectPrototype *prototype, size_t index);
size_t objectPrototypeGetCreatorFunction(ObjectPrototype *prototype);

// method management
bool objectPrototypeBindMethod(ObjectPrototype *prototype, Function *function);
Function *objectPrototypeGetMethod(ObjectPrototype *prototype, const char *name);
bool objectPrototypeHasMethod(ObjectPrototype *prototype, const char *name);
Function *objectPrototypeResolveMethod(ObjectPrototype *prototype, const char *name, uint16_t arity);
bool objectPrototypeRemoveMethod(ObjectPrototype *prototype, const char *name);

// hierarchy
bool objectPrototypeInherit(ObjectPrototype *prototype, ObjectPrototype *parent);
ObjectPrototype *objectPrototypeCloneShallow(ObjectPrototype *prototype);

// value management
Value *objectPrototypeGetValue(ObjectPrototype *prototype, const char *name);
Value *objectPrototypeSetValue(ObjectPrototype *prototype, const char *name, Value *value);
Value *objectPrototypeRemoveValue(ObjectPrototype *prototype, const char *name);
bool objectPrototypeHasValue(ObjectPrototype *prototype, const char *name);
ValueHashMap *objectPrototypeValues(ObjectPrototype *prototype);

// iteration
ObjectPrototypeValueIter objectPrototypeValueIterInit(ObjectPrototype *prototype);
bool objectPrototypeValueIterNext(ObjectPrototypeValueIter *iter, const char **name, Value **value);
ObjectPrototypeMethodIter objectPrototypeMethodIterInit(ObjectPrototype *prototype);
bool objectPrototypeMethodIterNext(ObjectPrototypeMethodIter *iter, Function **function);

#endif
