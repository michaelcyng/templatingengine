//
// Created by Cho Yiu Ng on 07.10.19.
//

#include <gtest/gtest.h>
#include <list>

#include <renderers/VariableRenderer.h>

class VariableRendererTest : public ::testing::Test {
protected:

    VariableRendererTest() = default;
    ~VariableRendererTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(VariableRendererTest, TestStringVariableRender) {
    templatingengine::VariableRenderer testRenderer("testVar");
    templatingengine::ParameterSet parameters;
    parameters["testVar"] = "Test Value";
    parameters["wrongVar"] = "Wrong Value";
    std::stringstream ss;
    testRenderer.render(ss, parameters);

    ASSERT_EQ(ss.str(), "Test Value");
}

TEST_F(VariableRendererTest, TestNonExistingVariable) {
    templatingengine::VariableRenderer testRenderer("nonExistingVar");
    templatingengine::ParameterSet parameters;
    parameters["var1"] = "Value1";
    parameters["var2"] = "Value2";
    std::stringstream ss;
    testRenderer.render(ss, parameters);

    ASSERT_EQ(ss.str(), "{{nonExistingVar}}");
}

TEST_F(VariableRendererTest, TestListVariableRender) {
    templatingengine::VariableRenderer testRenderer("listVar");
    templatingengine::ParameterSet parameters;
    parameters["listVar"] = {"Value 1", "Value 2"};
    std::stringstream ss;
    testRenderer.render(ss, parameters);

    ASSERT_EQ(ss.str(), "[Value 1, Value 2]");
}
