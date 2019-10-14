//
// Created by Cho Yiu Ng on 10.10.19.
//

#include <algorithm>
#include <regex>

#include <builders/TokenListBuilder.h>

#include <tokens/LoopClosingToken.h>
#include <tokens/LoopOpeningToken.h>
#include <tokens/PlainTextToken.h>
#include <tokens/VariableToken.h>

#include <utils/TokenUtils.h>


using namespace templatingengine;

const TokenListBuilder::TokenBuilderMap_t TokenListBuilder::ourTokenBuilderMap({
    {TokenType_t::LoopClose_e, &TokenListBuilder::buildLoopClosingToken},
    {TokenType_t::LoopOpen_e,  &TokenListBuilder::buildLoopOpeningToken},
    {TokenType_t::Unknown_e,   &TokenListBuilder::buildFallbackToken},
    {TokenType_t::Variable_e,  &TokenListBuilder::buildVariableToken}
});

TokenBasePtr_t TokenListBuilder::buildToken(const std::string &rawTokenText) {

    // Plain text token
    if (!TokenUtils::isCommandTokenText(rawTokenText)) {
        return buildPlainTextToken(rawTokenText);
    }

    // Command token
    const std::string& commandText = TokenUtils::getCommandFromToken(rawTokenText);
    TokenType_t tokenType = (TokenUtils::isVariableCommand(commandText) ?
                             TokenType_t::Variable_e : TokenUtils::getTokenTypeFromCommand(commandText));
    auto builderFunctor = ourTokenBuilderMap.at(tokenType);
    return (*builderFunctor)(rawTokenText, commandText);
}

TokenPtrList_t TokenListBuilder::buildTokenPtrList(const std::list<std::string>& rawTokenTextList) {
    TokenPtrList_t tokenPtrList;
    std::transform(rawTokenTextList.begin(), rawTokenTextList.end(), std::back_inserter(tokenPtrList),
                   &TokenListBuilder::buildToken);

    return tokenPtrList;
}

TokenBasePtr_t TokenListBuilder::buildPlainTextToken(const std::string &rawTokenText) {
    return std::make_shared<PlainTextToken>(rawTokenText);
}

TokenBasePtr_t TokenListBuilder::buildFallbackToken(const std::string &rawTokenText,
                                                    const std::string& commandText) {
    return buildPlainTextToken(rawTokenText);
}

TokenBasePtr_t TokenListBuilder::buildLoopClosingToken(const std::string &rawTokenText,
                                                       const std::string &commandText) {
    return std::make_shared<LoopClosingToken>(rawTokenText);
}

TokenBasePtr_t TokenListBuilder::buildLoopOpeningToken(const std::string &rawTokenText,
                                                       const std::string &commandText) {
    std::regex loopCommandRegex(R"(#LOOP\s+(\S+)\s+(\S+)$)");
    std::smatch loopCommandMatch;

    // Invalid loop syntax
    if (!std::regex_match(commandText, loopCommandMatch, loopCommandRegex)) {
        return buildFallbackToken(rawTokenText, commandText);
    }

    const std::string& valueListName = loopCommandMatch[1].str();
    const std::string& elementName = loopCommandMatch[2].str();

    return std::make_shared<LoopOpeningToken>(rawTokenText, valueListName, elementName);
}

TokenBasePtr_t TokenListBuilder::buildVariableToken(const std::string &rawTokenText,
                                                    const std::string &commandText) {
    return std::make_shared<VariableToken>(rawTokenText, commandText);
}
