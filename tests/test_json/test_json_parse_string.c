#include "../utils/utils.h"
#include <assert.h>
#include <parser.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>

void testJsonParseString(char *input, char *expected) {
  Json *result = NULL;
  JsonParser *parser = jsonParser_create(input, strlen(input));
  
  err_t err = jsonParser_parseValue(parser, &result);
  TEST_ASSERT_EQUAL_INT(OK, err);

  int expectedConsumend = strlen(input);
  TEST_ASSERT_EQUAL_INT32(expectedConsumend, parser->cursor + 1);

  Json exp = {J_STRING, {.string = {expected, strlen(expected)}}};
  testValue(&exp, result);
}

void test_json_parseString_1() {

  char *input1 = "\"Mohamed\"";
  char *expected1 = "Mohamed";
  testJsonParseString(input1, expected1);
}
void test_json_parseString_2() {

  char *input2 =
      "\"What is Lorem Ipsum?\\nLorem Ipsum is simply dummy text of the "
      "printing and \\\\typesetting industry\\\\. Lorem Ipsum has been the "
      "industry's standard dummy text ever since 1966, when designers at "
      "Letraset and James Mosley, the librarian at St Bride Printing Library "
      "in London, took a 1914 Cicero translation and scrambled it to make "
      "dummy text for Letraset's Body Type sheets. It has survived not only "
      "many decades, but also the leap into electronic typesetting, remaining "
      "essentially unchanged. It was popularised thanks to these sheets and "
      "more recently with desktop publishing software like Aldus PageMaker and "
      "Microsoft Word including versions of Lorem Ipsum.\\n\\nWhy do we use "
      "it?\\nIt is a long established fact that a reader will be distracted by "
      "the readable content of a page when looking at its layout. The point of "
      "using Lorem Ipsum is that it has a more-or-less normal distribution of "
      "letters, as opposed to using 'Content here, content here', making it "
      "look like readable English. Many desktop publishing packages and web "
      "page editors now use Lorem Ipsum as their default model text, and a "
      "search for 'lorem ipsum' will uncover many web sites still in their "
      "infancy. Various versions have evolved over the years, sometimes by "
      "accident, sometimes on purpose (injected humour and the "
      "like).\\n\\n\\nWhere does it come from?\\nContrary to popular belief, "
      "Lorem Ipsum is not simply random text. It has roots in a piece of "
      "classical Latin literature from 45 BC, making it over 2000 years old. "
      "Richard McClintock, a Latin professor at Hampden-Sydney College in "
      "Virginia, looked up one of the more obscure Latin words, consectetur, "
      "from a Lorem Ipsum passage, and going through the cites of the word in "
      "classical literature, discovered the undoubtable source. Lorem Ipsum "
      "comes from sections 1.10.32 and 1.10.33 of \\\"de Finibus Bonorum et "
      "Malorum\\\" (The Extremes of Good and Evil) by Cicero, written in 45 "
      "BC. This book is a treatise on the theory of ethics, very popular "
      "during the Renaissance. The first line of Lorem Ipsum, \\\"Lorem ipsum "
      "dolor sit amet..\\\", comes from a line in section 1.10.32.\\n\\nThe "
      "standard chunk of Lorem Ipsum used since 1966 is reproduced below for "
      "those interested. Sections 1.10.32 and 1.10.33 from \\\"de Finibus "
      "Bonorum et Malorum\\\" by Cicero are also reproduced in their exact "
      "original form, accompanied by English versions from the 1914 "
      "translation by H. Rackham.\\n\\nWhere can I get some?\\nThere are many "
      "variations of passages of Lorem Ipsum available, but the majority have "
      "suffered alteration in some form, by injected humour, or randomised "
      "words which don't look even slightly believable. If you are going to "
      "use a passage of Lorem Ipsum, you need to be sure there isn't anything "
      "embarrassing hidden in the middle of text. All the Lorem Ipsum "
      "generators on the Internet tend to repeat predefined chunks as "
      "necessary, making this the first true generator on the Internet. It "
      "uses a dictionary of over 200 Latin words, combined with a handful of "
      "model sentence structures, to generate Lorem Ipsum which looks "
      "reasonable. The generated Lorem Ipsum is therefore always free from "
      "repetition, injected humour, or non-characteristic words etc.\"";
  char *expected2 =
      "What is Lorem Ipsum?\nLorem Ipsum is simply dummy text of the printing "
      "and \\typesetting industry\\. Lorem Ipsum has been the industry's "
      "standard dummy text ever since 1966, when designers at Letraset and "
      "James Mosley, the librarian at St Bride Printing Library in London, "
      "took a 1914 Cicero translation and scrambled it to make dummy text for "
      "Letraset's Body Type sheets. It has survived not only many decades, but "
      "also the leap into electronic typesetting, remaining essentially "
      "unchanged. It was popularised thanks to these sheets and more recently "
      "with desktop publishing software like Aldus PageMaker and Microsoft "
      "Word including versions of Lorem Ipsum.\n\nWhy do we use it?\nIt is a "
      "long established fact that a reader will be distracted by the readable "
      "content of a page when looking at its layout. The point of using Lorem "
      "Ipsum is that it has a more-or-less normal distribution of letters, as "
      "opposed to using 'Content here, content here', making it look like "
      "readable English. Many desktop publishing packages and web page editors "
      "now use Lorem Ipsum as their default model text, and a search for "
      "'lorem ipsum' will uncover many web sites still in their infancy. "
      "Various versions have evolved over the years, sometimes by accident, "
      "sometimes on purpose (injected humour and the like).\n\n\nWhere does it "
      "come from?\nContrary to popular belief, Lorem Ipsum is not simply "
      "random text. It has roots in a piece of classical Latin literature from "
      "45 BC, making it over 2000 years old. Richard McClintock, a Latin "
      "professor at Hampden-Sydney College in Virginia, looked up one of the "
      "more obscure Latin words, consectetur, from a Lorem Ipsum passage, and "
      "going through the cites of the word in classical literature, discovered "
      "the undoubtable source. Lorem Ipsum comes from sections 1.10.32 and "
      "1.10.33 of \"de Finibus Bonorum et Malorum\" (The Extremes of Good and "
      "Evil) by Cicero, written in 45 BC. This book is a treatise on the "
      "theory of ethics, very popular during the Renaissance. The first line "
      "of Lorem Ipsum, \"Lorem ipsum dolor sit amet..\", comes from a line in "
      "section 1.10.32.\n\nThe standard chunk of Lorem Ipsum used since 1966 "
      "is reproduced below for those interested. Sections 1.10.32 and 1.10.33 "
      "from \"de Finibus Bonorum et Malorum\" by Cicero are also reproduced in "
      "their exact original form, accompanied by English versions from the "
      "1914 translation by H. Rackham.\n\nWhere can I get some?\nThere are "
      "many variations of passages of Lorem Ipsum available, but the majority "
      "have suffered alteration in some form, by injected humour, or "
      "randomised words which don't look even slightly believable. If you are "
      "going to use a passage of Lorem Ipsum, you need to be sure there isn't "
      "anything embarrassing hidden in the middle of text. All the Lorem Ipsum "
      "generators on the Internet tend to repeat predefined chunks as "
      "necessary, making this the first true generator on the Internet. It "
      "uses a dictionary of over 200 Latin words, combined with a handful of "
      "model sentence structures, to generate Lorem Ipsum which looks "
      "reasonable. The generated Lorem Ipsum is therefore always free from "
      "repetition, injected humour, or non-characteristic words etc.";
  testJsonParseString(input2, expected2);
}
void test_json_parseString_3() {
  TEST_IGNORE();

  char *input3 = "\"Moh😄😄😄 😃😃😃amed\"";
  char *expected3 = "Moh😄😄😄 😃😃😃amed";
  testJsonParseString(input3, expected3);
}
void test_json_parseString_4() {
  TEST_IGNORE();

  char *input4 = "\"Moh😄😄😄 \\u1234 😃😃😃amed\"";
  char *expected4 = "Moh😄😄😄 \u1234 😃😃😃amed";
  testJsonParseString(input4, expected4);
}

void test_json_parseString_5() {
  char *input1 = "\"\"";
  char *expected1 = "";
  testJsonParseString(input1, expected1);
}

void test_json_parseString_group() {
  RUN_TEST(test_json_parseString_1);
  RUN_TEST(test_json_parseString_2);
  RUN_TEST(test_json_parseString_3);
  RUN_TEST(test_json_parseString_4);
  RUN_TEST(test_json_parseString_5);
}