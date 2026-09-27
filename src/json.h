#ifndef JSONIFY_JSON_H
#define JSONIFY_JSON_H

#include "string_builder.h"
#include <stdbool.h>

typedef enum JSON_VALUE_TYPE {
  J_STRING,
  J_NUMBER,
  J_BOOL,
  J_LIST,
  J_OBJECT,
  J_NULL,
} JSON_VALUE_TYPE;

typedef struct JSON_OBJECT_PAIR {
  char *key;
  int keyLen;
  struct JSON *value;
} JSON_OBJECT_PAIR;

typedef struct JSON_OBJECT {
  int length;
  struct JSON_OBJECT_PAIR *elements;
} JSON_OBJECT;

typedef struct JSON_LIST {
  int length;
  struct JSON *elements;
} JSON_LIST;

typedef union JSON_VALUE {
  String string;
  double number;
  bool boolean;
  JSON_LIST *list;
  JSON_OBJECT *object;
} JSON_VALUE;

typedef struct JSON {
  enum JSON_VALUE_TYPE type;
  union JSON_VALUE value;
} JSON;

void parseString(char *s, int *i, int j, JSON **result);

void parseNumber(char *s, int *i, int j, JSON **result);

void parseBool(char *s, int *i, int j, JSON **result);

void parseObject(char *s, int *i, int j, JSON **result);

void parseArray(char *s, int *i, int j, JSON **result);

void parseValue(char *s, int *i, int j, JSON **result);

#endif // JSONIFY_JSON_H
