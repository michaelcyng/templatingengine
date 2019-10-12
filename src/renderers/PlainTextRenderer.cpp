//
// Created by Cho Yiu Ng on 07.10.19.
//

#include <renderers/PlainTextRenderer.h>

using namespace templatingengine;

PlainTextRenderer::PlainTextRenderer(std::string text) : myText(std::move(text)) {
}

void PlainTextRenderer::render(std::ostream &os, const templatingengine::ParameterSet &parameters) const {
    os << myText;
}
