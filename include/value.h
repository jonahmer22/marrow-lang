#ifndef VALUE_H
#define VALUE_H

#include "common.h"
#include "functionHash.h"


typedef enum ValueType{
	NUM,	// stored as double
	STR,	// stored as char*
	ARR,	// TBD possibly it's own type like Arr*
	MAP, 	// TBD possibly it's own type like Map*
	_BOOL, 	// stored as bool
	_NULL,	// NOT STORED LOGIC MUST BE IN VM
	CLASS	// TBD no clue
} ValueType;

typedef struct Value{
	// actual value storage
	ValueType type;
	union{
		double num;
		char *string;
		bool _bool;
	} is;	// get value as EX: val.is.num or val.is._bool
	
	// metadata like name and functions (both optional)
	char valueName[256];	// max name length of 256 chars not including terminating character
	FunctionHash methods;	// any attatched methods to a given value type
} Value;

#endif
