#ifndef MVM_H
#define MVM_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Forward declarations
typedef struct Frame Frame;
typedef struct Function Function;
typedef struct MethodTable MethodTable;
typedef struct ObjectPrototype ObjectPrototype;

// Marrow VM struct
typedef struct MVM{
	// Stack of frames (callstack).
	Frame *frames;	// Bytecode and values, should include a pointer to object which can be NULL, but could store objects fields and values.
	size_t frameCount;	// Number of frames currently on the stack
	size_t frameCapacity;	// Max number of frames currently allowed

	// storage of all functions 
	Function **functions;	// List of function pointers (only for references, lookup should be done durring compile and addresses put into bytecode so that traversal of this list should not be necessary)
	size_t functionCount;	// total count of functions
	size_t functionCapacity;	// current capacity of functions
	
	// primative methods table
	MethodTable *primMethods;

	// object prototypes
	ObjectPrototype *prototypes;
	size_t prototypeCount;
	size_t prototypeCapacity;
	
	// run state
	bool halted;	// true if the vm is currently halted
	bool traceExecution;	// determines whether stack trace, ip, and bytecode will be printed
	uint64_t instrCount;	// number of instructions
	struct {
		const char *msg;
		Function *function;
		size_t ip;
		size_t frameIndex;
	} lastError;
} MVM;

#endif
