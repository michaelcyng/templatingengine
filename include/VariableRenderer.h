//
// Created by Cho Yiu Ng on 07.10.19.
//

#ifndef TEMPLATINGENGINE_VARIABLERENDERER_H
#define TEMPLATINGENGINE_VARIABLERENDERER_H

#include <RendererBase.h>

namespace templatingengine {

    class VariableRenderer : public RendererBase {
    public:

        explicit VariableRenderer(std::string name);

        void render(std::ostream& os, const ParameterSet& parameters) const override;

    private:

        void defaultRender(std::ostream& os) const;

        std::string myName;

    };

}

#endif //TEMPLATINGENGINE_VARIABLERENDERER_H
