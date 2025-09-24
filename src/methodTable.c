#include "methodTable.h"
#include "function.h"

MethodTable *methodTableInit(size_t initialSize){
	(void)initialSize;
	return NULL;
}

void methodTableFree(MethodTable *table){
	(void)table;
}

void methodTableClear(MethodTable *table){
	(void)table;
}

bool methodTableEnsureCapacity(MethodTable *table, size_t minSize){
	(void)table;
	(void)minSize;
	return false;
}

Function *methodTableInsert(MethodTable *table, Function *function){
	(void)table;
	(void)function;
	return NULL;
}

Function *methodTableGet(MethodTable *table, const char *name){
	(void)table;
	(void)name;
	return NULL;
}

Function *methodTableRemove(MethodTable *table, const char *name){
	(void)table;
	(void)name;
	return NULL;
}

bool methodTableHas(MethodTable *table, const char *name){
	(void)table;
	(void)name;
	return false;
}

Function *methodTableFind(MethodTable *table, const char *name, uint16_t arity){
	(void)table;
	(void)name;
	(void)arity;
	return NULL;
}

Function *methodTableResolve(MethodTable *table, TypeID methodType, const char *name, uint16_t arity){
	(void)table;
	(void)methodType;
	(void)name;
	(void)arity;
	return NULL;
}

size_t methodTableCount(MethodTable *table){
	(void)table;
	return 0;
}

Function *methodTableAt(MethodTable *table, size_t index){
	(void)table;
	(void)index;
	return NULL;
}

MethodTable *methodTableClone(MethodTable *table){
	(void)table;
	return NULL;
}

MethodTableIter methodTableIterInit(MethodTable *table){
	MethodTableIter iter;
	iter.table = table;
	iter.index = 0;
	return iter;
}

bool methodTableIterNext(MethodTableIter *iter, Function **function){
	(void)iter;
	(void)function;
	return false;
}

