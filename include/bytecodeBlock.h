#ifndef BYTECODEBLOCK_H
#define BYTECODEBLOCK_H

#include "common.h"

enum Opcode{
	OP_PRINT,	// prints the value on top of the value stack
	OP_FETCH_VAL,	// fetches a value from mvm hash table based off of name
	OP_POP,		// pops the top value off of the value stack
	OP_RETURN	// exits from the function and replaces it's spot on parent's stack with value on top of it's own stack
};

typedef struct BytecodeBlock{
	uint8_t bytecode[1024*1024];
	uint32_t bytecodeIdx;
} BytecodeBlock;

void insertByte(BytecodeBlock *codeBlock);

#endif
