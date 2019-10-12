//
// Created by Cho Yiu Ng on 11.10.19.
//

#ifndef TEMPLATINGENGINE_TEMPLATETOKENIZER_H
#define TEMPLATINGENGINE_TEMPLATETOKENIZER_H

#include <istream>
#include <list>

namespace templatingengine {

    class TemplateTokenizer {
    public:

        void tokenize(std::istream& inputStream, std::list<std::string>& tokenTextList);

    private:

    };

}

#endif //TEMPLATINGENGINE_TEMPLATETOKENIZER_H
