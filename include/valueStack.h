#ifndef VALUESTACK_H
#define VALUESTACK_H

typedef struct ValueStack{
	Value *valueStack;
	
	int stackTop;
	int capacity;
} ValueStack;

void valueStackInit(ValueStack *stack);

void valueStackPush(Value *value);

Value *valueStackPop();

#endif
