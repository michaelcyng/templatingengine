//
// Created by Cho Yiu Ng on 10.10.19.
//

#include <gtest/gtest.h>

#include <tokens/LoopClosingToken.h>
#include <tokens/LoopOpeningToken.h>
#include <tokens/PlainTextToken.h>
#include <builders/RendererChainBuilder.h>
#include <tokens/Token.h>
#include <dataTypes/ValueList.h>
#include <tokens/VariableToken.h>
#include <dataTypes/StringValue.h>

class RendererChainBuilderTest : public ::testing::Test {
protected:

    RendererChainBuilderTest() = default;
    ~RendererChainBuilderTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(RendererChainBuilderTest, TestEmptyTokenChain) {
    templatingengine::TokenPtrList_t emptyTokenPtrList;
    templatingengine::RendererChainBuilder testBuilder;
    auto testRendererChainPtr = testBuilder.buildRendererChain(emptyTokenPtrList);
    templatingengine::ParameterSet parameterSet;
    std::stringstream ss;
    testRendererChainPtr->render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "");
}

TEST_F(RendererChainBuilderTest, TestPlainTextTokens) {
    templatingengine::TokenBasePtr_t testToken1 = std::make_shared<templatingengine::PlainTextToken>("Plain text 1.");
    templatingengine::TokenBasePtr_t testToken2 = std::make_shared<templatingengine::PlainTextToken>("Plain text 2.");
    templatingengine::TokenPtrList_t testTokenPtrList { testToken1, testToken2 };
    templatingengine::RendererChainBuilder testBuilder;
    auto testRendererChainPtr = testBuilder.buildRendererChain(testTokenPtrList);
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
    templatingengine::RendererChainBuilder testBuilder;
    auto testRendererChainPtr = testBuilder.buildRendererChain(testTokenPtrList);
    templatingengine::ParameterSet parameterSet;
    parameterSet["testVariable"] = std::make_shared<templatingengine::StringValue>("Variable text.");
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
    templatingengine::RendererChainBuilder testBuilder;
    auto testRendererChainPtr = testBuilder.buildRendererChain(tokenPtrList);

    templatingengine::ParameterSet parameterSet;
    auto testStringValueList = std::list<templatingengine::ValueBasePtr_t>({
        std::make_shared<templatingengine::StringValue>("Test String 1."),
        std::make_shared<templatingengine::StringValue>("Test String 2."),
        std::make_shared<templatingengine::StringValue>("Test String 3.")
    });
    templatingengine::ValueBasePtr_t valueListPtr = std::make_shared<templatingengine::ValueList>(testStringValueList);
    parameterSet["valueList"] = valueListPtr;

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
    templatingengine::RendererChainBuilder testBuilder;
    auto testRendererChainPtr = testBuilder.buildRendererChain(tokenPtrList);

    auto outerValueList = std::list<templatingengine::ValueBasePtr_t>({
        std::make_shared<templatingengine::StringValue>("Outer element 1."),
        std::make_shared<templatingengine::StringValue>("Outer element 2.")
    });
    templatingengine::ValueBasePtr_t outerValueListPtr = std::make_shared<templatingengine::ValueList>(outerValueList);

    auto innerValueList1 = std::list<templatingengine::ValueBasePtr_t>({
        std::make_shared<templatingengine::StringValue>("Inner loop 1 element 1."),
        std::make_shared<templatingengine::StringValue>("Inner loop 1 element 2."),
        std::make_shared<templatingengine::StringValue>("Inner loop 1 element 3.")
    });
    templatingengine::ValueBasePtr_t innerValueListPtr1 =
            std::make_shared<templatingengine::ValueList>(innerValueList1);

    auto innerValueList2 = std::list<templatingengine::ValueBasePtr_t>({
        std::make_shared<templatingengine::StringValue>("Inner loop 2 element 1."),
        std::make_shared<templatingengine::StringValue>("Inner loop 2 element 2."),
        std::make_shared<templatingengine::StringValue>("Inner loop 2 element 3."),
        std::make_shared<templatingengine::StringValue>("Inner loop 2 element 4.")
    });
    templatingengine::ValueBasePtr_t innerValueListPtr2 =
            std::make_shared<templatingengine::ValueList>(innerValueList2);

    templatingengine::ParameterSet parameterSet;
    parameterSet["outerValueList"] = outerValueListPtr;
    parameterSet["innerValueList1"] = innerValueListPtr1;
    parameterSet["innerValueList2"] = innerValueListPtr2;

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
    templatingengine::RendererChainBuilder testBuilder;
    auto testRendererChainPtr = testBuilder.buildRendererChain(tokenPtrList);

    auto valuePtrList = {
        std::make_shared<templatingengine::StringValue>("Value 1"),
        std::make_shared<templatingengine::StringValue>("Value 2")
    };
    templatingengine::ParameterSet parameterSet;
    parameterSet["valueList"] = std::make_shared<templatingengine::ValueList>(valuePtrList);

    std::stringstream ss;
    testRendererChainPtr->render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "{{#LOOP valueList element}}{{element}}Plain Text");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
