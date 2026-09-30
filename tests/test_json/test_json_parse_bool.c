#include "../utils/utils.h"
#include <assert.h>
#include <parser.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>

void testJsonParseBool(char *input, bool expected) {
  Json *result = NULL;
  JsonParser *parser = jsonParser_new(input, strlen(input));
  err_t err = jsonParser_parseValue(parser, &result);
  TEST_ASSERT_EQUAL_INT(OK, err);
  int expectedConsumend = strlen(input);
  TEST_ASSERT_EQUAL_INT32(expectedConsumend, parser->cursor + 1);

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