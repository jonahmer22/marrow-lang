#include "value.h"
#include "valueHashMap.h"

#include <stddef.h>

ValueHashMap *valueHashMapInit(){
	return NULL;
}

ValueHashMap *valueHashMapInitCapacity(size_t capacity){
	(void)capacity;
	return NULL;
}

bool valueHashMapEnsureCapacity(ValueHashMap *hashMap, size_t minCapacity){
	(void)hashMap;
	(void)minCapacity;
	return false;
}

Value *valueHashMapAdd(ValueHashMap *hashMap, const char *name, Value *value){
	(void)hashMap;
	(void)name;
	(void)value;
	return NULL;
}

Value *valueHashMapPut(ValueHashMap *hashMap, const char *name, Value *value){
	(void)hashMap;
	(void)name;
	(void)value;
	return NULL;
}

Value *valueHashMapGet(ValueHashMap *hashMap, const char *name){
	(void)hashMap;
	(void)name;
	return NULL;
}

Value *valueHashMapGetOrDefault(ValueHashMap *hashMap, const char *name, Value *defaultValue){
	(void)hashMap;
	(void)name;
	return defaultValue;
}

bool valueHashMapHas(ValueHashMap *hashMap, const char *name){
	(void)hashMap;
	(void)name;
	return false;
}

size_t valueHashMapCount(ValueHashMap *hashMap){
	(void)hashMap;
	return 0;
}

bool valueHashMapIsEmpty(ValueHashMap *hashMap){
	(void)hashMap;
	return true;
}

Value *valueHashMapRemove(ValueHashMap *hashMap, const char *name){
	(void)hashMap;
	(void)name;
	return NULL;
}

void valueHashMapClear(ValueHashMap *hashMap){
	(void)hashMap;
}

ValueHashMap *valueHashMapClone(ValueHashMap *hashMap){
	(void)hashMap;
	return NULL;
}

void valueHashMapFree(ValueHashMap *hashMap){
	(void)hashMap;
}

void valueHashMapRehash(ValueHashMap *hashMap, size_t newCapacity){
	(void)hashMap;
	(void)newCapacity;
}

ValueHashMapIter valueHashMapIterInit(ValueHashMap *hashMap){
	ValueHashMapIter iter;
	iter.hashMap = hashMap;
	iter.index = 0;
	return iter;
}

bool valueHashMapIterNext(ValueHashMapIter *iter, const char **name, Value **value){
	(void)iter;
	(void)name;
	(void)value;
	return false;
}

const char *valueHashMapKeyAt(ValueHashMap *hashMap, size_t index){
	(void)hashMap;
	(void)index;
	return NULL;
}

Value *valueHashMapValueAt(ValueHashMap *hashMap, size_t index){
	(void)hashMap;
	(void)index;
	return NULL;
}

