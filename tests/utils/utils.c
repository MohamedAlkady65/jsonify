#include <assert.h>
#include <json.h>
#include <stb_ds.h>
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
    TEST_ASSERT_EQUAL_INT32(expected->value.list->length,
                            actual->value.list->length);
    for (int i = 0; i < actual->value.list->length; i++) {
      testValue(expected->value.list->elements + i,
                actual->value.list->elements + i);
    }
    break;
  case J_OBJECT:
    TEST_ASSERT_EQUAL_INT32(expected->value.object->length,
                            actual->value.object->length);

    for (int i = 0; i < actual->value.object->length; i++) {
      testValue(expected->value.object->elements[i].value,
                shget(actual->value.object->elements,
                      expected->value.object->elements[i].key));
    }
    break;
  default:
    assert(false);
    break;
  }
}