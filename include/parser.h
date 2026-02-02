#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdlib.h>

typedef struct Parser{
	const char *buff;
	const char *curr;

	size_t line;
} Parser;

Parser *parserInit(const char *buff);
int unadvance(Parser *p);
int advance(Parser *p);
int peek(Parser *p);
int next(Parser *p);
int prev(Parser *p);

#endif
