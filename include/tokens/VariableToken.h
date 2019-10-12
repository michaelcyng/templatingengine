//
// Created by Cho Yiu Ng on 10.10.19.
//

#ifndef TEMPLATINGENGINE_VARIABLETOKEN_H
#define TEMPLATINGENGINE_VARIABLETOKEN_H

#include <string>

#include <tokens/StandaloneTokenBase.h>

namespace templatingengine {

    class VariableToken: public StandaloneTokenBase {
    public:

        VariableToken(const std::string& rawTokenText, std::string variableName);

        TokenType_t getTokenType() const noexcept override;
        std::string getVariableName() const noexcept;

    private:

        std::string myVariableName;

    };

}

#endif //TEMPLATINGENGINE_VARIABLETOKEN_H
