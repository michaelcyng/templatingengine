//
// Created by Cho Yiu Ng on 10.10.19.
//

#include <LoopOpeningToken.h>

using namespace templatingengine;

LoopOpeningToken::LoopOpeningToken(const std::string& rawTokenText,
                                   std::string valueListName,
                                   std::string elementName):
    TokenBlockOpeningBase(rawTokenText), myElementName(std::move(elementName)),
    myValueListName(std::move(valueListName)){
}

TokenType_t LoopOpeningToken::getClosingTokenType() const noexcept {
    return TokenType_t::LoopClose_e;
}

std::string LoopOpeningToken::getElementName() const noexcept {
    return myElementName;
}

TokenType_t LoopOpeningToken::getTokenType() const noexcept {
    return TokenType_t::LoopOpen_e;
}

std::string LoopOpeningToken::getValueListName() const noexcept {
    return myValueListName;
}
