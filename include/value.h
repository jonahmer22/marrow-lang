#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// forward declarations
typedef struct valueList valueList;
typedef struct valueString valueString;
typedef struct valueHashMap valueHashMap;
typedef struct valueObject valueObject;

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
		valueList *list;
		valueString *string;
		valueHashMap *hashMap;
		valueObject *object;
	} is;
} Value;

#endif
