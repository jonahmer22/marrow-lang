#include "value.h"
#include "valueList.h"

#include <stddef.h>

ValueList *valueListInit(){
	return NULL;
}

ValueList *valueListInitCapacity(size_t capacity){
	(void)capacity;
	return NULL;
}

bool valueListEnsureCapacity(ValueList *list, size_t minCapacity){
	(void)list;
	(void)minCapacity;
	return false;
}

Value *valueListGet(ValueList *list, size_t index){
	(void)list;
	(void)index;
	return NULL;
}

Value *valueListSet(ValueList *list, size_t index, Value *value){
	(void)list;
	(void)index;
	(void)value;
	return NULL;
}

Value *valueListPush(ValueList *list, Value *value){
	(void)list;
	(void)value;
	return NULL;
}

Value *valueListPop(ValueList *list){
	(void)list;
	return NULL;
}

bool valueListInsert(ValueList *list, size_t index, Value *value){
	(void)list;
	(void)index;
	(void)value;
	return false;
}

Value *valueListRemove(ValueList *list, size_t index){
	(void)list;
	(void)index;
	return NULL;
}

Value *valueListFront(ValueList *list){
	(void)list;
	return NULL;
}

Value *valueListBack(ValueList *list){
	(void)list;
	return NULL;
}

void valueListClear(ValueList *list){
	(void)list;
}

size_t valueListCount(ValueList *list){
	(void)list;
	return 0;
}

void valueListFree(ValueList *list){
	(void)list;
}

bool valueListIsEmpty(ValueList *list){
	(void)list;
	return true;
}

ValueListIter valueListIterInit(ValueList *list){
	ValueListIter iter;
	iter.list = list;
	iter.index = 0;
	return iter;
}

bool valueListIterNext(ValueListIter *iter, Value **value){
	(void)iter;
	(void)value;
	return false;
}

