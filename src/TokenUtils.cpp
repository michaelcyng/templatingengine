//
// Created by Cho Yiu Ng on 10.10.19.
//

#include <sstream>

#include <TokenBlockOpeningBase.h>
#include <TokenUtils.h>

using namespace templatingengine;

const std::regex TokenUtils::ourCommandRegex(R"(^\{\{[^\{\}]*\}\}$)");

const std::unordered_map<std::string, TokenType_t> TokenUtils::ourTokenTypeMap({
    {"#LOOP", TokenType_t::LoopOpen_e},
    {"/LOOP", TokenType_t::LoopClose_e}
});

TokenPtrListConstIter_t TokenUtils::findClosingTokenPtrIter(TokenPtrListConstIter_t openingTokenPtrIter,
                                                            TokenPtrListConstIter_t end) {
    unsigned int level = 0;
    const auto& openingTokenPtr = *openingTokenPtrIter;
    for (auto candidateIter = openingTokenPtrIter; candidateIter != end; ++candidateIter) {
        const auto& candidateTokenPtr = *candidateIter;

        if (candidateTokenPtr->isStandalone()) {
            continue;
        }

        if (candidateTokenPtr->getTokenType() == openingTokenPtr->getTokenType()) {
            ++level;
            continue;
        }

        if (candidateTokenPtr->getTokenType() ==
            std::static_pointer_cast<TokenBlockOpeningBase>(openingTokenPtr)->getClosingTokenType()) {
            --level;
            if (level == 0) {
                return candidateIter;
            }
        }
    }

    return end;
}

bool TokenUtils::isCommandTokenText(const std::string &tokenText) {
    return std::regex_match(tokenText, ourCommandRegex);
}

std::string TokenUtils::getCommandFromToken(const std::string &tokenText) {
    return std::regex_replace(tokenText, std::regex(R"(^\{\{\s*|\s*\}\}$)"), "");
}

TokenType_t TokenUtils::getTokenTypeFromCommand(const std::string &command) {
    std::smatch commandNameMatch;
    const std::regex commandNameRegex(R"(^\S*)");
    std::regex_search(command, commandNameMatch, commandNameRegex);
    const std::string& commandName = *(commandNameMatch.cbegin());

    auto tokenTypeMapIter = ourTokenTypeMap.find(commandName);
    return (tokenTypeMapIter == ourTokenTypeMap.end() ? TokenType_t::Unknown_e : tokenTypeMapIter->second);
}

// Caution: For efficiency reasons, it is assumed that the token text is a command
bool TokenUtils::isVariableCommand(const std::string &command) {
    return (command.length() == 0 || (command.at(0) != '#' && command.at(0) != '/'));
}

