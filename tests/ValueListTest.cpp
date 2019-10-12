//
// Created by Cho Yiu Ng on 07.10.19.
//

#include <gtest/gtest.h>
#include <list>

#include <dataTypes/StringValue.h>
#include <dataTypes/ValueList.h>

class ValueListTest : public ::testing::Test {
protected:

    ValueListTest() = default;
    ~ValueListTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(ValueListTest, TestIterator) {
    auto testStringValueList = std::list<templatingengine::ValueHandler>({
        templatingengine::ValueHandler("Test String 1"),
        templatingengine::ValueHandler("Test String 2")
    });
    templatingengine::ValueList testValueList(testStringValueList);
    auto iter = testValueList.begin();
    ASSERT_EQ(std::static_pointer_cast<templatingengine::StringValue>(
            static_cast<templatingengine::ValueBasePtr_t>(*iter))->getValue(), "Test String 1");
    ++iter;
    ASSERT_EQ(std::static_pointer_cast<templatingengine::StringValue>(
            static_cast<templatingengine::ValueBasePtr_t>(*iter))->getValue(), "Test String 2");
    ++iter;
    ASSERT_EQ(iter, testValueList.end());
}

TEST_F(ValueListTest, TestPrint) {
    auto testStringValueList = std::list<templatingengine::ValueHandler>({
        templatingengine::ValueHandler("Test String 1"),
        templatingengine::ValueHandler("Test String 2")
    });
    templatingengine::ValueList testValueList(testStringValueList);
    std::stringstream ss;
    ss << testValueList;
    ASSERT_EQ(ss.str(), "[Test String 1, Test String 2]");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
