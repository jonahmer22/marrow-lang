#include "common.h"

#include <stdio.h>
#include <stdlib.h>

void error(int errorNumber, const char *message, bool quit){
	char result[1024];

	snprintf(result, sizeof(result), "[%s=%d]: %s.", quit ? "FATAL" : "ERROR", errorNumber, message);
	
	fprintf(stderr, "%s\n", result);

	if(quit)
		exit(errorNumber);
}
