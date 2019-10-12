//
// Created by Cho Yiu Ng on 11.10.19.
//

#include <gtest/gtest.h>
#include <sstream>

#include <ParameterSet.h>
#include <dataTypes/StringValue.h>
#include <Template.h>
#include <dataTypes/ValueList.h>

class TemplateTest : public ::testing::Test {
protected:

    TemplateTest() = default;
    ~TemplateTest() override = default;
    void SetUp() override {}
    void TearDown() override {}

};

TEST_F(TemplateTest, TestEmptyTemplate) {
    std::stringstream templateStream;
    templatingengine::Template testTemplate(templateStream);
    templatingengine::ParameterSet parameterSet;
    std::stringstream resultStream;
    testTemplate.generateDocument(parameterSet, resultStream);

    ASSERT_EQ(resultStream.str(), "");
}

TEST_F(TemplateTest, TestPlainTextTemplate) {
    const std::string& templateText = "This is line 1.\nThis is line 2.";
    std::stringstream templateStream(templateText);
    templatingengine::Template testTemplate(templateStream);
    templatingengine::ParameterSet parameterSet;
    std::stringstream resultStream;
    testTemplate.generateDocument(parameterSet, resultStream);

    ASSERT_EQ(resultStream.str(), templateText);
}

TEST_F(TemplateTest, TestTemplateWithVariables) {
    const std::string& templateText =
            "This is line 1.\n"
            "Here is {{variable1}}.\n"
            "This is line 2.\n"
            "Here is {{variable2}}.\n";
    std::stringstream templateStream(templateText);
    templatingengine::Template testTemplate(templateStream);

    templatingengine::ParameterSet parameterSet;
    parameterSet["variable1"] = "variable 1";
    parameterSet["variable2"] = "variable 2";

    std::stringstream resultStream;
    testTemplate.generateDocument(parameterSet, resultStream);

    const std::string& expectedText =
            "This is line 1.\n"
            "Here is variable 1.\n"
            "This is line 2.\n"
            "Here is variable 2.\n";
    ASSERT_EQ(resultStream.str(), expectedText);
}

TEST_F(TemplateTest, TestTemplateWithLoop) {
    const std::string& templateText =
            "First line.\n"
            "{{header}}\n"
            "{{#LOOP somearray item}}"
            "This is a {{item}}.\n"
            "{{/LOOP}}"
            "{{footer}}\n"
            "Last line.";
    std::stringstream templateStream(templateText);
    templatingengine::Template testTemplate(templateStream);

    templatingengine::ParameterSet parameterSet;
    parameterSet["header"] = "Hello!";
    parameterSet["somearray"] = std::list<std::string>({"apple", "banana", "citrus"});
    parameterSet["footer"] = "That's it!";

    std::stringstream resultStream;
    testTemplate.generateDocument(parameterSet, resultStream);

    const std::string& expectedText =
            "First line.\n"
            "Hello!\n"
            "This is a apple.\n"
            "This is a banana.\n"
            "This is a citrus.\n"
            "That's it!\n"
            "Last line.";
    ASSERT_EQ(resultStream.str(), expectedText);
}

TEST_F(TemplateTest, TestTemplateWithNestedLoop) {
    const std::string& templateText =
            "This is the first line of the whole text.\n"
            "{{#LOOP outerList outerElement}}"
            "This is the first line of outer loop.\n"
            "Outer loop element: {{outerElement}}\n"
            "{{#LOOP innerList innerElement}}"
            "Inside inner loop.\n"
            "Outer loop element: {{outerElement}}\n"
            "Inner loop element: {{innerElement}}\n"
            "End of inner loop.\n"
            "{{/LOOP}}"
            "This is the last line of outer loop.\n"
            "{{/LOOP}}"
            "This is the last line of the whole text.";
    std::stringstream templateStream(templateText);
    templatingengine::Template testTemplate(templateStream);

    templatingengine::ParameterSet parameterSet;
    parameterSet["outerList"] = std::list<std::string>({"outer value 1", "outer value 2"});
    parameterSet["innerList"] = std::list<std::string>({"inner value 1", "inner value 2", "inner value 3"});

    std::stringstream resultStream;
    testTemplate.generateDocument(parameterSet, resultStream);

    const std::string& expectedText =
            "This is the first line of the whole text.\n"
            "This is the first line of outer loop.\n"
            "Outer loop element: outer value 1\n"
            "Inside inner loop.\n"
            "Outer loop element: outer value 1\n"
            "Inner loop element: inner value 1\n"
            "End of inner loop.\n"
            "Inside inner loop.\n"
            "Outer loop element: outer value 1\n"
            "Inner loop element: inner value 2\n"
            "End of inner loop.\n"
            "Inside inner loop.\n"
            "Outer loop element: outer value 1\n"
            "Inner loop element: inner value 3\n"
            "End of inner loop.\n"
            "This is the last line of outer loop.\n"
            "This is the first line of outer loop.\n"
            "Outer loop element: outer value 2\n"
            "Inside inner loop.\n"
            "Outer loop element: outer value 2\n"
            "Inner loop element: inner value 1\n"
            "End of inner loop.\n"
            "Inside inner loop.\n"
            "Outer loop element: outer value 2\n"
            "Inner loop element: inner value 2\n"
            "End of inner loop.\n"
            "Inside inner loop.\n"
            "Outer loop element: outer value 2\n"
            "Inner loop element: inner value 3\n"
            "End of inner loop.\n"
            "This is the last line of outer loop.\n"
            "This is the last line of the whole text.";
    ASSERT_EQ(resultStream.str(), expectedText);
}

TEST_F(TemplateTest, TestEscapeCharacter) {
    const std::string& templateText =
            "Enclosed variable: \\{\\{{{variable}}}}\n"
            "Escape backslash: \\\\\n"
            "No characters are escaped: \\a\\}\\";
    std::stringstream templateStream(templateText);
    templatingengine::Template testTemplate(templateStream);

    templatingengine::ParameterSet parameterSet;
    parameterSet["variable"] = "variable value here";

    std::stringstream resultStream;
    testTemplate.generateDocument(parameterSet, resultStream);

    const std::string& expectedText =
            "Enclosed variable: {{variable value here}}\n"
            "Escape backslash: \\\n"
            "No characters are escaped: \\a\\}\\";
    ASSERT_EQ(resultStream.str(), expectedText);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
