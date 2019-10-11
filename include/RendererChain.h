//
// Created by Cho Yiu Ng on 08.10.19.
//

#ifndef TEMPLATINGENGINE_RENDERERCHAIN_H
#define TEMPLATINGENGINE_RENDERERCHAIN_H

#include <list>
#include <memory>

#include <RendererBase.h>

namespace templatingengine {

    class RendererChain : public RendererBase {
    public:

        void addRenderer(RendererBasePtr_t& rendererPtr);
        void render(std::ostream& os, const ParameterSet& parameters) const override;

    private:

        std::list<RendererBasePtr_t> myRendererPtrList;

    };

    typedef std::shared_ptr<RendererChain> RendererChainPtr_t;

}

#endif //TEMPLATINGENGINE_RENDERERCHAIN_H
