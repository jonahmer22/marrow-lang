#include "common.h"
#include "frame.h"
#include "valueHashMap.h"

Frame *frameInit(Function *function, Value *arguements, uint16_t arity, Value *reciever){
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
