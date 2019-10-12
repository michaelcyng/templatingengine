//
// Created by Cho Yiu Ng on 08.10.19.
//

#ifndef TEMPLATINGENGINE_LOOPRENDERER_H
#define TEMPLATINGENGINE_LOOPRENDERER_H

#include <renderers/RendererBase.h>
#include <renderers/RendererChain.h>

namespace templatingengine {

    class LoopRenderer : public RendererBase {
    public:

        LoopRenderer(std::string valueListName,
                     std::string elementName,
                     RendererChainPtr_t& rendererChainPtr);

        void render(std::ostream& os, const ParameterSet& parameters) const override;

    private:

        void multiValueRender(std::ostream& os, const ParameterSet& parameters) const;
        void singleValueRender(std::ostream& os, const ParameterSet& parameters) const;

        std::string        myElementName;
        RendererChainPtr_t myRendererChainPtr;
        std::string        myValueListName;
    };

}

#endif //TEMPLATINGENGINE_LOOPRENDERER_H
