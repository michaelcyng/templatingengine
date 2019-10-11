//
// Created by Cho Yiu Ng on 10.10.19.
//

#ifndef TEMPLATINGENGINE_LOOPCLOSINGTOKEN_H
#define TEMPLATINGENGINE_LOOPCLOSINGTOKEN_H

#include <NonStandaloneTokenBase.h>

namespace templatingengine {

    class LoopClosingToken: public NonStandaloneTokenBase {
    public:

        explicit LoopClosingToken(const std::string& rawTokenText): NonStandaloneTokenBase(rawTokenText) {}

        TokenType_t getTokenType() const noexcept override { return TokenType_t::LoopClose_e; }

    private:
    };

}

#endif //TEMPLATINGENGINE_LOOPCLOSINGTOKEN_H
