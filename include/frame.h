#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>
#include <stddef.h>

#include "value.h"

typedef struct ValueHashMap ValueHashMap;
typedef struct Function Function;

typedef struct Frame{
	// function which includes the code for the 
	Function *function;
	size_t ip;
	
	// arguements
	Value *arguements;
	uint16_t arity;

	// a basic value stack
	Value *stack;
	size_t stackTop;
	size_t stackSize;

	// locals
	ValueHashMap *locals;

	// Value being enacted on (optional and only when using the var.function() syntax)
	Value *receiver;
} Frame;

#endif
