#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#include <assert.h>
#include "string_builder.h"
#include "json.h"

void skipWs(char *s, int *i, int j)
{
    while ((*i) <= j && (s[*i] == ' ' || s[*i] == '\n'))
    {
        (*i)++;
    }
    if (*i > j)
    {
        assert(false);
    }
}

JSON_VALUE_TYPE valueType(char ch)
{
    if (ch == '"')
    {
        return J_STRING;
    }
    else if (ch == '{')
    {
        return J_OBJECT;
    }
    else if (ch == '[')
    {
        return J_LIST;
    }
    else if (ch == 't' || ch == 'f')
    {
        return J_BOOL;
    }
    else if (ch == '-' || (ch >= '0' && ch <= '9'))
    {
        return J_NUMBER;
    }
    else if (ch == 'n')
    {
        return J_NULL;
    }
    assert(false);
}

void parseStringHelper(char *s, int i, int j, int *consumed, String *result)
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

void processArrayElement(char *s, int i, int j, int *totalConsumed, JSON **list)
{
    char *data;
    int consumed, keyLen;
    int start = i;

    JSON *value;
    parseValue(s, i, j, &consumed, &value);
    i += consumed;

    arrput(*list, *value);

    *totalConsumed = i - start;
}

void processPair(char *s, int i, int j, int *totalConsumed, JSON_OBJECT **object)
{
    char *data;
    int consumed, keyLen;
    int start = i;

    skipWs(s, &i, j);

    String key;
    parseStringHelper(s, i, j, &consumed, &key);
    i += consumed;

    skipWs(s, &i, j);

    assert(s[i] == ':');
    i++;

    JSON *value;
    parseValue(s, i, j, &consumed, &value);
    i += consumed;

    shputs(*object, ((JSON_OBJECT){.key = key.data, .keyLen = key.len, .value = value}));

    *totalConsumed = i - start;
}