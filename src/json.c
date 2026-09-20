#include <stdbool.h>
#define STB_DS_IMPLEMENTATION // in exactly ONE .c file
#include <stb_ds.h>
#include <assert.h>
#include "string_builder.h"
#include "json.h"

void parseString(char *s, int i, int j, int *consumed, String *result)
{
    StringBuilder builder = stringBuilder_new();

    int start = i;

    assert(s[i] == '"');

    bool escaping = false;

    i++;

    for (; i <= j; i++)
    {
        if (!escaping && s[i] == '"')
        {
            String builded = stringBuilder_build(&builder);
            *result = builded;

            *consumed = i - start + 1;
            return;
        }

        if (!escaping && s[i] == '\\')
        {
            escaping = true;
            continue;
        }

        if (escaping)
        {

            if (s[i] == '"' || s[i] == '\\')
            {
                stringBuilder_append(&builder, s + i, 1);
            }
            else if (s[i] == 'n')
            {
                char newLine = '\n';
                stringBuilder_append(&builder, (&newLine), 1);
            }
            else
            {
                assert(false);
            }

            escaping = false;
            continue;
        }

        stringBuilder_append(&builder, s + i, 1);
    }
    assert(false);
}

void parseNumber(char *s, int i, int j, int *consumed, double *result)
{

    double res = 0;
    bool negative = false;
    bool hasFraction = false;
    long long fractionFactor = 10;

    int start = i;

    if (s[i] == '-')
    {
        negative = true;
        i++;
    }

    if (i > j)
    {
        assert(false);
    }

    for (; i <= j; i++)
    {

        if (s[i] == '.')
        {
            if (hasFraction)
            {
                assert(false);
            }

            hasFraction = true;

            continue;
        }

        if (s[i] >= '0' && s[i] <= '9')
        {
            if (hasFraction)
            {
                res = res + ((double)(s[i] - '0') / fractionFactor);
                fractionFactor *= 10;
            }
            else
            {

                res = (res * 10) + (s[i] - '0');
            }

            continue;
        }

        break;
    }

    if (hasFraction && fractionFactor == 10)
    {
        assert(false);
    }

    if (negative)
    {
        res *= -1;
    }

    *result = res;
    *consumed = i - start;
}

void parseBool(char *s, int i, int j, int *consumed, bool *result)
{
    char *words[] = {"true", "false"};
    int wordsLen[] = {4, 5};
    bool wordsValue[] = {true, false};
    int start = i;

    for (int w = 0; w < 2; w++)
    {
        for (; i <= j && i < wordsLen[w]; i++)
        {
            if (s[i] != words[w][i])
            {
                break;
            }

            if (i == wordsLen[w] - 1)
            {
                *result = wordsValue[w];
                *consumed = i - start + 1;
                return;
            }
        }
    }

    assert(false);
}

int skipWs(char *s, int i, int j)
{
    while (i <= j && (s[i] == ' ' || s[i] == '\n'))
    {
        i++;
    }
    if (i > j)
    {
        assert(false);
    }
    return i;
}

void processPair(char *s, int i, int j, int *totalConsumed, JSON_OBJECT **object)
{
    char *data;
    int consumed, keyLen;
    int start = i;

    String key;
    parseString(s, i, j, &consumed, &key);
    i += consumed;

    i = skipWs(s, i, j);

    assert(s[i] == ':');
    i++;

    i = skipWs(s, i, j);

    String value;
    parseString(s, i, j, &consumed, &value);
    i += consumed;

    JSON *valueObj = malloc(sizeof(JSON));
    valueObj->type = J_STRING;
    valueObj->value.string = value;

    shputs(*object, ((JSON_OBJECT){.key = key.data, .keyLen = key.len, .value = valueObj}));

    *totalConsumed = i - start;
}

void parseObject(char *s, int i, int j, int *consumed, JSON_OBJECT **result)
{

    JSON_OBJECT *res = NULL;

    assert(s[i] == '{');
    i++;
    while (i <= j)
    {
        i = skipWs(s, i, j);
        int consumed;
        processPair(s, i, j, &consumed, &res);
        i += consumed;
        i = skipWs(s, i, j);
        if (s[i] != ',')
            break;
        i++;
    }

    if (i > j || s[i] != '}')
    {
        assert(false);
    }

    i++;
    *result = res;
    consumed += j - i + 1;
}

void processArrayElement(char *s, int i, int j, int *totalConsumed, JSON **list)
{
    char *data;
    int consumed, keyLen;
    int start = i;

    String value;
    parseString(s, i, j, &consumed, &value);
    i += consumed;

    JSON *valueObj = malloc(sizeof(JSON));
    valueObj->type = J_STRING;
    valueObj->value.string = value;

    arrput(*list, *valueObj);

    *totalConsumed = i - start;
}

void parseArray(char *s, int i, int j, int *consumed, JSON **result)
{
    JSON *res = NULL;

    assert(s[i] == '[');
    i++;
    while (i <= j)
    {
        i = skipWs(s, i, j);
        int consumed;
        processArrayElement(s, i, j, &consumed, &res);
        i += consumed;
        i = skipWs(s, i, j);
        if (s[i] != ',')
            break;
        i++;
    }

    if (i > j || s[i] != ']')
    {
        assert(false);
    }

    i++;
    *result = res;
    consumed += j - i + 1;
}

JSON *parse(char *text)
{
}