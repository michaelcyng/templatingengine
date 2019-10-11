//
// Created by Cho Yiu Ng on 10.10.19.
//

#include <LoopRenderer.h>
#include <PlainTextRenderer.h>
#include <PlainTextToken.h>
#include <RendererChainBuilder.h>
#include <TokenUtils.h>
#include <VariableRenderer.h>
#include <VariableToken.h>
#include <LoopOpeningToken.h>

using namespace templatingengine;

RendererChainBuilder::RendererChainBuilder(): myStandaloneRendererBuilderMap({
    {TokenType_t::PlainText_e, &RendererChainBuilder::buildPlainTextRenderer},
    {TokenType_t::Variable_e, &RendererChainBuilder::buildVariableRenderer}
}), myNonStandaloneRendererBuilderMap({
    {TokenType_t::LoopOpen_e, &RendererChainBuilder::buildLoopRenderer}
}){
}

RendererChainPtr_t RendererChainBuilder::buildRendererChain(const TokenPtrList_t& tokenPtrList) const {
    return buildRendererChain(tokenPtrList.begin(), tokenPtrList.end());
}

RendererChainPtr_t RendererChainBuilder::buildRendererChain(const TokenPtrListConstIter_t &begin,
                                                            const TokenPtrListConstIter_t &end) const {
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

RendererBasePtr_t RendererChainBuilder::buildRenderer(const TokenBasePtr_t& tokenPtr) const {
    auto builderFunctor = myStandaloneRendererBuilderMap.at(tokenPtr->getTokenType());
    return (this->*builderFunctor)(tokenPtr);
}

RendererBasePtr_t RendererChainBuilder::buildPlainTextRenderer(const TokenBasePtr_t& tokenPtr) const {
    return std::make_shared<PlainTextRenderer>(std::static_pointer_cast<PlainTextToken>(tokenPtr)->getText());
}

RendererBasePtr_t RendererChainBuilder::buildVariableRenderer(const TokenBasePtr_t& tokenPtr) const {
    return std::make_shared<VariableRenderer>(std::static_pointer_cast<VariableToken>(tokenPtr)->getVariableName());
}

RendererBasePtr_t RendererChainBuilder::buildRenderer(const TokenPtrListConstIter_t& begin,
                                                      const TokenPtrListConstIter_t& end) const {
    auto builderFunctor = myNonStandaloneRendererBuilderMap.at((*begin)->getTokenType());
    return (this->*builderFunctor)(begin, end);
}

RendererBasePtr_t RendererChainBuilder::buildLoopRenderer(const TokenPtrListConstIter_t& begin,
                                                          const TokenPtrListConstIter_t& end) const {
    auto loopContentBegin = begin;
    ++loopContentBegin;
    auto loopContentPtr = buildRendererChain(loopContentBegin, end);
    auto loopOpeningToken = std::static_pointer_cast<LoopOpeningToken>(*begin);
    auto loopRenderPtr = std::make_shared<LoopRenderer>(loopOpeningToken->getValueListName(),
                                                        loopOpeningToken->getElementName(),
                                                        loopContentPtr);
    return loopRenderPtr;
}

RendererBasePtr_t RendererChainBuilder::buildFallbackRenderer(const std::string &rawTokenText) const {
    return std::make_shared<PlainTextRenderer>(rawTokenText);
}
