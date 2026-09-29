#include "../utils/utils.h"
#include "string_builder.h"
#include <assert.h>
#include <json_object.h>
#include <parser.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>

void testJsonParseObject(char *input, char *keys[], Json *values, int n) {
  Json *result = NULL;
  JsonParser *parser = jsonParser_create(input, strlen(input));
  err_t err = jsonParser_parseValue(parser, &result);
  TEST_ASSERT_EQUAL_INT(OK, err);

  JsonObject *object = json_object_create();
  Json expected = {J_OBJECT, {.object = object}};

  for (int i = 0; i < n; i++) {
    json_object_put(object, (String){keys[i], strlen(keys[i])}, values + i);
  }

  testValue(&expected, result);
}

void test_json_parseObject_1() {
  char *input = "{\"name\":\"Mohamed\"}";
  char *keys[] = {"name"};
  Json values[] = {{.type = J_STRING, .value.string = "Mohamed"}};
  testJsonParseObject(input, keys, values, 1);
}

void test_json_parseObject_2() {
  char *input = "{\"name\":\"Mohamed\",\"age\":\"20\"}";
  char *keys[] = {"name", "age"};
  Json values[] = {{.type = J_STRING, .value.string = {"Mohamed"}},
                   {.type = J_STRING, .value.string = {"20"}}};
  testJsonParseObject(input, keys, values, 2);
}

void test_json_parseObject_3() {
  char *input =
      "{    \n\n\n      \"name\"         \n\n  \n    :   \n\n\n       "
      "\"Khaled\"  \n\n   , \"\":\"empty\"   ,       \"age\"    \n\n  \n    :  "
      "   \n\n  \n   \"\"           }                         ";
  char *keys[] = {"name", "", "age"};
  Json values[] = {{.type = J_STRING, .value.string = {"Khaled"}},
                   {.type = J_STRING, .value.string = {"empty"}},
                   {.type = J_STRING, .value.string = {""}}};
  testJsonParseObject(input, keys, values, 3);
}

void test_json_parseObject_4() {
  char *input = "{\n  \n  \n  \n  \"name\":\n  \n      \n   \n  \"Ali124\"\n   "
                "  \n  ,\n  \n  \"age\"\n  \n      :\n      \n       \"324\" "
                "\n    \n  ,\n  \n  \n  \"country\"    : \"EGY       PT\"\n}";
  char *keys[] = {"name", "age", "country"};
  Json values[] = {{.type = J_STRING, .value.string = {"Ali124"}},
                   {.type = J_STRING, .value.string = {"324"}},
                   {.type = J_STRING, .value.string = {"EGY       PT"}}};
  testJsonParseObject(input, keys, values, 3);
}

void test_json_parseObject_5() {
  char *input = "{}";
  char **keys = {};
  Json *values = {};
  testJsonParseObject(input, keys, values, 0);
}

void test_json_parseObject_group() {
  RUN_TEST(test_json_parseObject_1);
  RUN_TEST(test_json_parseObject_2);
  RUN_TEST(test_json_parseObject_3);
  RUN_TEST(test_json_parseObject_4);
  RUN_TEST(test_json_parseObject_5);
}