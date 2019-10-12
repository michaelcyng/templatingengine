//
// Created by Cho Yiu Ng on 08.10.19.
//

#include <gtest/gtest.h>
#include <memory>

#include <renderers/PlainTextRenderer.h>
#include <renderers/RendererChain.h>
#include <dataTypes/StringValue.h>
#include <renderers/VariableRenderer.h>

class RendererChainTest : public ::testing::Test {
protected:

    RendererChainTest() = default;
    ~RendererChainTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(RendererChainTest, TestEmptyRendererChain) {
    templatingengine::RendererChain testrendererChain;
    templatingengine::ParameterSet parameterSet;
    std::stringstream ss;
    testrendererChain.render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "");
}

TEST_F(RendererChainTest, TestLinearRendererChain) {
    templatingengine::RendererChain testrendererChain;

    const std::string& testString = "This is sentence 1.";
    templatingengine::RendererBasePtr_t testRenderer1 = std::make_shared<templatingengine::PlainTextRenderer>(testString);
    testrendererChain.addRenderer(testRenderer1);

    templatingengine::RendererBasePtr_t testRenderer2 = std::make_shared<templatingengine::PlainTextRenderer>("\n");
    testrendererChain.addRenderer(testRenderer2);

    templatingengine::RendererBasePtr_t testRenderer3 = std::make_shared<templatingengine::VariableRenderer>("sentence1");
    testrendererChain.addRenderer(testRenderer3);

    templatingengine::RendererBasePtr_t testRenderer4 = std::make_shared<templatingengine::VariableRenderer>("notExistingVar");
    testrendererChain.addRenderer(testRenderer4);

    templatingengine::RendererBasePtr_t testRenderer5 = std::make_shared<templatingengine::PlainTextRenderer>("");
    testrendererChain.addRenderer(testRenderer5);

    templatingengine::ParameterSet parameterSet;
    parameterSet["sentence1"] = templatingengine::ValueHandler("This is sentence 2.");
    parameterSet["sentence2"] = templatingengine::ValueHandler("Should not appear");

    std::stringstream ss;
    testrendererChain.render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "This is sentence 1.\nThis is sentence 2.{{notExistingVar}}");
}

TEST_F(RendererChainTest, TestNestedRendererChain) {
    templatingengine::RendererBasePtr_t testInnerRendererChain = std::make_shared<templatingengine::RendererChain>();
    
    const std::string& testString1 = "First inner plain text renderer.";
    templatingengine::RendererBasePtr_t testInnerRenderer1 =
            std::make_shared<templatingengine::PlainTextRenderer>(testString1);
    std::static_pointer_cast<templatingengine::RendererChain>(testInnerRendererChain)->addRenderer(testInnerRenderer1);

    templatingengine::RendererBasePtr_t testInnerRenderer2 = std::make_shared<templatingengine::VariableRenderer>("innerVar");
    std::static_pointer_cast<templatingengine::RendererChain>(testInnerRendererChain)->addRenderer(testInnerRenderer2);

    const std::string& testString2 = "Second inner plain text renderer.";
    templatingengine::RendererBasePtr_t testInnerRenderer3 =
            std::make_shared<templatingengine::PlainTextRenderer>(testString2);
    std::static_pointer_cast<templatingengine::RendererChain>(testInnerRendererChain)->addRenderer(testInnerRenderer3);

    templatingengine::RendererChain testOuterRendererChain;

    const std::string& testString3 = "First outer plain text renderer.";
    templatingengine::RendererBasePtr_t testOuterRenderer1 = std::make_shared<templatingengine::PlainTextRenderer>(testString3);
    testOuterRendererChain.addRenderer(testOuterRenderer1);

    templatingengine::RendererBasePtr_t testOuterRenderer2 = std::make_shared<templatingengine::VariableRenderer>("outerVar");
    testOuterRendererChain.addRenderer(testOuterRenderer2);

    testOuterRendererChain.addRenderer(testInnerRendererChain);

    const std::string& testString4 = "Second outer plain text renderer.";
    templatingengine::RendererBasePtr_t testOuterRenderer3 = std::make_shared<templatingengine::PlainTextRenderer>(testString4);
    testOuterRendererChain.addRenderer(testOuterRenderer3);

    templatingengine::ParameterSet parameterSet;
    parameterSet["innerVar"] = templatingengine::ValueHandler("Inner variable renderer.");
    parameterSet["outerVar"] = templatingengine::ValueHandler("Outer variable renderer.");

    std::stringstream ss;
    testOuterRendererChain.render(ss, parameterSet);

    ASSERT_EQ(ss.str(), "First outer plain text renderer."
                              "Outer variable renderer."
                              "First inner plain text renderer."
                              "Inner variable renderer."
                              "Second inner plain text renderer."
                              "Second outer plain text renderer.");

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
