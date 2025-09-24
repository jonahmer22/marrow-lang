#include "objectPrototype.h"
#include "methodTable.h"
#include "valueHashMap.h"
#include "function.h"
#include "value.h"

ObjectPrototype *objectPrototypeInit(const char *name){
	(void)name;
	return NULL;
}

ObjectPrototype *objectPrototypeClone(ObjectPrototype *prototype){
	(void)prototype;
	return NULL;
}

void objectPrototypeFree(ObjectPrototype *prototype){
	(void)prototype;
}

void objectPrototypeSetCreatorFunction(ObjectPrototype *prototype, size_t index){
	(void)prototype;
	(void)index;
}

size_t objectPrototypeGetCreatorFunction(ObjectPrototype *prototype){
	(void)prototype;
	return 0;
}

bool objectPrototypeBindMethod(ObjectPrototype *prototype, Function *function){
	(void)prototype;
	(void)function;
	return false;
}

Function *objectPrototypeGetMethod(ObjectPrototype *prototype, const char *name){
	(void)prototype;
	(void)name;
	return NULL;
}

bool objectPrototypeHasMethod(ObjectPrototype *prototype, const char *name){
	(void)prototype;
	(void)name;
	return false;
}

Function *objectPrototypeResolveMethod(ObjectPrototype *prototype, const char *name, uint16_t arity){
	(void)prototype;
	(void)name;
	(void)arity;
	return NULL;
}

bool objectPrototypeRemoveMethod(ObjectPrototype *prototype, const char *name){
	(void)prototype;
	(void)name;
	return false;
}

bool objectPrototypeInherit(ObjectPrototype *prototype, ObjectPrototype *parent){
	(void)prototype;
	(void)parent;
	return false;
}

ObjectPrototype *objectPrototypeCloneShallow(ObjectPrototype *prototype){
	(void)prototype;
	return NULL;
}

Value *objectPrototypeGetValue(ObjectPrototype *prototype, const char *name){
	(void)prototype;
	(void)name;
	return NULL;
}

Value *objectPrototypeSetValue(ObjectPrototype *prototype, const char *name, Value *value){
	(void)prototype;
	(void)name;
	(void)value;
	return NULL;
}

Value *objectPrototypeRemoveValue(ObjectPrototype *prototype, const char *name){
	(void)prototype;
	(void)name;
	return NULL;
}

bool objectPrototypeHasValue(ObjectPrototype *prototype, const char *name){
	(void)prototype;
	(void)name;
	return false;
}

ValueHashMap *objectPrototypeValues(ObjectPrototype *prototype){
	(void)prototype;
	return NULL;
}

ObjectPrototypeValueIter objectPrototypeValueIterInit(ObjectPrototype *prototype){
	ObjectPrototypeValueIter iter;
	iter.prototype = prototype;
	iter.index = 0;
	return iter;
}

bool objectPrototypeValueIterNext(ObjectPrototypeValueIter *iter, const char **name, Value **value){
	(void)iter;
	(void)name;
	(void)value;
	return false;
}

ObjectPrototypeMethodIter objectPrototypeMethodIterInit(ObjectPrototype *prototype){
	ObjectPrototypeMethodIter iter;
	iter.prototype = prototype;
	iter.index = 0;
	return iter;
}

bool objectPrototypeMethodIterNext(ObjectPrototypeMethodIter *iter, Function **function){
	(void)iter;
	(void)function;
	return false;
}

