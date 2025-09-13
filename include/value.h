#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

typedef struct List List;
typedef struct String String;
typedef struct HashMap HashMap;
typedef struct Object Object;

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

typedef struct Value{
	// type tag
	TypeID type;

	union{
		double num;
		bool _bool;
		List *list;
		String *string;
		HashMap *hashMap;
		Object *object;
	} is;
} Value;

#endif
