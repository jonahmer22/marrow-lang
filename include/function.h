#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"

typedef struct Function{
	FunctionHash functions;
	BytecodeBlock bytecode;
	ValueHash values;
} Function;

#endif
