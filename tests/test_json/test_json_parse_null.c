#include "../utils/utils.h"
#include "json.h"
#include <assert.h>
#include <parser.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>

void testJsonParseNull(char *input, int expectedConsumed) {
  Json *result = NULL;
  JsonParser *parser = jsonParser_new(input, strlen(input));
  err_t err = jsonParser_parseValue(parser, &result);
  TEST_ASSERT_EQUAL_INT(OK, err);

  expectedConsumed = expectedConsumed == -1 ? strlen(input) : expectedConsumed;

  TEST_ASSERT_EQUAL_INT32(expectedConsumed, parser->cursor + 1);

  Json exp = {J_NULL};
  testValue(&exp, result);
}

void test_json_parseNull_1() {
  char *input = "null";
  testJsonParseNull(input, -1);
}

void test_json_parseNull_2() {
  char *input = "   null       ";
  testJsonParseNull(input, 7);
}

void test_json_parseNull_group() {
  RUN_TEST(test_json_parseNull_1);
  RUN_TEST(test_json_parseNull_2);
}