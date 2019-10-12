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

TokenListBuilder::TokenListBuilder(): myTokenBuilderMap({
    {TokenType_t::LoopClose_e, &TokenListBuilder::buildLoopClosingToken},
    {TokenType_t::LoopOpen_e, &TokenListBuilder::buildLoopOpeningToken},
    {TokenType_t::Unknown_e, &TokenListBuilder::buildFallbackToken},
    {TokenType_t::Variable_e, &TokenListBuilder::buildVariableToken}
}) {
}

TokenBasePtr_t TokenListBuilder::buildToken(const std::string &rawTokenText) const {

    // Plain text token
    if (!TokenUtils::isCommandTokenText(rawTokenText)) {
        return buildPlainTextToken(rawTokenText);
    }

    // Command token
    const std::string& commandText = TokenUtils::getCommandFromToken(rawTokenText);
    TokenType_t tokenType = (TokenUtils::isVariableCommand(commandText) ?
                             TokenType_t::Variable_e : TokenUtils::getTokenTypeFromCommand(commandText));
    auto builderFunctor = myTokenBuilderMap.at(tokenType);
    return (this->*builderFunctor)(rawTokenText, commandText);
}

TokenPtrList_t TokenListBuilder::buildTokenPtrList(const std::list<std::string>& rawTokenTextList) const {
    TokenPtrList_t tokenPtrList;
    std::transform(rawTokenTextList.begin(), rawTokenTextList.end(), std::back_inserter(tokenPtrList),
                   [this](const std::string &tokenText) -> TokenBasePtr_t { return buildToken(tokenText); });

    return tokenPtrList;
}

TokenBasePtr_t TokenListBuilder::buildPlainTextToken(const std::string &rawTokenText) const {
    return std::make_shared<PlainTextToken>(rawTokenText);
}

TokenBasePtr_t TokenListBuilder::buildFallbackToken(const std::string &rawTokenText,
                                                    const std::string& commandText) const {
    return buildPlainTextToken(rawTokenText);
}

TokenBasePtr_t TokenListBuilder::buildLoopClosingToken(const std::string &rawTokenText,
                                                       const std::string &commandText) const {
    return std::make_shared<LoopClosingToken>(rawTokenText);
}

TokenBasePtr_t TokenListBuilder::buildLoopOpeningToken(const std::string &rawTokenText,
                                                       const std::string &commandText) const {
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
                                                    const std::string &commandText) const {
    return std::make_shared<VariableToken>(rawTokenText, commandText);
}
