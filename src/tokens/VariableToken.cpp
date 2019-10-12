//
// Created by Cho Yiu Ng on 10.10.19.
//

#include <tokens/VariableToken.h>

using namespace templatingengine;

VariableToken::VariableToken(const std::string& rawTokenText, std::string variableName):
    StandaloneTokenBase(rawTokenText), myVariableName(std::move(variableName)) {
}

TokenType_t VariableToken::getTokenType() const noexcept {
    return TokenType_t::Variable_e;
}

std::string VariableToken::getVariableName() const noexcept {
    return myVariableName;
}
