#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdint.h>
#include <stddef.h>

typedef struct Value Value;
typedef enum TypeID TypeID;

typedef struct Function{
	// identification
	TypeID methodType;	// only used when a method, else is TYPE_NONE
	const char name[256];
	uint16_t arity;
	
	// Bytecode
	const uint8_t *code;
	size_t codeLen;

	// constants
	const Value *constPool;
	size_t constCount;
} Function;

// initialization and cleanup
Function *functionInit(const char *name, uint16_t arity);
Function *functionInitMethod(TypeID methodType, const char *name, uint16_t arity);
Function *functionClone(Function *source);
void functionFree(Function *function);

// bytecode management
void functionSetBytecode(Function *function, const uint8_t *code, size_t codeLen);
void functionResetCode(Function *function);

// constant pool management
void functionSetConstants(Function *function, const Value *constPool, size_t constCount);
const Value *functionGetConst(Function *function, size_t index);
size_t functionGetConstCount(Function *function);

// metadata helpers
void functionSetMethodType(Function *function, TypeID methodType);
TypeID functionGetMethodType(Function *function);
uint16_t functionGetArity(Function *function);
const char *functionGetName(Function *function);

#endif
