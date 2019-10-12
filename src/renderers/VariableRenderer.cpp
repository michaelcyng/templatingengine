//
// Created by Cho Yiu Ng on 07.10.19.
//

#include <renderers/VariableRenderer.h>

using namespace templatingengine;

VariableRenderer::VariableRenderer(std::string name) : myName(std::move(name)) {
}

void VariableRenderer::render(std::ostream &os, const templatingengine::ParameterSet &parameters) const {
    auto iter = parameters.find(myName);

    // Case 1: variable is not set
    if (iter == parameters.end()) {
        defaultRender(os);
        return;
    }

    // Case 2: variable is set
    auto value = iter->second;
    os << *value;
}

void VariableRenderer::defaultRender(std::ostream &os) const {
    os << "{{" << myName << "}}";
}
