//
// Created by Cho Yiu Ng on 08.10.19.
//

#include <renderers/RendererChain.h>

using namespace templatingengine;

void RendererChain::addRenderer(RendererBasePtr_t& rendererPtr) {
    myRendererPtrList.push_back(rendererPtr);
}

void RendererChain::render(std::ostream &os, const templatingengine::ParameterSet &parameters) const {
    for (const auto& rendererPtr: myRendererPtrList) {
        rendererPtr->render(os, parameters);
    }
}
