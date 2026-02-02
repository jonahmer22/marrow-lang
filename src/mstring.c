// =======
// Strings
// =======

#include <string.h>

#include "mstring.h"
#include "arena.h"

String *stringInit(void){
	String *temp = arenaAlloc(sizeof(String));

	temp->buff = arenaAlloc(sizeof(char) * 8);
	temp->cap = 8;
	temp->len = 0;

	return temp;
}

void stringPushChar(String *s, char c){
	if(s->len + 1 >= s->cap){
		size_t newCap = s->cap * 2;
		char *temp = arenaAlloc(newCap);
		memcpy(temp, s->buff, s->len);
		s->buff = temp;
		s->cap = newCap;
	}

	s->buff[s->len++] = c;
	s->buff[s->len] = '\0';
}

bool stringCharCmp(String *s, char *c){
	if(strcmp(s->buff, c) == 0)
		return true;
	return false;
}

bool stringCmp(String *a, String *b){
	if(strcmp(a->buff, b->buff) == 0)
		return true;
	return false;
}
