//
// Created by Cho Yiu Ng on 10.10.19.
//

#include <builders/RendererChainBuilder.h>

#include <renderers/LoopRenderer.h>
#include <renderers/PlainTextRenderer.h>
#include <renderers/VariableRenderer.h>

#include <tokens/LoopOpeningToken.h>
#include <tokens/PlainTextToken.h>
#include <tokens/VariableToken.h>

#include <utils/TokenUtils.h>


using namespace templatingengine;

const RendererChainBuilder::StandaloneRendererBuilderMap_t RendererChainBuilder::ourStandaloneRendererBuilderMap({
    {TokenType_t::PlainText_e, &RendererChainBuilder::buildPlainTextRenderer},
    {TokenType_t::Variable_e,  &RendererChainBuilder::buildVariableRenderer}
});

const RendererChainBuilder::NonStandaloneRendererBuilderMap_t RendererChainBuilder::ourNonStandaloneRendererBuilderMap({
    {TokenType_t::LoopOpen_e, &RendererChainBuilder::buildLoopRenderer}
});

RendererChainPtr_t RendererChainBuilder::buildRendererChain(const TokenPtrList_t& tokenPtrList) {
    return buildRendererChain(tokenPtrList.begin(), tokenPtrList.end());
}

RendererChainPtr_t RendererChainBuilder::buildRendererChain(const TokenPtrListConstIter_t &begin,
                                                            const TokenPtrListConstIter_t &end) {
    RendererChainPtr_t rendererChainPtr = std::make_shared<RendererChain>();

    for (TokenPtrListConstIter_t tokenIter = begin; tokenIter != end;) {

        // Standalone token
        if ((*tokenIter)->isStandalone()) {
            auto rendererPtr = buildRenderer(*tokenIter);
            rendererChainPtr->addRenderer(rendererPtr);
            ++tokenIter;
            continue;
        }

        // Non-standalone token
        auto endTokenIter = TokenUtils::findClosingTokenPtrIter(tokenIter, end);

        // Closing token is not found
        if (endTokenIter == end) {
            auto rendererPtr = buildFallbackRenderer((*tokenIter)->getRawTokenText());
            rendererChainPtr->addRenderer(rendererPtr);
            ++tokenIter;
            continue;
        }

        auto rendererPtr = buildRenderer(tokenIter, endTokenIter);
        rendererChainPtr->addRenderer(rendererPtr);
        tokenIter = endTokenIter;
        ++tokenIter;
    }

    return rendererChainPtr;
}

RendererBasePtr_t RendererChainBuilder::buildRenderer(const TokenBasePtr_t& tokenPtr) {
    auto builderFunctor = ourStandaloneRendererBuilderMap.at(tokenPtr->getTokenType());
    return builderFunctor(tokenPtr);
}

RendererBasePtr_t RendererChainBuilder::buildPlainTextRenderer(const TokenBasePtr_t& tokenPtr) {
    return std::make_shared<PlainTextRenderer>(std::static_pointer_cast<PlainTextToken>(tokenPtr)->getText());
}

RendererBasePtr_t RendererChainBuilder::buildVariableRenderer(const TokenBasePtr_t& tokenPtr) {
    return std::make_shared<VariableRenderer>(std::static_pointer_cast<VariableToken>(tokenPtr)->getVariableName());
}

RendererBasePtr_t RendererChainBuilder::buildRenderer(const TokenPtrListConstIter_t& begin,
                                                      const TokenPtrListConstIter_t& end) {
    auto builderFunctor = ourNonStandaloneRendererBuilderMap.at((*begin)->getTokenType());
    return builderFunctor(begin, end);
}

RendererBasePtr_t RendererChainBuilder::buildLoopRenderer(const TokenPtrListConstIter_t& begin,
                                                          const TokenPtrListConstIter_t& end) {
    auto loopContentBegin = begin;
    ++loopContentBegin;
    auto loopContentPtr = buildRendererChain(loopContentBegin, end);
    auto loopOpeningToken = std::static_pointer_cast<LoopOpeningToken>(*begin);
    auto loopRenderPtr = std::make_shared<LoopRenderer>(loopOpeningToken->getValueListName(),
                                                        loopOpeningToken->getElementName(),
                                                        loopContentPtr);
    return loopRenderPtr;
}

RendererBasePtr_t RendererChainBuilder::buildFallbackRenderer(const std::string &rawTokenText) {
    return std::make_shared<PlainTextRenderer>(rawTokenText);
}
