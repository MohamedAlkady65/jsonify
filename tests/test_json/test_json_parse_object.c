#include <unity.h>
#include <json.h>
#include <string.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stb_ds.h>

void testJsonParseObject(char *input, char *keys[], char *values[], int n)
{
    JSON_OBJECT *result = NULL;
    int inputLen = strlen(input), consumed = 0;
    parseObject(input, 0, inputLen - 1, &consumed, &result);

    for (int i = 0; i < n; i++)
    {
        TEST_ASSERT_EQUAL_STRING(values[i], shget(result, keys[i])->value.string.data);
    }
}

void test_json_parseObject_1()
{
    char *input = "{\"name\":\"Mohamed\"}";
    char *keys[] = {"name"};
    char *values[] = {"Mohamed"};
    testJsonParseObject(input, keys, values, 1);
}

void test_json_parseObject_2()
{
    char *input = "{\"name\":\"Mohamed\",\"age\":\"20\"}";
    char *keys[] = {"name", "age"};
    char *values[] = {"Mohamed", "20"};
    testJsonParseObject(input, keys, values, 2);
}

void test_json_parseObject_3()
{
    char *input = "{    \n\n\n      \"name\"         \n\n  \n    :   \n\n\n       \"Khaled\"  \n\n   , \"\":\"empty\"   ,       \"age\"    \n\n  \n    :     \n\n  \n   \"\"           }                         ";
    char *keys[] = {"name", "age", ""};
    char *values[] = {"Khaled", "", "empty"};
    testJsonParseObject(input, keys, values, 3);
}

void test_json_parseObject_4()
{
    char *input = "{\n  \n  \n  \n  \"name\":\n  \n      \n   \n  \"Ali124\"\n     \n  ,\n  \n  \"age\"\n  \n      :\n      \n       \"324\" \n    \n  ,\n  \n  \n  \"country\"    : \"EGY       PT\"\n}";
    char *keys[] = {"name", "age", "country"};
    char *values[] = {"Ali124", "324", "EGY       PT"};
    testJsonParseObject(input, keys, values, 3);
}

void test_json_parseObject_5()
{
    TEST_IGNORE();

    char *input = "{}";
    char *keys[] = {};
    char *values[] = {};
    testJsonParseObject(input, keys, values, 0);
}

void test_json_parseObject_group()
{
    RUN_TEST(test_json_parseObject_1);
    RUN_TEST(test_json_parseObject_2);
    RUN_TEST(test_json_parseObject_3);
    RUN_TEST(test_json_parseObject_4);
    RUN_TEST(test_json_parseObject_5);
}