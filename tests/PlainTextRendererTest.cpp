//
// Created by Cho Yiu Ng on 07.10.19.
//

#include <gtest/gtest.h>

#include <PlainTextRenderer.h>

class PlainTextRendererTest : public ::testing::Test {
protected:

    PlainTextRendererTest() = default;
    ~PlainTextRendererTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(PlainTextRendererTest, TestEmptyTextRender) {
    templatingengine::PlainTextRenderer testRenderer("");
    std::stringstream ss;
    testRenderer.render(ss, templatingengine::ParameterSet());

    ASSERT_EQ(ss.str(), "");
}

TEST_F(PlainTextRendererTest, TestAppend) {
    const std::string& testString = "Test line 1\nTest line 2";
    templatingengine::PlainTextRenderer testRenderer(testString);
    std::stringstream ss;
    testRenderer.render(ss, templatingengine::ParameterSet());

    ASSERT_EQ(ss.str(), testString);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
