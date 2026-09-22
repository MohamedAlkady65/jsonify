#include <stdbool.h>
#include "string_builder.h"

typedef enum JSON_VALUE_TYPE
{
    J_STRING,
    J_NUMBER,
    J_BOOL,
    J_LIST,
    J_OBJECT,
    J_NULL,
} JSON_VALUE_TYPE;

typedef struct JSON_OBJECT
{
    char *key;
    int keyLen;
    struct JSON *value;
} JSON_OBJECT;

typedef union JSON_VALUE
{
    String string;
    double number;
    bool boolean;
    struct JSON **list;
    struct JSON_OBJECT *object;
} JSON_VALUE;

typedef struct JSON
{
    enum JSON_VALUE_TYPE type;
    union JSON_VALUE value;
} JSON;

void parseString(char *s, int i, int j, int *consumed, JSON **result);

void parseNumber(char *s, int i, int j, int *consumed, JSON **result);

void parseBool(char *s, int i, int j, int *consumed, JSON **result);

void parseObject(char *s, int i, int j, int *consumed, JSON **result);

void parseArray(char *s, int i, int j, int *consumed, JSON **result);

void parseValue(char *s, int i, int j, int *consumed, JSON **result);
