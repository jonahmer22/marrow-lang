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
	Value **stack;
	size_t stackTop;
	size_t stackSize;

	// locals are added to at runtime
	ValueHashMap *locals;

	// frame that when returning the value will be pushed onto the stack of
	struct Frame *reciever;
} Frame;

// create a frame with args for function, arguements, and reciever (any empty args should be NULL)
// initializes an empty stack and locals hashmap
Frame *frameInit(Function *function, Value *arguements, uint16_t arity, Frame *reciever);

// manage locals
Value *localAdd(Frame *frame, const char *name, Value *value);	// return is optional to use
Value *localGet(Frame *frame, const char *name);
Value *localRemove(Frame *frame, const char *name);

// manage stack
Value *stackPush(Frame *frame, Value *value);	// return is optional to use
Value *stackPeek(Frame *frame);
Value *stackPop(Frame *frame);

// IP modification returns of all these functions are entirely optional to use
inline size_t jmpRelIP(Frame *frame, int64_t jmp);	// jmp relative to current position
inline size_t jmpIP(Frame *frame, size_t idx);		// jmp to idx in the bytecode
inline size_t incrementIP(Frame *frame);		// advance the IP by 1

// return
// adds the value on top of the stack to the top of the stack of the reciever
Value *frameReturn(Frame *frame);

#endif
