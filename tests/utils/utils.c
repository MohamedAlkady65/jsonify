#include "string_builder.h"
#include <assert.h>
#include <json_list.h>
#include <json_object.h>
#include <parser.h>
#include <unity.h>

void testValue(Json *expected, Json *actual) {
  TEST_ASSERT_EQUAL_INT32(expected->type, actual->type);
  switch (expected->type) {
  case J_STRING:
    TEST_ASSERT_EQUAL_STRING(expected->value.string.data,
                             actual->value.string.data);
    break;
  case J_NUMBER:
    TEST_ASSERT_EQUAL_DOUBLE(expected->value.number, actual->value.number);
    break;
  case J_BOOL:
    TEST_ASSERT_EQUAL_INT8(expected->value.boolean, actual->value.boolean);
    break;
  case J_LIST:

    TEST_ASSERT_EQUAL_INT32(json_list_len(expected->value.list),
                            json_list_len(actual->value.list));

    for (int i = 0; i < json_list_len(expected->value.list); i++) {
      testValue(json_list_get(expected->value.list, i),
                json_list_get(actual->value.list, i));
    }
    break;
  case J_OBJECT:
    TEST_ASSERT_EQUAL_INT32(json_object_len(expected->value.object),
                            json_object_len(actual->value.object));

    String *keys = json_object_keys(expected->value.object);

    for (int i = 0; i < json_object_len(expected->value.object); i++) {

      testValue(json_object_get(expected->value.object, keys[i]),
                json_object_get(actual->value.object, keys[i]));
    }
    break;
  case J_NULL:
    break;
  default:
    assert(false);
    break;
  }
}