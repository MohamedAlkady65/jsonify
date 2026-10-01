#ifndef JSONIFY_STRING_H
#define JSONIFY_STRING_H

typedef struct String {
  char *data;
  int len;
} String;

String *string_new();

String *string_newFrom(char *data);

void string_destroy(String *this);

#endif // JSONIFY_STRING_H
