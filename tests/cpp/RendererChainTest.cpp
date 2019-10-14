//
// Created by Cho Yiu Ng on 08.10.19.
//

#include <gtest/gtest.h>
#include <memory>

#include <renderers/PlainTextRenderer.h>
#include <renderers/RendererChain.h>
#include <renderers/VariableRenderer.h>

class RendererChainTest : public ::testing::Test {
protected:

    RendererChainTest() = default;
    ~RendererChainTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(RendererChainTest, TestEmptyRendererChain) {
    templatingengine::RendererChain testRendererChain;
    templatingengine::ParameterSet parameterSet;
    std::stringstream ss;
    testRendererChain.render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "");
}

TEST_F(RendererChainTest, TestLinearRendererChain) {
    templatingengine::RendererChain testRendererChain;

    testRendererChain.addRenderer(std::make_shared<templatingengine::PlainTextRenderer>("This is sentence 1."));
    testRendererChain.addRenderer(std::make_shared<templatingengine::PlainTextRenderer>("\n"));
    testRendererChain.addRenderer(std::make_shared<templatingengine::VariableRenderer>("sentence1"));
    testRendererChain.addRenderer(std::make_shared<templatingengine::VariableRenderer>("notExistingVar"));
    testRendererChain.addRenderer(std::make_shared<templatingengine::PlainTextRenderer>(""));

    templatingengine::ParameterSet parameterSet;
    parameterSet["sentence1"] = "This is sentence 2.";
    parameterSet["sentence2"] = "Should not appear";

    std::stringstream ss;
    testRendererChain.render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "This is sentence 1.\nThis is sentence 2.{{notExistingVar}}");
}

TEST_F(RendererChainTest, TestNestedRendererChain) {
    auto testInnerRendererChain = std::make_shared<templatingengine::RendererChain>();
    
    testInnerRendererChain->addRenderer(std::make_shared<templatingengine::PlainTextRenderer>("First inner plain text renderer."));
    testInnerRendererChain->addRenderer(std::make_shared<templatingengine::VariableRenderer>("innerVar"));
    testInnerRendererChain->addRenderer(std::make_shared<templatingengine::PlainTextRenderer>("Second inner plain text renderer."));

    templatingengine::RendererChain testOuterRendererChain;
    testOuterRendererChain.addRenderer(std::make_shared<templatingengine::PlainTextRenderer>("First outer plain text renderer."));
    testOuterRendererChain.addRenderer(std::make_shared<templatingengine::VariableRenderer>("outerVar"));
    testOuterRendererChain.addRenderer(testInnerRendererChain);
    testOuterRendererChain.addRenderer(std::make_shared<templatingengine::PlainTextRenderer>("Second outer plain text renderer."));

    templatingengine::ParameterSet parameterSet;
    parameterSet["innerVar"] = "Inner variable renderer.";
    parameterSet["outerVar"] = "Outer variable renderer.";

    std::stringstream ss;
    testOuterRendererChain.render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "First outer plain text renderer."
                              "Outer variable renderer."
                              "First inner plain text renderer."
                              "Inner variable renderer."
                              "Second inner plain text renderer."
                              "Second outer plain text renderer.");

}
