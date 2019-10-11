//
// Created by Cho Yiu Ng on 10.10.19.
//

#ifndef TEMPLATINGENGINE_LOOPOPENINGTOKEN_H
#define TEMPLATINGENGINE_LOOPOPENINGTOKEN_H

#include <string>

#include <TokenBlockOpeningBase.h>

namespace templatingengine {

    class LoopOpeningToken: public TokenBlockOpeningBase {
    public:

        LoopOpeningToken(const std::string& rawTokenText,
                         std::string valueListName,
                         std::string elementName);

        TokenType_t getClosingTokenType() const noexcept override;
        std::string getElementName()      const noexcept;
        TokenType_t getTokenType()        const noexcept override;
        std::string getValueListName()    const noexcept;

    private:

        std::string myElementName;
        std::string myValueListName;

    };

}

#endif //TEMPLATINGENGINE_LOOPOPENINGTOKEN_H
