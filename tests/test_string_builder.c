#include <stdbool.h>
#include <string.h>
#include <string_builder.h>
#include <unity.h>

void testStringBuilderHelper(char *str) {
  StringBuilder *builder = stringBuilder_new();

  int n = strlen(str);

  bool brk = false;

  for (int i = 0; i < n;) {
    int step = 1 + i % 10;

    if (i + step >= n) {
      step = n - i;
      brk = true;
    }

    stringBuilder_append(builder, str + i, step);

    if (brk) {
      break;
    }

    i += step;
  }

  String result = stringBuilder_build(builder);

  TEST_ASSERT_EQUAL_STRING(str, result.data);
  TEST_ASSERT_EQUAL_INT32(n, result.len);

  stringBuilder_destroy(builder);
}

void test_stringBuilder_init() {
  StringBuilder *builder = stringBuilder_new();

  TEST_ASSERT_EQUAL_INT32(0, builder->len);
  TEST_ASSERT_EQUAL_INT32(INITIAL_STRING_BUILDER_BUFFER_SIZE,
                          builder->capacity);
  TEST_ASSERT_NOT_NULL(builder->buf);

  stringBuilder_destroy(builder);
}

void test_stringBuilder_1() {
  char *str1 =
      "What is Lorem Ipsum?\nLorem Ipsum is simply dummy text of the printing "
      "and typesetting industry. Lorem Ipsum has been the industry's standard "
      "dummy text ever since 1966, when designers at Letraset and James "
      "Mosley, the librarian at St Bride Printing Library in London, took a "
      "1914 Cicero translation and scrambled it to make dummy text for "
      "Letraset's Body Type sheets. It has survived not only many decades, but "
      "also the leap into electronic typesetting, remaining essentially "
      "unchanged. It was popularised thanks to these sheets and more recently "
      "with desktop publishing software like Aldus PageMaker and Microsoft "
      "Word including versions of Lorem Ipsum.\nWhy do we use it?\nIt is a "
      "long established fact that a reader will be distracted by the readable "
      "content of a page when looking at its layout. The point of using Lorem "
      "Ipsum is that it has a more-or-less normal distribution of letters, as "
      "opposed to using 'Content here, content here', making it look like "
      "readable English. Many desktop publishing packages and web page editors "
      "now use Lorem Ipsum as their default model text, and a search for "
      "'lorem ipsum' will uncover many web sites still in their infancy. "
      "Various versions have evolved over the years, sometimes by accident, "
      "sometimes on purpose (injected humour and the like).\nWhere does it "
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
      "section 1.10.32.\nThe standard chunk of Lorem Ipsum used since 1966 is "
      "reproduced below for those interested. Sections 1.10.32 and 1.10.33 "
      "from \"de Finibus Bonorum et Malorum\" by Cicero are also reproduced in "
      "their exact original form, accompanied by English versions from the "
      "1914 translation by H. Rackham.\nWhere can I get some?\nThere are many "
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
      "repetition, injected humour, or non-characteristic words etc.";
  testStringBuilderHelper(str1);
}

void test_stringBuilder_2() {
  char *str2 = "";
  testStringBuilderHelper(str2);
}

void test_stringBuilder_3() {
  char *str2 = "M";
  testStringBuilderHelper(str2);
}

void test_stringBuilder_4() {
  char *str = "";
  testStringBuilderHelper(str);
}

void test_stringBuilder_group() {
  RUN_TEST(test_stringBuilder_init);
  RUN_TEST(test_stringBuilder_1);
  RUN_TEST(test_stringBuilder_2);
  RUN_TEST(test_stringBuilder_3);
  RUN_TEST(test_stringBuilder_4);
}