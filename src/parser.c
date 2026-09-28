#include "parser.h"
#include "error.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

JsonParser *jsonParser_create(char *data, int n) {
  JsonParser *parser = malloc(sizeof(JsonParser));
  assert(parser != NULL);
  parser->data = data;
  parser->n = n;
  parser->cursor = 0;
  return parser;
}

err_t jsonParser_skipWs(JsonParser *parser) {
  while (parser->cursor < parser->n && (parser->data[parser->cursor] == ' ' ||
                                        parser->data[parser->cursor] == '\n')) {
    parser->cursor++;
  }
  if (parser->cursor >= parser->n) {
    return ERR;
  }
  return OK;
}

err_t jsonParser_parseStringHelper(JsonParser *parser, String *result) {
  StringBuilder *builder = stringBuilder_create();

  if (parser->data[parser->cursor] != '"') {
    return ERR;
  }

  bool escaping = false;

  (parser->cursor)++;

  for (; parser->cursor < parser->n; (parser->cursor)++) {
    if (!escaping && parser->data[parser->cursor] == '"') {
      String builded = stringBuilder_build(builder);
      *result = builded;
      return OK;
    }

    if (!escaping && parser->data[parser->cursor] == '\\') {
      escaping = true;
      continue;
    }

    if (escaping) {

      if (parser->data[parser->cursor] == '"' ||
          parser->data[parser->cursor] == '\\') {
        stringBuilder_append(builder, parser->data + parser->cursor, 1);
      } else if (parser->data[parser->cursor] == 'n') {
        char newLine = '\n';
        stringBuilder_append(builder, (&newLine), 1);
      } else {
        return ERR;
      }

      escaping = false;
      continue;
    }

    stringBuilder_append(builder, parser->data + parser->cursor, 1);
  }
  return ERR;
}

err_t jsonParser_parseString(JsonParser *parser, Json **result) {
  String str;
  err_t err = jsonParser_parseStringHelper(parser, &str);

  if (err != OK) {
    return err;
  }

  *result = malloc(sizeof(Json));
  assert(result != NULL);
  (*result)->type = J_STRING;
  (*result)->value.string = str;
  return OK;
}

err_t jsonParser_parseNumber(JsonParser *parser, Json **result) {

  double res = 0;
  bool negative = false;
  bool hasFraction = false;
  long long fractionFactor = 10;

  if (parser->data[parser->cursor] == '-') {
    negative = true;
    parser->cursor++;
  }

  if (parser->cursor >= parser->n) {
    return ERR;
  }

  for (; parser->cursor < parser->n; parser->cursor++) {

    if (parser->data[parser->cursor] == '.') {
      if (hasFraction) {
        return ERR;
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
    return ERR;
  }

  if (negative) {
    res *= -1;
  }

  *result = malloc(sizeof(Json));
  assert(result != NULL);
  (*result)->type = J_NUMBER;
  (*result)->value.number = res;
  parser->cursor--;
  return OK;
}

err_t jsonParser_parseBool(JsonParser *parser, Json **result) {
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
        assert(result != NULL);
        (*result)->type = J_BOOL;
        (*result)->value.boolean = wordsValue[w];
        return OK;
      }
    }
  }

  return ERR;
}

err_t jsonParser_processPair(JsonParser *parser, JsonObject **object) {
  String key;
  err_t err;

  err = jsonParser_parseStringHelper(parser, &key);

  if (err != OK) {
    return err;
  }
  parser->cursor++;

  err = jsonParser_skipWs(parser);
  if (err != OK) {
    return err;
  }

  if (parser->data[parser->cursor] != ':') {
    return OK;
  };
  parser->cursor++;

  Json *value;
  err = jsonParser_parseValue(parser, &value);

  if (err != OK) {
    return err;
  }

  parser->cursor++;

  err = jsonParser_skipWs(parser);
  if (err != OK) {
    return err;
  }
  shputs(
      (*object)->elements,
      ((JsonObjectPair){.key = key.data, .keyLen = key.len, .value = value}));
  (*object)->length++;

  return OK;
}

err_t jsonParser_parseObject(JsonParser *parser, Json **result) {
  JsonObject *res = malloc(sizeof(JsonObject));
  assert(res != NULL);
  res->length = 0;
  res->elements = NULL;

  if (parser->data[parser->cursor] != '{') {
    return OK;
  };

  parser->cursor++;
  while (parser->cursor < parser->n) {
    err_t err = jsonParser_skipWs(parser);
    if (err != OK) {
      return err;
    }
    if (parser->data[parser->cursor] == '}')
      break;
    err = jsonParser_processPair(parser, &res);
    if (err != OK) {
      return err;
    }
    if (parser->data[parser->cursor] != ',')
      break;
    parser->cursor++;
  }

  if (parser->cursor >= parser->n || parser->data[parser->cursor] != '}') {
    return ERR;
  }

  parser->cursor++;

  *result = malloc(sizeof(Json));
  assert(result != NULL);
  (*result)->type = J_OBJECT;
  (*result)->value.object = res;

  return OK;
}

err_t jsonParser_processArrayElement(JsonParser *parser, JsonList **list) {
  Json *value;
  err_t err = jsonParser_parseValue(parser, &value);
  if (err != OK) {
    return err;
  }
  parser->cursor++;
  err = jsonParser_skipWs(parser);
  if (err != OK) {
    return err;
  }
  arrput((*list)->elements, *value);
  (*list)->length++;

  return OK;
}

err_t jsonParser_parseArray(JsonParser *parser, Json **result) {
  JsonList *res = malloc(sizeof(JsonList));
  assert(res != NULL);

  res->length = 0;
  res->elements = NULL;

  if (parser->data[parser->cursor] != '[') {
    return OK;
  };
  parser->cursor++;
  while (parser->cursor < parser->n) {
    err_t err = jsonParser_skipWs(parser);
    if (err != OK) {
      return err;
    }

    if (parser->data[parser->cursor] == ']')
      break;

    err = jsonParser_processArrayElement(parser, &res);

    if (err != OK) {
      return err;
    }

    if (parser->data[parser->cursor] != ',')
      break;
    parser->cursor++;
  }

  if (parser->cursor >= parser->n || parser->data[parser->cursor] != ']') {
    return ERR;
  }

  parser->cursor++;

  *result = malloc(sizeof(Json));
  assert(result != NULL);
  (*result)->type = J_LIST;
  (*result)->value.list = res;

  return OK;
}

err_t jsonParser_valueType(char ch, JsonValueType *out) {
  if (ch == '"') {
    *out = J_STRING;
    return OK;
  } else if (ch == '{') {
    *out = J_OBJECT;
    return OK;
  } else if (ch == '[') {
    *out = J_LIST;
    return OK;
  } else if (ch == 't' || ch == 'f') {
    *out = J_BOOL;
    return OK;
  } else if (ch == '-' || (ch >= '0' && ch <= '9')) {
    *out = J_NUMBER;
    return OK;
  } else if (ch == 'n') {
    *out = J_NULL;
    return OK;
  }
  return ERR;
}

err_t jsonParser_parseValue(JsonParser *parser, Json **result) {

  err_t err;

  err = jsonParser_skipWs(parser);
  if (err != OK) {
    return err;
  }
  JsonValueType type;

  err = jsonParser_valueType(parser->data[parser->cursor], &type);
  if (err != OK) {
    return err;
  }

  switch (type) {
  case J_LIST:
    err = jsonParser_parseArray(parser, result);
    break;
  case J_OBJECT:
    err = jsonParser_parseObject(parser, result);
    break;
  case J_STRING:
    err = jsonParser_parseString(parser, result);
    break;
  case J_NUMBER:
    err = jsonParser_parseNumber(parser, result);
    break;
  case J_BOOL:
    err = jsonParser_parseBool(parser, result);
    break;
  default:
    return ERR;
    break;
  }

  if (err != OK) {
    return err;
  }
  return OK;
}