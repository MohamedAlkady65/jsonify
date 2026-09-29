#include "../utils/utils.h"
#include "error.h"
#include "json.h"
#include <assert.h>
#include <json_list.h>
#include <parser.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>

void testJsonParseArray(char *input, Json elements[], int n) {
  Json *result = NULL;
  JsonParser *parser = jsonParser_create(input, strlen(input));
  err_t err = jsonParser_parseValue(parser, &result);
  TEST_ASSERT_EQUAL_INT(OK, err);

  JsonList *expectedList = json_list_create();

  for (int i = 0; i < n; i++) {
    json_list_put(expectedList, elements + i);
  }

  Json expected = {J_LIST, {.list = expectedList}};

  testValue(&expected, result);
}

void test_json_parseArray_1() {
  char *input = "[\"mohamed\"]";
  Json elements[] = {{.type = J_STRING, .value.string = {"mohamed"}}};
  testJsonParseArray(input, elements, 1);
}

void test_json_parseArray_2() {
  char *input = "[\"mohamed\",\"ali\",\"1234\"  ,  \"\"   ]";
  Json elements[] = {
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
  Json elements[] = {
      {.type = J_STRING, .value.string = {"mohamed"}},
      {.type = J_STRING, .value.string = {"ali"}},
      {.type = J_STRING, .value.string = {"1234"}},
      {.type = J_STRING, .value.string = {""}},
  };
  testJsonParseArray(input, elements, 4);
}

void test_json_parseArray_4() {
  char *input = "[]";
  Json *elements = {};
  testJsonParseArray(input, elements, 0);
}

void test_json_parseArray_group() {
  RUN_TEST(test_json_parseArray_1);
  RUN_TEST(test_json_parseArray_2);
  RUN_TEST(test_json_parseArray_3);
  RUN_TEST(test_json_parseArray_4);
}