//
// Created by Cho Yiu Ng on 10.10.19.
//

#ifndef TEMPLATINGENGINE_TOKENLISTBUILDER_H
#define TEMPLATINGENGINE_TOKENLISTBUILDER_H

#include <unordered_map>

#include <tokens/Token.h>

namespace templatingengine {

    class TokenListBuilder {
    public:

        static TokenBasePtr_t buildToken(const std::string& rawTokenText);
        static TokenPtrList_t buildTokenPtrList(const std::list<std::string>& rawTokenTextList);

        typedef TokenBasePtr_t (*TokenBuilder_t)(const std::string& rawTokenText, const std::string& commandText);
        typedef std::unordered_map<TokenType_t, TokenBuilder_t, TokenTypeHasher> TokenBuilderMap_t;

    private:

        static TokenBasePtr_t buildPlainTextToken(const std::string& rawTokenText);

        // Builders for tokens other than the plain text token
        static TokenBasePtr_t buildFallbackToken(const std::string& rawTokenText, const std::string& commandText);
        static TokenBasePtr_t buildLoopClosingToken(const std::string& rawTokenText, const std::string& commandText);
        static TokenBasePtr_t buildLoopOpeningToken(const std::string& rawTokenText, const std::string& commandText);
        static TokenBasePtr_t buildVariableToken(const std::string& rawTokenText, const std::string& commandText);

        static const TokenBuilderMap_t ourTokenBuilderMap;

    };

}

#endif //TEMPLATINGENGINE_TOKENLISTBUILDER_H
