#include "value.h"
#include "valueList.h"
#include "valueString.h"
#include "valueHashMap.h"
#include "valueObject.h"

Value *valueInit(){
	return NULL;
}

Value *valueInitType(TypeID type){
	(void)type;
	return NULL;
}

Value *valueInitNull(){
	return NULL;
}

Value *valueInitNum(double num){
	(void)num;
	return NULL;
}

Value *valueInitBool(bool _bool){
	(void)_bool;
	return NULL;
}

Value *valueInitList(ValueList *list){
	(void)list;
	return NULL;
}

Value *valueInitString(ValueString *string){
	(void)string;
	return NULL;
}

Value *valueInitHashMap(ValueHashMap *hashMap){
	(void)hashMap;
	return NULL;
}

Value *valueInitObject(ValueObject *object){
	(void)object;
	return NULL;
}

Value *valueSetNull(Value *value){
	(void)value;
	return NULL;
}

Value *valueSetNum(Value *value, double num){
	(void)value;
	(void)num;
	return NULL;
}

Value *valueSetBool(Value *value, bool _bool){
	(void)value;
	(void)_bool;
	return NULL;
}

Value *valueSetList(Value *value, ValueList *list){
	(void)value;
	(void)list;
	return NULL;
}

Value *valueSetString(Value *value, ValueString *string){
	(void)value;
	(void)string;
	return NULL;
}

Value *valueSetHashMap(Value *value, ValueHashMap *hashMap){
	(void)value;
	(void)hashMap;
	return NULL;
}

Value *valueSetObject(Value *value, ValueObject *object){
	(void)value;
	(void)object;
	return NULL;
}

bool valueIsType(Value *value, TypeID type){
	(void)value;
	(void)type;
	return false;
}

TypeID valueGetType(Value *value){
	(void)value;
	return TYPE_NONE;
}

bool valueIsTruthy(Value *value){
	(void)value;
	return false;
}

Value *valueCopy(Value *dest, const Value *src){
	(void)dest;
	(void)src;
	return NULL;
}

void valueFree(Value *value){
	(void)value;
}

