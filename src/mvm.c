#include "common.h"

// =======
// private
// =======

static MVM mvm;
static Function *globalFunction;

// ======
// public
// ======

void mvmInit(){
	// initialize the valueStack as empty
	valueStackInit(&mvm.valueStack);
	
	// initialize and set the global function where execution starts as `__globalFunction`
	globalFunction = functionInit("__globalFunction");
	mvm.currentFunction = globalFunction;
}
