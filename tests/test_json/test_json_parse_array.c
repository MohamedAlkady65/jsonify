#include "../utils/utils.h"
#include "error.h"
#include "json.h"
#include <assert.h>
#include <json_list.h>
#include <parser.h>
#include <stdbool.h>


void testJsonParseArray(char *input, Json *elements[], int n) {
  Json *result = NULL;
  err_t err = json_parse(input, &result);
  TEST_ASSERT_EQUAL_INT(OK, err);

  JsonList *expectedList = json_list_new();

  for (int i = 0; i < n; i++) {
    json_list_put(expectedList, elements[i]);
  }

  Json *expected = json_new();
  *expected = (Json){J_LIST, {.list = expectedList}};

  testValue(expected, result);
  json_destroy(result);
  json_destroy(expected);
}

void test_json_parseArray_1() {
  char *input = "[\"mohamed\"]";
  Json* elements[] = {json_newStringFromData("mohamed")};
  testJsonParseArray(input, elements, 1);
}

void test_json_parseArray_2() {
  char *input = "[\"mohamed\",\"ali\",\"1234\"  ,  \"\"   ]";

  Json *elements[] = {
      json_newStringFromData("mohamed"),
      json_newStringFromData("ali"),
      json_newStringFromData("1234"),
      json_newStringFromData(""),
  };

  testJsonParseArray(input, elements, 4);
}

void test_json_parseArray_3() {
  char *input = "[       \n   \n\n  \n\n       \"mohamed\"  \n   \n\n  \n\n ,  "
                "  \n   \n\n  \n\n    \"ali\"  \n   \n\n  \n\n ,  \n   \n\n  "
                "\n\n \"1234\"   \n   \n\n  \n\n  ,    \n   \n\n  \n\n\"\"   "
                "\n   \n\n  \n\n  ]      \n   \n\n  \n\n   ";
  Json *elements[] = {
      json_newStringFromData("mohamed"),
      json_newStringFromData("ali"),
      json_newStringFromData("1234"),
      json_newStringFromData(""),
  };
  testJsonParseArray(input, elements, 4);
}

void test_json_parseArray_4() {
  char *input = "[]";
  Json **elements = {};
  testJsonParseArray(input, elements, 0);
}

void test_json_parseArray_group() {
  RUN_TEST(test_json_parseArray_1);
  RUN_TEST(test_json_parseArray_2);
  RUN_TEST(test_json_parseArray_3);
  RUN_TEST(test_json_parseArray_4);
}