//
// Created by Cho Yiu Ng on 09.10.19.
//

#ifndef TEMPLATINGENGINE_TOKEN_H
#define TEMPLATINGENGINE_TOKEN_H

#include <list>
#include <memory>
#include <string>

namespace templatingengine {

    enum class TokenType_t {
        LoopClose_e = 0,
        LoopOpen_e  = 1,
        PlainText_e = 2,
        Unknown_e   = 3,
        Variable_e  = 4
    };

    struct TokenTypeHasher {

        template <typename T>
        size_t operator()(T tokenType) const {
            return static_cast<size_t>(tokenType);
        }
    };

    class TokenBase {
    public:

        explicit TokenBase(std::string rawTokenText): myRawTokenText(std::move(rawTokenText)) {}

        std::string getRawTokenText() const noexcept { return myRawTokenText; }

        virtual TokenType_t getTokenType() const noexcept = 0;
        virtual bool        isStandalone() const noexcept = 0;

    private:

        std::string myRawTokenText;
    };

    typedef std::shared_ptr<TokenBase>     TokenBasePtr_t;
    typedef std::list<TokenBasePtr_t>      TokenPtrList_t;
    typedef TokenPtrList_t::const_iterator TokenPtrListConstIter_t;

}


#endif //TEMPLATINGENGINE_TOKEN_H
