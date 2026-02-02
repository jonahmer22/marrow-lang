#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "number.h"
#include "mstring.h"
#include "parser.h"

// all types of tokens
enum TokenType{
	// values
	TOK_NUM,
	TOK_STR,
	TOK_BOOL,
	TOK_NONE,
	// arithmatic
	TOK_ADD,
	TOK_ADD_EQ,
	TOK_SUB,
	TOK_SUB_EQ,
	TOK_DIV,
	TOK_DIV_EQ,
	TOK_MUL,
	TOK_MUL_EQ,
	TOK_POW,
	TOK_FLR,
	TOK_INC,
	TOK_DEC,
	TOK_MOD,
	TOK_MOD_EQ,
	TOK_ASSIGN,	// I don't know any better place to put this
	// comparison
	TOK_LEQ,
	TOK_GEQ,
	TOK_EQ,
	TOK_LT,
	TOK_GT,
	// control flow
	TOK_DOT,
	TOK_COMMA,
	TOK_IF,
	TOK_FOR,
	TOK_WHILE,
	TOK_VAR,
	TOK_FUN,
	TOK_LEFT_BRACE,
	TOK_RIGHT_BRACE,
	TOK_LEFT_BRACKET,
	TOK_RIGHT_BRACKET,
	TOK_LEFT_PAREN,
	TOK_RIGHT_PAREN,
	TOK_IDENT,
	TOK_PRINT,
	TOK_RETURN,
	// SPECIALS
	TOK_ERROR
};

typedef struct Token{
	enum TokenType type;
	union{
		Number *num;
		String *str;
		bool b;		// TODO eventual will have it's own struct
		void *none;
	} data;

	size_t line;

	struct Token *next;
	struct Token *prev;
} Token;

typedef struct TokenList{
	Token *head;
	Token *tail;

	size_t len;
} TokenList;

TokenList *tokenize(const char *buffer);
void tokenListPrint(TokenList *list);

#endif
