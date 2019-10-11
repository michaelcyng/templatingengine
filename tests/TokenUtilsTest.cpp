//
// Created by Cho Yiu Ng on 10.10.19.
//

#include <gtest/gtest.h>

#include <LoopClosingToken.h>
#include <LoopOpeningToken.h>
#include <PlainTextToken.h>
#include <TokenUtils.h>
#include <VariableToken.h>

class TokenUtilsTest : public ::testing::Test {
protected:

    TokenUtilsTest() = default;
    ~TokenUtilsTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(TokenUtilsTest, TestLoopClosing) {
    templatingengine::TokenBasePtr_t testToken1 = std::make_shared<templatingengine::PlainTextToken>("Plain text 1");
    templatingengine::TokenBasePtr_t loopOpenToken =
            std::make_shared<templatingengine::LoopOpeningToken>("{{#LOOP valueList element}}", "valueList",
                                                                 "element");
    templatingengine::TokenBasePtr_t testToken2 =
            std::make_shared<templatingengine::VariableToken>("{{element}}", "element");
    templatingengine::TokenBasePtr_t loopCloseToken =
            std::make_shared<templatingengine::LoopClosingToken>("{{/LOOP}}");
    templatingengine::TokenBasePtr_t testToken3 = std::make_shared<templatingengine::PlainTextToken>("Plain text 2");

    templatingengine::TokenPtrList_t tokenPtrList { testToken1, loopOpenToken, testToken2, loopCloseToken, testToken3 };
    auto openTokenIter = tokenPtrList.begin();
    std::advance(openTokenIter, 1);
    auto closeTokenIter = tokenPtrList.begin();
    std::advance(closeTokenIter, 3);

    ASSERT_EQ(templatingengine::TokenUtils::findClosingTokenPtrIter(openTokenIter, tokenPtrList.end()),
              closeTokenIter);
}

TEST_F(TokenUtilsTest, TestNestedLoopClosing) {
    templatingengine::TokenBasePtr_t outerLoopOpenToken =
            std::make_shared<templatingengine::LoopOpeningToken>("{{#LOOP outerList outerElement}}",
                                                                 "outerList", "outerElement");
    templatingengine::TokenBasePtr_t outerLoopCloseToken =
            std::make_shared<templatingengine::LoopClosingToken>("{{/LOOP}}");

    templatingengine::TokenBasePtr_t innerLoopOpenToken1 =
            std::make_shared<templatingengine::LoopOpeningToken>("{{#LOOP innerList1 innerElement1}}",
                                                                 "innerList1", "innerElement1");
    templatingengine::TokenBasePtr_t innerLoopCloseToken1 =
            std::make_shared<templatingengine::LoopClosingToken>("{{/LOOP}}");

    templatingengine::TokenBasePtr_t innerLoopOpenToken2 =
            std::make_shared<templatingengine::LoopOpeningToken>("{{#LOOP innerList2 innerElment2}}",
                                                                 "innerList2", "innerElement2");
    templatingengine::TokenBasePtr_t innerLoopCloseToken2 =
            std::make_shared<templatingengine::LoopClosingToken>("{{/LOOP}}");

    templatingengine::TokenPtrList_t tokenPtrList { outerLoopOpenToken, innerLoopOpenToken1, innerLoopCloseToken1,
                                       innerLoopOpenToken2, innerLoopCloseToken2, outerLoopCloseToken };

    auto outerOpenTokenIter = tokenPtrList.begin();
    auto outerCloseTokenIter = tokenPtrList.begin();
    std::advance(outerCloseTokenIter, 5);
    ASSERT_EQ(templatingengine::TokenUtils::findClosingTokenPtrIter(outerOpenTokenIter, tokenPtrList.end()),
              outerCloseTokenIter);

    auto innerOpenTokenIter1 = tokenPtrList.begin();
    std::advance(innerOpenTokenIter1, 1);
    auto innerCloseTokenIter1 = tokenPtrList.begin();
    std::advance(innerCloseTokenIter1, 2);
    ASSERT_EQ(templatingengine::TokenUtils::findClosingTokenPtrIter(innerOpenTokenIter1, tokenPtrList.end()),
              innerCloseTokenIter1);

    auto innerOpenTokenIter2 = tokenPtrList.begin();
    std::advance(innerOpenTokenIter2, 3);
    auto innerCloseTokenIter2 = tokenPtrList.begin();
    std::advance(innerCloseTokenIter2, 4);
    ASSERT_EQ(templatingengine::TokenUtils::findClosingTokenPtrIter(innerOpenTokenIter2, tokenPtrList.end()),
               innerCloseTokenIter2);
}

TEST_F(TokenUtilsTest, TestMissingLoopClosing) {
    templatingengine::TokenBasePtr_t openToken =
            std::make_shared<templatingengine::LoopOpeningToken>("{{#LOOP valueList element}}", "valueList",
                                                                 "element");
    templatingengine::TokenBasePtr_t testToken = std::make_shared<templatingengine::PlainTextToken>("Plain text");
    templatingengine::TokenPtrList_t tokenPtrList { openToken, testToken };
    ASSERT_EQ(templatingengine::TokenUtils::findClosingTokenPtrIter(tokenPtrList.begin(), tokenPtrList.end()),
              tokenPtrList.end());
}

TEST_F(TokenUtilsTest, TestIsCommandTokenText) {
    ASSERT_TRUE(templatingengine::TokenUtils::isCommandTokenText("{{variable}}"));
    ASSERT_TRUE(templatingengine::TokenUtils::isCommandTokenText("{{#LOOP valueList element}}"));
    ASSERT_TRUE(templatingengine::TokenUtils::isCommandTokenText("{{/LOOP}}"));
    ASSERT_TRUE(templatingengine::TokenUtils::isCommandTokenText("{{}}"));
    ASSERT_FALSE(templatingengine::TokenUtils::isCommandTokenText("{variable}"));
    ASSERT_FALSE(templatingengine::TokenUtils::isCommandTokenText(""));
    ASSERT_FALSE(templatingengine::TokenUtils::isCommandTokenText("{{variable}"));
    ASSERT_FALSE(templatingengine::TokenUtils::isCommandTokenText("{{variable"));
    ASSERT_FALSE(templatingengine::TokenUtils::isCommandTokenText("Plain Text"));
}

TEST_F(TokenUtilsTest, TestGetCommandFromToken) {
    ASSERT_EQ(templatingengine::TokenUtils::getCommandFromToken("{{variable}}"), "variable");
    ASSERT_EQ(templatingengine::TokenUtils::getCommandFromToken("{{#LOOP valueList element}}"),
              "#LOOP valueList element");
    ASSERT_EQ(templatingengine::TokenUtils::getCommandFromToken("{{/LOOP}}"), "/LOOP");
    ASSERT_EQ(templatingengine::TokenUtils::getCommandFromToken("{{}}"), "");
    ASSERT_EQ(templatingengine::TokenUtils::getCommandFromToken("{{ variable}}"), "variable");
    ASSERT_EQ(templatingengine::TokenUtils::getCommandFromToken("{{variable  }}"), "variable");
    ASSERT_EQ(templatingengine::TokenUtils::getCommandFromToken("{{ \tvariable   }}"), "variable");
}

TEST_F(TokenUtilsTest, TestGetTokenTypeFromCommand) {
    ASSERT_EQ(templatingengine::TokenUtils::getTokenTypeFromCommand("#LOOP valueList element"),
              templatingengine::TokenType_t::LoopOpen_e);
    ASSERT_EQ(templatingengine::TokenUtils::getTokenTypeFromCommand("/LOOP"),
              templatingengine::TokenType_t::LoopClose_e);
    ASSERT_EQ(templatingengine::TokenUtils::getTokenTypeFromCommand("#Some other commands"),
              templatingengine::TokenType_t::Unknown_e);
}

TEST_F(TokenUtilsTest, TestIsVariableCommand) {
    ASSERT_TRUE(templatingengine::TokenUtils::isVariableCommand("variable"));
    ASSERT_FALSE(templatingengine::TokenUtils::isVariableCommand("#LOOP valueList element"));
    ASSERT_FALSE(templatingengine::TokenUtils::isVariableCommand("/LOOP"));
    ASSERT_TRUE(templatingengine::TokenUtils::isVariableCommand(""));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
