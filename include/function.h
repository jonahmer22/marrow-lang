#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"

#include "functionHash.h"
#include "bytecodeBlock.h"
#include "valueHash.h"

typedef struct Function{
	FunctionHash functions;
	BytecodeBlock bytecode;
	ValueHash values;
} Function;

Function *functionInit(const char* name);

#endif
