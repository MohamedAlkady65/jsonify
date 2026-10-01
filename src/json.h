#ifndef JSONIFY_JSON_H
#define JSONIFY_JSON_H

#include "custom_string.h"
#include "error.h"
#include <stdbool.h>

typedef enum JsonValueType {
  J_STRING,
  J_NUMBER,
  J_BOOL,
  J_LIST,
  J_OBJECT,
  J_NULL,
} JsonValueType;

typedef struct JsonList JsonList;
typedef struct JsonObject JsonObject;

typedef union JsonValue {
  String *string;
  double number;
  bool boolean;
  JsonList *list;
  JsonObject *object;
} JsonValue;

typedef struct Json {
  enum JsonValueType type;
  union JsonValue value;
} Json;

Json *json_new();

void json_destroy(Json *this);

err_t json_parse(char *data, Json **out);

#endif // JSONIFY_JSON_H
