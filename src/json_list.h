#ifndef JSONIFY_JSON_LIST_H
#define JSONIFY_JSON_LIST_H

#include "json.h"

JsonList *json_list_new();

void json_list_destroy(JsonList *this);

void json_list_put(JsonList *this, Json *value);

Json *json_list_get(JsonList *this, int i);

int json_list_len(JsonList *this);

#endif // JSONIFY_JSON_LIST_H
