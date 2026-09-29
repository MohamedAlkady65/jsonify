#include "parser.h"
#include "error.h"
#include "json_list.h"
#include "json_object.h"
#include <assert.h>
#include <stdlib.h>

JsonParser *jsonParser_create(char *data, int n) {
  JsonParser *parser = malloc(sizeof(JsonParser));
  assert(parser != NULL);
  parser->data = data;
  parser->n = n;
  parser->cursor = 0;
  return parser;
}

err_t jsonParser_skipWs(JsonParser *this) {
  while (this->cursor < this->n && (this->data[this->cursor] == ' ' ||
                                    this->data[this->cursor] == '\n')) {
    this->cursor++;
  }
  if (this->cursor >= this->n) {
    return ERR;
  }
  return OK;
}

err_t jsonParser_parseStringHelper(JsonParser *this, String *result) {
  StringBuilder *builder = stringBuilder_create();

  if (this->data[this->cursor] != '"') {
    return ERR;
  }

  bool escaping = false;

  (this->cursor)++;

  for (; this->cursor < this->n; (this->cursor)++) {
    if (!escaping && this->data[this->cursor] == '"') {
      String builded = stringBuilder_build(builder);
      *result = builded;
      return OK;
    }

    if (!escaping && this->data[this->cursor] == '\\') {
      escaping = true;
      continue;
    }

    if (escaping) {

      if (this->data[this->cursor] == '"' || this->data[this->cursor] == '\\') {
        stringBuilder_append(builder, this->data + this->cursor, 1);
      } else if (this->data[this->cursor] == 'n') {
        char newLine = '\n';
        stringBuilder_append(builder, (&newLine), 1);
      } else {
        return ERR;
      }

      escaping = false;
      continue;
    }

    stringBuilder_append(builder, this->data + this->cursor, 1);
  }
  return ERR;
}

err_t jsonParser_parseString(JsonParser *this, Json **result) {
  String str;
  err_t err = jsonParser_parseStringHelper(this, &str);

  if (err != OK) {
    return err;
  }

  *result = malloc(sizeof(Json));
  assert(result != NULL);
  (*result)->type = J_STRING;
  (*result)->value.string = str;
  return OK;
}

err_t jsonParser_parseNumber(JsonParser *this, Json **result) {

  double res = 0;
  bool negative = false;
  bool hasFraction = false;
  long long fractionFactor = 10;

  if (this->data[this->cursor] == '-') {
    negative = true;
    this->cursor++;
  }

  if (this->cursor >= this->n) {
    return ERR;
  }

  for (; this->cursor < this->n; this->cursor++) {

    if (this->data[this->cursor] == '.') {
      if (hasFraction) {
        return ERR;
      }

      hasFraction = true;

      continue;
    }

    if (this->data[this->cursor] >= '0' && this->data[this->cursor] <= '9') {
      if (hasFraction) {
        res = res + ((double)(this->data[this->cursor] - '0') / fractionFactor);
        fractionFactor *= 10;
      } else {

        res = (res * 10) + (this->data[this->cursor] - '0');
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
  this->cursor--;
  return OK;
}

err_t jsonParser_parseBool(JsonParser *this, Json **result) {
  char *words[] = {"true", "false"};
  int wordsLen[] = {4, 5};
  bool wordsValue[] = {true, false};

  for (int w = 0; w < 2; w++) {
    for (int i = 0; this->cursor < this->n && i < wordsLen[w];
         this->cursor++, i++) {
      if (this->data[this->cursor] != words[w][i]) {
        break;
      }

      if (i == wordsLen[w] - 1) {
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

err_t jsonParser_parseNull(JsonParser *this, Json **result) {
  char *word = "null";
  int wordLen = 4;

  for (int i = 0; this->cursor < this->n && i < wordLen; this->cursor++, i++) {
    if (this->data[this->cursor] != word[i]) {
      break;
    }

    if (i == wordLen - 1) {
      *result = malloc(sizeof(Json));
      assert(result != NULL);
      (*result)->type = J_NULL;
      return OK;
    }
  }

  return ERR;
}

err_t jsonParser_processPair(JsonParser *this, JsonObject *object) {
  String key;
  err_t err;

  err = jsonParser_parseStringHelper(this, &key);

  if (err != OK) {
    return err;
  }
  this->cursor++;

  err = jsonParser_skipWs(this);
  if (err != OK) {
    return err;
  }

  if (this->data[this->cursor] != ':') {
    return OK;
  };
  this->cursor++;

  Json *value;
  err = jsonParser_parseValue(this, &value);

  if (err != OK) {
    return err;
  }

  this->cursor++;

  err = jsonParser_skipWs(this);
  if (err != OK) {
    return err;
  }

  json_object_put(object, key, value);

  return OK;
}

err_t jsonParser_parseObject(JsonParser *this, Json **result) {
  JsonObject *res = json_object_create();
  assert(res != NULL);

  if (this->data[this->cursor] != '{') {
    return OK;
  };

  this->cursor++;
  while (this->cursor < this->n) {
    err_t err = jsonParser_skipWs(this);
    if (err != OK) {
      return err;
    }
    if (this->data[this->cursor] == '}')
      break;
    err = jsonParser_processPair(this, res);
    if (err != OK) {
      return err;
    }
    if (this->data[this->cursor] != ',')
      break;
    this->cursor++;
  }

  if (this->cursor >= this->n || this->data[this->cursor] != '}') {
    return ERR;
  }

  this->cursor++;

  *result = malloc(sizeof(Json));
  assert(result != NULL);
  (*result)->type = J_OBJECT;
  (*result)->value.object = res;

  return OK;
}

err_t jsonParser_processArrayElement(JsonParser *this, JsonList *list) {
  Json *value;
  err_t err = jsonParser_parseValue(this, &value);
  if (err != OK) {
    return err;
  }
  this->cursor++;
  err = jsonParser_skipWs(this);
  if (err != OK) {
    return err;
  }

  json_list_put(list, value);

  return OK;
}

err_t jsonParser_parseArray(JsonParser *this, Json **result) {
  JsonList *res = json_list_create();
  assert(res != NULL);

  if (this->data[this->cursor] != '[') {
    return OK;
  };
  this->cursor++;
  while (this->cursor < this->n) {
    err_t err = jsonParser_skipWs(this);
    if (err != OK) {
      return err;
    }

    if (this->data[this->cursor] == ']')
      break;

    err = jsonParser_processArrayElement(this, res);

    if (err != OK) {
      return err;
    }

    if (this->data[this->cursor] != ',')
      break;
    this->cursor++;
  }

  if (this->cursor >= this->n || this->data[this->cursor] != ']') {
    return ERR;
  }

  this->cursor++;

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

err_t jsonParser_parseValue(JsonParser *this, Json **result) {

  err_t err;

  err = jsonParser_skipWs(this);
  if (err != OK) {
    return err;
  }
  JsonValueType type;

  err = jsonParser_valueType(this->data[this->cursor], &type);
  if (err != OK) {
    return err;
  }

  switch (type) {
  case J_LIST:
    err = jsonParser_parseArray(this, result);
    break;
  case J_OBJECT:
    err = jsonParser_parseObject(this, result);
    break;
  case J_STRING:
    err = jsonParser_parseString(this, result);
    break;
  case J_NUMBER:
    err = jsonParser_parseNumber(this, result);
    break;
  case J_BOOL:
    err = jsonParser_parseBool(this, result);
    break;
  case J_NULL:
    err = jsonParser_parseNull(this, result);
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