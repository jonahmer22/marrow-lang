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

#endif
