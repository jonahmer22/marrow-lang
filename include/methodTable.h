#ifndef METHODTABLE_H
#define METHODTABLE_H

// prototypes
typedef struct Function Function;

typedef struct MethodTable{
	Function *functions;	// raw array of slots for functions
	// unused values are stored as NULL
	// gravestones will be functions of TYPE_NONE and no name, which should not exist in normal opperations
	
	// size and count indexes
	size_t size;
	size_t count;
	

} MethodTable;

#endif
