//
// Created by Cho Yiu Ng on 10.10.19.
//

#ifndef TEMPLATINGENGINE_TOKENLISTBUILDER_H
#define TEMPLATINGENGINE_TOKENLISTBUILDER_H

#include <unordered_map>

#include <Token.h>

namespace templatingengine {

    class TokenListBuilder {
    public:

        TokenListBuilder();

        TokenBasePtr_t buildToken(const std::string& rawTokenText) const;
        TokenPtrList_t buildTokenPtrList(const std::list<std::string>& rawTokenTextList) const;

    private:

        TokenBasePtr_t buildPlainTextToken(const std::string& rawTokenText) const;

        // Builders for tokens other than the plain text token
        TokenBasePtr_t buildFallbackToken(const std::string& rawTokenText, const std::string& commandText) const;
        TokenBasePtr_t buildLoopClosingToken(const std::string& rawTokenText, const std::string& commandText) const;
        TokenBasePtr_t buildLoopOpeningToken(const std::string& rawTokenText, const std::string& commandText) const;
        TokenBasePtr_t buildVariableToken(const std::string& rawTokenText, const std::string& commandText) const;

        typedef TokenBasePtr_t (TokenListBuilder::*TokenBuilder_t)(const std::string& rawTokenText,
                                                                   const std::string& commandText) const;
        typedef std::unordered_map<TokenType_t, TokenBuilder_t, TokenTypeHasher> TokenBuilderMap_t;

        TokenBuilderMap_t myTokenBuilderMap;

    };

}

#endif //TEMPLATINGENGINE_TOKENLISTBUILDER_H
