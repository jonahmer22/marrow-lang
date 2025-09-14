#ifndef VALUEOBJECT_H
#define VALUEOBJECT_H

// forward delcarations
typedef struct ValueHashMap ValueHashMap;
typedef struct MethodTable MethodTable;
typedef struct ObjectPrototype ObjectPrototype;

// object type for marrow vm. this is used for user defined values (classes)
typedef struct ValueObject{
	ValueHashMap *fields;
	MethodTable *methods;
	ObjectPrototype *prototype;
} ValueObject;

#endif
