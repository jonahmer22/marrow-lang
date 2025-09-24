#include "function.h"

Function *functionInit(const char *name, uint16_t arity){
	(void)name;
	(void)arity;
	return NULL;
}

Function *functionInitMethod(TypeID methodType, const char *name, uint16_t arity){
	(void)methodType;
	(void)name;
	(void)arity;
	return NULL;
}

Function *functionClone(Function *source){
	(void)source;
	return NULL;
}

void functionFree(Function *function){
	(void)function;
}

void functionSetBytecode(Function *function, const uint8_t *code, size_t codeLen){
	(void)function;
	(void)code;
	(void)codeLen;
}

void functionResetCode(Function *function){
	(void)function;
}

void functionSetConstants(Function *function, const Value *constPool, size_t constCount){
	(void)function;
	(void)constPool;
	(void)constCount;
}

const Value *functionGetConst(Function *function, size_t index){
	(void)function;
	(void)index;
	return NULL;
}

size_t functionGetConstCount(Function *function){
	(void)function;
	return 0;
}

void functionSetMethodType(Function *function, TypeID methodType){
	(void)function;
	(void)methodType;
}

TypeID functionGetMethodType(Function *function){
	(void)function;
	return TYPE_NONE;
}

uint16_t functionGetArity(Function *function){
	(void)function;
	return 0;
}

const char *functionGetName(Function *function){
	(void)function;
	return NULL;
}

