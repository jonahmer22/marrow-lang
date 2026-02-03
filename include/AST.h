#ifndef AST_H
#define AST_H

#include "tokenizer.h"

typedef enum SyntaxType{
	unary,
	binary,
	literal
} SyntaxType;

typedef struct Syntax{
	SyntaxType type;
	Token *tok;

	struct Syntax *left;
	struct Syntax *right;
} Syntax;


// file may have multiple ASTs for multiple functions
typedef struct AST{
	Syntax *headl;

	size_t size;
} AST;

#endif

