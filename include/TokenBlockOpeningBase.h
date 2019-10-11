//
// Created by Cho Yiu Ng on 10.10.19.
//

#ifndef TEMPLATINGENGINE_TOKENBLOCKOPENING_H
#define TEMPLATINGENGINE_TOKENBLOCKOPENING_H

#include <NonStandaloneTokenBase.h>

namespace templatingengine {

    class TokenBlockOpeningBase: public NonStandaloneTokenBase {
    public:

        explicit TokenBlockOpeningBase(const std::string& rawTokenText): NonStandaloneTokenBase(rawTokenText) {}

        virtual TokenType_t getClosingTokenType() const noexcept = 0;

    };

}

#endif //TEMPLATINGENGINE_TOKENBLOCKOPENING_H
