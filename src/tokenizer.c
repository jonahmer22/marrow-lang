// =========
// Tokenizer
// =========

#include "tokenizer.h"
#include "arena.h"

Token *tokenInit(void){
	Token *temp = arenaAlloc(sizeof(Token));

	temp->type = TOK_NONE;
	temp->data.none = NULL;
	temp->line = 0;
	temp->next = NULL;
	temp->prev = NULL;

	return temp;
}

char *tokenPrint(Token *token){
	int size = 1024;

	char *outS = arenaAlloc(sizeof(char) * size);
	switch(token->type){
		// need to have a case for every type
		case TOK_NUM:
			snprintf(outS, size, "%s", numberString(token->data.num));
			break;
		case TOK_STR:
			snprintf(outS, size, "%s", token->data.str->buff);
			break;
		case TOK_BOOL:
			snprintf(outS, size, (token->data.b ? "TRUE" : "FALSE"));
			break;
		case TOK_NONE:
			snprintf(outS, size, "NONE");
			break;
		case TOK_ADD:
			snprintf(outS, size, "+");
			break;
		case TOK_ADD_EQ:
			snprintf(outS, size, "+=");
			break;
		case TOK_INC:
			snprintf(outS, size, "++");
			break;
		case TOK_SUB:
			snprintf(outS, size, "-");
			break;
		case TOK_SUB_EQ:
			snprintf(outS, size, "-=");
			break;
		case TOK_DEC:
			snprintf(outS, size, "--");
			break;
		case TOK_DIV:
			snprintf(outS, size, "/");
			break;
		case TOK_DIV_EQ:
			snprintf(outS, size, "/=");
			break;
		case TOK_FLR:
			snprintf(outS, size, "//");
			break;
		case TOK_MUL:
			snprintf(outS, size, "*");
			break;
		case TOK_MUL_EQ:
			snprintf(outS, size, "*=");
			break;
		case TOK_POW:
			snprintf(outS, size, "**");
			break;
		case TOK_MOD:
			snprintf(outS, size, "%%");
			break;
		case TOK_MOD_EQ:
			snprintf(outS, size, "%%=");
			break;
		case TOK_ASSIGN:
			snprintf(outS, size, "=");
			break;
		case TOK_EQ:
			snprintf(outS, size, "==");
			break;
		case TOK_LT:
			snprintf(outS, size, "<");
			break;
		case TOK_LEQ:
			snprintf(outS, size, "<=");
			break;
		case TOK_GT:
			snprintf(outS, size, ">");
			break;
		case TOK_GEQ:
			snprintf(outS, size, ">=");
			break;
		case TOK_DOT:
			snprintf(outS, size, ".");
			break;
		case TOK_COMMA:
			snprintf(outS, size, ",");
			break;
		case TOK_IF:
			snprintf(outS, size, "IF");
			break;
		case TOK_FOR:
			snprintf(outS, size, "FOR");
			break;
		case TOK_WHILE:
			snprintf(outS, size, "WHILE");
			break;
		case TOK_LEFT_BRACE:
			snprintf(outS, size, "{");
			break;
		case TOK_RIGHT_BRACE:
			snprintf(outS, size, "}");
			break;
		case TOK_LEFT_BRACKET:
			snprintf(outS, size, "[");
			break;
		case TOK_RIGHT_BRACKET:
			snprintf(outS, size, "]");
			break;
		case TOK_LEFT_PAREN:
			snprintf(outS, size, "(");
			break;
		case TOK_RIGHT_PAREN:
			snprintf(outS, size, ")");
			break;
		case TOK_IDENT:
			snprintf(outS, size, "%s", token->data.str->buff);
			break;
		case TOK_VAR:
			snprintf(outS, size, "VAR");
			break;
		case TOK_FUN:
			snprintf(outS, size, "FUNCTION");
			break;
		case TOK_PRINT:
			snprintf(outS, size, "PRINT");
			break;
		case TOK_RETURN:
			snprintf(outS, size, "RETURN");
			break;
		case TOK_ERROR:
			snprintf(outS, size, "ERROR");
			break;
		default:
			return NULL;
	}

	return outS;
}

TokenList *tokenListInit(void){
	TokenList *temp = arenaAlloc(sizeof(TokenList));
	
	temp->head = temp->tail = NULL;
	temp->len = 0;

	return temp;
}

void tokenListAppend(TokenList *list, Token *token){
	if(list->head == NULL){
		list->head = list->tail = token;
		list->len++;
		return;
	}

	list->tail->next = token;
	token->prev = list->tail;
	list->len++;

	list->tail = token;
}

Token *tokenListPop(TokenList *list){
	if(list->len < 1){
		list->head = list->tail = NULL;
		list->len = 0;
		return NULL;
	}

	Token *temp = list->tail;
	
	if(list->len > 1){
		list->tail = temp->prev;
		list->len--;
		list->tail->next = NULL;
	}
	else{
		list->head = list->tail = NULL;
		list->len = 0;
	}

	temp->next = temp->prev = NULL;
	return temp;
}

void tokenListPrint(TokenList *list){
	for(Token *temp = list->head; temp != NULL; temp = temp->next){
		printf("[%s]", tokenPrint(temp));
		if(temp->next != NULL){
			printf("%s", temp->next->prev == temp ? "-" : " >");
			continue;
		}
	}
	printf("\n");
}

// make token list

// tokenize values
Token *tokenizeNum(Parser *p){
	// rules:
	// 	- must in one of the following forms:
	// 		- 123456
	// 		- 123.456
	Number *result = numberInit();
	
	result->neg = false;
	result->scale = 0;

	bool decimalSeen;

	int c = prev(p);
	if(c < '0' || c > '9'){
		fprintf(stderr, "[FATAL 0x%04X]: impropper parser state.\n", 0x0031);

		exit(EXIT_FAILURE);
	}

	numberAppend(result, c - '0');

	while(true){
		int pk = peek(p);

		if(pk >= '0' && pk <= '9'){
			c = advance(p);
			
			numberAppend(result, c - '0');

			if(decimalSeen)
				result->scale++;

			continue;
		}

		if(pk == '.'){
			if(decimalSeen){
				fprintf(stderr, "[Line %ld]: invalid number literal (multiple '.').\n", p->line + 1);

				exit(EXIT_FAILURE);
			}

			decimalSeen = true;
			advance(p);

			continue;
		}

		break;
	}

	// create token from value
	Token *temp = tokenInit();
	
	temp->type = TOK_NUM;
	temp->data.num = result;
	temp->line = p->line;

	return temp;
}
Token *tokenizeString(Parser *p){
	// reads a string from a starting \" to ending \"
	// generates an error at the starting line of the string
	// if there is no ending \" (reaches EOF before end of quote)
	String *s = stringInit();
	int c;

	size_t sLine = p->line;

	while((c = advance(p)) != '"'){
		if(c == '\0'){
			// TODO: better error printing
			fprintf(stderr, "[Line %ld]: unterminated string.\n", sLine + 1);
			
			exit(EXIT_FAILURE);
		}
		if(c == '\n') p->line++;

		if(s->len + 1 > s->cap){
			size_t newSize = s->cap * 2;
			
			char *temp = arenaAlloc(sizeof(char) * newSize);
			memcpy(temp, s->buff, s->len);

			s->buff = temp;
			s->cap = newSize;
		}

		s->len++;
		s->buff[s->len - 1] = c;
	}
	s->buff[s->len] = '\0';

	Token *temp = tokenInit();

	temp->type = TOK_STR;
	temp->data.str = s;
	temp->line = p->line;

	return temp;
}
static short parenDepth = 0;	// state holders
static short bracketDepth = 0;
static short braceDepth = 0;
Token *tokenizeParenthesis(Parser *p){
	// ()
	Token *temp = tokenInit();
	switch(prev(p)){
		case '(':
			parenDepth++;
			
			temp->type = TOK_LEFT_PAREN;
			temp->data.none = NULL;
			temp->line = p->line;

			break;
		case ')':
			parenDepth -= 1;
			if(parenDepth < 0){
				fprintf(stderr, "[Line %ld]: closing unpaired closing parenthesis.\n", p->line + 1);

				exit(EXIT_FAILURE);
			}

			temp->type = TOK_RIGHT_PAREN;
			temp->data.none = NULL;
			temp->line = p->line;

			break;
		default:
			fprintf(stderr, "[Line %ld]: error parsing parenthesis, invalid character.\n", p->line + 1);

			exit(EXIT_FAILURE);
			break;
	}

	return temp;
}
Token *tokenizeBrackets(Parser *p){
	// []
	Token *temp = tokenInit();
	switch(prev(p)){
		case '[':
			bracketDepth++;
			
			temp->type = TOK_LEFT_BRACKET;
			temp->data.none = NULL;
			temp->line = p->line;

			break;
		case ']':
			bracketDepth -= 1;
			if(bracketDepth < 0){
				fprintf(stderr, "[Line %ld]: closing unpaired closing bracket.\n", p->line + 1);

				exit(EXIT_FAILURE);
			}

			temp->type = TOK_RIGHT_BRACKET;
			temp->data.none = NULL;
			temp->line = p->line;

			break;
		default:
			fprintf(stderr, "[Line %ld]: error parsing bracket, invalid character.\n", p->line + 1);

			exit(EXIT_FAILURE);
			break;
	}

	return temp;
}
Token *tokenizeBraces(Parser *p){
	// {}
	Token *temp = tokenInit();
	switch(prev(p)){
		case '{':
			braceDepth++;
			
			temp->type = TOK_LEFT_BRACE;
			temp->data.none = NULL;
			temp->line = p->line;

			break;
		case '}':
			braceDepth -= 1;
			if(braceDepth < 0){
				fprintf(stderr, "[Line %ld]: closing unpaired closing brace.\n", p->line + 1);

				exit(EXIT_FAILURE);
			}

			temp->type = TOK_RIGHT_BRACE;
			temp->data.none = NULL;
			temp->line = p->line;

			break;
		default:
			fprintf(stderr, "[Line %ld]: error parsing brace, invalid character.\n", p->line + 1);

			exit(EXIT_FAILURE);
			break;
	}

	return temp;
}

bool isIdentStart(int c){
	return (c == '_') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
bool isIdentCont(int c){
	return isIdentStart(c) || (c >= '0' && c <= '9');
}
Token *tokenizeThing(Parser *p){
	// tokenizes keywords and variable names
	// anything that doesn't fit into the values
	//
	// anything left over is stored as an
	// identifier token as long as it's identifier
	// is valid
	int start = prev(p);

	if(!isIdentStart(start)){
		fprintf(stderr, "[Line %ld]: unexpected character '%c'.\n", p->line + 1, start);

		exit(EXIT_FAILURE);
	}

	String *s = stringInit();
	stringPushChar(s, start);

	while(isIdentCont(peek(p))){
		stringPushChar(s, advance(p));
	}

	Token *tok = tokenInit();
	tok->line = p->line;

	// keywords
	if(stringCharCmp(s, "if")){
		tok->type = TOK_IF;
		return tok;
	}
	else if(stringCharCmp(s, "for")){
		tok->type = TOK_FOR;
		return tok;
	}
	else if(stringCharCmp(s, "while")){
		tok->type = TOK_WHILE;
		return tok;
	}
	else if(stringCharCmp(s, "fun") || stringCharCmp(s, "def")){
		// both fun and def work to declare functions
		tok->type = TOK_FUN;
		return tok;
	}
	else if(stringCharCmp(s, "return")){
		tok->type = TOK_RETURN;
		return tok;
	}
	else if(stringCharCmp(s, "print")){
		tok->type = TOK_PRINT;
		return tok;
	}

	// literals
	else if(stringCharCmp(s, "true")){
		tok->type = TOK_BOOL;
		tok->data.b = true;
		return tok;
	}
	else if(stringCharCmp(s, "false")){
		tok->type = TOK_BOOL;
		tok->data.b = false;
		return tok;
	}
	else if(stringCharCmp(s, "none") || stringCharCmp(s, "null")){
		tok->type = TOK_NONE;
		tok->data.none = NULL;
		return tok;
	}

	// var
	else if(stringCharCmp(s, "var")){
		tok->type = TOK_VAR;
		return tok;
	}


	// if none of the above it must be an identifier
	else{
		tok->type = TOK_IDENT;
		tok->data.str = s;
		return tok;
	}
}

TokenList *tokenize(const char *buffer){
	// switch to initialize parsing of next token
	TokenList *list = tokenListInit();
	Parser *p = parserInit(buffer);

	int c;

	while((c = advance(p)) != '\0'){
		Token *tok = NULL;
		switch(c){
			case '#':{
				int ch;
				while((ch = advance(p)) != '\0'){
					if(ch == '\n'){
						(p->line)++;
						break;
					}
					if(peek(p) == '\0')
						break;
				}
				break;
			}
			case '\t': case '\r': case ' ':
				continue;
				break;
			case '\n':
				(p->line)++;
				continue;	// so that diagnostic stuff isn't printed every new line
				break;
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				tok = tokenizeNum(p);
				tokenListAppend(list, tok);
				break;
			case '+':
				// check increment
				if(peek(p) == '+'){
					advance(p);

					tok = tokenInit();
					tok->type = TOK_INC;
					tok->data.none = NULL;
					tok->line = p->line;

					tokenListAppend(list, tok);

					break;
				}
				else if(peek(p) == '='){
					advance(p);
					
					tok = tokenInit();
					tok->type = TOK_ADD_EQ;
					tok->data.none = NULL;
					tok->line = p->line;

					tokenListAppend(list, tok);

					break;
				}

				tok = tokenInit();
				tok->type = TOK_ADD;
				tok->data.none = NULL;
				tok->line = p->line;

				tokenListAppend(list, tok);

				break;
			case '-':
				// check decrement
				if(peek(p) == '-'){
					advance(p);
					
					tok = tokenInit();
					tok->type = TOK_DEC;
					tok->data.none = NULL;
					tok->line = p->line;

					tokenListAppend(list, tok);

					break;
				}
				else if(peek(p) == '='){
					advance(p);

					tok = tokenInit();
					tok->type = TOK_SUB_EQ;
					tok->data.none = NULL;
					tok->line = p->line;

					tokenListAppend(list, tok);

					break;
				}
				
				tok = tokenInit();
				tok->type = TOK_SUB;
				tok->data.none = NULL;
				tok->line = p->line;

				tokenListAppend(list, tok);

				break;
			case '/':
				// check floor
				if(peek(p) == '/'){
					advance(p);

					tok = tokenInit();
					tok->type = TOK_FLR;
					tok->data.none = NULL;
					tok->line = p->line;
					
					tokenListAppend(list, tok);

					break;
				}
				else if(peek(p) == '='){
					advance(p);

					tok = tokenInit();
					tok->type = TOK_DIV_EQ;
					tok->data.none = NULL;
					tok->line = p->line;

					tokenListAppend(list, tok);

					break;
				}

				tok = tokenInit();
				tok->type = TOK_DIV;
				tok->data.none = NULL;
				tok->line = p->line;

				tokenListAppend(list, tok);

				break;
			case '*':
				// check power
				if(peek(p) == '*'){
					advance(p);

					tok = tokenInit();
					tok->type = TOK_POW;
					tok->data.none = NULL;
					tok->line = p->line;

					tokenListAppend(list, tok);

					break;
				}
				else if(peek(p) == '='){
					advance(p);

					tok = tokenInit();
					tok->type = TOK_MUL_EQ;
					tok->data.none = NULL;
					tok->line = p->line;

					tokenListAppend(list, tok);

					break;
				}

				tok = tokenInit();
				tok->type = TOK_MUL;
				tok->data.none = NULL;
				tok->line = p->line;

				tokenListAppend(list, tok);

				break;
			case '%':
				if(peek(p) == '='){
					advance(p);

					tok = tokenInit();
					tok->type = TOK_MOD_EQ;
					tok->data.none = NULL;
					tok->line = p->line;

					tokenListAppend(list, tok);

					break;
				}

				tok = tokenInit();
				tok->type = TOK_MOD;
				tok->data.none = NULL;
				tok->line = p->line;

				tokenListAppend(list, tok);

				break;
			case '=':
				// check ==
				if(peek(p) == '='){
					advance(p);

					tok = tokenInit();
					tok->type = TOK_EQ;
					tok->data.none = NULL;
					tok->line = p->line;

					tokenListAppend(list, tok);

					break;
				}

				tok = tokenInit();
				tok->type = TOK_ASSIGN;
				tok->data.none = NULL;
				tok->line = p->line;

				tokenListAppend(list, tok);

				break;
			case '<':
				// check <=
				if(peek(p) == '='){
					advance(p);

					tok = tokenInit();
					tok->type = TOK_LEQ;
					tok->data.none = NULL;
					tok->line = p->line;

					tokenListAppend(list, tok);

					break;
				}

				tok = tokenInit();
				tok->type = TOK_LT;
				tok->data.none = NULL;
				tok->line = p->line;

				tokenListAppend(list, tok);

				break;
			case '>':
				// check >=
				if(peek(p) == '='){
					advance(p);

					tok = tokenInit();
					tok->type = TOK_GEQ;
					tok->data.none = NULL;
					tok->line = p->line;

					tokenListAppend(list, tok);

					break;
				}

				tok = tokenInit();
				tok->type = TOK_GT;
				tok->data.none = NULL;
				tok->line = p->line;

				tokenListAppend(list, tok);

				break;
			case '.':
				tok = tokenInit();
				tok->type = TOK_DOT;
				tok->data.none = NULL;
				tok->line = p->line;

				tokenListAppend(list, tok);

				break;
			case ',':
				tok = tokenInit();
				tok->type = TOK_COMMA;
				tok->data.none = NULL;
				tok->line = p->line;
				
				tokenListAppend(list, tok);

				break;
			case '"':
				tok = tokenizeString(p);
				tokenListAppend(list, tok);
				break;
			case '(':
			case ')':
				// must have a closing )
				// prob it's own function to keep state if
				// entered or exiting a parenthesis block
				// should also contain an error message generator
				tok = tokenizeParenthesis(p);
				tokenListAppend(list, tok);
				break;
			case '[':
			case ']':
				tok = tokenizeBrackets(p);
				tokenListAppend(list, tok);
				break;
			case '{':
			case '}':
				// must have a closing }
				// same thing with the parenthesis block
				// probably own function to track state
				tok = tokenizeBraces(p);
				tokenListAppend(list, tok);
				break;
			case ';':
				break;
			default:
				if(isIdentStart(c)){
					tok = tokenizeThing(p);
					tokenListAppend(list, tok);
				}
				else{
					fprintf(stderr, "[Line %ld]: unexpected character '%c'.\n", p->line + 1, c);

					exit(EXIT_FAILURE);
				}
				break;
		}
	}
	if(parenDepth != 0){
		fprintf(stderr, "[ERROR]: non-valid parenthesis depth.\n");

		// TODO: all of these exits need to arenaDestroy() for memory safety
		
		// TODO: should also have better printing and tell the user where 
		// 	 the issue might be
		exit(EXIT_FAILURE);
	}
	if(bracketDepth != 0){
		fprintf(stderr, "[ERROR]: non-valid bracket depth.\n");

		// TODO: all of these exits need to arenaDestroy() for memory safety
		
		// TODO: should also have better printing and tell the user where 
		// 	 the issue might be
		exit(EXIT_FAILURE);
	}
	if(braceDepth != 0){
		fprintf(stderr, "[ERROR]: non-valid brace depth.\n");

		// TODO: all of these exits need to arenaDestroy() for memory safety
		
		// TODO: should also have better printing and tell the user where 
		// 	 the issue might be
		exit(EXIT_FAILURE);
	}

	return list;
}
