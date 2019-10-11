//
// Created by Cho Yiu Ng on 07.10.19.
//

#include <gtest/gtest.h>
#include <list>

#include <StringValue.h>
#include <ValueList.h>
#include <VariableRenderer.h>

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
    parameters["testVar"] = std::make_shared<templatingengine::StringValue>("Test Value");
    parameters["wrongVar"] = std::make_shared<templatingengine::StringValue>("Wrong Value");
    std::stringstream ss;
    testRenderer.render(ss, parameters);

    ASSERT_EQ(ss.str(), "Test Value");
}

TEST_F(VariableRendererTest, TestNonExistingVariable) {
    templatingengine::VariableRenderer testRenderer("nonExistingVar");
    templatingengine::ParameterSet parameters;
    parameters["var1"] = std::make_shared<templatingengine::StringValue>("Value1");
    parameters["var2"] = std::make_shared<templatingengine::StringValue>("Value2");
    std::stringstream ss;
    testRenderer.render(ss, parameters);

    ASSERT_EQ(ss.str(), "{{nonExistingVar}}");
}

TEST_F(VariableRendererTest, TestListVariableRender) {
    templatingengine::VariableRenderer testRenderer("listVar");
    templatingengine::ParameterSet parameters;
    auto value1Ptr = std::make_shared<templatingengine::StringValue>("Value 1");
    auto value2Ptr = std::make_shared<templatingengine::StringValue>("Value 2");
    parameters["listVar"] = std::make_shared<templatingengine::ValueList>(std::list<templatingengine::ValueBasePtr_t>(
            {value1Ptr, value2Ptr}));
    std::stringstream ss;
    testRenderer.render(ss, parameters);

    ASSERT_EQ(ss.str(), "[Value 1, Value 2]");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
