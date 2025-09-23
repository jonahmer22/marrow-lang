#ifndef VALUEOBJECT_H
#define VALUEOBJECT_H

#include <stdbool.h>

// forward delcarations
typedef struct Value Value;
typedef struct Function Function;
typedef struct ValueHashMap ValueHashMap;
typedef struct MethodTable MethodTable;
typedef struct ObjectPrototype ObjectPrototype;

// object type for marrow vm. this is used for user defined values (classes)
typedef struct ValueObject{
	ValueHashMap *fields;
	MethodTable *methods;
	ObjectPrototype *prototype;
} ValueObject;

// initialization and cleanup
ValueObject *valueObjectInit(ObjectPrototype *prototype);
void valueObjectFree(ValueObject *object);

// prototype management
void valueObjectSetPrototype(ValueObject *object, ObjectPrototype *prototype);
ObjectPrototype *valueObjectGetPrototype(ValueObject *object);

// field management
Value *valueObjectGetField(ValueObject *object, const char *name);
Value *valueObjectSetField(ValueObject *object, const char *name, Value *value);
Value *valueObjectRemoveField(ValueObject *object, const char *name);
bool valueObjectHasField(ValueObject *object, const char *name);

// method management
Function *valueObjectGetMethod(ValueObject *object, const char *name);
bool valueObjectBindMethod(ValueObject *object, Function *function);
bool valueObjectRemoveMethod(ValueObject *object, const char *name);
Function *valueObjectResolveMethod(ValueObject *object, const char *name);

// iteration helpers
bool valueObjectIterFields(ValueObject *object, size_t *index, const char **name, Value **value);
bool valueObjectIterMethods(ValueObject *object, size_t *index, Function **function);

// raw accessors
ValueHashMap *valueObjectFields(ValueObject *object);
MethodTable *valueObjectMethodTable(ValueObject *object);

#endif
