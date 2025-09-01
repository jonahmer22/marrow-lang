#ifndef VALUESTACK_H
#define VALUESTACK_H

#include "common.h"
#include "value.h"

typedef struct ValueStack{
	Value **valueStack;
	
	int stackTop;
	int capacity;
} ValueStack;

void valueStackInit(ValueStack *stack);

void valueStackPush(ValueStack *stack, Value *value);

Value *valueStackPop(ValueStack* stack);

#endif
