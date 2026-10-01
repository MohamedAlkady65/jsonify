#include "json_object.h"
#include "custom_string.h"
#include "json.h"
#include <assert.h>
#include <stb_ds.h>
#include <stdlib.h>

typedef struct JsonObjectPair {
  char *key;
  struct String *keyObject;
  struct Json *value;
} JsonObjectPair;

typedef struct JsonObject {
  int length;
  struct JsonObjectPair *elements;
} JsonObject;

JsonObject *json_object_new() {
  JsonObject *object = malloc(sizeof(JsonObject));
  assert(object != NULL);
  object->elements = NULL;
  object->length = 0;
  return object;
}

void json_object_put(JsonObject *this, String *key, Json *value) {
  shputs(
      this->elements,
      ((JsonObjectPair){.key = key->data, .keyObject = key, .value = value}));

  this->length++;
}

Json *json_object_get(JsonObject *this, String key) {
  Json *value = shget(this->elements, key.data);
  return value;
}

String *json_object_keys(JsonObject *this) {
  int n = json_object_len(this);

  String *keys = malloc(sizeof(String) * n);

  for (int i = 0; i < n; i++) {
    keys[i] = (String){this->elements[i].key, this->elements->keyObject->len};
  }

  return keys;
}

int json_object_len(JsonObject *this) { return this->length; }

void json_object_destroy(JsonObject *this) {
  if (this == NULL) {
    return;
  }

  int n = this->length;
  for (int i = 0; i < n; i++) {
    string_destroy(this->elements[i].keyObject);
    json_destroy(this->elements[i].value);
  }

  if (this->elements != NULL) {
    shfree(this->elements);
  }

  free(this);
}
