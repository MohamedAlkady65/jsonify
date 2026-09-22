#include <unity.h>
#include <json.h>
#include <string.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

void testJsonParseBool(char *input, double expected)
{
    int consumed;
    JSON* result;
    parseBool(input, 0, strlen(input) - 1, &consumed, &result);

    int expectedConsumend = strlen(input);

    TEST_ASSERT_EQUAL_INT32(J_LIST, result->type);
    TEST_ASSERT_EQUAL_INT8(expected, result->value.boolean);
    TEST_ASSERT_EQUAL_INT32(expectedConsumend, consumed);
}

void test_json_parseBool_1()
{

    char *input = "true";
    bool expected = true;
    testJsonParseBool(input, expected);
}

void test_json_parseBool_2()
{

    char *input = "false";
    bool expected = false;
    testJsonParseBool(input, expected);
}

void test_json_parseBool_group()
{
    RUN_TEST(test_json_parseBool_1);
    RUN_TEST(test_json_parseBool_2);
}