#include "../utils/utils.h"
#include "unity.h"
#include <json.h>
#include <string.h>

void testJsonParseNumber(char *input, double expected, int expectedConsumed) {
  int i = 0;
  JSON *result;
  parseValue(input, &i, strlen(input) - 1, &result);

  int expectedConsumend =
      expectedConsumed == -1 ? strlen(input) : expectedConsumed;

  TEST_ASSERT_EQUAL_INT32(expectedConsumend, i + 1);

  JSON exp = {J_NUMBER, {.number = expected}};
  testValue(&exp, result);
}

void test_json_parseNumber_1() {

  char *input = "5";
  double expected = 5;
  testJsonParseNumber(input, expected, -1);
}

void test_json_parseNumber_2() {

  char *input = "-7";
  double expected = -7;
  testJsonParseNumber(input, expected, -1);
}

void test_json_parseNumber_3() {
  char *input = "0";
  double expected = 0;
  testJsonParseNumber(input, expected, -1);
}

void test_json_parseNumber_4() {
  char *input = "3472989";
  double expected = 3472989;
  testJsonParseNumber(input, expected, -1);
}

void test_json_parseNumber_5() {
  char *input = "-987423";
  double expected = -987423;
  testJsonParseNumber(input, expected, -1);
}

void test_json_parseNumber_6() {
  char *input = "3.6";
  double expected = 3.6;
  testJsonParseNumber(input, expected, -1);
}

void test_json_parseNumber_7() {
  char *input = "8743.7264";
  double expected = 8743.7264;
  testJsonParseNumber(input, expected, -1);
}

void test_json_parseNumber_8() {
  char *input = "0.345";
  double expected = 0.345;
  testJsonParseNumber(input, expected, -1);
}

void test_json_parseNumber_9() {
  char *input = "-4.5";
  double expected = -4.5;
  testJsonParseNumber(input, expected, -1);
}

void test_json_parseNumber_10() {
  char *input = "-0.123";
  double expected = -0.123;
  testJsonParseNumber(input, expected, -1);
}

void test_json_parseNumber_11() {
  char *input = "-3489.983742";
  double expected = -3489.983742;
  testJsonParseNumber(input, expected, -1);
}

void test_json_parseNumber_12() {
  char *input = "7   ";
  double expected = 7;
  testJsonParseNumber(input, expected, 1);
}
void test_json_parseNumber_13() {
  char *input = "-974   ";
  double expected = -974;
  testJsonParseNumber(input, expected, 4);
}

void test_json_parseNumber_14() {
  char *input = "8.94   ";
  double expected = 8.94;
  testJsonParseNumber(input, expected, 4);
}
void test_json_parseNumber_15() {
  char *input = "-23.5085   ";
  double expected = -23.5085;
  testJsonParseNumber(input, expected, 8);
}

void test_json_parseNumber_group() {
  RUN_TEST(test_json_parseNumber_1);
  RUN_TEST(test_json_parseNumber_2);
  RUN_TEST(test_json_parseNumber_3);
  RUN_TEST(test_json_parseNumber_4);
  RUN_TEST(test_json_parseNumber_5);
  RUN_TEST(test_json_parseNumber_6);
  RUN_TEST(test_json_parseNumber_7);
  RUN_TEST(test_json_parseNumber_8);
  RUN_TEST(test_json_parseNumber_9);
  RUN_TEST(test_json_parseNumber_10);
  RUN_TEST(test_json_parseNumber_11);
  RUN_TEST(test_json_parseNumber_12);
  RUN_TEST(test_json_parseNumber_13);
  RUN_TEST(test_json_parseNumber_14);
  RUN_TEST(test_json_parseNumber_15);
}