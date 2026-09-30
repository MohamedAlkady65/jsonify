#ifndef JSONIFY_PARSER_H
#define JSONIFY_PARSER_H

#include "error.h"
#include "json.h"

typedef struct JsonParser {
  char *data;
  int n;
  int cursor;
} JsonParser;

JsonParser *jsonParser_new(char *data, int n);

void jsonParser_destroy(JsonParser *this);

err_t jsonParser_parseValue(JsonParser *this, Json **result);

#endif // JSONIFY_PARSER_H
