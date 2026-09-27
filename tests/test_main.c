#include "test_json/test_json_parse_array.c"
#include "test_json/test_json_parse_bool.c"
#include "test_json/test_json_parse_number.c"
#include "test_json/test_json_parse_object.c"
#include "test_json/test_json_parse_string.c"
#include "test_string_builder.c"
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

int main() {
  UNITY_BEGIN();

  test_stringBuilder_group();

  test_json_parseString_group();

  test_json_parseNumber_group();

  test_json_parseBool_group();

  test_json_parseObject_group();

  test_json_parseArray_group();

  return UNITY_END();
}