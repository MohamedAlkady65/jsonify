#ifndef JSONIFY_STRING_BUILDER_H
#define JSONIFY_STRING_BUILDER_H

#include "custom_string.h"

extern const int INITIAL_STRING_BUILDER_BUFFER_SIZE;
typedef struct StringBuilder {
  char *buf;
  int len;
  int capacity;
} StringBuilder;

StringBuilder *stringBuilder_new();

void stringBuilder_destroy(StringBuilder *this);

void stringBuilder_append(StringBuilder *builder, char *str, int n);

String *stringBuilder_build(StringBuilder *builder);

#endif // JSONIFY_STRING_BUILDER_H
