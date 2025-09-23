#ifndef VALUESTRING_H
#define VALUESTRING_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Marrow String obbject. simple, only char * and length
typedef struct ValueString{
	const char *data;
	size_t len;
} ValueString;

// creation
ValueString *valueStringInit(const char *data, size_t len);
ValueString *valueStringFromLiteral(const char *literal);

// transformations
ValueString *valueStringClone(ValueString *string);
ValueString *valueStringConcat(ValueString *left, ValueString *right);
ValueString *valueStringSlice(ValueString *string, size_t start, size_t end);

// comparisons
bool valueStringEquals(ValueString *left, ValueString *right);
int valueStringCompare(ValueString *left, ValueString *right);

// helpers
const char *valueStringCStr(ValueString *string);
void valueStringFree(ValueString *string);
bool valueStringIsEmpty(ValueString *string);
size_t valueStringHash(ValueString *string);

#endif
