//
// Created by Cho Yiu Ng on 11.10.19.
//

#include <list>

#include <Template.h>

#include <builders/RendererChainBuilder.h>
#include <builders/TokenListBuilder.h>

#include <utils/TemplateTokenizer.h>


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

    auto tokenPtrList = TokenListBuilder::buildTokenPtrList(tokenTextList);

    return RendererChainBuilder::buildRendererChain(tokenPtrList);
}
