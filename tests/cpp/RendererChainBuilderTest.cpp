//
// Created by Cho Yiu Ng on 10.10.19.
//

#include <gtest/gtest.h>

#include <tokens/LoopClosingToken.h>
#include <tokens/LoopOpeningToken.h>
#include <tokens/PlainTextToken.h>
#include <builders/RendererChainBuilder.h>
#include <tokens/Token.h>
#include <tokens/VariableToken.h>

class RendererChainBuilderTest : public ::testing::Test {
protected:

    RendererChainBuilderTest() = default;
    ~RendererChainBuilderTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(RendererChainBuilderTest, TestEmptyTokenChain) {
    templatingengine::TokenPtrList_t emptyTokenPtrList;
    auto testRendererChainPtr = templatingengine::RendererChainBuilder::buildRendererChain(emptyTokenPtrList);
    templatingengine::ParameterSet parameterSet;
    std::stringstream ss;
    testRendererChainPtr->render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "");
}

TEST_F(RendererChainBuilderTest, TestPlainTextTokens) {
    templatingengine::TokenBasePtr_t testToken1 = std::make_shared<templatingengine::PlainTextToken>("Plain text 1.");
    templatingengine::TokenBasePtr_t testToken2 = std::make_shared<templatingengine::PlainTextToken>("Plain text 2.");
    templatingengine::TokenPtrList_t testTokenPtrList { testToken1, testToken2 };
    auto testRendererChainPtr = templatingengine::RendererChainBuilder::buildRendererChain(testTokenPtrList);
    templatingengine::ParameterSet parameterSet;
    std::stringstream ss;
    testRendererChainPtr->render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "Plain text 1.Plain text 2.");
}

TEST_F(RendererChainBuilderTest, TestVariableToken) {
    templatingengine::TokenBasePtr_t testToken1 =
            std::make_shared<templatingengine::VariableToken>("{{testVariable}}", "testVariable");
    templatingengine::TokenBasePtr_t testToken2 = std::make_shared<templatingengine::PlainTextToken>("Plain text.");
    templatingengine::TokenPtrList_t testTokenPtrList { testToken1, testToken2 };
    auto testRendererChainPtr = templatingengine::RendererChainBuilder::buildRendererChain(testTokenPtrList);
    templatingengine::ParameterSet parameterSet;
    parameterSet["testVariable"] = "Variable text.";
    std::stringstream ss;
    testRendererChainPtr->render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "Variable text.Plain text.");
}

TEST_F(RendererChainBuilderTest, TestLinearLoop) {
    templatingengine::TokenBasePtr_t outerToken1 = std::make_shared<templatingengine::PlainTextToken>("Loop begins:");
    templatingengine::TokenBasePtr_t loopOpeningToken =
            std::make_shared<templatingengine::LoopOpeningToken>("{{#LOOP valueList element}}", "valueList",
                                                                 "element");
    templatingengine::TokenBasePtr_t innerToken1 =
            std::make_shared<templatingengine::PlainTextToken>("Iteration starts.");
    templatingengine::TokenBasePtr_t innerToken2 =
            std::make_shared<templatingengine::VariableToken>("{{element}}", "element");
    templatingengine::TokenBasePtr_t innerToken3 =
            std::make_shared<templatingengine::PlainTextToken>("Iteration ends.");
    templatingengine::TokenBasePtr_t loopClosingToken =
            std::make_shared<templatingengine::LoopClosingToken>("{{/LOOP}}");
    templatingengine::TokenBasePtr_t outerToken2 = std::make_shared<templatingengine::PlainTextToken>("Loop ends.");
    templatingengine::TokenPtrList_t tokenPtrList { outerToken1,
                                                    loopOpeningToken,
                                                    innerToken1, innerToken2, innerToken3,
                                                    loopClosingToken,
                                                    outerToken2 };
    auto testRendererChainPtr = templatingengine::RendererChainBuilder::buildRendererChain(tokenPtrList);

    templatingengine::ParameterSet parameterSet;
    parameterSet["valueList"] = {"Test String 1.", "Test String 2.", "Test String 3."};

    std::stringstream ss;
    testRendererChainPtr->render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "Loop begins:"
                             "Iteration starts."
                             "Test String 1."
                             "Iteration ends."
                             "Iteration starts."
                             "Test String 2."
                             "Iteration ends."
                             "Iteration starts."
                             "Test String 3."
                             "Iteration ends."
                             "Loop ends.");
}

TEST_F(RendererChainBuilderTest, TestNestedLoop) {
    templatingengine::TokenBasePtr_t outermostToken1 =
            std::make_shared<templatingengine::PlainTextToken>("Outermost text 1.");
    templatingengine::TokenBasePtr_t outermostToken2 =
            std::make_shared<templatingengine::PlainTextToken>("Outermost text 2.");

    templatingengine::TokenBasePtr_t outerLoopOpening =
            std::make_shared<templatingengine::LoopOpeningToken>("{{#LOOP outerValueList outerElement}}",
                                                                 "outerValueList", "outerElement");
    templatingengine::TokenBasePtr_t outerLoopToken1 =
            std::make_shared<templatingengine::PlainTextToken>("Outer loop text 1.");
    templatingengine::TokenBasePtr_t outerLoopToken2 =
            std::make_shared<templatingengine::VariableToken>("{{outerElement}}", "outerElement");
    templatingengine::TokenBasePtr_t outerLoopToken3 =
            std::make_shared<templatingengine::PlainTextToken>("Outer loop text 2.");
    templatingengine::TokenBasePtr_t outerLoopClosing =
            std::make_shared<templatingengine::LoopClosingToken>("{{/LOOP}}");

    templatingengine::TokenBasePtr_t innerLoopOpening1 =
            std::make_shared<templatingengine::LoopOpeningToken>("{{#LOOP innerValueList1 innerElment}}",
                                                                 "innerValueList1", "innerElement");
    templatingengine::TokenBasePtr_t innerLoopToken1 =
            std::make_shared<templatingengine::VariableToken>("{{innerElement}}", "innerElement");
    templatingengine::TokenBasePtr_t innerLoopClosing1 =
            std::make_shared<templatingengine::LoopClosingToken>("{{/LOOP}}");

    templatingengine::TokenBasePtr_t innerLoopOpening2 =
            std::make_shared<templatingengine::LoopOpeningToken>("{{#LOOP innerValueList2 innerElement}}",
                                                                 "innerValueList2", "innerElement");
    templatingengine::TokenBasePtr_t innerLoopToken2 =
            std::make_shared<templatingengine::VariableToken>("{{innerElement}}", "innerElement");
    templatingengine::TokenBasePtr_t innerLoopClosing2 =
            std::make_shared<templatingengine::LoopClosingToken>("{{/LOOP}}");

    templatingengine::TokenPtrList_t tokenPtrList{outermostToken1,
                                                  outerLoopOpening,
                                                  outerLoopToken1,
                                                  innerLoopOpening1, innerLoopToken1, innerLoopClosing1,
                                                  outerLoopToken2,
                                                  innerLoopOpening2, innerLoopToken2, innerLoopClosing2,
                                                  outerLoopToken3,
                                                  outerLoopClosing,
                                                  outermostToken2};
    auto testRendererChainPtr = templatingengine::RendererChainBuilder::buildRendererChain(tokenPtrList);

    templatingengine::ParameterSet parameterSet;
    parameterSet["outerValueList"] = {"Outer element 1.", "Outer element 2."};
    parameterSet["innerValueList1"] = {"Inner loop 1 element 1.", "Inner loop 1 element 2.", "Inner loop 1 element 3."};
    parameterSet["innerValueList2"] = {"Inner loop 2 element 1.", "Inner loop 2 element 2.", "Inner loop 2 element 3.",
                                       "Inner loop 2 element 4."};

    std::stringstream ss;
    testRendererChainPtr->render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "Outermost text 1."
                              "Outer loop text 1."
                              "Inner loop 1 element 1."
                              "Inner loop 1 element 2."
                              "Inner loop 1 element 3."
                              "Outer element 1."
                              "Inner loop 2 element 1."
                              "Inner loop 2 element 2."
                              "Inner loop 2 element 3."
                              "Inner loop 2 element 4."
                              "Outer loop text 2."
                              "Outer loop text 1."
                              "Inner loop 1 element 1."
                              "Inner loop 1 element 2."
                              "Inner loop 1 element 3."
                              "Outer element 2."
                              "Inner loop 2 element 1."
                              "Inner loop 2 element 2."
                              "Inner loop 2 element 3."
                              "Inner loop 2 element 4."
                              "Outer loop text 2."
                              "Outermost text 2.");
}

TEST_F(RendererChainBuilderTest, TestMissingLoopClosingToken) {
    templatingengine::TokenBasePtr_t loopOpenTokenPtr =
            std::make_shared<templatingengine::LoopOpeningToken>("{{#LOOP valueList element}}", "valueList",
                                                                 "element");
    templatingengine::TokenBasePtr_t variableTokenPtr =
            std::make_shared<templatingengine::VariableToken>("{{element}}", "element");
    templatingengine::TokenBasePtr_t plainTextTokenPtr =
            std::make_shared<templatingengine::PlainTextToken>("Plain Text");
    templatingengine::TokenPtrList_t tokenPtrList { loopOpenTokenPtr, variableTokenPtr, plainTextTokenPtr };
    auto testRendererChainPtr = templatingengine::RendererChainBuilder::buildRendererChain(tokenPtrList);

    templatingengine::ParameterSet parameterSet;
    parameterSet["valueList"] = {"Value 1", "Value 2"};

    std::stringstream ss;
    testRendererChainPtr->render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "{{#LOOP valueList element}}{{element}}Plain Text");
}

