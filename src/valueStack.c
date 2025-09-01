#include "common.h"

#define SMALLEST_STACK 8

// =======
// private
// =======

static void growStack(ValueStack *stack){
	// initialize new values
	tempCap = stack.capacity * 2;
	tempStack = gcAlloc(sizeof(Value*) * tempCap);

	// copy over old values
	for(int i = 0; i < stack->capacity; i++){
		tempStack[i] = stack->valueStack[i];
	}

	// replace old values
	stack->valueStack = tempStack;
	stack->capacity = tempCap;
}

// ======
// public
// ======

void valueStackInit(ValueStack *stack){
	stack->valueStack = gcAlloc(sizeof(Value*) * SMALLEST_STACK);
	
	stack->stackTop = 0;
	stack->capacity = SMALLEST_STACK;
}

void valueStackPush(ValueStack *stack, Value *value){
	// check capacity
	if(stack->stackTop == stack->capacity){
		growStack(stack);
	}

	// insert and increment
	stack->valueStack[stack->stackTop] = value;
	stack->stackTop++;
}

Value *valueStackPop(ValueStack *stack){
	Value *result = NULL;	// initialize return value

	stack->stackTop--;	// move the head back
	result = stack->valueStack[stack->stackTop];	// store the value we pop

	return result;
}
