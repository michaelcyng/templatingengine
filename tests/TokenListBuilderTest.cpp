//
// Created by Cho Yiu Ng on 10.10.19.
//

#include <gtest/gtest.h>

#include <tokens/LoopOpeningToken.h>
#include <tokens/PlainTextToken.h>
#include <builders/TokenListBuilder.h>
#include <tokens/VariableToken.h>

class TokenListBuilderTest : public ::testing::Test {
protected:

    TokenListBuilderTest() = default;
    ~TokenListBuilderTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(TokenListBuilderTest, TestPlainTextTokenBuild) {
    templatingengine::TokenListBuilder testBuilder;

    auto tokenPtr = testBuilder.buildToken("Plain Text");
    ASSERT_EQ(tokenPtr->getTokenType(), templatingengine::TokenType_t::PlainText_e);
    ASSERT_EQ(std::static_pointer_cast<templatingengine::PlainTextToken>(tokenPtr)->getText(), "Plain Text");
}

TEST_F(TokenListBuilderTest, TestVariableTokenBuild) {
    templatingengine::TokenListBuilder testBuilder;

    auto tokenPtr = testBuilder.buildToken("{{ variable\t\t}}");
    ASSERT_EQ(tokenPtr->getTokenType(), templatingengine::TokenType_t::Variable_e);
    ASSERT_EQ(std::static_pointer_cast<templatingengine::VariableToken>(tokenPtr)->getVariableName(),
              "variable");
    ASSERT_EQ(tokenPtr->getRawTokenText(), "{{ variable\t\t}}");
}

TEST_F(TokenListBuilderTest, TestLoopOpeningTokenBuild) {
    templatingengine::TokenListBuilder testBuilder;

    auto tokenPtr = testBuilder.buildToken("{{\t#LOOP  valueList  \t\telement }}");
    ASSERT_EQ(tokenPtr->getTokenType(), templatingengine::TokenType_t::LoopOpen_e);
    ASSERT_EQ(std::static_pointer_cast<templatingengine::LoopOpeningToken>(tokenPtr)->getValueListName(),
              "valueList");
    ASSERT_EQ(std::static_pointer_cast<templatingengine::LoopOpeningToken>(tokenPtr)->getElementName(),
              "element");

    auto tokenPtr1 = testBuilder.buildToken("{{#LOOP}}");
    ASSERT_EQ(tokenPtr1->getTokenType(), templatingengine::TokenType_t::PlainText_e);
    ASSERT_EQ(std::static_pointer_cast<templatingengine::PlainTextToken>(tokenPtr1)->getText(), "{{#LOOP}}");

    auto tokenPtr2 = testBuilder.buildToken("{{#LOOP valueList}}");
    ASSERT_EQ(tokenPtr2->getTokenType(), templatingengine::TokenType_t::PlainText_e);
    ASSERT_EQ(std::static_pointer_cast<templatingengine::PlainTextToken>(tokenPtr2)->getText(),
             "{{#LOOP valueList}}");

    auto tokenPtr3 = testBuilder.buildToken("{{#LOOP valueList element something else}}");
    ASSERT_EQ(tokenPtr3->getTokenType(), templatingengine::TokenType_t::PlainText_e);
    ASSERT_EQ(std::static_pointer_cast<templatingengine::PlainTextToken>(tokenPtr3)->getText(),
              "{{#LOOP valueList element something else}}");
}

TEST_F(TokenListBuilderTest, TestLoopClosingTokenBuild) {
    templatingengine::TokenListBuilder testBuilder;

    auto tokenPtr = testBuilder.buildToken("{{/LOOP}}");
    ASSERT_EQ(tokenPtr->getTokenType(), templatingengine::TokenType_t::LoopClose_e);
}

TEST_F(TokenListBuilderTest, TestUnrecognizedTokenBuild) {
    templatingengine::TokenListBuilder testBuilder;

    auto tokenPtr = testBuilder.buildToken("{{#Unrecognizable command}}");
    ASSERT_EQ(tokenPtr->getTokenType(), templatingengine::TokenType_t::PlainText_e);
    ASSERT_EQ(tokenPtr->getRawTokenText(), "{{#Unrecognizable command}}");
    ASSERT_EQ(std::static_pointer_cast<templatingengine::PlainTextToken>(tokenPtr)->getText(),
              "{{#Unrecognizable command}}");
}

TEST_F(TokenListBuilderTest, TestBuildTokenPtrList) {
    std::list<std::string> tokenTextList {
        "This is plain text 1.",
        "{{variable}}",
        "{{#LOOP valueList element}}",
        "{{/LOOP}}"
    };

    templatingengine::TokenListBuilder testBuilder;
    auto tokenPtrList = testBuilder.buildTokenPtrList(tokenTextList);
    ASSERT_EQ(tokenPtrList.size(), tokenTextList.size());

    auto tokenPtrIter = tokenPtrList.begin();
    auto tokenPtr1 = *tokenPtrIter;
    ASSERT_EQ(tokenPtr1->getTokenType(), templatingengine::TokenType_t::PlainText_e);
    ASSERT_EQ(std::static_pointer_cast<templatingengine::PlainTextToken>(tokenPtr1)->getRawTokenText(),
              "This is plain text 1.");

    ++tokenPtrIter;
    tokenPtr1 = *tokenPtrIter;
    ASSERT_EQ(tokenPtr1->getTokenType(), templatingengine::TokenType_t::Variable_e);
    ASSERT_EQ(std::static_pointer_cast<templatingengine::VariableToken>(tokenPtr1)->getVariableName(),
              "variable");

    ++tokenPtrIter;
    tokenPtr1 = *tokenPtrIter;
    ASSERT_EQ(tokenPtr1->getTokenType(), templatingengine::TokenType_t::LoopOpen_e);
    ASSERT_EQ(std::static_pointer_cast<templatingengine::LoopOpeningToken>(tokenPtr1)->getValueListName(),
             "valueList");
    ASSERT_EQ(std::static_pointer_cast<templatingengine::LoopOpeningToken>(tokenPtr1)->getElementName(),
              "element");

    ++tokenPtrIter;
    tokenPtr1 = *tokenPtrIter;
    ASSERT_EQ(tokenPtr1->getTokenType(), templatingengine::TokenType_t::LoopClose_e);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

