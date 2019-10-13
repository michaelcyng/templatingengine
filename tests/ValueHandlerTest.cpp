//
// Created by Cho Yiu Ng on 13.10.19.
//

#include <gtest/gtest.h>
#include <list>
#include <vector>

#include <dataTypes/StringValue.h>
#include <dataTypes/ValueHandler.h>
#include <dataTypes/ValueList.h>

class ValueHandlerTest : public ::testing::Test {
protected:

    ValueHandlerTest() = default;
    ~ValueHandlerTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(ValueHandlerTest, TestStringAssignment) {
    templatingengine::ValueHandler valueHandler = "This is a const char*";
    templatingengine::ValueBasePtr_t valuePtr = valueHandler;
    ASSERT_EQ(std::static_pointer_cast<templatingengine::StringValue>(valuePtr)->getValue(),
            "This is a const char*");

    templatingengine::ValueHandler valueHandler1 = std::string("This is a string");
    valuePtr = valueHandler1;
    ASSERT_EQ(std::static_pointer_cast<templatingengine::StringValue>(valuePtr)->getValue(),
              "This is a string");
}

TEST_F(ValueHandlerTest, TestStringListAssignment) {
    templatingengine::ValueHandler valueHandler = {"string 1", "string 2"};
    std::list<std::string> valueList;
    for (const auto& valuePtr: static_cast<templatingengine::ValueList>(valueHandler)) {
        valueList.emplace_back(std::static_pointer_cast<templatingengine::StringValue>(valuePtr)->getValue());
    }
    ASSERT_EQ(valueList, std::list<std::string>({"string 1", "string 2"}));

    valueHandler = {};
    const auto& emptyList = static_cast<templatingengine::ValueList>(valueHandler);
    ASSERT_TRUE(emptyList.empty());

    valueHandler = std::list<std::string>({"string 3", "string 4"});
    valueList.clear();
    for (const auto& valuePtr: static_cast<templatingengine::ValueList>(valueHandler)) {
        valueList.emplace_back(std::static_pointer_cast<templatingengine::StringValue>(valuePtr)->getValue());
    }
    ASSERT_EQ(valueList, std::list<std::string>({"string 3", "string 4"}));

    valueHandler = std::vector<std::string>({"string 5", "string 6"});
    valueList.clear();
    for (const auto& valuePtr: static_cast<templatingengine::ValueList>(valueHandler)) {
        valueList.emplace_back(std::static_pointer_cast<templatingengine::StringValue>(valuePtr)->getValue());
    }
    ASSERT_EQ(valueList, std::list<std::string>({"string 5", "string 6"}));

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
