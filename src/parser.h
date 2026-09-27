#ifndef JSONIFY_PARSER_H
#define JSONIFY_PARSER_H

#include "json.h"

typedef struct JsonParser {
  char *data;
  int n;
  int j;
  int cursor;
} JsonParser;

JsonParser *jsonParser_create(char *data, int n);

void jsonParser_parseValue(JsonParser *parser, Json **result);

#endif // JSONIFY_PARSER_H
