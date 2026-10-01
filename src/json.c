
#include "json.h"
#include "custom_string.h"
#include "error.h"
#include "json_list.h"
#include "json_object.h"
#include "parser.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

Json *json_new() {
  Json *this = malloc(sizeof(Json));
  assert(this != NULL);
  return this;
}

void json_destroy(Json *this) {
  if (this == NULL) {
    return;
  }

  if (this->type == J_OBJECT) {
    json_object_destroy(this->value.object);
  } else if (this->type == J_LIST) {
    json_list_destroy(this->value.list);
  } else if (this->type == J_STRING) {
    string_destroy(this->value.string);
  }

  free(this);
}

err_t json_parse(char *data, Json **out) {
  JsonParser *parser = jsonParser_new(data, strlen(data));
  err_t err = jsonParser_parseValue(parser, out);
  jsonParser_destroy(parser);
  return err;
}

Json *json_newStringFromData(char *data) {
  String *str = string_newFrom(data);
  Json *result = json_new();
  *result = (Json){J_STRING, {.string = str}};
  return result;
}