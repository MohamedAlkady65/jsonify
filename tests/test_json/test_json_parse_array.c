#include <unity.h>
#include <json.h>
#include <string.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stb_ds.h>

void testJsonParseArray(char *input, char *elements[], int n)
{
    JSON *result = NULL;
    int inputLen = strlen(input), i = 0;
    parseArray(input, &i, inputLen - 1, &result);
}

void test_json_parseArray_1()
{
    char *input = "[\"mohamed\"]";
    char *elements[] = {"mohamed"};
    testJsonParseArray(input, elements, 1);
}

void test_json_parseArray_2()
{
    char *input = "[\"mohamed\",\"ali\",\"1234\"  ,  \"\"   ]";
    char *elements[] = {"mohamed", "ali", "1234", ""};
    testJsonParseArray(input, elements, 4);
}

void test_json_parseArray_3()
{
    char *input = "[       \n   \n\n  \n\n       \"mohamed\"  \n   \n\n  \n\n ,    \n   \n\n  \n\n    \"ali\"  \n   \n\n  \n\n ,  \n   \n\n  \n\n \"1234\"   \n   \n\n  \n\n  ,    \n   \n\n  \n\n\"\"   \n   \n\n  \n\n  ]      \n   \n\n  \n\n   ";
    char *elements[] = {"mohamed", "ali", "1234", ""};
    testJsonParseArray(input, elements, 4);
}

void test_json_parseArray_4()
{
    TEST_IGNORE();

    char *input = "[]";
    char *elements[] = {};
    testJsonParseArray(input, elements, 0);
}

void test_json_parseArray_group()
{
    RUN_TEST(test_json_parseArray_1);
    RUN_TEST(test_json_parseArray_2);
    RUN_TEST(test_json_parseArray_3);
    RUN_TEST(test_json_parseArray_4);
}