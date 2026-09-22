#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#include <assert.h>
#include "string_builder.h"
#include "json.h"
#include "utils.h"

void parseString(char *s, int *i, int j, JSON **result)
{
    String str;
    parseStringHelper(s, i, j, &str);

    *result = malloc(sizeof(JSON));
    (*result)->type = J_STRING;
    (*result)->value.string = str;
}

void parseNumber(char *s, int *i, int j, JSON **result)
{

    double res = 0;
    bool negative = false;
    bool hasFraction = false;
    long long fractionFactor = 10;

    if (s[*i] == '-')
    {
        negative = true;
        (*i)++;
    }

    if (*i > j)
    {
        assert(false);
    }

    for (; (*i) <= j; (*i)++)
    {

        if (s[*i] == '.')
        {
            if (hasFraction)
            {
                assert(false);
            }

            hasFraction = true;

            continue;
        }

        if (s[*i] >= '0' && s[*i] <= '9')
        {
            if (hasFraction)
            {
                res = res + ((double)(s[*i] - '0') / fractionFactor);
                fractionFactor *= 10;
            }
            else
            {

                res = (res * 10) + (s[*i] - '0');
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

    *result = malloc(sizeof(JSON));
    (*result)->type = J_NUMBER;
    (*result)->value.number = res;
    (*i)--;
}

void parseBool(char *s, int *i, int j, JSON **result)
{
    char *words[] = {"true", "false"};
    int wordsLen[] = {4, 5};
    bool wordsValue[] = {true, false};

    for (int w = 0; w < 2; w++)
    {
        for (; (*i) <= j && (*i) < wordsLen[w]; (*i)++)
        {
            if (s[*i] != words[w][*i])
            {
                break;
            }

            if (*i == wordsLen[w] - 1)
            {
                *result = malloc(sizeof(JSON));
                (*result)->type = J_BOOL;
                (*result)->value.boolean = wordsValue[w];
                return;
            }
        }
    }

    assert(false);
}

void parseObject(char *s, int *i, int j, JSON **result)
{

    JSON_OBJECT *res = NULL;

    assert(s[*i] == '{');
    (*i)++;
    while ((*i) <= j)
    {
        processPair(s, i, j, &res);
        if (s[*i] != ',')
            break;
        (*i)++;
    }

    if ((*i) > j || s[*i] != '}')
    {
        assert(false);
    }

    (*i)++;

    *result = malloc(sizeof(JSON));
    (*result)->type = J_OBJECT;
    (*result)->value.object = res;
}

void parseArray(char *s, int *i, int j, JSON **result)
{
    JSON *res = NULL;

    assert(s[*i] == '[');
    (*i)++;
    while ((*i) <= j)
    {
        processArrayElement(s, i, j, &res);
        if (s[*i] != ',')
            break;
        (*i)++;
    }

    if ((*i) > j || s[*i] != ']')
    {
        assert(false);
    }

    (*i)++;

    *result = malloc(sizeof(JSON));
    (*result)->type = J_LIST;
    (*result)->value.list = &res;
}

void parseValue(char *s, int *i, int j, JSON **result)
{

    skipWs(s, i, j);

    JSON_VALUE_TYPE type = valueType(s[*i]);

    switch (type)
    {
    case J_LIST:
        parseArray(s, i, j, result);
        break;
    case J_OBJECT:
        parseObject(s, i, j, result);
        break;
    case J_STRING:
        parseString(s, i, j, result);
        break;
    case J_NUMBER:
        parseNumber(s, i, j, result);
        break;
    case J_BOOL:
        parseBool(s, i, j, result);
        break;
    default:
        assert(false);
        break;
    }
}