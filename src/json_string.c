
#include "json_string.h"
#include "custom_string.h"


Json *json_string_newFromData(char *data) {
  String *str = string_newFrom(data);
  Json *result = json_new();
  *result = (Json){J_STRING, {.string = str}};
  return result;
}