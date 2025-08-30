#ifndef BYTECODEBLOCK_H
#define BYTECODEBLOCK_H

typedef struct BytecodeBlock{
	uint8_t bytecode[1024*1024];
	uint32_t bytecodeIdx;
} BytecodeBlock;

#endif
