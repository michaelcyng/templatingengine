//
// Created by Cho Yiu Ng on 10.10.19.
//

#ifndef TEMPLATINGENGINE_TOKENBLOCK_H
#define TEMPLATINGENGINE_TOKENBLOCK_H

#include <tokens/Token.h>

namespace templatingengine {

    class NonStandaloneTokenBase: public TokenBase {
    public:

        explicit NonStandaloneTokenBase(const std::string& rawTokenText): TokenBase(rawTokenText) {}

        bool isStandalone() const noexcept override { return false; };

    private:
    };

}

#endif //TEMPLATINGENGINE_TOKENBLOCK_H
