#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"
#include "Stack.h"

typedef struct Function{
	// function specific call stack
	Stack funCallStack;	// 512 slots

	// function specific stack
	Stack funStack;		// 1024 slots

	// bytecode for the function
	Bytecode funBytecode;	// 4096 slots
} Function;

#endif
