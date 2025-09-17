#include "common.h"
#include "frame.h"
#include "valueHashMap.h"
#include "function.h"
#include "value.h"

#include <stdbool.h>

Frame *frameInit(Function *function, Value *arguements, uint16_t arity, Frame *reciever){
	Frame *result = gcAlloc(sizeof(Frame));

	// intiailize function and ip
	result->function = function;
	result->ip = 0;

	// initialize arguements and arity
	result->arguements = arguements;
	result->arity = arity;

	// initialize stack
	result->stack = gcAlloc(sizeof(Value) * 16);
	result->stackTop = 0;
	result->stackSize = 16;

	// initialize locals map
	result->locals = valueHashMapInit();

	// initialize the reciever
	result->reciever = reciever;

	// return the finished result
	return result;
}

Value *localAdd(Frame *frame, const char *name, Value *value){
	return (valueHashMapAdd(frame->locals, name, value));
}

Value *localGet(Frame *frame, const char *name){
	return (valueHashMapGet(frame->locals, name));
}

Value *localRemove(Frame *frame, const char *name){
	return (valueHashMapRemove(frame->locals, name));
}

Value *stackPush(Frame *frame, Value *value){
	// if the stack is at capacity grow it
	// no need to shrink because the stack only lives as long as the function
	if(frame->stackTop == frame->stackSize){
		// compute newSize
		size_t newSize = frame->stackSize * 2;

		// allocate new stack data arrat
		Value **temp = gcAlloc(sizeof(Value*) * newSize);
		
		// copy over all the data
		for(size_t i = 0; i < frame->stackSize; i++){
			temp[i] = frame->stack[i];
		}

		// assign new values
		frame->stack = temp;
		frame->stackSize = newSize;
		temp = NULL;
	}

	// add new value
	frame->stack[frame->stackTop] = value;
	frame->stackTop++;

	// return the value we added, entirely redundant but why not
	return (frame->stack[frame->stackTop - 1]);
}

Value *stackPeek(Frame *frame){
	return (frame->stack[frame->stackTop - 1]);
}

Value *stackPop(Frame *frame){
	// check the stack isn't empty and return NULL if it is
	if(frame->stackTop == 0){
		return NULL;
	}

	// move back the head
	frame->stackTop--;

	// return what was the top of the stack
	return (frame->stack[frame->stackTop]);
}

inline size_t jmpRelIP(Frame *frame, int64_t jmp){
	if(frame->ip - jmp >= 0 && frame->function->codeLen > frame->ip + jmp){
		frame->ip += jmp;
		
		return frame->ip;
	}
	
	// shouldn't get here
	error(10, "Attempting to relative jump outsize of bytecode block", true);
	return 0;
}

inline size_t jmpIP(Frame *frame, size_t idx){
	if(idx >= 0 && idx < frame->function->codeLen){
		frame->ip = idx;

		return frame->ip;
	}

	// shouldn't get here
	error(11, "Attempting to jump outside of bytecode block", true);
	return 0;
}

inline size_t incrementIP(Frame *frame){
	if(frame->ip + 1 < frame->function->codeLen){
		return ++(frame->ip);
	}

	// need I say it again????
	error(12, "Attempting to increment ip past bytecode bock", true);
	return 0;
}

Value *frameReturn(Frame *frame){
	// get the value off of the top of the stack (presumedly the last item)
	Value *result = stackPop(frame);
	
	// if there is a recieving frame
	if(frame->reciever){
		stackPush(frame->reciever, result);
	}

	// return the result of the frame, entirely optional to use
	return (stackPeek(frame->reciever));
}
