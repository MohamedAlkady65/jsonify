#include "../utils/utils.h"
#include <assert.h>
#include <json.h>
#include <stb_ds.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>

void testJsonParseObject(char *input, char *keys[], Json *values, int n) {
  Json *result = NULL;
  int inputLen = strlen(input), i = 0;
  parseValue(input, &i, inputLen - 1, &result);

  JsonObject object = {0, NULL};
  Json expected = {J_OBJECT, {.object = &object}};

  for (int i = 0; i < n; i++) {
    shputs(object.elements, ((JsonObjectPair){.key = keys[i],
                                              .keyLen = strlen(keys[i]),
                                              .value = values + i}));
    object.length++;
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