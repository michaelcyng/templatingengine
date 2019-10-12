//
// Created by Cho Yiu Ng on 07.10.19.
//

#include <gtest/gtest.h>
#include <list>

#include <dataTypes/StringValue.h>
#include <dataTypes/ValueList.h>
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
    parameters["testVar"] = templatingengine::ValueHandler("Test Value");
    parameters["wrongVar"] = templatingengine::ValueHandler("Wrong Value");
    std::stringstream ss;
    testRenderer.render(ss, parameters);

    ASSERT_EQ(ss.str(), "Test Value");
}

TEST_F(VariableRendererTest, TestNonExistingVariable) {
    templatingengine::VariableRenderer testRenderer("nonExistingVar");
    templatingengine::ParameterSet parameters;
    parameters["var1"] = templatingengine::ValueHandler("Value1");
    parameters["var2"] = templatingengine::ValueHandler("Value2");
    std::stringstream ss;
    testRenderer.render(ss, parameters);

    ASSERT_EQ(ss.str(), "{{nonExistingVar}}");
}

TEST_F(VariableRendererTest, TestListVariableRender) {
    templatingengine::VariableRenderer testRenderer("listVar");
    templatingengine::ParameterSet parameters;
    parameters["listVar"] = std::list<std::string>({"Value 1", "Value 2"});
    std::stringstream ss;
    testRenderer.render(ss, parameters);

    ASSERT_EQ(ss.str(), "[Value 1, Value 2]");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
