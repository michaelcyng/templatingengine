//
// Created by Cho Yiu Ng on 09.10.19.
//

#include <dataTypes/ValueList.h>
#include <renderers/LoopRenderer.h>

using namespace templatingengine;

LoopRenderer::LoopRenderer(std::string valueListName,
                           std::string elementName,
                           RendererChainPtr_t& rendererChainPtr) :
    myElementName(std::move(elementName)), myRendererChainPtr(rendererChainPtr),
    myValueListName(std::move(valueListName)) {
}

void LoopRenderer::render(std::ostream& os, const ParameterSet& parameters) const {
    auto paramIter = parameters.find(myValueListName);

    // If the parameter does not exist, render nothing
    if (paramIter == parameters.end()) {
        return;
    }

    // Differentiate the rendering of a single-valued parameter from the multi-valued parameter
    if (paramIter->second->isMultiValueType()) {
        multiValueRender(os, parameters);
    }
    else {
        singleValueRender(os, parameters);
    }
}

void LoopRenderer::multiValueRender(std::ostream &os, const templatingengine::ParameterSet &parameters) const {
    templatingengine::ParameterSet newParameters(parameters);

    auto valueList = static_cast<ValueBasePtr_t>(parameters.at(myValueListName));
    for (const auto& elementValue: *(std::static_pointer_cast<ValueList>(valueList))) {
        newParameters[myElementName] = elementValue;
        myRendererChainPtr->render(os, newParameters);
    }
}

void LoopRenderer::singleValueRender(std::ostream &os, const templatingengine::ParameterSet &parameters) const {
    templatingengine::ParameterSet newParameters(parameters);
    newParameters[myElementName] = parameters.at(myValueListName);
    myRendererChainPtr->render(os, newParameters);
}
