// ============
// Basic Parser
// ============

#include "parser.h"
#include "arena.h"

Parser *parserInit(const char *buff){
	Parser *temp = arenaAlloc(sizeof(Parser));
	temp->curr = temp->buff = buff;
	temp->line = 0;

	return temp;
}

int advance(Parser *p){
	return *(p->curr++);
}

int peek(Parser *p){
	return *(p->curr);
}

int next(Parser *p){
	return *(p->curr + 1);
}

int unadvance(Parser *p){
	return *(p->curr--);
}

int prev(Parser *p){
	return *(p->curr - 1);
}
