#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// forward declarations
typedef struct ValueList ValueList;
typedef struct ValueString ValueString;
typedef struct ValueHashMap ValueHashMap;
typedef struct ValueObject ValueObject;

// used to label types and functions that are attatched to types (methods)
typedef enum TypeID{
	TYPE_NONE = 0,
	TYPE_NULL,
	TYPE_NUM,
	TYPE_BOOL,
	TYPE_LIST,
	TYPE_STRING,
	TYPE_HASH_MAP,
	TYPE_OBJECT
} TypeID;

// main storage medium of language
typedef struct Value{
	// type tag
	TypeID type;
	
	// actual stuff (mostly pointers)
	union{
		double num;
		bool _bool;
		ValueList *list;
		ValueString *string;
		ValueHashMap *hashMap;
		ValueObject *object;
	} is;
} Value;

#endif
