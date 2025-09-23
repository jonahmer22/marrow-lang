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

// initialization
Value *valueInit();
Value *valueInitType(TypeID type);
Value *valueInitNull();
Value *valueInitNum(double num);
Value *valueInitBool(bool _bool);
Value *valueInitList(ValueList *list);
Value *valueInitString(ValueString *string);
Value *valueInitHashMap(ValueHashMap *hashMap);
Value *valueInitObject(ValueObject *object);

// setters
Value *valueSetNull(Value *value);
Value *valueSetNum(Value *value, double num);
Value *valueSetBool(Value *value, bool _bool);
Value *valueSetList(Value *value, ValueList *list);
Value *valueSetString(Value *value, ValueString *string);
Value *valueSetHashMap(Value *value, ValueHashMap *hashMap);
Value *valueSetObject(Value *value, ValueObject *object);

// helpers
bool valueIsType(Value *value, TypeID type);
TypeID valueGetType(Value *value);
bool valueIsTruthy(Value *value);
Value *valueCopy(Value *dest, const Value *src);

void valueFree(Value *value);

#endif
