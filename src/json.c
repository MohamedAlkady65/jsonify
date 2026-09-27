#include "json.h"
#include "string_builder.h"
#include "utils/utils.c"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

void parseString(char *s, int *i, int j, Json **result) {
  String str;
  parseStringHelper(s, i, j, &str);

  *result = malloc(sizeof(Json));
  (*result)->type = J_STRING;
  (*result)->value.string = str;
}

void parseNumber(char *s, int *i, int j, Json **result) {

  double res = 0;
  bool negative = false;
  bool hasFraction = false;
  long long fractionFactor = 10;

  if (s[*i] == '-') {
    negative = true;
    (*i)++;
  }

  if (*i > j) {
    assert(false);
  }

  for (; (*i) <= j; (*i)++) {

    if (s[*i] == '.') {
      if (hasFraction) {
        assert(false);
      }

      hasFraction = true;

      continue;
    }

    if (s[*i] >= '0' && s[*i] <= '9') {
      if (hasFraction) {
        res = res + ((double)(s[*i] - '0') / fractionFactor);
        fractionFactor *= 10;
      } else {

        res = (res * 10) + (s[*i] - '0');
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
  (*i)--;
}

void parseBool(char *s, int *i, int j, Json **result) {
  char *words[] = {"true", "false"};
  int wordsLen[] = {4, 5};
  bool wordsValue[] = {true, false};

  for (int w = 0; w < 2; w++) {
    for (; (*i) <= j && (*i) < wordsLen[w]; (*i)++) {
      if (s[*i] != words[w][*i]) {
        break;
      }

      if (*i == wordsLen[w] - 1) {
        *result = malloc(sizeof(Json));
        (*result)->type = J_BOOL;
        (*result)->value.boolean = wordsValue[w];
        return;
      }
    }
  }

  assert(false);
}

void parseObject(char *s, int *i, int j, Json **result) {

  JsonObject *res = malloc(sizeof(JsonObject));
  res->length = 0;
  res->elements = NULL;

  assert(s[*i] == '{');
  (*i)++;
  while ((*i) <= j) {
    skipWs(s, i, j);
    if (s[*i] == '}')
      break;
    processPair(s, i, j, &res);
    if (s[*i] != ',')
      break;
    (*i)++;
  }

  if ((*i) > j || s[*i] != '}') {
    assert(false);
  }

  (*i)++;

  *result = malloc(sizeof(Json));
  (*result)->type = J_OBJECT;
  (*result)->value.object = res;
}

void parseArray(char *s, int *i, int j, Json **result) {
  JsonList *res = malloc(sizeof(JsonList));
  res->length = 0;
  res->elements = NULL;

  assert(s[*i] == '[');
  (*i)++;
  while ((*i) <= j) {
    skipWs(s, i, j);
    if (s[*i] == ']')
      break;
    processArrayElement(s, i, j, &res);
    if (s[*i] != ',')
      break;
    (*i)++;
  }

  if ((*i) > j || s[*i] != ']') {
    assert(false);
  }

  (*i)++;

  *result = malloc(sizeof(Json));
  (*result)->type = J_LIST;
  (*result)->value.list = res;
}

void parseValue(char *s, int *i, int j, Json **result) {

  skipWs(s, i, j);

  JsonValueType type = valueType(s[*i]);

  switch (type) {
  case J_LIST:
    parseArray(s, i, j, result);
    break;
  case J_OBJECT:
    parseObject(s, i, j, result);
    break;
  case J_STRING:
    parseString(s, i, j, result);
    break;
  case J_NUMBER:
    parseNumber(s, i, j, result);
    break;
  case J_BOOL:
    parseBool(s, i, j, result);
    break;
  default:
    assert(false);
    break;
  }
}