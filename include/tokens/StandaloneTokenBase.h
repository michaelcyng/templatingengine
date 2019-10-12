//
// Created by Cho Yiu Ng on 10.10.19.
//

#ifndef TEMPLATINGENGINE_STANDALONETOKENBASE_H
#define TEMPLATINGENGINE_STANDALONETOKENBASE_H

#include <tokens/Token.h>

namespace templatingengine {

    class StandaloneTokenBase: public TokenBase {
    public:

        explicit StandaloneTokenBase(const std::string& rawTokenText): TokenBase(rawTokenText) {}

        bool isStandalone() const noexcept override { return true; }

    private:
    };

}

#endif //TEMPLATINGENGINE_STANDALONETOKENBASE_H
