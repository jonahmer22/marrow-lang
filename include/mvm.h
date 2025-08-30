#ifndef MVM_H
#define MVM_H

#include <stdint.h>

#include "bytecodeBlock.h"

typedef struct MVM{
	BytecodeBlock *currentBlock;	
} MVM;

void mvmInit();

int mvmInsertByte(uint8_t byte);

#endif
