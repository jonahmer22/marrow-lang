#include <stddef.h>

#include "valueObject.h"
#include "objectPrototype.h"
#include "valueHashMap.h"
#include "methodTable.h"
#include "value.h"

ValueObject *valueObjectInit(ObjectPrototype *prototype){
	(void)prototype;
	return NULL;
}

void valueObjectFree(ValueObject *object){
	(void)object;
}

void valueObjectSetPrototype(ValueObject *object, ObjectPrototype *prototype){
	(void)object;
	(void)prototype;
}

ObjectPrototype *valueObjectGetPrototype(ValueObject *object){
	(void)object;
	return NULL;
}

Value *valueObjectGetField(ValueObject *object, const char *name){
	(void)object;
	(void)name;
	return NULL;
}

Value *valueObjectSetField(ValueObject *object, const char *name, Value *value){
	(void)object;
	(void)name;
	(void)value;
	return NULL;
}

Value *valueObjectRemoveField(ValueObject *object, const char *name){
	(void)object;
	(void)name;
	return NULL;
}

bool valueObjectHasField(ValueObject *object, const char *name){
	(void)object;
	(void)name;
	return false;
}

Function *valueObjectGetMethod(ValueObject *object, const char *name){
	(void)object;
	(void)name;
	return NULL;
}

bool valueObjectBindMethod(ValueObject *object, Function *function){
	(void)object;
	(void)function;
	return false;
}

bool valueObjectRemoveMethod(ValueObject *object, const char *name){
	(void)object;
	(void)name;
	return false;
}

Function *valueObjectResolveMethod(ValueObject *object, const char *name){
	(void)object;
	(void)name;
	return NULL;
}

bool valueObjectIterFields(ValueObject *object, size_t *index, const char **name, Value **value){
	(void)object;
	(void)index;
	(void)name;
	(void)value;
	return false;
}

bool valueObjectIterMethods(ValueObject *object, size_t *index, Function **function){
	(void)object;
	(void)index;
	(void)function;
	return false;
}

ValueHashMap *valueObjectFields(ValueObject *object){
	(void)object;
	return NULL;
}

MethodTable *valueObjectMethodTable(ValueObject *object){
	(void)object;
	return NULL;
}
