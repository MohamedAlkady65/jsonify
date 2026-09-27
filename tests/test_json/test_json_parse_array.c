#include <assert.h>
#include <json.h>
#include <stb_ds.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unity.h>

void testJsonParseArray(char *input, JSON elements[], int n) {
  JSON *result = NULL;
  int inputLen = strlen(input), i = 0;
  parseArray(input, &i, inputLen - 1, &result);

  TEST_ASSERT_EQUAL_INT32(J_LIST, result->type);

  JSON_LIST *list = result->value.list;

  TEST_ASSERT_EQUAL_INT32(n, result->value.list->length);

  for (int i = 0; i < n; i++) {

    JSON actual = list->elements[i];
    JSON expected = elements[i];

    TEST_ASSERT_EQUAL_INT32(expected.type, actual.type);

    switch (expected.type) {
    case J_STRING:
      TEST_ASSERT_EQUAL_STRING(expected.value.string.data,
                               actual.value.string.data);
      break;
    case J_NUMBER:
      TEST_ASSERT_EQUAL_DOUBLE(expected.value.number, actual.value.number);
      break;
    case J_BOOL:
      TEST_ASSERT_EQUAL_INT8(expected.value.boolean, actual.value.boolean);
      break;
    default:
      assert(false);
      break;
    }
  }
}

void test_json_parseArray_1() {
  char *input = "[\"mohamed\"]";
  JSON elements[] = {{.type = J_STRING, .value.string = {"mohamed"}}};
  testJsonParseArray(input, elements, 1);
}

void test_json_parseArray_2() {
  char *input = "[\"mohamed\",\"ali\",\"1234\"  ,  \"\"   ]";
  JSON elements[] = {
      {.type = J_STRING, .value.string = {"mohamed"}},
      {.type = J_STRING, .value.string = {"ali"}},
      {.type = J_STRING, .value.string = {"1234"}},
      {.type = J_STRING, .value.string = {""}},
  };
  testJsonParseArray(input, elements, 4);
}

void test_json_parseArray_3() {
  char *input = "[       \n   \n\n  \n\n       \"mohamed\"  \n   \n\n  \n\n ,  "
                "  \n   \n\n  \n\n    \"ali\"  \n   \n\n  \n\n ,  \n   \n\n  "
                "\n\n \"1234\"   \n   \n\n  \n\n  ,    \n   \n\n  \n\n\"\"   "
                "\n   \n\n  \n\n  ]      \n   \n\n  \n\n   ";
  JSON elements[] = {
      {.type = J_STRING, .value.string = {"mohamed"}},
      {.type = J_STRING, .value.string = {"ali"}},
      {.type = J_STRING, .value.string = {"1234"}},
      {.type = J_STRING, .value.string = {""}},
  };
  testJsonParseArray(input, elements, 4);
}

void test_json_parseArray_4() {
  char *input = "[]";
  JSON *elements = {};
  testJsonParseArray(input, elements, 0);
}

void test_json_parseArray_group() {
  RUN_TEST(test_json_parseArray_1);
  RUN_TEST(test_json_parseArray_2);
  RUN_TEST(test_json_parseArray_3);
  RUN_TEST(test_json_parseArray_4);
}