#include "custom_string.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

String *string_newFrom(char *data) {
  int n = strlen(data);
  char *resultData = malloc(n + 1);
  assert(resultData != NULL);
  memcpy(resultData, data, n + 1);

  String *result = malloc(sizeof(String));
  assert(result != NULL);

  result->data = resultData;
  result->len = n;

  return result;
}

String *string_new() {
  char *resultData = malloc(1);
  assert(resultData != NULL);

  resultData[0] = 0;

  String *result = malloc(sizeof(String));
  assert(result != NULL);

  result->data = resultData;
  result->len = 0;

  return result;
}

void string_destroy(String *this) {
  if (this == NULL) {
    return;
  }

  if (this->data != NULL) {
    free(this->data);
  }

  free(this);
}