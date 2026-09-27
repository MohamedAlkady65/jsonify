#include "json.h"
#include "string_builder.h"
#include <assert.h>
#include <stb_ds.h>
#include <stdbool.h>

void skipWs(char *s, int *i, int j) {
  while ((*i) <= j && (s[*i] == ' ' || s[*i] == '\n')) {
    (*i)++;
  }
  if (*i > j) {
    assert(false);
  }
}

JsonValueType valueType(char ch) {
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

void parseStringHelper(char *s, int *i, int j, String *result) {
  StringBuilder builder = stringBuilder_new();

  assert(s[*i] == '"');

  bool escaping = false;

  (*i)++;

  for (; *i <= j; (*i)++) {
    if (!escaping && s[*i] == '"') {
      String builded = stringBuilder_build(&builder);
      *result = builded;

      return;
    }

    if (!escaping && s[*i] == '\\') {
      escaping = true;
      continue;
    }

    if (escaping) {

      if (s[*i] == '"' || s[*i] == '\\') {
        stringBuilder_append(&builder, s + *i, 1);
      } else if (s[*i] == 'n') {
        char newLine = '\n';
        stringBuilder_append(&builder, (&newLine), 1);
      } else {
        assert(false);
      }

      escaping = false;
      continue;
    }

    stringBuilder_append(&builder, s + *i, 1);
  }
  assert(false);
}

void processArrayElement(char *s, int *i, int j, JsonList **list) {
  Json *value;
  parseValue(s, i, j, &value);

  (*i)++;
  skipWs(s, i, j);

  arrput((*list)->elements, *value);
  (*list)->length++;
}

void processPair(char *s, int *i, int j, JsonObject **object) {
  String key;
  parseStringHelper(s, i, j, &key);
  (*i)++;

  skipWs(s, i, j);

  assert(s[*i] == ':');
  (*i)++;

  Json *value;
  parseValue(s, i, j, &value);
  (*i)++;

  skipWs(s, i, j);
  shputs(
      (*object)->elements,
      ((JsonObjectPair){.key = key.data, .keyLen = key.len, .value = value}));
  (*object)->length++;
}