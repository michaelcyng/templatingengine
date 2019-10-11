//
// Created by Cho Yiu Ng on 07.10.19.
//

#ifndef TEMPLATINGENGINE_PLAINTEXTRENDERER_H
#define TEMPLATINGENGINE_PLAINTEXTRENDERER_H

#include <RendererBase.h>

namespace templatingengine {

    class PlainTextRenderer : public RendererBase {
    public:

        explicit PlainTextRenderer(std::string text);

        void render(std::ostream& os, const ParameterSet& parameters) const override;

    private:

        std::string myText;

    };

}

#endif //TEMPLATINGENGINE_PLAINTEXTRENDERER_H
