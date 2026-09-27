#ifndef JSONIFY_JSON_H
#define JSONIFY_JSON_H

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


#endif // JSONIFY_JSON_H
