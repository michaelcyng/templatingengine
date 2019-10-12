//
// Created by Cho Yiu Ng on 07.10.19.
//

#ifndef TEMPLATINGENGINE_RENDERERBASE_H
#define TEMPLATINGENGINE_RENDERERBASE_H

#include <memory>
#include <ostream>

#include <ParameterSet.h>

namespace templatingengine {

    class RendererBase {
    public:

        virtual ~RendererBase() = default;

        virtual void render(std::ostream& os, const ParameterSet& parameters) const = 0;

    private:
    };

    typedef std::shared_ptr<RendererBase> RendererBasePtr_t;

}

#endif //TEMPLATINGENGINE_RENDERERBASE_H
