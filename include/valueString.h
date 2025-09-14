#ifndef VALUESTRING_H
#define VALUESTRING_H

#include <stdint.h>
#include <stddef.h>

// Marrow String obbject. simple, only char * and length
typedef struct ValueString{
	const char *data;
	size_t len;
} ValueString;

#endif
