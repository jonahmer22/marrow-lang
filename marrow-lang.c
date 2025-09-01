#include "common.h"

#include <stdio.h>

int main(){
	// initialize the GC, no code before this point
	int stackTopHint;
	gcInit(&stackTopHint, false);
	// --------------------------
	
	printf("this compiles\n");

	// safely close the GC, no code beyond this point
	gcDestroy();
	return 0;
	// ------
}
