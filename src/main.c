#include "common.h"

int main(){
	// initialize the GC, no code before this point
	int stackTopHint;
	gcInit(&stackTopHint, false);
	// --------------------------
	

	// safely close the GC, no code beyond this point
	gcDestroy();
	return 0;
	// ------
}
