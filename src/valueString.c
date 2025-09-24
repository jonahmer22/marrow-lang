#include "valueString.h"

#include <stddef.h>

ValueString *valueStringInit(const char *data, size_t len){
	(void)data;
	(void)len;
	return NULL;
}

ValueString *valueStringFromLiteral(const char *literal){
	(void)literal;
	return NULL;
}

ValueString *valueStringClone(ValueString *string){
	(void)string;
	return NULL;
}

ValueString *valueStringConcat(ValueString *left, ValueString *right){
	(void)left;
	(void)right;
	return NULL;
}

ValueString *valueStringSlice(ValueString *string, size_t start, size_t end){
	(void)string;
	(void)start;
	(void)end;
	return NULL;
}

bool valueStringEquals(ValueString *left, ValueString *right){
	(void)left;
	(void)right;
	return false;
}

int valueStringCompare(ValueString *left, ValueString *right){
	(void)left;
	(void)right;
	return 0;
}

const char *valueStringCStr(ValueString *string){
	(void)string;
	return NULL;
}

void valueStringFree(ValueString *string){
	(void)string;
}

bool valueStringIsEmpty(ValueString *string){
	(void)string;
	return true;
}

size_t valueStringHash(ValueString *string){
	(void)string;
	return 0;
}

