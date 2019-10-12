//
// Created by Cho Yiu Ng on 10.10.19.
//

#include <tokens/PlainTextToken.h>

using namespace templatingengine;

PlainTextToken::PlainTextToken(const std::string& text): StandaloneTokenBase(text), myText(text) {
}

std::string PlainTextToken::getText() const noexcept {
    return myText;
}

TokenType_t PlainTextToken::getTokenType() const noexcept {
    return TokenType_t::PlainText_e;
}
