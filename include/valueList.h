#ifndef VALUELIST_H
#define VALUELIST_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// forward declarations
typedef struct Value Value;

// basic list for the marrow vm, only items and len
typedef struct ValueList{
	Value *items;
	size_t len;
	size_t capacity;
} ValueList;

typedef struct ValueListIter{
	ValueList *list;
	size_t index;
} ValueListIter;

// initialization
ValueList *valueListInit();
ValueList *valueListInitCapacity(size_t capacity);

// capacity management
bool valueListEnsureCapacity(ValueList *list, size_t minCapacity);

// basic operations
Value *valueListGet(ValueList *list, size_t index);
Value *valueListSet(ValueList *list, size_t index, Value *value);
Value *valueListPush(ValueList *list, Value *value);
Value *valueListPop(ValueList *list);
bool valueListInsert(ValueList *list, size_t index, Value *value);
Value *valueListRemove(ValueList *list, size_t index);
Value *valueListFront(ValueList *list);
Value *valueListBack(ValueList *list);

// maintenance
void valueListClear(ValueList *list);
size_t valueListCount(ValueList *list);
void valueListFree(ValueList *list);
bool valueListIsEmpty(ValueList *list);

// iteration
ValueListIter valueListIterInit(ValueList *list);
bool valueListIterNext(ValueListIter *iter, Value **value);

#endif
