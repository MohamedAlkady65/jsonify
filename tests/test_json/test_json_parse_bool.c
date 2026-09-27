#include "../utils/utils.h"
#include <assert.h>
#include <json.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>

void testJsonParseBool(char *input, bool expected) {
  int i = 0;
  Json *result;
  parseValue(input, &i, strlen(input) - 1, &result);

  int expectedConsumend = strlen(input);
  TEST_ASSERT_EQUAL_INT32(expectedConsumend, i + 1);

  Json exp = {J_BOOL, {.boolean = expected}};
  testValue(&exp, result);
}

void test_json_parseBool_1() {

  char *input = "true";
  bool expected = true;
  testJsonParseBool(input, expected);
}

void test_json_parseBool_2() {

  char *input = "false";
  bool expected = false;
  testJsonParseBool(input, expected);
}

void test_json_parseBool_group() {
  RUN_TEST(test_json_parseBool_1);
  RUN_TEST(test_json_parseBool_2);
}