#include "json_list.h"
#include "json.h"
#include <assert.h>
#include <stb_ds.h>
#include <stdlib.h>

typedef struct JsonList {
  int length;
  struct Json **elements;
} JsonList;


JsonList *json_list_create() {
  JsonList *list = malloc(sizeof(JsonList));
  assert(list != NULL);
  list->elements = NULL;
  list->length = 0;
  return list;
}

void json_list_put(JsonList *this, Json *value) {
  arrput(this->elements, value);
  this->length++;
}

Json *json_list_get(JsonList *this, int i) {
  Json *value = this->elements[i];
  return value;
}

int json_list_len(JsonList *this) { return this->length; }
