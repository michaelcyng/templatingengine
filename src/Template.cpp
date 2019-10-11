//
// Created by Cho Yiu Ng on 11.10.19.
//

#include <list>

#include <RendererChainBuilder.h>
#include <Template.h>
#include <TemplateTokenizer.h>
#include <TokenListBuilder.h>

using namespace templatingengine;

Template::Template(std::istream &inputStream) : myRendererChainPtr(compile(inputStream)) {
}

void Template::generateDocument(const ParameterSet &parameters, std::ostream& outputStream) const {
    myRendererChainPtr->render(outputStream, parameters);
}

RendererChainPtr_t Template::compile(std::istream &inputStream) {
    TemplateTokenizer tokenizer;
    std::list<std::string> tokenTextList;
    tokenizer.tokenize(inputStream, tokenTextList);

    TokenListBuilder tokenListBuilder;
    auto tokenPtrList = tokenListBuilder.buildTokenPtrList(tokenTextList);

    RendererChainBuilder rendererChainBuilder;
    return rendererChainBuilder.buildRendererChain(tokenPtrList);
}
