//
// Created by Cho Yiu Ng on 10.10.19.
//

#ifndef TEMPLATINGENGINE_PLAINTEXTTOKEN_H
#define TEMPLATINGENGINE_PLAINTEXTTOKEN_H

#include <string>

#include <StandaloneTokenBase.h>

namespace templatingengine {

    class PlainTextToken: public StandaloneTokenBase {
    public:

        explicit PlainTextToken(const std::string& text);

        std::string getText() const noexcept;
        TokenType_t getTokenType() const noexcept override;

    private:

        std::string myText;
    };

}

#endif //TEMPLATINGENGINE_PLAINTEXTTOKEN_H
