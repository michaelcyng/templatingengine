//
// Created by Cho Yiu Ng on 11.10.19.
//

#include <gtest/gtest.h>
#include <sstream>

#include <utils/EscapeStream.h>

class EscapeStreamTest : public ::testing::Test {
protected:

    EscapeStreamTest() = default;
    ~EscapeStreamTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(EscapeStreamTest, TestEmptyStream) {
    std::stringstream ss;
    templatingengine::EscapeStream es(ss);

    ASSERT_TRUE(es.isEof());
}

TEST_F(EscapeStreamTest, TestNoEscapeCharacter) {
    const std::string& testString = "This is a text";
    std::stringstream testInputStream(testString);
    templatingengine::EscapeStream es(testInputStream);
    std::stringstream ss;
    while (!es.isEof()) {
        ss << es.getChar().get();
    }
    ASSERT_EQ(ss.str(), testString);
}

TEST_F(EscapeStreamTest, TestEscapeCharacter) {
    const std::string& testString = R"(\\\{\a\)";
    std::stringstream testInputStream(testString);
    templatingengine::EscapeStream es(testInputStream);

    auto ch = es.getChar();
    ASSERT_TRUE(ch.isEscaped());
    ASSERT_EQ(ch.get(), '\\');

    ch = es.getChar();
    ASSERT_TRUE(ch.isEscaped());
    ASSERT_EQ(ch.get(), '{');

    ch = es.getChar();
    ASSERT_FALSE(ch.isEscaped());
    ASSERT_EQ(ch.get(), '\\');

    ch = es.getChar();
    ASSERT_FALSE(ch.isEscaped());
    ASSERT_EQ(ch.get(), 'a');

    ch = es.getChar();
    ASSERT_FALSE(ch.isEscaped());
    ASSERT_EQ(ch.get(), '\\');
}
