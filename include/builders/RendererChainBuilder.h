//
// Created by Cho Yiu Ng on 10.10.19.
//

#ifndef TEMPLATINGENGINE_RENDERERCHAINBUILDER_H
#define TEMPLATINGENGINE_RENDERERCHAINBUILDER_H

#include <unordered_map>

#include <renderers/RendererChain.h>
#include <tokens/Token.h>

namespace templatingengine {

    class RendererChainBuilder {
    public:

        static RendererChainPtr_t buildRendererChain(const TokenPtrList_t& tokenPtrList);

        typedef RendererBasePtr_t (*StandaloneRendererBuilder_t)(const TokenBasePtr_t&);
        typedef std::unordered_map<TokenType_t, StandaloneRendererBuilder_t, TokenTypeHasher>
                StandaloneRendererBuilderMap_t;

        typedef RendererBasePtr_t (*NonStandaloneRendererBuilder_t) (const TokenPtrListConstIter_t&,
                                                                     const TokenPtrListConstIter_t&);
        typedef std::unordered_map<TokenType_t, NonStandaloneRendererBuilder_t, TokenTypeHasher>
                NonStandaloneRendererBuilderMap_t;

    private:

        static RendererChainPtr_t buildRendererChain(const TokenPtrListConstIter_t& begin,
                                              const TokenPtrListConstIter_t& end);

        // Renderer builders for standalone tokens
        static RendererBasePtr_t buildRenderer(const TokenBasePtr_t& tokenPtr);
        static RendererBasePtr_t buildPlainTextRenderer(const TokenBasePtr_t& tokenPtr);
        static RendererBasePtr_t buildVariableRenderer(const TokenBasePtr_t& tokenPtr);

        // Renderer builders for non-standalone tokens
        static RendererBasePtr_t buildRenderer(const TokenPtrListConstIter_t& begin, const TokenPtrListConstIter_t& end);
        static RendererBasePtr_t buildLoopRenderer(const TokenPtrListConstIter_t& begin,
                                                   const TokenPtrListConstIter_t& end);

        static RendererBasePtr_t buildFallbackRenderer(const std::string& rawTokenText);

        static const StandaloneRendererBuilderMap_t    ourStandaloneRendererBuilderMap;
        static const NonStandaloneRendererBuilderMap_t ourNonStandaloneRendererBuilderMap;

    };

}

#endif //TEMPLATINGENGINE_RENDERERCHAINBUILDER_H
