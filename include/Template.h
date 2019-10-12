//
// Created by Cho Yiu Ng on 11.10.19.
//

#ifndef TEMPLATINGENGINE_TEMPLATE_H
#define TEMPLATINGENGINE_TEMPLATE_H

#include <istream>
#include <ostream>

#include <ParameterSet.h>
#include <renderers/RendererChain.h>

namespace templatingengine {

    class Template {
    public:

        explicit Template(std::istream& inputStream);

        void generateDocument(const ParameterSet& parameters, std::ostream& outputStream) const;

    private:

        RendererChainPtr_t compile(std::istream& inputStream);

        RendererChainPtr_t myRendererChainPtr;

    };

}

#endif //TEMPLATINGENGINE_TEMPLATE_H
