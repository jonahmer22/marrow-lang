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

uint64_t hashString(const char *str, size_t len){
	const uint64_t fnvOffset = 1469598103934665603ULL;
	const uint64_t fnvPrime = 1099511628211ULL;
	uint64_t hash = fnvOffset;

	if(!str){
		return 0;
	}

	if(len == 0){
		for(size_t i = 0; str[i] != '\0'; i++){
			hash ^= (uint8_t)str[i];
			hash *= fnvPrime;
		}
		return hash;
	}

	for(size_t i = 0; i < len && str[i] != '\0'; i++){
		hash ^= (uint8_t)str[i];
		hash *= fnvPrime;
	}

	return hash;
}
