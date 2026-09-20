#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "string_builder.h"

const int INITIAL_STRING_BUILDER_BUFFER_SIZE = 4;

StringBuilder stringBuilder_new()
{
    char *buf = malloc(INITIAL_STRING_BUILDER_BUFFER_SIZE);
    assert(buf != NULL);
    return (StringBuilder){buf, 0, INITIAL_STRING_BUILDER_BUFFER_SIZE};
}

void stringBuilder_append(StringBuilder *builder, char *str, int n)
{
    int newLen = builder->len + n;
    if (newLen > builder->capacity)
    {
        int newCapacity = builder->capacity;
        while (newLen > newCapacity)
            newCapacity *= 2;
        char *temp = realloc(builder->buf, newCapacity);
        assert(temp != NULL);
        builder->buf = temp;
        builder->capacity = newCapacity;
    }
    memcpy(builder->buf + builder->len, str, n);
    builder->len += n;
}

String stringBuilder_build(StringBuilder *builder)
{

    char terminal = 0;

    stringBuilder_append(builder, &terminal, 1);

    char *result;
    result = realloc(builder->buf, builder->len);
    assert(result != NULL);

    return (String){result, builder->len - 1};
}
