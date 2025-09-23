#ifndef METHODTABLE_H
#define METHODTABLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// prototypes
typedef struct Function Function;
typedef enum TypeID TypeID;

typedef struct MethodTable{
	Function *functions;	// raw array of slots for functions
	// unused values are stored as NULL
	// gravestones will be functions of TYPE_NONE and no name, which should not exist in normal opperations

	// size and count indexes
	size_t size;
	size_t count;
} MethodTable;

typedef struct MethodTableIter{
	MethodTable *table;
	size_t index;
} MethodTableIter;

// initialization and cleanup
MethodTable *methodTableInit(size_t initialSize);
void methodTableFree(MethodTable *table);
void methodTableClear(MethodTable *table);

// capacity management
bool methodTableEnsureCapacity(MethodTable *table, size_t minSize);

// method handling
Function *methodTableInsert(MethodTable *table, Function *function);
Function *methodTableGet(MethodTable *table, const char *name);
Function *methodTableRemove(MethodTable *table, const char *name);
bool methodTableHas(MethodTable *table, const char *name);
Function *methodTableFind(MethodTable *table, const char *name, uint16_t arity);
Function *methodTableResolve(MethodTable *table, TypeID methodType, const char *name, uint16_t arity);

// metadata
size_t methodTableCount(MethodTable *table);
Function *methodTableAt(MethodTable *table, size_t index);
MethodTable *methodTableClone(MethodTable *table);

// iteration
MethodTableIter methodTableIterInit(MethodTable *table);
bool methodTableIterNext(MethodTableIter *iter, Function **function);

#endif
