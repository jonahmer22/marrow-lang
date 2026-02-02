#ifndef NUMBER_H
#define NUMBER_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct Digit{
	uint8_t digit;

	struct Digit *next;
	struct Digit *prev;
} Digit;

typedef struct Number{
	Digit *head;
	Digit *tail;

	size_t len;

	size_t scale;
	bool neg;
} Number;

Digit *digitInit(void);
Number *numberInit(void);
void numberAppend(Number *n, int c);
void numberPrepend(Number *n, int c);
void numberAppendZeros(Number *n, size_t k);
bool numberIsZero(Number *n);
void numberNormalize(Number *n);
Number *numberClone(Number *n);
Number *numberFromU64(uint64_t x);
Number *numberOneAtScale(size_t scale);
void alignScalesCopy(Number *a, Number *b, Number **outA, Number **outB, size_t *outScale);
int cmpMagSameScale(Number *a, Number *b);
Number *addMagSameScale(Number *a, Number *b, size_t scale);
Number *subMagSameScale(Number *a, Number *b, size_t scale);
uint8_t *digitsLSDArray(Number *n);
Number *mulMag(Number *a, Number *b, size_t scaleOut);
int cmpIntMag(Number *a, Number *b);
void intMagMul10AddDigit(Number *result, uint8_t digit);
void intMagSubInPlace(Number *a, Number *b);
void divmodIntMag(Number *a, Number *b, Number **outQ, Number **outR);

// public
Number *numberAdd(Number *a, Number *b);
Number *numberSub(Number *a, Number *b);
Number *numberMul(Number *a, Number *b);
Number *numberDiv(Number *a, Number *b);
Number *numberInc(Number *n);
Number *numberDec(Number *n);
Number *numberPow(Number *n, uint8_t m);
Number *numberFlr(Number *a, Number *b);
Number *numberMod(Number *a, Number *b);
char *numberString(Number *n);
void numberPrint(Number *n);

#endif
