#include "../utils/utils.h"
#include "json.h"
#include <assert.h>
#include <json_object.h>
#include <parser.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>
#include <json_string.h>

void testJsonParseObject(char *input, char *keys[], Json **values, int n) {
  Json *result = NULL;
  err_t err = json_parse(input, &result);
  TEST_ASSERT_EQUAL_INT(OK, err);

  JsonObject *object = json_object_new();

  Json *expected = json_new();
  *expected = (Json){J_OBJECT, {.object = object}};

  for (int i = 0; i < n; i++) {
    json_object_put(object, string_newFrom(keys[i]), values[i]);
  }

  testValue(expected, result);
  json_destroy(result);
  json_destroy(expected);
}

void test_json_parseObject_1() {
  char *input = "{\"name\":\"Mohamed\"}";
  char *keys[] = {"name"};
  Json *values[] = {json_string_newFromData("Mohamed")};
  testJsonParseObject(input, keys, values, 1);
}

void test_json_parseObject_2() {
  char *input = "{\"name\":\"Mohamed\",\"age\":\"20\"}";
  char *keys[] = {"name", "age"};
  Json *values[] = {json_string_newFromData("Mohamed"),
                    json_string_newFromData("20")};

  testJsonParseObject(input, keys, values, 2);
}

void test_json_parseObject_3() {
  char *input =
      "{    \n\n\n      \"name\"         \n\n  \n    :   \n\n\n       "
      "\"Khaled\"  \n\n   , \"\":\"empty\"   ,       \"age\"    \n\n  \n    :  "
      "   \n\n  \n   \"\"           }                         ";
  char *keys[] = {"name", "", "age"};
  Json *values[] = {json_string_newFromData("Khaled"),
                    json_string_newFromData("empty"),
                    json_string_newFromData("")};

  testJsonParseObject(input, keys, values, 3);
}

void test_json_parseObject_4() {
  char *input = "{\n  \n  \n  \n  \"name\":\n  \n      \n   \n  \"Ali124\"\n   "
                "  \n  ,\n  \n  \"age\"\n  \n      :\n      \n       \"324\" "
                "\n    \n  ,\n  \n  \n  \"country\"    : \"EGY       PT\"\n}";
  char *keys[] = {"name", "age", "country"};
  Json *values[] = {json_string_newFromData("Ali124"),
                    json_string_newFromData("324"),
                    json_string_newFromData("EGY       PT")};

  testJsonParseObject(input, keys, values, 3);
}

void test_json_parseObject_5() {
  char *input = "{}";
  char **keys = {};
  Json **values = {};
  testJsonParseObject(input, keys, values, 0);
}

void test_json_parseObject_group() {
  RUN_TEST(test_json_parseObject_1);
  RUN_TEST(test_json_parseObject_2);
  RUN_TEST(test_json_parseObject_3);
  RUN_TEST(test_json_parseObject_4);
  RUN_TEST(test_json_parseObject_5);
}