#ifndef JSONIFY_STRING_BUILDER_H
#define JSONIFY_STRING_BUILDER_H

extern const int INITIAL_STRING_BUILDER_BUFFER_SIZE;

typedef struct String {
  char *data;
  int len;
} String;

typedef struct StringBuilder {
  char *buf;
  int len;
  int capacity;
} StringBuilder;

StringBuilder *stringBuilder_create();

void stringBuilder_append(StringBuilder *builder, char *str, int n);

String stringBuilder_build(StringBuilder *builder);

#endif // JSONIFY_STRING_BUILDER_H
