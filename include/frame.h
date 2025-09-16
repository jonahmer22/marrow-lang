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

	// locals are added to at runtime
	ValueHashMap *locals;

	// Value being enacted on (optional and only when using the var.function() syntax)
	Value *receiver;
} Frame;

// create a frame with args for function, arguements, and reciever (any empty args should be NULL)
// initializes an empty stack and locals hashmap
Frame *frameInit(Function *function, Value *arguements, uint16_t arity, Value *reciever);
// destroys a frame
void frameDestroy(Frame *frame);

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

#endif
