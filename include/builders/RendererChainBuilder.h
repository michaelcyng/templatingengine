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

        RendererChainBuilder();

        RendererChainPtr_t buildRendererChain(const TokenPtrList_t& tokenPtrList) const;

    private:

        RendererChainPtr_t buildRendererChain(const TokenPtrListConstIter_t& begin,
                                              const TokenPtrListConstIter_t& end) const;

        // Renderer builders for standalone tokens
        RendererBasePtr_t buildRenderer(const TokenBasePtr_t& tokenPtr) const;
        RendererBasePtr_t buildPlainTextRenderer(const TokenBasePtr_t& tokenPtr) const;
        RendererBasePtr_t buildVariableRenderer(const TokenBasePtr_t& tokenPtr) const;

        // Renderer builders for non-standalone tokens
        RendererBasePtr_t buildRenderer(const TokenPtrListConstIter_t& begin, const TokenPtrListConstIter_t& end) const;
        RendererBasePtr_t buildLoopRenderer(const TokenPtrListConstIter_t& begin,
                                            const TokenPtrListConstIter_t& end) const;

        RendererBasePtr_t buildFallbackRenderer(const std::string& rawTokenText) const;

        typedef RendererBasePtr_t (RendererChainBuilder::*StandaloneRendererBuilder_t)(const TokenBasePtr_t&) const;
        typedef std::unordered_map<TokenType_t, StandaloneRendererBuilder_t,
                                   TokenTypeHasher> StandaloneRendererBuilderMap_t;

        typedef RendererBasePtr_t (RendererChainBuilder::*NonStandaloneRendererBuilder_t)
                                  (const TokenPtrListConstIter_t&, const TokenPtrListConstIter_t&) const;
        typedef std::unordered_map<TokenType_t, NonStandaloneRendererBuilder_t,
                                   TokenTypeHasher> NonStandaloneRendererBuilderMap_t;

        StandaloneRendererBuilderMap_t    myStandaloneRendererBuilderMap;
        NonStandaloneRendererBuilderMap_t myNonStandaloneRendererBuilderMap;

    };

}

#endif //TEMPLATINGENGINE_RENDERERCHAINBUILDER_H
