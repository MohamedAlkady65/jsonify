#ifndef JSONIFY_JSON_OBJECT_H
#define JSONIFY_JSON_OBJECT_H

#include "json.h"

JsonObject *json_object_create();

void json_object_put(JsonObject *this, String key, Json *value);

Json *json_object_get(JsonObject *this, String key);

String *json_object_keys(JsonObject *this);

int json_object_len(JsonObject *this);

#endif // JSONIFY_JSON_OBJECT_H
