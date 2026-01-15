#include <stdio.h>
#include <stddef.h>
#include <stdalign.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// =====
// Arena
// =====

// arena uses error numbers 0x0020

#define BLOCK_SIZE (1024)

typedef struct MemBlock{
	char *buffer;
	char *head;

	size_t used;
	size_t size;

	struct MemBlock *nextBlock;
} MemBlock;

MemBlock *memBlockInit(size_t size){
	size_t allocSize = (size <= BLOCK_SIZE ? BLOCK_SIZE : size);

	MemBlock *block = malloc(sizeof(MemBlock));
	if(!block){
		fprintf(stderr, "[FATAL 0x%04X]: Not enough memory to allocate arena block struct.\n", 0x0021);
		exit(EXIT_FAILURE);
	}
	block->head = block->buffer = malloc(sizeof(char) * allocSize);
	if(!block->head){
		fprintf(stderr, "[FATAL 0x%04X]: Not enough memory to allocate arena block of %ld bytes.\n", 0x0022, allocSize);
		exit(EXIT_FAILURE);
	}
	block->used = 0;
	block->size = allocSize;
	block->nextBlock = NULL;
	
	return block;
}

MemBlock *memBlockAppend(MemBlock *curr, size_t size){
	MemBlock *temp = memBlockInit(size);

	curr->nextBlock = temp;
	
	return temp;
}

MemBlock *memBlockDestroy(MemBlock *curr){
	free(curr->buffer);
	curr->buffer = NULL;
	curr->head = NULL;
	curr->size = 0;
	curr->used = 0;

	MemBlock *temp = curr->nextBlock;
	curr->nextBlock = NULL;
	free(curr);

	return temp;
}

typedef struct Arena{
	MemBlock *blocks;
	MemBlock *head;
	
	size_t numBlocks;
	size_t numBytes;
} Arena;

static Arena *arena;	// global arena

void arenaInit(){
	arena = malloc(sizeof(Arena));
	if(!arena){
		fprintf(stderr, "[FATAL 0x%04X]: Not enough memory to allocate arena struct.\n", 0x0023);
		exit(EXIT_FAILURE);
	}
	
	arena->blocks = NULL;
	arena->head = NULL;
	arena->numBytes = 0;
	arena->numBlocks = 0;
}

void *arenaAlloc(size_t size){
	if(arena->blocks == NULL){
		arena->head = arena->blocks = memBlockInit(size);
		arena->numBlocks++;
	}

	uintptr_t p = (uintptr_t)arena->head->head;
	size_t a = alignof(max_align_t);
	uintptr_t aligned = (p + (a - 1)) & ~(uintptr_t)(a - 1);

	size_t pad = (size_t)(aligned - p);
	size_t need = pad + size;


	size_t available = arena->head->size - arena->head->used;
	if(available < need){
		arena->head = memBlockAppend(arena->head, need);
		arena->numBlocks++;
		p = (uintptr_t)arena->head->head;
		aligned = (p + (a - 1)) & ~(uintptr_t)(a - 1);
		pad = (size_t)(aligned - p);
	}

	arena->head->head += pad;
	arena->head->used += pad;

	void *retAddr = arena->head->head;
	arena->head->head += size;
	arena->head->used += size;
	arena->numBytes += need;

	return retAddr;
}

void arenaDestroy(){
	if(arena->numBlocks != 0){
		for(MemBlock *temp = memBlockDestroy(arena->blocks); temp != NULL; temp = memBlockDestroy(temp));
	}
	free(arena);
}

// =======
// General
// =======

// read in a file at a path and return it's contents in a buffer
char *readFile(const char *path, size_t *outLen){
	FILE *file = fopen(path, "rb");	// read as binary
	if(!file){
		fprintf(stderr, "[FATAL 0x%04X]: Could not open file at \"%s\".\n", 0x0011, path);
		exit(EXIT_FAILURE);
	}

	// get file size
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	// allocate size of buffer
	char *buffer = malloc(sizeof(char) * size + 1);
	if(!buffer){
		fprintf(stderr, "[FATAL 0x%04X]: Not enough memory to allocate buffer of size %ld bytes.\n", 0x0012, size + 1);
		exit(EXIT_FAILURE);
	}

	if(fread(buffer, 1, size, file) != (size_t)size){
		free(buffer);
		fclose(file);

		fprintf(stderr, "[FATAL 0x%04X]: Read in buffer does not match size of file.\n", 0x0013);
		exit(EXIT_FAILURE);
	}

	buffer[size] = '\0';
	if(outLen)
		*outLen = (size_t)size;

	fclose(file);
	return buffer;
}

// ============
// Basic Parser
// ============

typedef struct Parser{
	const char *buff;
	const char *curr;

	size_t line;
} Parser;

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

// =======
// Numbers
// =======

#ifndef NUBMER_DIV_PREC
#define NUMBER_DIV_PREC 16
#endif

typedef struct Digit{
	uint8_t digit;

	struct Digit *next;
	struct Digit *prev;
} Digit;

Digit *digitInit(){
	Digit *temp = arenaAlloc(sizeof(Digit));
	
	temp->digit = 0;
	temp->next = temp->prev = NULL;

	return temp;
}

typedef struct Number{
	Digit *head;
	Digit *tail;

	size_t len;

	size_t scale;
	bool neg;
} Number;

Number *numberInit(){
	Number *temp = arenaAlloc(sizeof(Number));

	temp->head = temp->tail = NULL;
	temp->len = temp->scale = 0;
	temp->neg = false;

	return temp;
}

void numberAppend(Number *n, int c){
	Digit *d = digitInit();
	d->digit = c;

	if(n->head && n->tail){
		n->tail->next = d;
		d->prev = n->tail;

		n->tail = d;
	}
	else{
		n->head = n->tail = d;
	}

	n->len++;
}
void numberPrepend(Number *n, int c){
	Digit *d = digitInit();
	d->digit = c;

	if(n->head && n->tail){
		n->head->prev = d;
		d->next = n->head;

		n->head = d;
	}
	else{
		n->head = n->tail = d;
	}

	n->len++;
}

void numberAppendZeros(Number *n, size_t k){
	for(size_t i = 0; i < k; i++) numberAppend(n, 0);
}

bool numberIsZero(Number *n){
	return !n || n->len == 0 || (n->len == 1 && n->head && n->head->digit == 0);
}

void numberNormalize(Number *n){
	if(!n || n->len == 0 || n->head == NULL){
		if(n){
			n->neg = false;
			n->scale = 0;
		}

		return;
	}

	// strip leading
	while(n->len > 1 && n->head && n->head->digit == 0){
		Digit *old = n->head;
		n->head = old->next;
		
		if(n->head)
			n->head->prev = NULL;

		n->len--;
	}

	// fix any wrong state the number could be in after removing leading zeros
	if(!n->head){
		n->len = 0;
		n->tail = NULL;
		n->neg = false;
		n->scale = 0;

		return;
	}
	if(!n->tail){
		Digit *temp = n->head;
		while(temp && temp->next)
			temp = temp->next;
		n->tail = temp;
	}

	// strip tailing
	while(n->scale > 0 && n->len > 1 && n->tail && n->tail->digit == 0){
		Digit *old = n->tail;
		n->tail = old->prev;

		if(n->tail)
			n->tail->next = NULL;

		n->len--;
		n->scale--;
	}

	// if it was just 0 fix any broken state
	if(numberIsZero(n)){
		n->neg = false;
		n->scale = 0;

		if(n->len == 0 || n->head == NULL){
			n->head = n->tail = digitInit();
			n->len = 1;
		}
		else if(n->len > 1){
			n->head->digit = 0;
		}
	}
}

Number *numberClone(Number *n){
	Number *result = numberInit();
	
	result->neg = n->neg;
	result->scale = n->scale;

	for(Digit *dig = n->head; dig; dig = dig->next){
		numberAppend(result, dig->digit);
	}

	if(n->len == 0)
		numberAppend(result, 0);

	return result;
}

Number *numberFromU64(uint64_t x){
	Number *result = numberInit();

	if(x == 0){
		numberAppend(result, 0);

		return result;
	}

	while(x > 0){
		uint8_t digit = (uint8_t)(x % 10);

		numberPrepend(result, digit);

		x /= 10;
	}

	return result;
}

Number *numberOneAtScale(size_t scale){
	Number *result = numberInit();
	
	result->scale = scale;
	numberAppend(result, 1);
	numberAppendZeros(result, scale);

	return result;
}

void alignScalesCopy(Number *a, Number *b, Number **outA, Number **outB, size_t *outScale){
	size_t s = (a->scale > b->scale) ? a->scale : b->scale;

	Number *aa = numberClone(a);
	Number *bb = numberClone(b);

	if(aa->scale < s){
		numberAppendZeros(aa, s - aa->scale);
		aa->scale = s;
	}
	if(bb->scale < s){
		numberAppendZeros(bb, s - bb->scale);
		bb->scale = s;
	}

	*outA = aa;
	*outB = bb;

	if(outScale)
		*outScale = s;
}

int cmpMagSameScale(Number *a, Number *b){
	if(a->len < b->len)
		return -1;
	if(a->len > b->len)
		return 1;

	Digit *digA = a->head;
	Digit *digB = b->head;
	while(digA && digB){
		if(digA->digit < digB->digit)
			return -1;
		if(digA->digit > digB->digit)
			return 1;

		digA = digA->next;
		digB = digB->next;
	}

	return 0;
}

Number *addMagSameScale(Number *a, Number *b, size_t scale){
	Number *result = numberInit();
	result->scale = scale;

	Digit *digA = a->tail;
	Digit *digB = b->tail;

	int carry = 0;
	while(digA || digB || carry){
		int sum = carry;
		
		if(digA){
			sum += digA->digit;
			digA = digA->prev;
		}
		if(digB){
			sum += digB->digit;
			digB = digB->prev;
		}
		carry = sum / 10;

		uint8_t digit = (uint8_t)(sum % 10);
		numberPrepend(result, digit);
	}

	numberNormalize(result);

	return result;
}

Number *subMagSameScale(Number *a, Number *b, size_t scale){
	Number *result = numberInit();
	result->scale = scale;

	Digit *digA = a->tail;
	Digit *digB = b->tail;

	int borrow = 0;
	while(digA){
		int diff = (int)digA->digit - borrow - (digB ? (int)digB->digit : 0);

		if(diff < 0){
			diff += 10;
			borrow = 1;
		}
		else
			borrow = 0;

		numberPrepend(result, (uint8_t)diff);

		digA = digA->prev;
		if(digB)
			digB = digB->prev;
	}

	numberNormalize(result);

	return result;
}

uint8_t *digitsLSDArray(Number *n){
	uint8_t *arr = arenaAlloc(sizeof(uint8_t) * n->len);
	
	size_t i = 0;
	for(Digit *dig = n->tail; dig; dig = dig->prev){
		arr[i++] = dig->digit;
	}

	return arr;
}

Number *mulMag(Number *a, Number *b, size_t scaleOut){
	if(numberIsZero(a) || numberIsZero(b)){
		Number *result = numberFromU64(0);

		return result;
	}

	uint8_t *A = digitsLSDArray(a);
	uint8_t *B = digitsLSDArray(b);

	size_t n = a->len, m = b->len;
	size_t outN = n + m;

	uint32_t *acc = arenaAlloc(sizeof(uint32_t) * outN);
	for(size_t i = 0; i < outN; i++)
		acc[i] = 0;

	for(size_t i = 0; i < n; i++){
		uint32_t carry = 0;
		for(size_t j = 0; j < m; j++){
			uint32_t t = acc[i + j] + (uint32_t)A[i] * (uint32_t)B[j] + carry;
			acc[i + j] = t % 10;
			carry = t / 10;
		}
		size_t k = i + m;
		while(carry){
			uint32_t t = acc[k] + carry;
			acc[k] = t % 10;
			carry = t / 10;
			k++;
		}
	}

	size_t top = (ssize_t)outN - 1;
	while(top > 0 && acc[top] == 0)
		top--;

	Number *result = numberInit();
	result->scale = scaleOut;

	for(ssize_t i = top; i >= 0; i--)
		numberAppend(result, (uint8_t)acc[i]);
	numberNormalize(result);

	return result;
}

int cmpIntMag(Number *a, Number *b){
	if(a->len < b->len)
		return -1;
	if(a->len > b->len)
		return 1;

	Digit *digA = a->head, *digB = b->head;
	while(digA && digB){
		if(digA->digit < digB->digit)
			return -1;
		if(digA->digit > digB->digit)
			return 1;

		digA = digA->next;
		digB = digB->next;
	}

	return 0;
}

void intMagMul10AddDigit(Number *result, uint8_t digit){
	if(result->len == 1 && result->head && result->head->digit == 0){
		result->head->digit = digit;

		return;
	}

	numberAppend(result, digit);
}

void intMagSubInPlace(Number *a, Number *b){
	Digit *digA = a->tail;
	Digit *digB = b->tail;
	
	int borrow = 0;

	while(digA){
		int diff = (int)digA->digit - borrow - (digB ? (int)digB->digit : 0);
		
		if(diff < 0){
			diff += 10;
			borrow = 1;
		}
		else
			borrow = 0;
		
		digA->digit = (uint8_t)diff;
		
		digA = digA->prev;
		if(digB)
			digB = digB->prev;
	}

	numberNormalize(a);
}

void divmodIntMag(Number *a, Number *b, Number **outQ, Number **outR){
	if(numberIsZero(b)){
		fprintf(stderr, "[ERROR]: division by zero.\n");

		exit(EXIT_FAILURE);
	}

	if(cmpIntMag(a, b) < 0){
		Number *q = numberFromU64(0);
		Number *r = numberClone(a);

		*outQ = q;
		*outR = r;

		return;
	}

	Number *q = numberInit();
	Number *r = numberFromU64(0);

	for(Digit *dig = a->head; dig; dig = dig->next){
		intMagMul10AddDigit(r, dig->digit);
		numberNormalize(r);

		uint8_t x = 0;
		if(cmpIntMag(r, b) >= 0){
			Number *rb = numberClone(r);
			while(x < 9 && cmpIntMag(rb, b) >= 0){
				intMagSubInPlace(rb, b);
				x++;
			}
			r = rb;
		}

		numberAppend(q, x);
	}

	numberNormalize(q);
	numberNormalize(r);

	*outQ = q;
	*outR = r;
}

// public number api

Number *numberAdd(Number *a, Number *b){
	Number *aa, *bb;
	size_t s;
	alignScalesCopy(a, b, &aa, &bb, &s);

	Number *result = NULL;
	if(aa->neg == bb->neg){
		result = addMagSameScale(aa, bb, s);
		result->neg = aa->neg;
	}
	else{
		int cmp = cmpMagSameScale(aa, bb);
		if(cmp == 0){
			result = numberFromU64(0);
		}
		else if(cmp > 0){
			result = subMagSameScale(aa, bb, s);
			result->neg = aa->neg;
		}
		else{
			result = subMagSameScale(bb, aa, s);
			result->neg = bb->neg;
		}
	}
	numberNormalize(result);

	return result;
}
Number *numberSub(Number *a, Number *b){
	Number *nb = numberClone(b);
	nb->neg = !nb->neg;
	
	return numberAdd(a, nb);
}
Number *numberMul(Number *a, Number *b){
	Number *aa = numberClone(a);
	Number *bb = numberClone(b);

	size_t outScale = a->scale + b->scale;
	Number *result = mulMag(aa, bb, outScale);
	result->neg = (a->neg != b->neg) && !numberIsZero(result);
	numberNormalize(result);

	return result;
}
Number *numberDiv(Number *a, Number *b){
	if(numberIsZero(b)){
		fprintf(stderr, "[ERROR]: division by zero.\n");

		exit(EXIT_FAILURE);
	}
	if(numberIsZero(a)){
		return numberFromU64(0);
	}

	Number *A = numberClone(a);
	Number *B = numberClone(b);

	if(b->scale > a->scale)
		numberAppendZeros(A, b->scale - a->scale);
	else if(a->scale > b->scale)
		numberAppendZeros(B, a->scale - b->scale);

	A->scale = 0;
	B->scale = 0;

	numberAppendZeros(A, NUMBER_DIV_PREC);

	Number *q, *rrem;
	divmodIntMag(A, B, &q, &rrem);

	q->scale = NUMBER_DIV_PREC;
	q->neg = (a->neg != b->neg) && !numberIsZero(q);
	numberNormalize(q);

	return q;
}
Number *numberInc(Number *n){
	return numberAdd(n, numberOneAtScale(n->scale));
}
Number *numberDec(Number *n){
	return numberSub(n, numberOneAtScale(n->scale));
}
Number *numberPow(Number *n, uint8_t m){
	if(m == 0)
		return numberFromU64(1);
	if(m == 1)
		return numberClone(n);

	Number *base = numberClone(n);
	Number *result = numberFromU64(1);

	uint8_t e = m;
	while(e > 0){
		if(e & 1)
			result = numberMul(result, base);
		e >>= 1;
		if(e)
			base = numberMul(base, base);
	}
	numberNormalize(result);

	return result;
}
Number *numberFlr(Number *a, Number *b){
	if(numberIsZero(b)){
		fprintf(stderr, "[ERROR]: division by zero.\n");

		exit(EXIT_FAILURE);
	}
	if(numberIsZero(a)){
		return numberFromU64(0);
	}

	Number *A = numberClone(a);
	A->neg = false;
	Number *B = numberClone(b);
	B->neg = false;

	if(b->scale > a->scale)
		numberAppendZeros(A, b->scale - a->scale);
	else if(a->scale > b->scale)
		numberAppendZeros(B, a->scale - b->scale);
	A->scale = 0;
	B->scale = 0;
	numberNormalize(A);
	numberNormalize(B);

	Number *q, *rrem;
	divmodIntMag(A, B, &q, &rrem);

	bool neg_out = (a->neg != b->neg);
	q->scale = 0;

	if(neg_out && !numberIsZero(rrem)){
		Number *one = numberFromU64(1);
		Number *qp1 = numberAdd(q, one);
		q = qp1;
	}

	q->neg = neg_out && !numberIsZero(q);
	numberNormalize(q);

	return q;
}
Number *numberMod(Number *a, Number *b){
	if(numberIsZero(b)){
		fprintf(stderr, "[ERROR]: division by zero.\n");

		exit(EXIT_FAILURE);
	}

	Number *q = numberFlr(a, b);
	Number *qb = numberMul(q, b);
	Number *r = numberSub(a, qb);
	numberNormalize(r);

	return r;
}

char *numberString(Number *n){
	if(!n){
		return "(none)";
	}

	numberNormalize(n);

	bool isZero = numberIsZero(n);
	bool isNeg = n->neg && !isZero;

	size_t digitsLen = (n->len > 0 && n->head != NULL) ? n->len : 0;
	size_t scale = n->scale;

	size_t outLen = 0;

	if(digitsLen == 0){
		outLen = (isNeg ? 1 : 0) + 1;
		if(scale > 0)
			outLen += 1 + scale;
	}
	else if(scale == 0){
		outLen = (isNeg ? 1 : 0) + digitsLen;	
	}
	else if(scale >= digitsLen){
		outLen = (isNeg ? 1 : 0) + 2 + scale;
	}
	else{
		outLen = (isNeg ? 1 : 0) + digitsLen + 1;
	}

	char *result = arenaAlloc((sizeof(char) * outLen) + 1);
	char *temp = result;

	if(isNeg)
		*temp++ = '-';

	if(digitsLen == 0){
		*temp++ = '0';
		if(scale > 0){
			*temp++ = '.';

			for(size_t i = 0; i < scale; i++)
				*temp++ = '0';
		}
		*temp = '\0';

		return result;
	}

	if(scale == 0){
		for(Digit *dig = n->head; dig; dig = dig->next)
			*temp++ = dig->digit + '0';
		*temp = '\0';

		return result;
	}

	if(scale >= digitsLen){
		*temp++ = '0';
		*temp++ = '.';

		size_t leadingZeros = scale - digitsLen;

		for(size_t i = 0; i < leadingZeros; i++, *temp++ = '0');
		for(Digit *dig = n->head; dig; dig = dig->next)
			*temp++ = dig->digit + '0';
		*temp = '\0';

		return result;
	}

	size_t intLen = digitsLen - scale;
	size_t i = 0;

	for(Digit *dig = n->head; dig; dig = dig->next, i++){
		if(i == intLen)
			*temp++ = '.';
		*temp++ = dig->digit + '0';
	}
	*temp = '\0';

	return result;
}

void numberPrint(Number *n){
	if(!n){
		puts("(none)");
		
		return;
	}

	numberNormalize(n);

	if(n->neg && !numberIsZero(n))
		putchar('-');

	if(n->len == 0 || n->head == NULL){
		putchar('0');
		if(n->scale){
			putchar('.');
			for(size_t i = 0; i < n->scale; i++, putchar('0'));
		}
		
		return;
	}

	if(n->scale == 0){
		for(Digit *temp = n->head; temp; temp = temp->next)
			putchar(temp->digit + '0');

		return;
	}

	if(n->scale >= n->len){
		putchar('0');
		putchar('.');
		
		size_t leadingZeros = n->scale - n->len;
		for(size_t i = 0; i < leadingZeros; i++, putchar('0'));
		for(Digit *temp = n->head; temp; temp = temp->next)
			putchar(temp->digit + '0');

		return;
	}

	size_t intLen = n->len - n->scale;
	size_t i = 0;

	for(Digit *temp = n->head; temp; temp = temp->next, i++){
		if(i == intLen)
			putchar('.');
		putchar(temp->digit + '0');
	}
}

// =======
// Strings
// =======

typedef struct String{
	char *buff;
	size_t len;

	size_t cap;
} String;

String *stringInit(){
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

// =========
// Tokenizer
// =========

// token definition

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

Token *tokenInit(){
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
			// TODO
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
			snprintf(outS, size, token->data.str->buff);
			break;
		case TOK_VAR:
			snprintf(outS, size, "VAR");
			break;
		case TOK_FUN:
			snprintf(outS, size, "FUNCTION");
			break;
		case TOK_PRINT:
			// TODO
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

// token list definition

typedef struct TokenList{
	Token *head;
	Token *tail;

	size_t len;
} TokenList;

TokenList *tokenListInit(){
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
	// TODO: should eventually have it's own arbitrary accuracy type
	// 	 for now should be just a double.
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
	// TODO
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
			case '#':
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

	tokenListPrint(list);

	return list;
}

// ====
// Main
// ====

int main(int argc, char **argv){
	if(argc <= 1){
		fprintf(stderr, "[FATAL 0x%04X]: Not enough arguements.\n", 0x0001);
		exit(EXIT_FAILURE);
	}
	arenaInit();
	size_t len = 0;
	char *buff = readFile(argv[1], &len);

	// execution
	// 	TODO: tokenize the text
	TokenList *list = tokenize(buff);
	// 	TODO: convert the tokens into an AST
	// 	TODO: emit bytecode based on AST
	free(buff);	// buffer can be freed after emition of bytecode	
	//	TODO: execute bytecode

	arenaDestroy();
	return EXIT_SUCCESS;
}

