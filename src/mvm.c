#include "mvm.h"
#include "frame.h"
#include "function.h"
#include "methodTable.h"
#include "objectPrototype.h"
#include "value.h"

MVM *mvmInit(){
	return NULL;
}

void mvmDestroy(MVM *vm){
	(void)vm;
}

void mvmReset(MVM *vm){
	(void)vm;
}

void mvmResetExecution(MVM *vm){
	(void)vm;
}

bool mvmRun(MVM *vm){
	(void)vm;
	return false;
}

bool mvmStep(MVM *vm){
	(void)vm;
	return false;
}

void mvmHalt(MVM *vm){
	(void)vm;
}

bool mvmIsHalted(MVM *vm){
	(void)vm;
	return true;
}

void mvmTrace(MVM *vm, bool enabled){
	(void)vm;
	(void)enabled;
}

bool mvmDispatch(MVM *vm){
	(void)vm;
	return false;
}

Frame *mvmPushFrame(MVM *vm, Function *function, Value *arguements, uint16_t arity){
	(void)vm;
	(void)function;
	(void)arguements;
	(void)arity;
	return NULL;
}

Frame *mvmPopFrame(MVM *vm){
	(void)vm;
	return NULL;
}

Frame *mvmPeekFrame(MVM *vm){
	(void)vm;
	return NULL;
}

Frame *mvmCurrentFrame(MVM *vm){
	(void)vm;
	return NULL;
}

bool mvmEnsureFrameCapacity(MVM *vm, size_t capacity){
	(void)vm;
	(void)capacity;
	return false;
}

void mvmResetFrameStack(MVM *vm){
	(void)vm;
}

bool mvmRegisterFunction(MVM *vm, Function *function){
	(void)vm;
	(void)function;
	return false;
}

Function *mvmGetFunction(MVM *vm, size_t index){
	(void)vm;
	(void)index;
	return NULL;
}

size_t mvmFunctionCount(MVM *vm){
	(void)vm;
	return 0;
}

bool mvmLoadFunction(MVM *vm, Function *function){
	(void)vm;
	(void)function;
	return false;
}

bool mvmRegisterPrototype(MVM *vm, ObjectPrototype *prototype){
	(void)vm;
	(void)prototype;
	return false;
}

ObjectPrototype *mvmFindPrototype(MVM *vm, const char *name){
	(void)vm;
	(void)name;
	return NULL;
}

size_t mvmPrototypeCount(MVM *vm){
	(void)vm;
	return 0;
}

bool mvmRegisterPrimMethod(MVM *vm, const char *name, Function *function){
	(void)vm;
	(void)name;
	(void)function;
	return false;
}

void mvmRaiseError(MVM *vm, const char *msg, Function *function, size_t ip){
	(void)vm;
	(void)msg;
	(void)function;
	(void)ip;
}

void mvmClearError(MVM *vm){
	(void)vm;
}

uint64_t mvmInstructionCount(MVM *vm){
	(void)vm;
	return 0;
}

bool mvmPushValue(MVM *vm, Value *value){
	(void)vm;
	(void)value;
	return false;
}

Value *mvmPopValue(MVM *vm){
	(void)vm;
	return NULL;
}

Value *mvmPeekValue(MVM *vm){
	(void)vm;
	return NULL;
}

void mvmRootValue(MVM *vm, Value *value){
	(void)vm;
	(void)value;
}

void mvmUnrootValue(MVM *vm, Value *value){
	(void)vm;
	(void)value;
}

