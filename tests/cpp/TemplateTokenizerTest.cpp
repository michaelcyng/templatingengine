//
// Created by Cho Yiu Ng on 11.10.19.
//

#include <gtest/gtest.h>
#include <sstream>

#include <utils/TemplateTokenizer.h>

class TemplateTokenizerTest : public ::testing::Test {
protected:

    TemplateTokenizerTest() = default;
    ~TemplateTokenizerTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(TemplateTokenizerTest, TestPlainTextOnly) {
    std::stringstream ss("This is line 1.\nThis is line 2.");
    templatingengine::TemplateTokenizer tokenizer;
    std::list<std::string> tokenTextList;
    tokenizer.tokenize(ss, tokenTextList);

    ASSERT_EQ(tokenTextList.size(), 1);
    ASSERT_EQ(*(tokenTextList.begin()), "This is line 1.\nThis is line 2.");
}

TEST_F(TemplateTokenizerTest, TestEmptyStream) {
    std::stringstream ss;
    templatingengine::TemplateTokenizer tokenizer;
    std::list<std::string> tokenTextList;
    tokenizer.tokenize(ss, tokenTextList);

    ASSERT_EQ(tokenTextList.size(), 0);
}

TEST_F(TemplateTokenizerTest, TestEscapeCharacters) {
    std::stringstream ss(R"(This is a \{\{variable}})");
    templatingengine::TemplateTokenizer tokenizer;
    std::list<std::string> tokenTextList;
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 1);
    ASSERT_EQ(*(tokenTextList.begin()), "This is a {{variable}}");

    std::stringstream ss1(R"(\\\a\)");
    tokenTextList.clear();
    tokenizer.tokenize(ss1, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 1);
    ASSERT_EQ(*(tokenTextList.begin()), R"(\\a\)");
}

TEST_F(TemplateTokenizerTest, TestCommand) {
    std::stringstream ss(R"(This is a text.{{variable}}This is another text.)");
    templatingengine::TemplateTokenizer tokenizer;
    std::list<std::string> tokenTextList;
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 3);
    auto iter = tokenTextList.begin();
    ASSERT_EQ(*iter, "This is a text.");
    ++iter;
    ASSERT_EQ(*iter, "{{variable}}");
    ++iter;
    ASSERT_EQ(*iter, "This is another text.");

    ss = std::stringstream(R"({{variable1}}{{variable2}}This is a text.)");
    tokenTextList.clear();
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 3);
    iter = tokenTextList.begin();
    ASSERT_EQ(*iter, "{{variable1}}");
    ++iter;
    ASSERT_EQ(*iter, "{{variable2}}");
    ++iter;
    ASSERT_EQ(*iter, "This is a text.");

    ss = std::stringstream(R"(This is a text.{{variable}})");
    tokenTextList.clear();
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 2);
    iter = tokenTextList.begin();
    ASSERT_EQ(*iter, "This is a text.");
    ++iter;
    ASSERT_EQ(*iter, "{{variable}}");

    ss = std::stringstream(R"(This is a text.{{variable)");
    tokenTextList.clear();
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 1);
    ASSERT_EQ(*(tokenTextList.begin()), "This is a text.{{variable");

    ss = std::stringstream(R"(This is a text.{{variable})");
    tokenTextList.clear();
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 1);
    ASSERT_EQ(*(tokenTextList.begin()), "This is a text.{{variable}");

    ss = std::stringstream(R"(This is a text.{variable})");
    tokenTextList.clear();
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 1);
    ASSERT_EQ(*(tokenTextList.begin()), "This is a text.{variable}");

    ss = std::stringstream(R"(This is a text.})");
    tokenTextList.clear();
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 1);
    ASSERT_EQ(*(tokenTextList.begin()), "This is a text.}");

    ss = std::stringstream(R"(This is a text.}})");
    tokenTextList.clear();
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 1);
    ASSERT_EQ(*(tokenTextList.begin()), "This is a text.}}");

    ss = std::stringstream(R"(Some texts...{{{{}}}})");
    tokenTextList.clear();
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 3);
    iter = tokenTextList.begin();
    ASSERT_EQ(*iter, "Some texts...");
    ++iter;
    ASSERT_EQ(*iter, "{{{{}}");
    ++iter;
    ASSERT_EQ(*iter, "}}");

    ss = std::stringstream(R"(\{{{variable}}})");
    tokenTextList.clear();
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 3);
    iter = tokenTextList.begin();
    ASSERT_EQ(*iter, "{");
    ++iter;
    ASSERT_EQ(*iter, "{{variable}}");
    ++iter;
    ASSERT_EQ(*iter, "}");

    ss = std::stringstream(R"({{vari\\ab\{le}})");
    tokenTextList.clear();
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 1);
    ASSERT_EQ(*(tokenTextList.begin()), R"({{vari\ab{le}})");

    ss = std::stringstream(R"({{vari\}}able}})");
    tokenTextList.clear();
    tokenizer.tokenize(ss, tokenTextList);
    ASSERT_EQ(tokenTextList.size(), 2);
    iter = tokenTextList.begin();
    ASSERT_EQ(*(iter), R"({{vari\}})");
    ++iter;
    ASSERT_EQ(*(iter), "able}}");
}
