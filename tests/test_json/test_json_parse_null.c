#include "../utils/utils.h"
#include "json.h"
#include <assert.h>
#include <parser.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>

void testJsonParseNull(char *input) {
  Json *result = NULL;
  err_t err = json_parse(input, &result);
  TEST_ASSERT_EQUAL_INT(OK, err);

  Json exp = {J_NULL};
  testValue(&exp, result);
  json_destroy(result);
}

void test_json_parseNull_1() {
  char *input = "null";
  testJsonParseNull(input);
}

void test_json_parseNull_2() {
  char *input = "   null       ";
  testJsonParseNull(input);
}

void test_json_parseNull_group() {
  RUN_TEST(test_json_parseNull_1);
  RUN_TEST(test_json_parseNull_2);
}