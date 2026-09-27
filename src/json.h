#ifndef JsonIFY_Json_H
#define JsonIFY_Json_H

#include "string_builder.h"
#include <stdbool.h>

typedef enum JsonValueType {
  J_STRING,
  J_NUMBER,
  J_BOOL,
  J_LIST,
  J_OBJECT,
  J_NULL,
} JsonValueType;

typedef struct JsonObjectPair {
  char *key;
  int keyLen;
  struct Json *value;
} JsonObjectPair;

typedef struct JsonObject {
  int length;
  struct JsonObjectPair *elements;
} JsonObject;

typedef struct JsonList {
  int length;
  struct Json *elements;
} JsonList;

typedef union JsonValue {
  String string;
  double number;
  bool boolean;
  JsonList *list;
  JsonObject *object;
} JsonValue;

typedef struct Json {
  enum JsonValueType type;
  union JsonValue value;
} Json;

void parseValue(char *s, int *i, int j, Json **result);

#endif // JsonIFY_Json_H
