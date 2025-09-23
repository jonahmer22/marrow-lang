#ifndef MVM_H
#define MVM_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Forward declarations
typedef struct Value Value;
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

// initialization and teardown
MVM *mvmInit();
void mvmDestroy(MVM *vm);
void mvmReset(MVM *vm);
void mvmResetExecution(MVM *vm);

// execution control
bool mvmRun(MVM *vm);
bool mvmStep(MVM *vm);
void mvmHalt(MVM *vm);
bool mvmIsHalted(MVM *vm);
void mvmTrace(MVM *vm, bool enabled);
bool mvmDispatch(MVM *vm);

// frame control
Frame *mvmPushFrame(MVM *vm, Function *function, Value *arguements, uint16_t arity);
Frame *mvmPopFrame(MVM *vm);
Frame *mvmPeekFrame(MVM *vm);
Frame *mvmCurrentFrame(MVM *vm);
bool mvmEnsureFrameCapacity(MVM *vm, size_t capacity);
void mvmResetFrameStack(MVM *vm);

// function management
bool mvmRegisterFunction(MVM *vm, Function *function);
Function *mvmGetFunction(MVM *vm, size_t index);
size_t mvmFunctionCount(MVM *vm);
bool mvmLoadFunction(MVM *vm, Function *function);

// prototype management
bool mvmRegisterPrototype(MVM *vm, ObjectPrototype *prototype);
ObjectPrototype *mvmFindPrototype(MVM *vm, const char *name);
size_t mvmPrototypeCount(MVM *vm);
bool mvmRegisterPrimMethod(MVM *vm, const char *name, Function *function);

// error handling
void mvmRaiseError(MVM *vm, const char *msg, Function *function, size_t ip);
void mvmClearError(MVM *vm);

// instrumentation
uint64_t mvmInstructionCount(MVM *vm);

// value stack helpers (for top frame)
bool mvmPushValue(MVM *vm, Value *value);
Value *mvmPopValue(MVM *vm);
Value *mvmPeekValue(MVM *vm);

// GC integration helpers
void mvmRootValue(MVM *vm, Value *value);
void mvmUnrootValue(MVM *vm, Value *value);

#endif
