#include <assert.h>
#include <json.h>
#include <stb_ds.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unity.h>

void testJsonParseObject(char *input, char *keys[], JSON *values, int n) {
  JSON *result = NULL;
  int inputLen = strlen(input), i = 0;
  parseObject(input, &i, inputLen - 1, &result);

  TEST_ASSERT_EQUAL_INT32(J_OBJECT, result->type);

  JSON_OBJECT *object = result->value.object;

  TEST_ASSERT_EQUAL_INT32(n, result->value.object->length);

  for (int i = 0; i < n; i++) {

    JSON *actual = shget(object->elements, keys[i]);
    JSON expected = values[i];

    TEST_ASSERT_EQUAL_INT32(expected.type, actual->type);

    switch (expected.type) {
    case J_STRING:
      TEST_ASSERT_EQUAL_STRING(expected.value.string.data,
                               actual->value.string.data);
      break;
    case J_NUMBER:
      TEST_ASSERT_EQUAL_DOUBLE(expected.value.number, actual->value.number);
      break;
    case J_BOOL:
      TEST_ASSERT_EQUAL_INT8(expected.value.boolean, actual->value.boolean);
      break;
    default:
      assert(false);
      break;
    }
  }
}

void test_json_parseObject_1() {
  char *input = "{\"name\":\"Mohamed\"}";
  char *keys[] = {"name"};
  JSON values[] = {{.type = J_STRING, .value.string = "Mohamed"}};
  testJsonParseObject(input, keys, values, 1);
}

void test_json_parseObject_2() {
  char *input = "{\"name\":\"Mohamed\",\"age\":\"20\"}";
  char *keys[] = {"name", "age"};
  JSON values[] = {{.type = J_STRING, .value.string = {"Mohamed"}},
                   {.type = J_STRING, .value.string = {"20"}}};
  testJsonParseObject(input, keys, values, 2);
}

void test_json_parseObject_3() {
  char *input =
      "{    \n\n\n      \"name\"         \n\n  \n    :   \n\n\n       "
      "\"Khaled\"  \n\n   , \"\":\"empty\"   ,       \"age\"    \n\n  \n    :  "
      "   \n\n  \n   \"\"           }                         ";
  char *keys[] = {"name", "", "age"};
  JSON values[] = {{.type = J_STRING, .value.string = {"Khaled"}},
                   {.type = J_STRING, .value.string = {"empty"}},
                   {.type = J_STRING, .value.string = {""}}};
  testJsonParseObject(input, keys, values, 3);
}

void test_json_parseObject_4() {
  char *input = "{\n  \n  \n  \n  \"name\":\n  \n      \n   \n  \"Ali124\"\n   "
                "  \n  ,\n  \n  \"age\"\n  \n      :\n      \n       \"324\" "
                "\n    \n  ,\n  \n  \n  \"country\"    : \"EGY       PT\"\n}";
  char *keys[] = {"name", "age", "country"};
  JSON values[] = {{.type = J_STRING, .value.string = {"Ali124"}},
                   {.type = J_STRING, .value.string = {"324"}},
                   {.type = J_STRING, .value.string = {"EGY       PT"}}};
  testJsonParseObject(input, keys, values, 3);
}

void test_json_parseObject_5() {
  char *input = "{}";
  char **keys = {};
  JSON *values = {};
  testJsonParseObject(input, keys, values, 0);
}

void test_json_parseObject_group() {
  RUN_TEST(test_json_parseObject_1);
  RUN_TEST(test_json_parseObject_2);
  RUN_TEST(test_json_parseObject_3);
  RUN_TEST(test_json_parseObject_4);
  RUN_TEST(test_json_parseObject_5);
}