//
// Created by Cho Yiu Ng on 10.10.19.
//

#ifndef TEMPLATINGENGINE_TOKENUTILS_H
#define TEMPLATINGENGINE_TOKENUTILS_H

#include <regex>
#include <unordered_map>

#include <Token.h>

namespace templatingengine {

    class TokenUtils {
    public:

        static TokenPtrListConstIter_t findClosingTokenPtrIter(TokenPtrListConstIter_t openingTokenPtrIter,
                                                               TokenPtrListConstIter_t end);

        static bool        isCommandTokenText(const std::string& tokenText);
        static std::string getCommandFromToken(const std::string& tokenText); // Caution: assume the token is command
        static TokenType_t getTokenTypeFromCommand(const std::string& command);
        static bool        isVariableCommand(const std::string& command);

    private:

        static const std::regex                                   ourCommandRegex;
        static const std::unordered_map<std::string, TokenType_t> ourTokenTypeMap;

    };

}

#endif //TEMPLATINGENGINE_TOKENUTILS_H
