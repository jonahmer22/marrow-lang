// =======
// Numbers
// =======

#include "number.h"
#include "arena.h"

#ifndef NUBMER_DIV_PREC
#define NUMBER_DIV_PREC 16
#endif

Digit *digitInit(void){
	Digit *temp = arenaAlloc(sizeof(Digit));
	
	temp->digit = 0;
	temp->next = temp->prev = NULL;

	return temp;
}

Number *numberInit(void){
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
