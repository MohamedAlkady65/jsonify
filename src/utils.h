#ifndef JSONIFY_UTILS_H
#define JSONIFY_UTILS_H

#include "json.h"
#include "string_builder.h"
#include <stdbool.h>

void skipWs(char *s, int *i, int j);

JSON_VALUE_TYPE valueType(char ch);

void parseStringHelper(char *s, int *i, int j, String *result);

void processArrayElement(char *s, int *i, int j, JSON_LIST **list);

void processPair(char *s, int *i, int j, JSON_OBJECT **object);

#endif // JSONIFY_UTILS_H