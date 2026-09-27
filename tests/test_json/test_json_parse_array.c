#include "../utils/utils.h"
#include <assert.h>
#include <json.h>
#include <stb_ds.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>

void testJsonParseArray(char *input, JSON elements[], int n) {
  JSON *result = NULL;
  int inputLen = strlen(input), i = 0;
  parseValue(input, &i, inputLen - 1, &result);

  JSON_LIST list = {n, elements};
  JSON expected = {J_LIST, {.list = &list}};
  testValue(&expected, result);
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