//
// Created by Cho Yiu Ng on 08.10.19.
//

#include <gtest/gtest.h>
#include <memory>

#include <renderers/LoopRenderer.h>
#include <renderers/PlainTextRenderer.h>
#include <dataTypes/StringValue.h>
#include <dataTypes/ValueList.h>
#include <renderers/VariableRenderer.h>

class LoopRendererTest : public ::testing::Test {
protected:

    LoopRendererTest() = default;
    ~LoopRendererTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(LoopRendererTest, TestSingleLoop) {
    templatingengine::RendererChainPtr_t testRendererChainPtr = std::make_shared<templatingengine::RendererChain>();
    templatingengine::RendererBasePtr_t testVariableRendererPtr =
            std::make_shared<templatingengine::VariableRenderer>("testListElement");
    testRendererChainPtr->addRenderer(testVariableRendererPtr);

    templatingengine::RendererBasePtr_t testPlainTextRendererPtr =
            std::make_shared<templatingengine::PlainTextRenderer>(".");
    testRendererChainPtr->addRenderer(testPlainTextRendererPtr);

    templatingengine::ParameterSet parameterSet;
    parameterSet["testList"] = {"Value 1", "Value 2", "Value 3"};

    templatingengine::LoopRenderer testLoopRender("testList",
                                                  "testListElement",
                                                  testRendererChainPtr);
    std::stringstream ss;
    testLoopRender.render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "Value 1.Value 2.Value 3.");
}

TEST_F(LoopRendererTest, TestEmptyList) {
    templatingengine::RendererBasePtr_t testRendererPtr1 = std::make_shared<templatingengine::PlainTextRenderer>("a");

    templatingengine::RendererChainPtr_t testRendererChainPtr = std::make_shared<templatingengine::RendererChain>();
    testRendererChainPtr->addRenderer(testRendererPtr1);

    templatingengine::ValueHandler emptyValueListPtr = std::static_pointer_cast<templatingengine::ValueBase>(std::make_shared<templatingengine::ValueList>());

    templatingengine::ParameterSet parameterSet;
    parameterSet["emptyList"] = emptyValueListPtr;

    templatingengine::LoopRenderer testLoopRenderer("emptyList",
                                        "element",
                                                  testRendererChainPtr);
    std::stringstream ss;
    testLoopRenderer.render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "");
}

TEST_F(LoopRendererTest, TestNonExistingList) {
    templatingengine::RendererBasePtr_t testRendererPtr1 = std::make_shared<templatingengine::PlainTextRenderer>("a");

    templatingengine::RendererChainPtr_t testRendererChainPtr = std::make_shared<templatingengine::RendererChain>();
    testRendererChainPtr->addRenderer(testRendererPtr1);

    templatingengine::ValueHandler emptyValueListPtr = std::static_pointer_cast<templatingengine::ValueBase>(
            std::make_shared<templatingengine::ValueList>());

    templatingengine::ParameterSet parameterSet;

    templatingengine::LoopRenderer testLoopRenderer("nonExistingList",
                                                    "element",
                                                    testRendererChainPtr);
    std::stringstream ss;
    testLoopRenderer.render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "");
}

TEST_F(LoopRendererTest, TestSingleValueAsList) {
    templatingengine::RendererChainPtr_t testRendererChainPtr = std::make_shared<templatingengine::RendererChain>();
    templatingengine::RendererBasePtr_t testVariableRendererPtr =
            std::make_shared<templatingengine::VariableRenderer>("testListElement");
    testRendererChainPtr->addRenderer(testVariableRendererPtr);

    templatingengine::RendererBasePtr_t testPlainTextRendererPtr =
            std::make_shared<templatingengine::PlainTextRenderer>(".");
    testRendererChainPtr->addRenderer(testPlainTextRendererPtr);

    templatingengine::ValueHandler testSingleValuePtr("Test String");
    templatingengine::ParameterSet parameterSet;
    parameterSet["testList"] = "Test String";

    templatingengine::LoopRenderer testLoopRenderer("testList",
                                                  "testListElement",
                                                  testRendererChainPtr);
    std::stringstream ss;
    testLoopRenderer.render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "Test String.");
}

TEST_F(LoopRendererTest, TestLocalVarOverrideGlobal) {
    templatingengine::RendererChainPtr_t testLoopRendererChainPtr = std::make_shared<templatingengine::RendererChain>();
    templatingengine::RendererBasePtr_t testLoopVariableRendererPtr =
            std::make_shared<templatingengine::VariableRenderer>("testListElement");
    testLoopRendererChainPtr->addRenderer(testLoopVariableRendererPtr);

    templatingengine::RendererBasePtr_t testPlainTextRendererPtr =
            std::make_shared<templatingengine::PlainTextRenderer>(".");
    testLoopRendererChainPtr->addRenderer(testPlainTextRendererPtr);

    templatingengine::ParameterSet parameterSet;
    parameterSet["testList"] = {"Inner String 1", "Inner String 2", "Inner String 3"};

    templatingengine::RendererBasePtr_t testLoopRendererPtr =
            std::make_shared<templatingengine::LoopRenderer>("testList",
                                                             "testListElement",
                                                             testLoopRendererChainPtr);

    templatingengine::RendererBasePtr_t testVariableRendererPtr =
            std::make_shared<templatingengine::VariableRenderer>("testListElement");

    templatingengine::RendererChain testRendererChain;
    testRendererChain.addRenderer(testVariableRendererPtr);
    testRendererChain.addRenderer(testLoopRendererPtr);

    parameterSet["testListElement"] = "Outer String.";
    std::stringstream ss;
    testRendererChain.render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "Outer String.Inner String 1.Inner String 2.Inner String 3.");
}

TEST_F(LoopRendererTest, TestNestedLoop) {
    templatingengine::RendererBasePtr_t innerLoopVariableRendererPtr =
            std::make_shared<templatingengine::VariableRenderer>("innerLoopElement");
    templatingengine::RendererChainPtr_t innerLoopRendererChainPtr =
            std::make_shared<templatingengine::RendererChain>();
    innerLoopRendererChainPtr->addRenderer(innerLoopVariableRendererPtr);
    templatingengine::RendererBasePtr_t innerLoopRendererPtr =
            std::make_shared<templatingengine::LoopRenderer>("innerValueList",
                                                             "innerLoopElement",
                                                             innerLoopRendererChainPtr);

    templatingengine::RendererBasePtr_t outerLoopVariableRendererPtr =
            std::make_shared<templatingengine::VariableRenderer>("outputLoopOpenElement");
    templatingengine::RendererChainPtr_t outerLoopRendererChainPtr =
            std::make_shared<templatingengine::RendererChain>();
    outerLoopRendererChainPtr->addRenderer(outerLoopVariableRendererPtr);
    outerLoopRendererChainPtr->addRenderer(innerLoopRendererPtr);
    templatingengine::LoopRenderer outerLoopRenderer("outerOpenValueList",
                                                     "outputLoopOpenElement",
                                                     outerLoopRendererChainPtr);

    templatingengine::ParameterSet parameterSet;
    parameterSet["innerValueList"] = {"Inner String 1.", "Inner String 2.", "Inner String 3."};
    parameterSet["outerOpenValueList"] = {"Outer Opening String 1.", "Outer Opening String 2.",
                                          "Outer Opening String 3."};

    std::stringstream ss;
    outerLoopRenderer.render(ss, parameterSet);

    ASSERT_EQ(ss.str(),"Outer Opening String 1."
                                  "Inner String 1."
                                  "Inner String 2."
                                  "Inner String 3."
                                  "Outer Opening String 2."
                                  "Inner String 1."
                                  "Inner String 2."
                                  "Inner String 3."
                                  "Outer Opening String 3."
                                  "Inner String 1."
                                  "Inner String 2."
                                  "Inner String 3.");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
