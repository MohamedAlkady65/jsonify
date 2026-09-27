#include "parser.h"
#include <assert.h>
#include <stdlib.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

JsonParser *jsonParser_create(char *data, int n) {
  JsonParser *parser = malloc(sizeof(JsonParser));
  parser->data = data;
  parser->n = n;
  parser->cursor = 0;
  return parser;
}

void jsonParser_skipWs(JsonParser *parser) {
  while (parser->cursor < parser->n &&
         (parser->data[parser->cursor] == ' ' ||
          parser->data[parser->cursor] == '\n')) {
    parser->cursor++;
  }
  if (parser->cursor >= parser->n) {
    assert(false);
  }
}

void jsonParser_parseStringHelper(JsonParser *parser, String *result) {
  StringBuilder builder = stringBuilder_new();

  assert(parser->data[parser->cursor] == '"');

  bool escaping = false;

  (parser->cursor)++;

  for (; parser->cursor < parser->n; (parser->cursor)++) {
    if (!escaping && parser->data[parser->cursor] == '"') {
      String builded = stringBuilder_build(&builder);
      *result = builded;

      return;
    }

    if (!escaping && parser->data[parser->cursor] == '\\') {
      escaping = true;
      continue;
    }

    if (escaping) {

      if (parser->data[parser->cursor] == '"' ||
          parser->data[parser->cursor] == '\\') {
        stringBuilder_append(&builder, parser->data + parser->cursor, 1);
      } else if (parser->data[parser->cursor] == 'n') {
        char newLine = '\n';
        stringBuilder_append(&builder, (&newLine), 1);
      } else {
        assert(false);
      }

      escaping = false;
      continue;
    }

    stringBuilder_append(&builder, parser->data + parser->cursor, 1);
  }
  assert(false);
}

void jsonParser_parseString(JsonParser *parser, Json **result) {
  String str;
  jsonParser_parseStringHelper(parser, &str);

  *result = malloc(sizeof(Json));
  (*result)->type = J_STRING;
  (*result)->value.string = str;
}

void jsonParser_parseNumber(JsonParser *parser, Json **result) {

  double res = 0;
  bool negative = false;
  bool hasFraction = false;
  long long fractionFactor = 10;

  if (parser->data[parser->cursor] == '-') {
    negative = true;
    parser->cursor++;
  }

  if (parser->cursor >= parser->n) {
    assert(false);
  }

  for (; parser->cursor < parser->n; parser->cursor++) {

    if (parser->data[parser->cursor] == '.') {
      if (hasFraction) {
        assert(false);
      }

      hasFraction = true;

      continue;
    }

    if (parser->data[parser->cursor] >= '0' &&
        parser->data[parser->cursor] <= '9') {
      if (hasFraction) {
        res = res +
              ((double)(parser->data[parser->cursor] - '0') / fractionFactor);
        fractionFactor *= 10;
      } else {

        res = (res * 10) + (parser->data[parser->cursor] - '0');
      }

      continue;
    }

    break;
  }

  if (hasFraction && fractionFactor == 10) {
    assert(false);
  }

  if (negative) {
    res *= -1;
  }

  *result = malloc(sizeof(Json));
  (*result)->type = J_NUMBER;
  (*result)->value.number = res;
  parser->cursor--;
}

void jsonParser_parseBool(JsonParser *parser, Json **result) {
  char *words[] = {"true", "false"};
  int wordsLen[] = {4, 5};
  bool wordsValue[] = {true, false};

  for (int w = 0; w < 2; w++) {
    for (; parser->cursor < parser->n && parser->cursor < wordsLen[w];
         parser->cursor++) {
      if (parser->data[parser->cursor] != words[w][parser->cursor]) {
        break;
      }

      if (parser->cursor == wordsLen[w] - 1) {
        *result = malloc(sizeof(Json));
        (*result)->type = J_BOOL;
        (*result)->value.boolean = wordsValue[w];
        return;
      }
    }
  }

  assert(false);
}

void jsonParser_processPair(JsonParser *parser, JsonObject **object) {
  String key;
  jsonParser_parseStringHelper(parser, &key);
  parser->cursor++;

  jsonParser_skipWs(parser);

  assert(parser->data[parser->cursor] == ':');
  parser->cursor++;

  Json *value;
  jsonParser_parseValue(parser, &value);
  parser->cursor++;

  jsonParser_skipWs(parser);
  shputs(
      (*object)->elements,
      ((JsonObjectPair){.key = key.data, .keyLen = key.len, .value = value}));
  (*object)->length++;
}

void jsonParser_parseObject(JsonParser *parser, Json **result) {

  JsonObject *res = malloc(sizeof(JsonObject));
  res->length = 0;
  res->elements = NULL;

  assert(parser->data[parser->cursor] == '{');
  parser->cursor++;
  while (parser->cursor < parser->n) {
    jsonParser_skipWs(parser);
    if (parser->data[parser->cursor] == '}')
      break;
    jsonParser_processPair(parser, &res);
    if (parser->data[parser->cursor] != ',')
      break;
    parser->cursor++;
  }

  if (parser->cursor >= parser->n || parser->data[parser->cursor] != '}') {
    assert(false);
  }

  parser->cursor++;

  *result = malloc(sizeof(Json));
  (*result)->type = J_OBJECT;
  (*result)->value.object = res;
}

void jsonParser_processArrayElement(JsonParser *parser, JsonList **list) {
  Json *value;
  jsonParser_parseValue(parser, &value);

  parser->cursor++;
  jsonParser_skipWs(parser);

  arrput((*list)->elements, *value);
  (*list)->length++;
}

void jsonParser_parseArray(JsonParser *parser, Json **result) {
  JsonList *res = malloc(sizeof(JsonList));
  res->length = 0;
  res->elements = NULL;

  assert(parser->data[parser->cursor] == '[');
  parser->cursor++;
  while (parser->cursor < parser->n) {
    jsonParser_skipWs(parser);
    if (parser->data[parser->cursor] == ']')
      break;
    jsonParser_processArrayElement(parser, &res);
    if (parser->data[parser->cursor] != ',')
      break;
    parser->cursor++;
  }

  if (parser->cursor >= parser->n || parser->data[parser->cursor] != ']') {
    assert(false);
  }

  parser->cursor++;

  *result = malloc(sizeof(Json));
  (*result)->type = J_LIST;
  (*result)->value.list = res;
}

JsonValueType jsonParser_valueType(char ch) {
  if (ch == '"') {
    return J_STRING;
  } else if (ch == '{') {
    return J_OBJECT;
  } else if (ch == '[') {
    return J_LIST;
  } else if (ch == 't' || ch == 'f') {
    return J_BOOL;
  } else if (ch == '-' || (ch >= '0' && ch <= '9')) {
    return J_NUMBER;
  } else if (ch == 'n') {
    return J_NULL;
  }
  assert(false);
}

void jsonParser_parseValue(JsonParser *parser, Json **result) {

  jsonParser_skipWs(parser);

  JsonValueType type = jsonParser_valueType(parser->data[parser->cursor]);

  switch (type) {
  case J_LIST:
    jsonParser_parseArray(parser, result);
    break;
  case J_OBJECT:
    jsonParser_parseObject(parser, result);
    break;
  case J_STRING:
    jsonParser_parseString(parser, result);
    break;
  case J_NUMBER:
    jsonParser_parseNumber(parser, result);
    break;
  case J_BOOL:
    jsonParser_parseBool(parser, result);
    break;
  default:
    assert(false);
    break;
  }
}