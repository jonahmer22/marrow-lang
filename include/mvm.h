#ifndef MVM_H
#define MVM_H

#include "common.h"
#include "function.h"

typedef struct MVM{
	// call stack
	Stack callStack;	// 2048 slots
	
	// main stack
	Stack stack;		// 4096 slots
	
	// main bytecode
	Bytecode bytecode;	// 16284 slots
	
	// temp vm emulate lesser vm for not global (main) function
	Stack *funCallStack;
	Stack *funStack;
	Bytecode *funBytecode;
} MVM;

#endif
