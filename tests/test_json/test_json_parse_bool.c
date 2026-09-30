#include "../utils/utils.h"
#include <assert.h>
#include <parser.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>

void testJsonParseBool(char *input, bool expected) {
  Json *result = NULL;
  err_t err = json_parse(input, &result);
  TEST_ASSERT_EQUAL_INT(OK, err);

  Json exp = {J_BOOL, {.boolean = expected}};
  testValue(&exp, result);
  json_destroy(result);
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