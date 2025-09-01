#ifndef MVM_H
#define MVM_H

#include <stdint.h>

#include "common.h"

typedef struct MVM{
	// Value Stack is persistent accross vm state
	// The stack includes function calls, when a function finishes it places it's return in it's place on the stack
	ValueStack valueStack;

	// Hold function specific data
	Function *currentFunction;
} MVM;

void mvmInit();

#endif
