#ifndef MSTRING_H
#define MSTRING_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct String{
	char *buff;
	size_t len;

	size_t cap;
} String;

String *stringInit(void);
void stringPushChar(String *s, char c);
bool stringCharCmp(String *s, char *c);
bool stringCmp(String *a, String *b);

#endif
